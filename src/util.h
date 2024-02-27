// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#ifndef KTAILCTL_UTIL_H
#define KTAILCTL_UTIL_H

#include <QDateTime>
#include <QIcon>
#include <QObject>
#include <QString>

#include <tuple>

void setClipboardText(const QString &text);
QString formatCapacityHumanReadable(long bytes);
QString formatSpeedHumanReadable(double bytes_per_second);
QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to = QDateTime::currentDateTime());
QString fileUrlToString(const QUrl &url);
QStringList fileUrlsToStrings(const QList<QUrl> &urls);
qint64 toMSecsSinceEpoch(const QDateTime &dateTime);
QIcon loadOsIcon(QString osname);
void setExitNode(const QString &node);
void unsetExitNode();
void setIconTheme(const QString &theme);

class Util : public QObject
{
    Q_OBJECT

public:
    explicit Util(QObject *parent = nullptr);
    virtual ~Util() = default;

    Q_INVOKABLE static void setClipboardText(const QString &text);
    Q_INVOKABLE static QString formatCapacityHumanReadable(long bytes);
    Q_INVOKABLE static QString formatSpeedHumanReadable(double bytes_per_second);
    Q_INVOKABLE static QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to = QDateTime::currentDateTime());
    Q_INVOKABLE static QString fileUrlToString(const QUrl &url);
    Q_INVOKABLE static QStringList fileUrlsToStrings(const QList<QUrl> &urls);
    Q_INVOKABLE static qint64 toMSecsSinceEpoch(const QDateTime &dateTime);
    Q_INVOKABLE static QIcon loadOsIcon(const QString &operating_system);
    Q_INVOKABLE static void setExitNode(const QString &node);
    Q_INVOKABLE static void unsetExitNode();
    Q_INVOKABLE static void setIconTheme(const QString &theme);
};

#endif /* KTAILCTL_UTIL_H */
