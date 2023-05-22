// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_TAILDROP_RECEIVER_H
#define KTAILCTL_TAILDROP_RECEIVER_H

#include <QObject>
#include <QThread>

class TaildropReceiver : public QThread
{
    Q_OBJECT

private:
    bool mEnabled;
    QString mDirectory;
    QString mStrategy;

public:
    TaildropReceiver(bool enabled, const QString &directory, const QString &strategy, QObject *parent = nullptr);

protected:
    void run() override;

public slots:
    void setEnabled(bool enabled);
    void setDirectory(const QString &directory);
    void setStrategy(const QString &strategy);
};

#endif /* KTAILCTL_TAILDROP_RECEIVER_H */
