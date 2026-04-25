#include "property_list_model.hpp"
#include <QAbstractItemModelTester>
#include <QBindable>
#include <QProperty>
#include <QSignalSpy>
#include <QTest>

namespace
{
// Simple QObject with two Q_PROPERTYs to use as model items
class SimpleItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label BINDABLE bindableLabel)
    Q_PROPERTY(int value READ value BINDABLE bindableValue)

    QProperty<QString> mLabel;
    QProperty<int> mValue{0};

public:
    explicit SimpleItem(const QString &label, const int value, QObject *parent = nullptr)
        : QObject(parent)
        , mLabel(label)
        , mValue(value)
    {
    }

    QString label() const
    {
        return {mLabel};
    }
    int value() const
    {
        return mValue;
    }
    QBindable<QString> bindableLabel()
    {
        return {&mLabel};
    }
    QBindable<int> bindableValue()
    {
        return {&mValue};
    }
};

// ReSharper disable once CppRedundantTemplateArguments
using OwningModel = PropertyListModel<SimpleItem, PropertyListModelOwnership::Owning>;
using ExternalModel = PropertyListModel<SimpleItem, PropertyListModelOwnership::External>;

class TestPropertyListModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void emptyModelHasZeroRows()
    {
        const OwningModel model;
        QCOMPARE(model.rowCount({}), 0);
    }

    static void addItemIncreasesRowCount()
    {
        OwningModel model;
        model.addItem(new SimpleItem(QStringLiteral("a"), 1));
        QCOMPARE(model.rowCount({}), 1);
        model.addItem(new SimpleItem(QStringLiteral("b"), 2));
        QCOMPARE(model.rowCount({}), 2);
    }

    static void addNullItemIsNoOp()
    {
        OwningModel model;
        const int result = model.addItem(nullptr);
        QCOMPARE(result, -1);
        QCOMPARE(model.rowCount({}), 0);
    }

    static void roleNamesContainPropertyNames()
    {
        const OwningModel model;
        const QHash<int, QByteArray> roles = model.roleNames();

        QVERIFY(roles.values().contains(QByteArrayLiteral("self")));
        QVERIFY(roles.values().contains(QByteArrayLiteral("label")));
        QVERIFY(roles.values().contains(QByteArrayLiteral("value")));
    }

    static void dataReturnsCorrectPropertyValues()
    {
        static constexpr int value = 42;
        OwningModel model;
        model.addItem(new SimpleItem(QStringLiteral("hello"), value));

        const QHash<int, QByteArray> roles = model.roleNames();
        const int labelRole = roles.key(QByteArrayLiteral("label"));
        const int valueRole = roles.key(QByteArrayLiteral("value"));

        const QModelIndex idx = model.index(0, 0);
        QCOMPARE(model.data(idx, labelRole).toString(), QStringLiteral("hello"));
        QCOMPARE(model.data(idx, valueRole).toInt(), value);
    }

    static void selfRoleReturnsQObjectPointer()
    {
        OwningModel model;
        auto *item = new SimpleItem(QStringLiteral("x"), 0);
        model.addItem(item);

        constexpr int selfRole = PropertyListModel<SimpleItem>::SelfRole;
        const QModelIndex idx = model.index(0, 0);
        const QVariant selfData = model.data(idx, selfRole);

        QCOMPARE(selfData.value<QObject *>(), item);
    }

    static void invalidIndexReturnsInvalidVariant()
    {
        OwningModel model;
        model.addItem(new SimpleItem(QStringLiteral("x"), 0));

        QVERIFY(!model.data(QModelIndex{}, Qt::DisplayRole).isValid());
        QVERIFY(!model.data(model.index(5, 0), Qt::DisplayRole).isValid());
        QVERIFY(!model.data(model.index(-1, 0), Qt::DisplayRole).isValid());
    }

    static void removeItemDecreasesRowCount()
    {
        OwningModel model;
        model.addItem(new SimpleItem(QStringLiteral("a"), 1));
        model.addItem(new SimpleItem(QStringLiteral("b"), 2));

        const bool removed = model.removeItem(0);
        QVERIFY(removed);
        QCOMPARE(model.rowCount({}), 1);
    }

    static void removeItemOutOfRangeFails()
    {
        OwningModel model;
        QVERIFY(!model.removeItem(0));
        QVERIFY(!model.removeItem(-1));
    }

    static void clearRemovesAllItems()
    {
        OwningModel model;
        model.addItem(new SimpleItem(QStringLiteral("a"), 1));
        model.addItem(new SimpleItem(QStringLiteral("b"), 2));
        model.addItem(new SimpleItem(QStringLiteral("c"), 3));

        model.clear();
        QCOMPARE(model.rowCount({}), 0);
    }

    static void indexOfFindsItem()
    {
        OwningModel model;
        auto *item1 = new SimpleItem(QStringLiteral("a"), 1);
        auto *item2 = new SimpleItem(QStringLiteral("b"), 2);
        model.addItem(item1);
        model.addItem(item2);

        QCOMPARE(model.indexOf(item1), 0);
        QCOMPARE(model.indexOf(item2), 1);
    }

    static void atReturnsCorrectItem()
    {
        static constexpr int value1 = 10;
        static constexpr int value2 = 20;

        OwningModel model;
        auto *item1 = new SimpleItem(QStringLiteral("first"), value1);
        auto *item2 = new SimpleItem(QStringLiteral("second"), value2);
        model.addItem(item1);
        model.addItem(item2);

        QCOMPARE(model.at(0), item1);
        QCOMPARE(model.at(1), item2);
        QCOMPARE(model.at(-1), nullptr);
        QCOMPARE(model.at(99), nullptr);
    }

    static void roleIndexForPropertyFindsCorrectRole()
    {
        const OwningModel model;
        const int labelRole = model.roleIndexForProperty(QByteArrayLiteral("label"));
        const int valueRole = model.roleIndexForProperty(QByteArrayLiteral("value"));
        const int bogus = model.roleIndexForProperty(QByteArrayLiteral("doesnotexist"));

        QVERIFY(labelRole != -1);
        QVERIFY(valueRole != -1);
        QCOMPARE(bogus, -1);
        QVERIFY(labelRole != valueRole);
    }

    static void externalOwnershipDoesNotDeleteItems()
    {
        SimpleItem item(QStringLiteral("ext"), 0);
        {
            ExternalModel model;
            model.addItem(&item);
            QCOMPARE(model.rowCount({}), 1);
            // Model goes out of scope — must NOT delete &item
        }
        // If we reach here without a crash, the item was not deleted by the model
        QCOMPARE(item.label(), QStringLiteral("ext"));
    }

    static void destroyedItemIsRemovedFromModel()
    {
        // When an item owned externally is destroyed, the model should auto-remove the row
        ExternalModel model;
        auto *item = new SimpleItem(QStringLiteral("tmp"), 0);
        model.addItem(item);
        QCOMPARE(model.rowCount({}), 1);

        delete item;
        QCOMPARE(model.rowCount({}), 0);
    }

    static void modelPassesAbstractItemModelTester()
    {
        OwningModel model;
        const QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::Fatal);
        model.addItem(new SimpleItem(QStringLiteral("a"), 1));
        model.addItem(new SimpleItem(QStringLiteral("b"), 2));
        model.removeItem(0);
        model.clear();
    }
};
}

QTEST_GUILESS_MAIN(TestPropertyListModel)
#include "test_property_list_model.moc"
