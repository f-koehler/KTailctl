#include "mullvad_country_model.hpp"
#include <QTest>
#include <QMap>
#include <QString>

class TestMullvadCountryModel : public QObject
{
    Q_OBJECT

private slots:
    void testUnboundedGrowth()
    {
        MullvadCountryModel model;
        
        QMap<QString, QString> countries40;
        for (int i = 0; i < 40; ++i) {
            countries40.insert(QString::number(i), QStringLiteral("Country ") + QString::number(i));
        }

        QMap<QString, QString> countries41 = countries40;
        countries41.insert(QStringLiteral("40"), QStringLiteral("Country 40"));

        QMap<QString, QString> countries39 = countries40;
        countries39.remove(QStringLiteral("39"));

        // Initial setup
        model.update(countries40);
        QCOMPARE(model.rowCount(), 40);

        // Simulate fluctuating node counts during regular refresh cycles
        // Unpatched code will grow geometrically with every call
        for (int i = 0; i < 300; ++i) {
            model.update(countries41);
            QCOMPARE(model.rowCount(), 41);
            
            model.update(countries39);
            QCOMPARE(model.rowCount(), 39);

            model.update(countries40);
            QCOMPARE(model.rowCount(), 40);
        }
    }
};

QTEST_MAIN(TestMullvadCountryModel)
#include "test_mullvad_country_model.moc"
