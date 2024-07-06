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
    QMenu *mMullvadMenu;
    QMenu *mSelfHostedMenu;
    QMap<QString, QMenu *> mMullvadCountryMenus;
    QAction *mQuitAction;

    void addExitNodeMenu(QMenu *menu);
    void addMullvadMenu(QMenu *menu);
    void addExitNodeActions(QMenu *menu);
    void buildPeerMenu(QMenu *menu);

public slots:
    void updateIcon();
    void regenerate();

signals:
    void quitClicked();

public:
    explicit TrayIcon(QObject *parent = nullptr);
    virtual ~TrayIcon() = default;

    void setWindow(QQuickWindow *window);
};

#endif /* KTAILCTL_TRAY_ICON_HPP */
