// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_TAILDROP_RECEIVER_H
#define KTAILCTL_TAILDROP_RECEIVER_H

#include "ktailctlconfig.h"
#include <QObject>
#include <QThread>

class TaildropReceiver : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString directory READ directory WRITE setDirectory NOTIFY directoryChanged)
    Q_PROPERTY(QString strategy READ strategy WRITE setStrategy NOTIFY strategyChanged)

private:
    KTailctlConfig *mConfig;
    bool mEnabled;
    QString mDirectory;
    QString mStrategy;

public:
    TaildropReceiver(QObject *parent = nullptr);

protected:
    void run() override;

public slots:
    bool enabled() const;
    const QString &directory() const;
    const QString &strategy() const;

    void setEnabled(bool enabled);
    void setDirectory(const QString &directory);
    void setStrategy(const QString &strategy);

signals:
    void enabledChanged(bool enabled);
    void directoryChanged(const QString &directory);
    void strategyChanged(const QString &strategy);
};

#endif /* KTAILCTL_TAILDROP_RECEIVER_H */
