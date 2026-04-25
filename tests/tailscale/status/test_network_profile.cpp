#include <QJsonObject>
#include <QTest>

#include "tailscale/status/network_profile.hpp"

namespace
{

class TestNetworkProfile : public QObject
{
    Q_OBJECT

private slots:
    static void parseFullJson()
    {
        QJsonObject json{
            {QStringLiteral("MagicDNSName"), QStringLiteral("alice.example.ts.net")},
            {QStringLiteral("DomainName"), QStringLiteral("example.ts.net")},
            {QStringLiteral("DisplayName"), QStringLiteral("Example Network")},
        };

        NetworkProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.magicDnsName(), QStringLiteral("alice.example.ts.net"));
        QCOMPARE(profile.domainName(), QStringLiteral("example.ts.net"));
        QCOMPARE(profile.displayName(), QStringLiteral("Example Network"));
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        NetworkProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.magicDnsName(), QString{});
        QCOMPARE(profile.domainName(), QString{});
        QCOMPARE(profile.displayName(), QString{});
    }

    static void updateOverwritesPreviousValues()
    {
        QJsonObject json1{{QStringLiteral("DomainName"), QStringLiteral("a.ts.net")}};
        QJsonObject json2{{QStringLiteral("DomainName"), QStringLiteral("b.ts.net")}};

        NetworkProfile profile;
        profile.updateFromJson(json1);
        QCOMPARE(profile.domainName(), QStringLiteral("a.ts.net"));

        profile.updateFromJson(json2);
        QCOMPARE(profile.domainName(), QStringLiteral("b.ts.net"));
    }
};

}

QTEST_GUILESS_MAIN(TestNetworkProfile)
#include "test_network_profile.moc"
