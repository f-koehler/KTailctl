#ifndef KTAILCTL_DATA_TEST_DATA_H
#define KTAILCTL_DATA_TEST_DATA_H

#include <QTest>

class TestJSON : public QObject
{
    Q_OBJECT

private slots:
    static void toQString();
    static void toQStringList();
    static void toLocationData();
    static void toPeerData();
};

#endif /* KTAILCTL_DATA_TEST_DATA_H */
