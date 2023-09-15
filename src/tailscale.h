// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_TAILSCALE_H
#define KTAILCTL_TAILSCALE_H

#include "preferences.h"
#include "statistics.h"
#include "status.h"
#include "taildrop_receiver.h"
#include "taildrop_sender.h"

#include <QObject>

class Tailscale : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Status *status READ status CONSTANT)
    Q_PROPERTY(Statistics *statistics READ statistics CONSTANT)
    Q_PROPERTY(Preferences *preferences READ preferences CONSTANT)
    Q_PROPERTY(TaildropReceiver *taildropReceiver READ taildropReceiver CONSTANT)

private:
    Status *mStatus;
    Statistics *mStatistics;
    Preferences *mPreferences;
    TaildropReceiver *mTaildropReceiver;

public slots:
    Q_INVOKABLE void toggle();

public:
    Tailscale(QObject *parent = nullptr);

    Q_INVOKABLE static void up();
    Q_INVOKABLE static void down();

    const QString &executable() const;
    Status *status();
    Statistics *statistics();
    Preferences *preferences();
    TaildropReceiver *taildropReceiver();
};

#endif /* KTAILCTL_TAILSCALE_H */
