#ifndef KTAILCTL_TESTS_H
#define KTAILCTL_TESTS_H

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

#endif /* KTAILCTL_TESTS_H */
