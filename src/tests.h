#ifndef KTAILCTL_TESTS_H
#define KTAILCTL_TESTS_H

#include <QTest>

class TestJSON : public QObject
{
    Q_OBJECT

private slots:
    void toQString();
    void toQStringList();
    void toLocationData();
    void toPeerData();
};

#endif /* KTAILCTL_TESTS_H */
