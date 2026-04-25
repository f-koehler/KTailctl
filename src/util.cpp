#include "util.hpp"

QString Util::formatDurationHumanReadable(const QDateTime &dateTime)
{
    static constexpr qint64 seconds_per_minute = 60UL;
    static constexpr qint64 seconds_per_hour = 3600UL;
    static constexpr qint64 seconds_per_day = 86400UL;
    if (!dateTime.isValid()) {
        return QString{};
    }
    const qint64 secs = dateTime.secsTo(QDateTime::currentDateTimeUtc());
    if (secs <seconds_per_minute ) {
        return QStringLiteral("just now");
    }
    if (secs < seconds_per_hour) {
        return QString::number(secs / seconds_per_minute) + QStringLiteral(" minutes ago");
    }
    if (secs < seconds_per_day) {
        return QString::number(secs / seconds_per_hour) + QStringLiteral(" hours ago");
    }
    return QString::number(secs / seconds_per_day) + QStringLiteral(" days ago");
}

void Util::setClipboardText(const QString &text)
{
    // KSystemClipboard takes ownership of the allocated QMimeData (see: https://api.kde.org/ksystemclipboard.html#setMimeData)
    // ReSharper disable once CppDFAMemoryLeak
    auto *data = new QMimeData();
    data->setData(QStringLiteral("text/plain"), text.toUtf8());
    KSystemClipboard::instance()->setMimeData(data, QClipboard::Clipboard);
}
