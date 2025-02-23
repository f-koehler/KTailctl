// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_UTIL_UTIL_CPP
#define KTAILCTL_UTIL_UTIL_CPP
#include "util.hpp"

#include "libktailctl_wrapper.h"
#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QMimeData>
#include <QUrl>
#include <ksystemclipboard.h>

Util::Util(QObject *parent)
    : QObject(parent)
{
}

void setClipboardText(const QString &text)
{
    auto *data = new QMimeData();
    data->setData(QStringLiteral("text/plain"), text.toUtf8());
    KSystemClipboard::instance()->setMimeData(data, QClipboard::Clipboard);
}

QIcon loadOsIcon(QString osname)
{
    osname = osname.toLower();
    if (osname == "linux") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "windows") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "macos") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "android") {
        return QIcon::fromTheme("smartphone");
    }
    if (osname == "ios") {
        return QIcon::fromTheme("smartphone");
    }
    return QIcon::fromTheme("network-vpn");
}

QString getUsername()
{
    QString username = qgetenv("USER");
    if (username.isEmpty()) {
        username = qgetenv("USERNAME");
    }
    return username;
}

QString formatCapacityHumanReadable(long bytes)
{
    static constexpr std::array<const char *, 5> units = {"TiB", "GiB", "MiB", "KiB", "B"};
    static constexpr std::array<qint64, 5> limits = {1LL << 40LL, 1LL << 30LL, 1LL << 20LL, 1LL << 10LL, 1LL};
    for (std::size_t i = 0; i < units.size(); ++i) {
        if (bytes >= limits.at(i)) {
            return QString("%1 %2").arg(static_cast<double>(bytes) / static_cast<double>(limits.at(i)), 0, 'f', 3).arg(units.at(i));
        }
    }
    return "0 B";
}

QString formatSpeedHumanReadable(double bytes_per_second)
{
    static constexpr std::array<const char *, 5> units = {"TiB/s", "GiB/s", "MiB/s", "KiB/s", "B/s"};
    static constexpr std::array<double, 5> limits = {static_cast<double>(1LL << 40LL),
                                                     static_cast<double>(1LL << 30LL),
                                                     static_cast<double>(1LL << 20LL),
                                                     static_cast<double>(1LL << 10LL),
                                                     1.0};
    for (std::size_t i = 0; i < units.size(); ++i) {
        if (bytes_per_second >= limits.at(i)) {
            return QString("%1 %2").arg(bytes_per_second / static_cast<double>(limits.at(i)), 0, 'f', 2).arg(units.at(i));
        }
    }
    return "0 B/s";
}

QString formatDurationHumanReadable(const QDateTime &startTime, const QDateTime &endTime)
{
    // for whatever reason tailscale uses the following special datetime to denote "now"
    static const QDateTime tailscaleNow = QDateTime(QDate(1, 1, 1), QTime(0, 0, 0, 0));

    if ((startTime == tailscaleNow) || (startTime == endTime)) {
        return QStringLiteral("");
    }

    static constexpr std::array<qint64, 6> conversions = {365LL * 30LL * 24LL * 60LL * 60LL * 1000LL,
                                                          30LL * 24LL * 60LL * 60LL * 1000LL,
                                                          24LL * 60LL * 60LL * 1000LL,
                                                          60LL * 60LL * 1000LL,
                                                          60LL * 1000LL,
                                                          1000LL};
    static constexpr std::array<const char *, 6> units = {"year", "month", "day", "hour", "minute", "second"};

    QString result = "";
    if (startTime > endTime) {
        return result;
    }
    qint64 msecs = startTime.msecsTo(endTime);
    if (msecs < 1000) {
        return result;
    }
    for (std::size_t i = 0; i < conversions.size(); ++i) {
        if (msecs >= conversions.at(i)) {
            if (!result.isEmpty()) {
                result += ", ";
            }
            qint64 const count = msecs / conversions.at(i);
            result += QString("%1 %2%3 ").arg(count).arg(units.at(i)).arg(count > 1 ? "s" : "");
            msecs %= conversions.at(i);
        }
    }

    return result;
}

QString fileUrlToString(const QUrl &url)
{
    return url.toString(QUrl::PreferLocalFile);
}

QStringList fileUrlsToStrings(const QList<QUrl> &urls)
{
    QStringList result;
    std::transform(urls.begin(), urls.end(), std::back_inserter(result), fileUrlToString);
    return result;
}

qint64 toMSecsSinceEpoch(const QDateTime &dateTime)
{
    return dateTime.toMSecsSinceEpoch();
}
void openUrl(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

void Util::setClipboardText(const QString &text)
{
    ::setClipboardText(text);
}
QString Util::formatCapacityHumanReadable(long bytes)
{
    return ::formatCapacityHumanReadable(bytes);
}
QString Util::formatSpeedHumanReadable(double bytes_per_second)
{
    return ::formatSpeedHumanReadable(bytes_per_second);
}
QString Util::formatDurationHumanReadable(const QDateTime &startTime, const QDateTime &endTime)
{
    return ::formatDurationHumanReadable(startTime, endTime);
}
QString Util::fileUrlToString(const QUrl &url)
{
    return ::fileUrlToString(url);
}
QStringList Util::fileUrlsToStrings(const QList<QUrl> &urls)
{
    return ::fileUrlsToStrings(urls);
}
qint64 Util::toMSecsSinceEpoch(const QDateTime &dateTime)
{
    return ::toMSecsSinceEpoch(dateTime);
}
QIcon Util::loadOsIcon(const QString &operating_system)
{
    return ::loadOsIcon(operating_system);
}
QString Util::getUsername()
{
    return ::getUsername();
}
void Util::openUrl(const QUrl &url)
{
    ::openUrl(url);
}

#endif /* KTAILCTL_UTIL_UTIL_CPP */
