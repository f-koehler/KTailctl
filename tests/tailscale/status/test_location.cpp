#include <QJsonObject>
#include <QTest>

#include "tailscale/status/location.hpp"

class TestLocation : public QObject
{
    Q_OBJECT

private slots:
    static void parseFullJson()
    {
        static constexpr double latitude = 50.1109;
        static constexpr double longitude = 8.6821;
        static constexpr int priority = 100;

        QJsonObject json{
            {QStringLiteral("Country"), QStringLiteral("Germany")},
            {QStringLiteral("CountryCode"), QStringLiteral("DE")},
            {QStringLiteral("City"), QStringLiteral("Frankfurt")},
            {QStringLiteral("CityCode"), QStringLiteral("fra")},
            {QStringLiteral("Latitude"), latitude},
            {QStringLiteral("Longitude"), longitude},
            {QStringLiteral("Priority"), priority},
        };

        Location loc;
        loc.updateFromJson(json);

        QCOMPARE(loc.country(), QStringLiteral("Germany"));
        QCOMPARE(loc.countryCode(), QStringLiteral("DE"));
        QCOMPARE(loc.city(), QStringLiteral("Frankfurt"));
        QCOMPARE(loc.cityCode(), QStringLiteral("fra"));
        QCOMPARE(loc.latitude(), latitude);
        QCOMPARE(loc.longitude(), longitude);
        QCOMPARE(loc.priority(), priority);
    }

    static void parseMissingFields()
    {
        QJsonObject json;
        Location loc;
        loc.updateFromJson(json);

        QCOMPARE(loc.country(), QString{});
        QCOMPARE(loc.countryCode(), QString{});
        QCOMPARE(loc.city(), QString{});
        QCOMPARE(loc.cityCode(), QString{});
        QCOMPARE(loc.latitude(), 0.0);
        QCOMPARE(loc.longitude(), 0.0);
        QCOMPARE(loc.priority(), 0);
    }

    static void updateOverwritesPreviousValues()
    {
        static constexpr int old_priority = 10;
        static constexpr int new_priority = 20;
        QJsonObject json1{
            {QStringLiteral("Country"), QStringLiteral("Germany")},
            {QStringLiteral("CountryCode"), QStringLiteral("DE")},
            {QStringLiteral("Priority"), old_priority},
        };
        QJsonObject json2{
            {QStringLiteral("Country"), QStringLiteral("Sweden")},
            {QStringLiteral("CountryCode"), QStringLiteral("SE")},
            {QStringLiteral("Priority"), new_priority},
        };

        Location loc;
        loc.updateFromJson(json1);
        QCOMPARE(loc.country(), QStringLiteral("Germany"));
        QCOMPARE(loc.priority(), old_priority);

        loc.updateFromJson(json2);
        QCOMPARE(loc.country(), QStringLiteral("Sweden"));
        QCOMPARE(loc.countryCode(), QStringLiteral("SE"));
        QCOMPARE(loc.priority(), new_priority);
    }

    static void negativePriority()
    {
        static constexpr int priority = -5;
        QJsonObject json{{QStringLiteral("Priority"), priority}};
        Location loc;
        loc.updateFromJson(json);
        QCOMPARE(loc.priority(), priority);
    }
};

QTEST_GUILESS_MAIN(TestLocation)
#include "test_location.moc"
