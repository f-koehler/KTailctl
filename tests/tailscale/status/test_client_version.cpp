#include <QJsonObject>
#include <QTest>

#include "tailscale/status/client_version.hpp"

namespace
{

class TestClientVersion : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void parseRunningLatest()
    {
        QJsonObject json{
            {QStringLiteral("RunningLatest"), true},
            {QStringLiteral("Version"), QStringLiteral("1.80.0")},
            {QStringLiteral("UrgentSecurityUpdate"), false},
            {QStringLiteral("Notify"), false},
            {QStringLiteral("NotifyUrl"), QString{}},
            {QStringLiteral("NotifyText"), QString{}},
        };

        ClientVersion client_version;
        client_version.updateFromJson(json);

        QCOMPARE(client_version.runningLatest(), true);
        QCOMPARE(client_version.latestVersion(), QStringLiteral("1.80.0"));
        QCOMPARE(client_version.urgentSecurityUpdate(), false);
        QCOMPARE(client_version.notify(), false);
        QCOMPARE(client_version.notifyUrl(), QString{});
        QCOMPARE(client_version.notifyText(), QString{});
    }

    static void parseUpdateAvailable()
    {
        QJsonObject json{
            {QStringLiteral("RunningLatest"), false},
            {QStringLiteral("Version"), QStringLiteral("1.82.0")},
            {QStringLiteral("UrgentSecurityUpdate"), true},
            {QStringLiteral("Notify"), true},
            {QStringLiteral("NotifyUrl"), QStringLiteral("https://pkgs.tailscale.com/stable/")},
            {QStringLiteral("NotifyText"), QStringLiteral("Critical security update available")},
        };

        ClientVersion client_version;
        client_version.updateFromJson(json);

        QCOMPARE(client_version.runningLatest(), false);
        QCOMPARE(client_version.latestVersion(), QStringLiteral("1.82.0"));
        QCOMPARE(client_version.urgentSecurityUpdate(), true);
        QCOMPARE(client_version.notify(), true);
        QCOMPARE(client_version.notifyUrl(), QStringLiteral("https://pkgs.tailscale.com/stable/"));
        QCOMPARE(client_version.notifyText(), QStringLiteral("Critical security update available"));
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        ClientVersion client_version;
        client_version.updateFromJson(json);

        QCOMPARE(client_version.runningLatest(), false);
        QCOMPARE(client_version.latestVersion(), QString{});
        QCOMPARE(client_version.urgentSecurityUpdate(), false);
        QCOMPARE(client_version.notify(), false);
    }
};

}

QTEST_GUILESS_MAIN(TestClientVersion)
#include "test_client_version.moc"
