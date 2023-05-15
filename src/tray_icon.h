// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef TAILCTL_TRAY_ICON_H
#define TAILCTL_TRAY_ICON_H

#include "tailscale.h"

#include <QQuickWindow>
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    Tailscale *mTailscale;
    QQuickWindow *mWindow;

public slots:
    void regenerate();

public:
    TrayIcon(Tailscale *tailscale, QObject *parent = nullptr);

    void setWindow(QQuickWindow *window);
};

#endif /* TAILCTL_TRAY_ICON_H */
