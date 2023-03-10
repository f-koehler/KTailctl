// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_APP_H
#define TAILCTL_APP_H

#include "status.h"

#include <QObject>

class QQuickWindow;

class App : public QObject {
  Q_OBJECT

private:
    Status m_status;

public:
  // Restore current window geometry
  Q_INVOKABLE void
  restoreWindowGeometry(QQuickWindow *window,
                        const QString &group = QStringLiteral("main")) const;
  // Save current window geometry
  Q_INVOKABLE void
  saveWindowGeometry(QQuickWindow *window,
                     const QString &group = QStringLiteral("main")) const;

  Q_INVOKABLE void
  refreshStatus(QQuickWindow *window);
};

#endif /* TAILCTL_APP_H */
