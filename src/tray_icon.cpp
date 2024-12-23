// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tray_icon.hpp"
#include "ktailctlconfig.h"
#include "taildrop_sender.hpp"
#include "util.hpp"

#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGuiApplication>
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QMessageBox>

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
    , mTailscale(Tailscale::instance())
    , mConfig(KTailctlConfig::self())
{
    updateIcon();

    mContextMenu = new QMenu();
    setContextMenu(mContextMenu);

    mOpenAction = mContextMenu->addAction(QIcon::fromTheme(QStringLiteral("window")), QStringLiteral("Open"), [this]() {
        mWindow->show();
    });
    mContextMenu->addSeparator();
    mPeerMenu = mContextMenu->addMenu(QIcon::fromTheme(QStringLiteral("applications-network")), QStringLiteral("Peers"));
    mExitNodeMenu = mContextMenu->addMenu(QIcon::fromTheme(QStringLiteral("internet-services")), QStringLiteral("Exit nodes"));
    mUnsetAction = mExitNodeMenu->addAction(QIcon::fromTheme(QStringLiteral("dialog-cancel")), QStringLiteral("Unset"), [this] {
        mTailscale->unsetExitNode();
    });
    mSuggestedAction = mExitNodeMenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Use suggested"), [this]() {
        mTailscale->setExitNode(mTailscale->suggestedExitNode().mDnsName);
    });
    mLastUsedAction = mExitNodeMenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Last used"), [this]() {
        mTailscale->setExitNode(mConfig->lastUsedExitNode());
    });
    mExitNodeMenu->addSeparator();
    mMullvadMenu = mExitNodeMenu->addMenu(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Mullvad"));
    mSelfHostedMenu = mExitNodeMenu->addMenu(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Self-hosted"));
    mToggleAction = mContextMenu->addAction(QStringLiteral("Toggle"), [this]() {
        mTailscale->toggle();
    });
    mContextMenu->addSeparator();
    mQuitAction = mContextMenu->addAction(QIcon::fromTheme(QStringLiteral("application-exit")), QStringLiteral("Quit"), [this]() {
        emit quitClicked();
    });

    buildUnsetAction();
    buildUseSuggestedAction();
    buildLastUsedAction();

    connect(mTailscale, &Tailscale::backendStateChanged, [this]() {
        updateIcon();
        if (mTailscale->backendState() == QStringLiteral("Running")) {
            mToggleAction->setChecked(true);
            mToggleAction->setText(QStringLiteral("Stop Tailscale"));
            mToggleAction->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
            buildPeerMenu();
        } else {
            mToggleAction->setChecked(false);
            mToggleAction->setText(QStringLiteral("Start Tailscale"));
            mToggleAction->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
            mExitNodeMenu->setEnabled(false);
        }
    });
    connect(mTailscale, &Tailscale::successChanged, [this]() {
        if (mTailscale->success()) {
            buildPeerMenu();
            buildMullvadMenu();
            mPeerMenu->setEnabled(true);
            if (mTailscale->isOperator()) {
                mToggleAction->setEnabled(true);
                mExitNodeMenu->setEnabled(true);
            }
        } else {
            mPeerMenu->setEnabled(false);
            mToggleAction->setEnabled(false);
            mExitNodeMenu->setEnabled(false);
        }
    });
    connect(mTailscale, &Tailscale::isOperatorChanged, [this]() {
        if (mTailscale->isOperator()) {
            mToggleAction->setEnabled(true);
            mPeerMenu->setEnabled(true);
            mExitNodeMenu->setEnabled(true);
        } else {
            mToggleAction->setEnabled(false);
            mPeerMenu->setEnabled(false);
            mExitNodeMenu->setEnabled(false);
        }
    });
    connect(mTailscale, &Tailscale::hasSuggestedExitNodeChanged, this, &TrayIcon::buildUseSuggestedAction);
    connect(mTailscale, &Tailscale::suggestedExitNodeChanged, this, &TrayIcon::buildUseSuggestedAction);
    connect(mConfig, &KTailctlConfig::lastUsedExitNodeChanged, this, &TrayIcon::buildLastUsedAction);
    connect(mTailscale, &Tailscale::hasCurrentExitNodeChanged, this, &TrayIcon::buildUnsetAction);
    connect(mTailscale, &Tailscale::currentExitNodeChanged, this, &TrayIcon::buildUnsetAction);
    connect(mTailscale, &Tailscale::refreshed, this, &TrayIcon::buildMullvadMenu);

    connect(contextMenu(), &QMenu::aboutToShow, this, &TrayIcon::regenerate);
    connect(mConfig, &KTailctlConfig::trayIconThemeChanged, this, &TrayIcon::updateIcon);
    connect(this, &QSystemTrayIcon::activated, [this](const QSystemTrayIcon::ActivationReason &reason) {
        switch (reason) {
        case QSystemTrayIcon::ActivationReason::Trigger:
        case QSystemTrayIcon::ActivationReason::DoubleClick:
        case QSystemTrayIcon::ActivationReason::MiddleClick:
            if (mWindow == nullptr) {
                return;
            }
            if (mWindow->isVisible()) {
                mWindow->hide();
            } else {
                mWindow->show();
            }
            break;
        default:
            break;
        };
    });

    show();
}

void TrayIcon::buildMullvadMenu()
{
    if (mTailscale->mullvadNodeModel()->rowCount() == 0) {
        mMullvadMenu->setEnabled(false);
        mMullvadMenu->setTitle(QStringLiteral("No Mullvad"));
        setExitNodeMenuEnabled(mTailscale->exitNodeModel()->rowCount() > 0);
        return;
    }
    setExitNodeMenuEnabled(true);
    mMullvadMenu->setEnabled(true);
    mMullvadMenu->setTitle(QStringLiteral("Mullvad"));

    // check if rebuild is necessary
    const int newNumMullvadCountries = mTailscale->mullvadCountryModel()->rowCount({});
    const int newNumMullvadNodes = mTailscale->mullvadNodeModel()->rowCount({});
    if ((newNumMullvadCountries == mMullvadCountryMenus.size()) && (newNumMullvadNodes == mNumMullvadNodes)) {
        return;
    }
    mNumMullvadNodes = newNumMullvadNodes;

    // clear mullvad menu
    for (auto &countryMenu : mMullvadCountryMenus) {
        countryMenu->clear();
    }

    // create per-country menus
    if (newNumMullvadCountries != mMullvadCountryMenus.size()) {
        mMullvadMenu->clear();
        for (int i = 0; i < newNumMullvadCountries; ++i) {
            const QModelIndex index = mTailscale->mullvadCountryModel()->index(i, 0);
            const QString countryCode = index.data(MullvadCountryModel::CountryCode).toString().toLower();

            // cppcheck-suppress constVariablePointer
            QMenu *countryMenu = mMullvadMenu->addMenu(QIcon(QString(":/country-flags/country-flag-%1").arg(countryCode)), countryCode);
            mMullvadCountryMenus.insert(countryCode, countryMenu);
        }
    }

    // Add nodes to the country menus
    // The nodes are sorted by country code and then by DNS name
    for (int i = 0; i < newNumMullvadNodes; ++i) {
        const QModelIndex index = mTailscale->mullvadNodeModel()->index(i, 0);
        const QString countryCode = index.data(PeerModel::CountryCodeRole).toString().toLower();
        mMullvadCountryMenus[countryCode]->addAction(QIcon(QString(":/country-flags/country-flag-%1").arg(countryCode)),
                                                     index.data(PeerModel::DnsNameRole).toString(),
                                                     [this, index]() {
                                                         mTailscale->setExitNode(index.data(PeerModel::DnsNameRole).toString());
                                                     });
    }
}

void TrayIcon::buildSelfHostedMenu()
{
    if (mTailscale->exitNodeModel()->rowCount() == 0) {
        mSelfHostedMenu->setEnabled(false);
        mSelfHostedMenu->setTitle(QStringLiteral("No self-hosted"));
        setExitNodeMenuEnabled(mTailscale->mullvadNodeModel()->rowCount() > 0);
        return;
    }
    setExitNodeMenuEnabled(true);
    mSelfHostedMenu->setTitle(QStringLiteral("Self-hosted"));
    mSelfHostedMenu->setEnabled(true);

    mSelfHostedMenu->clear();
    const int numNodes = mTailscale->exitNodeModel()->rowCount();
    for (int i = 0; i < numNodes; ++i) {
        const QModelIndex index = mTailscale->exitNodeModel()->index(i, 0);
        mSelfHostedMenu->addAction(loadOsIcon(index.data(PeerModel::OsRole).toString()), index.data(PeerModel::DnsNameRole).toString(), [this, index]() {
            mTailscale->setExitNode(index.data(PeerModel::DnsNameRole).toString());
        });
    }
}

void TrayIcon::buildPeerMenu()
{
    mPeerMenu->clear();

    auto createCopyAction = [](QMenu *menu, const QString &text) {
        QAction *action = menu->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")), text, [text]() {
            setClipboardText(text);
        });
        return action;
    };

    for (const PeerData &peer : mTailscale->peerModel()->peers()) {
        if (peer.mIsMullvad) {
            continue;
        }
        QMenu *submenu = mPeerMenu->addMenu(loadOsIcon(peer.mOs), peer.mDnsName);
        createCopyAction(submenu, peer.mDnsName);
        for (const QString &ip : peer.mTailscaleIps) {
            createCopyAction(submenu, ip);
        }

        if (!mTailscale->isOperator()) {
            continue;
        }

        submenu->addSection(QStringLiteral("Taildrop Send"));
        submenu->addAction(QIcon::fromTheme(QStringLiteral("document-send")), QStringLiteral("Send file(s)"), [&peer]() {
            TaildropSendJobFactory::selectAndSendFiles(peer.mDnsName);
        });

        if (peer.mIsCurrentExitNode) {
            submenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Unset exit node"), [this]() {
                mTailscale->unsetExitNode();
            });
        } else if (peer.mIsExitNode) {
            submenu->addAction(QIcon::fromTheme(QStringLiteral("network-vpn")), QStringLiteral("Set as exit node"), [this, &peer]() {
                mTailscale->setExitNode(peer.mDnsName);
            });
        }
    }
}
void TrayIcon::buildUseSuggestedAction()
{
    mSuggestedAction->setEnabled(mTailscale->hasSuggestedExitNode());
    if (mTailscale->hasSuggestedExitNode()) {
        mSuggestedAction->setText(QStringLiteral("Use suggested (%1)").arg(mTailscale->suggestedExitNode().mDnsName));
        if (!mTailscale->suggestedExitNode().mCountryCode.isEmpty()) {
            mSuggestedAction->setIcon(QIcon(QString(":/country-flags/country-flag-%1").arg(mTailscale->suggestedExitNode().mCountryCode.toLower())));
        } else {
            mSuggestedAction->setIcon(loadOsIcon(mTailscale->suggestedExitNode().mOs));
        }
    } else {
        mSuggestedAction->setText(QStringLiteral("No suggestion"));
    }
}
void TrayIcon::buildLastUsedAction()
{
    const QString lastUsedExitNode = mConfig->lastUsedExitNode();
    mLastUsedAction->setEnabled(!lastUsedExitNode.isEmpty());
    mLastUsedAction->setVisible(!lastUsedExitNode.isEmpty());
    if (lastUsedExitNode.isEmpty()) {
        return;
    }
    mLastUsedAction->setIcon(QIcon::fromTheme("folder-recent-symbolic"));
    mLastUsedAction->setText(QStringLiteral("Last used (%1)").arg(lastUsedExitNode));
}
void TrayIcon::buildUnsetAction()
{
    mUnsetAction->setEnabled(mTailscale->hasCurrentExitNode());
    if (mTailscale->hasCurrentExitNode()) {
        mUnsetAction->setText(QStringLiteral("Unset (%1)").arg(mTailscale->currentExitNode().mDnsName));
    } else {
        mUnsetAction->setText(QStringLiteral("None set"));
    }
}

void TrayIcon::updateIcon()
{
    setIcon(QIcon(QString(":/icons/%1-%2")
                      .arg((mTailscale->backendState() == "Running") ? QStringLiteral("online") : QStringLiteral("offline"), KTailctlConfig::trayIconTheme())));
}
void TrayIcon::regenerate()
{
    buildPeerMenu();
    buildSelfHostedMenu();
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
    mOpenAction->setVisible(mWindow != nullptr);
}

void TrayIcon::setExitNodeMenuEnabled(bool enabled)
{
    mExitNodeMenu->setEnabled(enabled);
    if (!enabled) {
        mExitNodeMenu->setTitle(QStringLiteral("No exit nodes"));
    } else {
        mExitNodeMenu->setTitle(QStringLiteral("Exit nodes"));
    }
}