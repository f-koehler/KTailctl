// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "app.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QQuickWindow>

#include <functional>

QString strategyToString(const TailctlConfig::EnumTaildropStrategy::type &strategy)
{
    switch (strategy) {
    case TailctlConfig::EnumTaildropStrategy::Overwrite:
        return "overwrite";
    case TailctlConfig::EnumTaildropStrategy::Skip:
        return "skip";
    default:
        return "rename";
    }
}

App::App(QObject *parent)
    : QObject(parent)
    , mConfig(TailctlConfig::self())
    , mTaildropProcess(mConfig->tailscaleExecutable(),
                       mConfig->taildropEnabled(),
                       mConfig->taildropDirectory(),
                       strategyToString(mConfig->taildropStrategy()),
                       this)
    , mTrayIcon(this)
{
    mTrayIcon.setContextMenu(new QMenu());

    QObject::connect(mConfig, &TailctlConfig::tailscaleExecutableChanged, [this]() {
        this->mTaildropProcess.setExecutable(mConfig->tailscaleExecutable());
    });
    QObject::connect(mConfig, &TailctlConfig::taildropEnabledChanged, [this]() {
        this->mTaildropProcess.setEnabled(mConfig->taildropEnabled());
    });
    QObject::connect(mConfig, &TailctlConfig::taildropDirectoryChanged, [this]() {
        this->mTaildropProcess.setDirectory(mConfig->taildropDirectory());
    });
    QObject::connect(mConfig, &TailctlConfig::taildropStrategyChanged, [this]() {
        this->mTaildropProcess.setStrategy(strategyToString(mConfig->taildropStrategy()));
    });

    QObject::connect(&mStatus, &Status::peersChanged, &mPeerModel, &PeerModel::updatePeers);
    QObject::connect(&mStatus, &Status::refreshed, &mPeerDetails, &Peer::updateFromStatus);
    QObject::connect(mTrayIcon.contextMenu(), &QMenu::aboutToShow, this, &App::updateTrayMenu);

    mTrayIcon.setIcon(QIcon::fromTheme(QStringLiteral("online")));
    mTrayIcon.show();
}

TailctlConfig *App::config()
{
    return mConfig;
}
Status *App::status()
{
    return &mStatus;
}
Peer *App::peerDetails()
{
    return &mPeerDetails;
}
PeerModel *App::peerModel()
{
    return &mPeerModel;
}

void App::setWindow(QQuickWindow *window)
{
    mWindow = window;
}

void App::restoreWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::restoreWindowSize(window, windowGroup);
    KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group) const
{
    KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig, QStandardPaths::AppDataLocation);
    KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
    KWindowConfig::saveWindowPosition(window, windowGroup);
    KWindowConfig::saveWindowSize(window, windowGroup);
    dataResource.sync();
}

void App::setPeerDetails(const QString &id)
{
    if (mStatus.self()->id() == id) {
        mPeerDetails = *mStatus.self();
        emit peerDetailsChanged();
    } else {
        auto pos = std::find_if(mStatus.peers().begin(), mStatus.peers().end(), [&id](Peer *peer) {
            return peer->id() == id;
        });
        if (pos == mStatus.peers().end()) {
            qWarning() << "Peer" << id << "not found";
            return;
        }
        if (mPeerDetails.setTo(*pos)) {
            emit peerDetailsChanged();
        }
    }
}

void App::setClipboardText(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

QIcon App::loadOsIcon(const QString &os) const
{
    const auto osname = os.toLower();
    if (osname == "linux") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "windows") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "macos") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "android") {
        return QIcon::fromTheme("smartphone");
    }
    if (osname == "ios") {
        return QIcon::fromTheme("smartphone");
    }
    return QIcon::fromTheme("question");
}

void App::updateTrayMenu()
{
    QMenu *menu = mTrayIcon.contextMenu();
    menu->clear();

    menu->addAction("Open", this, [this]() {
        if (mWindow != nullptr) {
            mWindow->show();
        }
    });
    menu->addSeparator();

    QClipboard *clipboard = QGuiApplication::clipboard();
    auto create_action = [clipboard](QMenu *menu, const QString &text) {
        auto *action = menu->addAction(text);
        connect(action, &QAction::triggered, [clipboard, &text]() {
            clipboard->setText(text);
        });
        return action;
    };
    for (auto peer : mStatus.peers()) {
        auto *submenu = menu->addMenu(loadOsIcon(peer->os()), peer->hostName());
        create_action(submenu, peer->dnsName());
        for (auto address : peer->tailscaleIps()) {
            create_action(submenu, address);
        }
    }

    menu->addSeparator();
    menu->addAction(QIcon::fromTheme("application-exit"), "Quit", this, qApp->quit);
    mTrayIcon.setContextMenu(menu);
}

QString App::formatCapacityHumanReadable(long bytes) const
{
    static constexpr const char *units[] = {"TiB", "GiB", "MiB", "KiB", "B"};
    static constexpr long limits[] = {1L << 40, 1L << 30, 1L << 20, 1L << 10, 1L};
    for (int i = 0; i < 5; ++i) {
        if (bytes >= limits[i]) {
            return QString("%1 %2").arg(static_cast<double>(bytes) / static_cast<double>(limits[i]), 0, 'f', 3).arg(units[i]);
        }
    }
    return "0 B";
}

QString App::formatSpeedHumanReadable(double bytes_per_second) const
{
    static constexpr const char *units[] = {"TiB/s", "GiB/s", "MiB/s", "KiB/s", "B/s"};
    static constexpr double limits[] = {static_cast<double>(1L << 40),
                                        static_cast<double>(1L << 30),
                                        static_cast<double>(1L << 20),
                                        static_cast<double>(1L << 10),
                                        1.0};
    for (int i = 0; i < 5; ++i) {
        if (bytes_per_second >= limits[i]) {
            return QString("%1 %2").arg(bytes_per_second / static_cast<double>(limits[i]), 0, 'f', 2).arg(units[i]);
        }
    }
    return "0 B/s";
}

QString App::formatDurationHumanReadable(const QDateTime &from, const QDateTime &to) const
{
    static constexpr qint64 conversions[] =
        {365l * 30l * 24l * 60l * 60l * 1000l, 34 * 30l * 60l * 60l * 1000l, 24l * 60l * 60l * 1000l, 60l * 60l * 1000l, 60l * 1000l, 1000l};
    static constexpr const char *units[] = {"year", "month", "day", "hour", "minute", "second"};

    QString result = "";
    if (from > to) {
        return result;
    }
    qint64 msecs = from.msecsTo(to);
    if (msecs < 1000) {
        return result;
    }
    for (int i = 0; i < 6; ++i) {
        if (msecs >= conversions[i]) {
            if (!result.isEmpty()) {
                result += ", ";
            }
            qint64 count = msecs / conversions[i];
            result += QString("%1 %2%3 ").arg(count).arg(units[i]).arg(count > 1 ? "s" : "");
            msecs %= conversions[i];
        }
    }

    return result;
}

QString App::fileUrlToString(const QUrl &url) const
{
    return url.toString(QUrl::PreferLocalFile);
}

qint64 App::toMSecsSinceEpoch(const QDateTime &dateTime) const
{
    return dateTime.toMSecsSinceEpoch();
}