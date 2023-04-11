#ifndef TAILCTL_TAILSCALE_H
#define TAILCTL_TAILSCALE_H

#include "statistics.h"
#include "status.h"

#include <QObject>

class Tailscale : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString executable READ executable NOTIFY executableChanged)
    Q_PROPERTY(Status *status READ status NOTIFY statusChanged)
    Q_PROPERTY(Statistics *statistics READ statistics NOTIFY statisticsChanged)

private:
    QString mExecutable;
    Status *mStatus;
    Statistics *mStatistics;

signals:
    void executableChanged();
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

    void setExecutable(const QString &executable);
};

#endif /* TAILCTL_TAILSCALE_H */
