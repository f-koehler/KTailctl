// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_TRAY_ICON_H
#define KTAILCTL_TRAY_ICON_H

#include "ktailctlconfig.h"
#include "tailscale.h"

#include <QQuickWindow>
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    Tailscale *mTailscale;
    QQuickWindow *mWindow{};
    KTailctlConfig *mConfig;

    void addToggleAction(QMenu *menu);
    void addExitNodeMenu(QMenu *menu);
    void addMullvadMenu(QMenu *menu);
    void addExitNodeActions(QMenu *menu);
    void addPeerMenu(QMenu *menu);

public slots:
    void regenerate();
    void updateIcon();

signals:
    void quitClicked();

public:
    explicit TrayIcon(QObject *parent = nullptr);
    virtual ~TrayIcon() = default;

    void setWindow(QQuickWindow *window);
};

#endif /* KTAILCTL_TRAY_ICON_H */
