#ifndef TAILCTL_TRAY_ICON_H
#define TAILCTL_TRAY_ICON_H

#include "status.h"

#include <QQuickWindow>
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

private:
    Status *mStatus;
    QQuickWindow *mWindow;

public slots:
    void regenerate();

public:
    TrayIcon(Status *status, QObject *parent = nullptr);

    void setWindow(QQuickWindow *window);
};

#endif /* TAILCTL_TRAY_ICON_H */
