#ifndef KTAILCTL_TEST_DATA_HPP
#define KTAILCTL_TEST_DATA_HPP

#include <QTest>

class TestJSON : public QObject
{
    Q_OBJECT

private slots:
    static void toQString();
    static void toQStringList();
    static void toPeerData();
};

#endif /* KTAILCTL_TEST_DATA_HPP */
