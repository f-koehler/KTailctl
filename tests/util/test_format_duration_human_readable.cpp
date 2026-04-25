#include <QDateTime>
#include <QObject>
#include <QTest>

#include "util.hpp"

namespace
{

class TestFormatDurationHumanReadable : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void invalidDateTimeReturnsEmpty()
    {
        QCOMPARE(Util::formatDurationHumanReadable(QDateTime{}), QString{});
    }

    static void recentDateReturnsJustNow()
    {
        // 10 seconds ago —> within the 60-second "just now" window
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(-10);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("just now"));
    }

    static void exactlyOneSecondAgoIsJustNow()
    {
        // 1 second ago —> within the 60-second "just now" window
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(-1);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("just now"));
    }

    static void twoMinutesAgo()
    {
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(-150);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("2 minutes ago"));
    }

    static void fiftyNineMinutesAgo()
    {
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(qint64{-59} * 60);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("59 minutes ago"));
    }

    static void twoHoursAgo()
    {
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(-7200);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("2 hours ago"));
    }

    static void twentyThreeHoursAgo()
    {
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(qint64{-23} * 3600);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("23 hours ago"));
    }

    static void threeDaysAgo()
    {
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addSecs(qint64{-3} * 24 * 3600);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("3 days ago"));
    }

    static void thirtyDaysAgo()
    {
        const QDateTime timestamp = QDateTime::currentDateTimeUtc().addDays(-30);
        QCOMPARE(Util::formatDurationHumanReadable(timestamp), QStringLiteral("30 days ago"));
    }
};

} // namespace

QTEST_GUILESS_MAIN(TestFormatDurationHumanReadable)
#include "test_format_duration_human_readable.moc"
