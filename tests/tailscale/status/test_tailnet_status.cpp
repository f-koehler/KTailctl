#include <QJsonObject>
#include <QTest>

#include "tailscale/status/tailnet_status.hpp"

namespace
{

class TestTailnetStatus : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void parseFullJson()
    {
        QJsonObject json{
            {QStringLiteral("Name"), QStringLiteral("example.ts.net")},
            {QStringLiteral("MagicDnsSuffix"), QStringLiteral(".example.ts.net")},
            {QStringLiteral("MagicDnsEnabled"), true},
        };

        TailnetStatus status;
        status.updateFromJson(json);

        QCOMPARE(status.name(), QStringLiteral("example.ts.net"));
        QCOMPARE(status.magicDnsSuffix(), QStringLiteral(".example.ts.net"));
        QCOMPARE(status.magicDnsEnabled(), true);
    }

    static void parseMagicDnsDisabled()
    {
        QJsonObject json{
            {QStringLiteral("Name"), QStringLiteral("corp.ts.net")},
            {QStringLiteral("MagicDnsSuffix"), QStringLiteral(".corp.ts.net")},
            {QStringLiteral("MagicDnsEnabled"), false},
        };

        TailnetStatus status;
        status.updateFromJson(json);

        QCOMPARE(status.name(), QStringLiteral("corp.ts.net"));
        QCOMPARE(status.magicDnsEnabled(), false);
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        TailnetStatus status;
        status.updateFromJson(json);

        QCOMPARE(status.name(), QString{});
        QCOMPARE(status.magicDnsSuffix(), QString{});
        QCOMPARE(status.magicDnsEnabled(), false);
    }
};

}

QTEST_GUILESS_MAIN(TestTailnetStatus)
#include "test_tailnet_status.moc"
