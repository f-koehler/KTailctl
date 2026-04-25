#include <QJsonArray>
#include <QJsonObject>
#include <QTest>

#include "tailscale/status/exit_node_status.hpp"

namespace
{

class TestExitNodeStatus : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void parseFullJson()
    {
        QJsonObject json{
            {QStringLiteral("ID"), QStringLiteral("nodekey:abc123")},
            {QStringLiteral("Online"), true},
            {QStringLiteral("TailscaleIPs"),
             QJsonArray{
                 QStringLiteral("100.64.0.1"),
                 QStringLiteral("fd7a:115c::1"),
             }},
        };

        ExitNodeStatus status;
        status.updateFromJson(json);

        QCOMPARE(status.id(), QStringLiteral("nodekey:abc123"));
        QCOMPARE(status.isOnline(), true);
        QCOMPARE(status.tailscaleIps().size(), 2);
        QCOMPARE(status.tailscaleIps().at(0), QStringLiteral("100.64.0.1"));
        QCOMPARE(status.tailscaleIps().at(1), QStringLiteral("fd7a:115c::1"));
    }

    static void parseOfflineNode()
    {
        QJsonObject json{
            {QStringLiteral("ID"), QStringLiteral("nodekey:def456")},
            {QStringLiteral("Online"), false},
        };

        ExitNodeStatus status;
        status.updateFromJson(json);

        QCOMPARE(status.id(), QStringLiteral("nodekey:def456"));
        QCOMPARE(status.isOnline(), false);
        QVERIFY(status.tailscaleIps().isEmpty());
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        ExitNodeStatus status;
        status.updateFromJson(json);

        QCOMPARE(status.id(), QString{});
        QCOMPARE(status.isOnline(), false);
        QVERIFY(status.tailscaleIps().isEmpty());
    }
};

}

QTEST_GUILESS_MAIN(TestExitNodeStatus)
#include "test_exit_node_status.moc"
