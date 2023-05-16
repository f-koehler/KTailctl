// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_NOTIFIER_H
#define KTAILCTL_NOTIFIER_H

#include "status.h"

#include <KNotification>
#include <QObject>

class Notifier : public QObject
{
private:
    QString mLastBackendState;

public:
    Notifier(QObject *parent = nullptr);

public slots:
    void statusRefreshed(const Status &status);
};

#endif /* KTAILCTL_NOTIFIER_H */
