// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_TRAY_ICON_HPP
#define KTAILCTL_TRAY_ICON_HPP

#include "ktailctlconfig.h"
#include "tailscale.hpp"

#include <QQuickWindow>
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    Tailscale *mTailscale;
    QQuickWindow *mWindow{};
    KTailctlConfig *mConfig;

    QMenu *mContextMenu;
    QAction *mOpenAction;
    QAction *mToggleAction;
    QMenu *mPeerMenu;
    QMenu *mExitNodeMenu;
    QAction *mUnsetAction;
    QAction *mSuggestedAction;
    QAction *mLastUsedAction;
    QMenu *mMullvadMenu;
    QMenu *mSelfHostedMenu;
    QMap<QString, QMenu *> mMullvadCountryMenus;
    QAction *mQuitAction;

    int mNumMullvadNodes;

public slots:
    void updateIcon();
    void regenerate();
    void buildSelfHostedMenu();
    void buildMullvadMenu();
    void buildPeerMenu();
    void buildUseSuggestedAction();
    void buildLastUsedAction();
    void buildUnsetAction();

signals:
    void quitClicked();

public:
    explicit TrayIcon(QObject *parent = nullptr);
    virtual ~TrayIcon() = default;

    void setWindow(QQuickWindow *window);

protected:
    void setExitNodeMenuEnabled(bool enabled);
};

#endif /* KTAILCTL_TRAY_ICON_HPP */
