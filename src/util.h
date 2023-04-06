#ifndef TAILCTL_UTIL_H
#define TAILCTL_UTIL_H

#include <QDateTime>
#include <QIcon>
#include <QObject>
#include <QString>

void setClipboardText(const QString &text);
QString formatCapacityHumanReadable(long bytes);
QString formatSpeedHumanReadable(double bytes_per_second);
QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to = QDateTime::currentDateTime());
QString fileUrlToString(const QUrl &url);
qint64 toMSecsSinceEpoch(const QDateTime &dateTime);
QIcon loadOsIcon(const QString &os);

class Util : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE void setClipboardText(const QString &text) const;
    Q_INVOKABLE QString formatCapacityHumanReadable(long bytes) const;
    Q_INVOKABLE QString formatSpeedHumanReadable(double bytes_per_second) const;
    Q_INVOKABLE QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to = QDateTime::currentDateTime()) const;
    Q_INVOKABLE QString fileUrlToString(const QUrl &url) const;
    Q_INVOKABLE qint64 toMSecsSinceEpoch(const QDateTime &dateTime) const;
    Q_INVOKABLE QIcon loadOsIcon(const QString &os) const;
};

#endif /* TAILCTL_UTIL_H */