// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_TAILSCALE_H
#define KTAILCTL_TAILSCALE_H

#include "statistics.h"
#include "status.h"

#include <QObject>

class Tailscale : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Status *status READ status NOTIFY statusChanged)
    Q_PROPERTY(Statistics *statistics READ statistics NOTIFY statisticsChanged)

private:
    Status *mStatus;
    Statistics *mStatistics;

signals:
    void statusChanged();
    void statisticsChanged();

public slots:
    Q_INVOKABLE void toggle();

public:
    Tailscale(QObject *parent = nullptr);

    Q_INVOKABLE void up();
    Q_INVOKABLE void down();

    const QString &executable() const;
    Status *status();
    Statistics *statistics();
};

#endif /* KTAILCTL_TAILSCALE_H */
