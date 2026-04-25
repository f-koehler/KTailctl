#include <QJsonObject>
#include <QTest>

#include "tailscale/status/login_profile.hpp"

namespace
{
class TestLoginProfile : public QObject
{
    Q_OBJECT

private slots:
    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    static void parseFullJson()
    {
        static constexpr qint64 user_id = 42L;
        QJsonObject userJson{
            {QStringLiteral("ID"), user_id},
            {QStringLiteral("LoginName"), QStringLiteral("alice@example.com")},
            {QStringLiteral("DisplayName"), QStringLiteral("Alice")},
            {QStringLiteral("ProfilePicURL"), QStringLiteral("https://example.com/pic.jpg")},
        };
        QJsonObject networkJson{
            {QStringLiteral("MagicDNSName"), QStringLiteral("alice.example.ts.net")},
            {QStringLiteral("DomainName"), QStringLiteral("example.ts.net")},
            {QStringLiteral("DisplayName"), QStringLiteral("Example Network")},
        };
        QJsonObject json{
            {QStringLiteral("ID"), QStringLiteral("profile:abc123")},
            {QStringLiteral("Name"), QStringLiteral("alice@example.com")},
            {QStringLiteral("Key"), QStringLiteral("key:xyz")},
            {QStringLiteral("NodeID"), QStringLiteral("n:nodeid123")},
            {QStringLiteral("LocalUserID"), QStringLiteral("localuser:1")},
            {QStringLiteral("ControlURL"), QStringLiteral("https://controlplane.tailscale.com")},
            {QStringLiteral("UserProfile"), userJson},
            {QStringLiteral("NetworkProfile"), networkJson},
        };

        LoginProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.id(), QStringLiteral("profile:abc123"));
        QCOMPARE(profile.name(), QStringLiteral("alice@example.com"));
        QCOMPARE(profile.key(), QStringLiteral("key:xyz"));
        QCOMPARE(profile.nodeId(), QStringLiteral("n:nodeid123"));
        QCOMPARE(profile.localUserId(), QStringLiteral("localuser:1"));
        QCOMPARE(profile.controlUrl(), QStringLiteral("https://controlplane.tailscale.com"));

        QVERIFY(profile.userProfile() != nullptr);
        QCOMPARE(profile.userProfile()->userId(), user_id);
        QCOMPARE(profile.userProfile()->loginName(), QStringLiteral("alice@example.com"));
        QCOMPARE(profile.userProfile()->displayName(), QStringLiteral("Alice"));

        QVERIFY(profile.networkProfile() != nullptr);
        QCOMPARE(profile.networkProfile()->domainName(), QStringLiteral("example.ts.net"));
        QCOMPARE(profile.networkProfile()->magicDnsName(), QStringLiteral("alice.example.ts.net"));
    }

    static void parseMissingNestedObjects()
    {
        // No "UserProfile" or "NetworkProfile" key → sub-objects parsed from empty QJsonObject
        QJsonObject json{
            {QStringLiteral("ID"), QStringLiteral("profile:empty")},
            {QStringLiteral("Name"), QStringLiteral("test")},
        };

        LoginProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.id(), QStringLiteral("profile:empty"));
        QCOMPARE(profile.name(), QStringLiteral("test"));

        // Sub-objects are always instantiated in the LoginProfile constructor
        QVERIFY(profile.userProfile() != nullptr);
        QVERIFY(profile.networkProfile() != nullptr);

        // Fields are empty because no JSON was provided for them
        QCOMPARE(profile.userProfile()->loginName(), QString{});
        QCOMPARE(profile.networkProfile()->domainName(), QString{});
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        LoginProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.id(), QString{});
        QCOMPARE(profile.name(), QString{});
        QCOMPARE(profile.key(), QString{});
        QCOMPARE(profile.nodeId(), QString{});
        QCOMPARE(profile.localUserId(), QString{});
        QCOMPARE(profile.controlUrl(), QString{});
    }
};
}

QTEST_GUILESS_MAIN(TestLoginProfile)
#include "test_login_profile.moc"
