// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "tray_icon.h"
#include "util.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>

TrayIcon::TrayIcon(Tailscale *tailscale, QObject *parent)
    : QSystemTrayIcon(parent)
    , mTailscale(tailscale)
{
    if (mTailscale->status()->backendState() == "Running") {
        setIcon(QIcon::fromTheme(QStringLiteral("ktailctl-tray-online-dark")));
    } else {
        setIcon(QIcon::fromTheme(QStringLiteral("ktailctl-tray-offline")));
    }

    setContextMenu(new QMenu());

    QObject::connect(contextMenu(), &QMenu::aboutToShow, this, &TrayIcon::regenerate);
    QObject::connect(this, &QSystemTrayIcon::activated, [this](const QSystemTrayIcon::ActivationReason &) {
        if (mWindow == nullptr) {
            return;
        }
        if (mWindow->isVisible()) {
            mWindow->hide();
        } else {
            mWindow->show();
        }
    });

    show();
}

void TrayIcon::regenerate()
{
    QMenu *menu = contextMenu();
    menu->clear();

    if (mWindow != nullptr) {
        menu->addAction(QIcon::fromTheme("window"), "Open", this, [this]() {
            mWindow->show();
        });
        menu->addSeparator();
    }

    auto action_toggle = menu->addAction("Toggle", [this]() {
        mTailscale->toggle();
    });
    if (mTailscale->status()->backendState() == "Running") {
        action_toggle->setChecked(true);
        action_toggle->setText("Stop Tailscale");

        setIcon(QIcon::fromTheme(QStringLiteral("ktailctl-tray-online-dark")));
    } else {
        action_toggle->setChecked(false);
        action_toggle->setText("Start Tailscale");

        setIcon(QIcon::fromTheme(QStringLiteral("ktailctl-tray-offline")));
    }
    menu->addSeparator();

    auto create_action = [](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(text);
        connect(action, &QAction::triggered, [text]() {
            setClipboardText(text);
        });
        return action;
    };
    for (auto peer : mTailscale->status()->peers()) {
        auto *submenu = menu->addMenu(loadOsIcon(peer->os()), peer->hostName());
        create_action(submenu, peer->dnsName());
        for (auto address : peer->tailscaleIps()) {
            create_action(submenu, address);
        }
        submenu->addSection("Statistics");

        auto statsUp = mTailscale->statistics()->speedUp(peer->id());
        auto statsDown = mTailscale->statistics()->speedDown(peer->id());
        auto actionUp = submenu->addAction(QIcon::fromTheme("vcs-push"), formatSpeedHumanReadable(statsUp->average()));
        auto actionDown = submenu->addAction(QIcon::fromTheme("vcs-pull"), formatSpeedHumanReadable(statsDown->average()));

        QObject::connect(statsUp, &SpeedStatistics::refreshed, [actionUp, statsUp]() {
            actionUp->setText(formatSpeedHumanReadable(statsUp->average()));
        });
        QObject::connect(statsDown, &SpeedStatistics::refreshed, [actionDown, statsDown]() {
            actionDown->setText(formatSpeedHumanReadable(statsDown->average()));
        });
    }
    menu->addSeparator();
    menu->addAction(QIcon::fromTheme("application-exit"), "Quit", [this]() {
        emit quitClicked();
    });
    setContextMenu(menu);
}

void TrayIcon::setWindow(QQuickWindow *window)
{
    mWindow = window;
}