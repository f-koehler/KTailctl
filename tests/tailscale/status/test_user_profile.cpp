#include <QJsonObject>
#include <QTest>

#include "tailscale/status/user_profile.hpp"

namespace
{

class TestUserProfile : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void parseFullJson()
    {
        static constexpr qint64 ExampleUserId = 123456789L;
        QJsonObject json{
            {QStringLiteral("ID"), ExampleUserId},
            {QStringLiteral("LoginName"), QStringLiteral("alice@example.com")},
            {QStringLiteral("DisplayName"), QStringLiteral("Alice Example")},
            {QStringLiteral("ProfilePicURL"), QStringLiteral("https://avatars.example.com/alice.jpg")},
        };

        UserProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.userId(), ExampleUserId);
        QCOMPARE(profile.loginName(), QStringLiteral("alice@example.com"));
        QCOMPARE(profile.displayName(), QStringLiteral("Alice Example"));
        QCOMPARE(profile.profilePicUrl(), QStringLiteral("https://avatars.example.com/alice.jpg"));
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        UserProfile profile;
        profile.updateFromJson(json);

        QCOMPARE(profile.userId(), static_cast<qint64>(0));
        QCOMPARE(profile.loginName(), QString{});
        QCOMPARE(profile.displayName(), QString{});
        QCOMPARE(profile.profilePicUrl(), QString{});
    }

    static void updateOverwritesPreviousValues()
    {
        QJsonObject json1{
            {QStringLiteral("ID"), qint64{1L}},
            {QStringLiteral("LoginName"), QStringLiteral("alice@example.com")},
            {QStringLiteral("DisplayName"), QStringLiteral("Alice")},
        };
        QJsonObject json2{
            {QStringLiteral("ID"), qint64{2L}},
            {QStringLiteral("LoginName"), QStringLiteral("bob@example.com")},
            {QStringLiteral("DisplayName"), QStringLiteral("Bob")},
        };

        UserProfile profile;
        profile.updateFromJson(json1);
        QCOMPARE(profile.loginName(), QStringLiteral("alice@example.com"));

        profile.updateFromJson(json2);
        QCOMPARE(profile.userId(), qint64{2L});
        QCOMPARE(profile.loginName(), QStringLiteral("bob@example.com"));
        QCOMPARE(profile.displayName(), QStringLiteral("Bob"));
    }
};

}

QTEST_GUILESS_MAIN(TestUserProfile)
#include "test_user_profile.moc"
