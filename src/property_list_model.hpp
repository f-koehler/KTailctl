#ifndef KTAILCTL_PROPERTY_LIST_MODEL_HPP
#define KTAILCTL_PROPERTY_LIST_MODEL_HPP

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include <QMetaObject>
#include <QMetaProperty>
#include <QModelIndex>
#include <QPointer>
#include <QPropertyNotifier>
#include <QVariant>
#include <ranges>

enum class PropertyListModelOwnership : bool {
    Owning,
    External,
};

template<typename T, PropertyListModelOwnership O = PropertyListModelOwnership::Owning>
class PropertyListModel : QAbstractListModel
{
public:
    using Type = T;
    static constexpr PropertyListModelOwnership Ownership = O;

private:
    QVector<QPointer<Type>> mItems;
    QHash<int, int> mRoleToPropertyIndex;
    QHash<int, QByteArray> mRoleNames;

    struct NotifierGroup {
        QVector<QPropertyNotifier> notifiers;
    };

    QVector<NotifierGroup> mNotifierGroups;

    void setupMetaRoles()
    {
        const QMetaObject &metaObject = T::staticMetaObject;
        int role = Qt::UserRole + 1;

        for (int i = 0; i < metaObject.propertyCount(); ++i) {
            QMetaProperty property = metaObject.property(i);

            if (!property.isValid()) {
                continue;
            }
            if (!property.isBindable()) {
                if (const QByteArray typeName = property.typeName(); !typeName.contains('*') && !typeName.contains("QObject")) {
                    continue;
                }
            }

            const QByteArray name = property.name();
            mRoleToPropertyIndex.insert(role, i);
            mRoleNames.insert(role, name);
            ++role;
        }
    }

    void setupNotifiersForItem(Type *item, int row)
    {
        NotifierGroup group;
        const QMetaObject &metaObject = item->metaObject();

        for (auto it = mRoleToPropertyIndex.constBegin(); it != mRoleToPropertyIndex.constEnd(); ++it) {
            int role = it.key();
            QMetaProperty property = metaObject.property(role);
            if (!property.isValid()) {
                continue;
            }
            if (!property.isBindable()) {
                continue;
            }
            QUntypedBindable bindable = property.bindable(item);
            group.notifiers.emplace_back([this, row, role]() {
                if (row < 0 || row >= rowCount()) {
                    return;
                }
                emit dataChanged(index(row), index(row), {role});
            });
        }
        mNotifierGroups.push_back(std::move(group));
    }

public:
    explicit PropertyListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        setupMetaRoles();
    }

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override
    {
        if (parent.isValid()) {
            return 0;
        }
        return mItems.size();
    }

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid()) {
            return {};
        }

        int row = index.row();
        if (row < 0 || row >= mItems.size()) {
            return {};
        }

        Type *obj = mItems[row];
        if (obj == nullptr) {
            return {};
        }

        int propertyIndex = mRoleToPropertyIndex.value(role, -1);
        if (propertyIndex < 0) {
            return {};
        }

        const QMetaProperty property = obj->metaObject()->property(propertyIndex);
        QVariant value = property.read(obj);

        if (value.userType() == qMetaTypeId<QObject *>()) {
            auto *qobj = value.value<QObject *>();
            if (qobject_cast<QAbstractItemModel *>(qobj) != nullptr) {
                // return nested model
                return QVariant::fromValue(qobj);
            }
        }

        return value;
    }

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override
    {
        return mRoleNames;
    }

    void addItem(Type *item)
    {
        if (item == nullptr) {
            return;
        }
        const int insertIndex = mItems.size();
        beginInsertRows({}, insertIndex, insertIndex);

        if constexpr (Ownership == PropertyListModelOwnership::Owning) {
            if (item->parent() == nullptr) {
                item->setParent(this);
            }
        }

        mItems.append(item);
        setupNotifiersForItem(item, insertIndex);

        // listen for external destruction of item
        connect(item, &QObject::destroyed, this, [this]() {
            QObject *obj = sender();
            if (obj == nullptr) {
                return;
            }
            Type *t = static_cast<Type *>(obj);
            int i = indexOf(t);
            if (i >= 0) {
                beginRemoveRows({}, i, i);
                mItems.removeAt(i);
                mNotifierGroups.removeAt(i);
                endRemoveRows();
            }
        });

        endInsertRows();
    }

    bool removeItem(int row)
    {
        if (row < 0 || row >= mItems.size()) {
            return false;
        }

        beginRemoveRows({}, row, row);
        if constexpr (Ownership == PropertyListModelOwnership::Owning) {
            mItems[row]->deleteLater();
        }
        mItems.removeAt(row);
        mNotifierGroups.removeAt(row);
        endRemoveRows();
        return true;
    }

    void clear()
    {
        beginResetModel();
        if constexpr (Ownership == PropertyListModelOwnership::Owning) {
            for (auto &item : mItems) {
                if (item != nullptr) {
                    item->deleteLater();
                }
            }
        }
        mItems.clear();
        mNotifierGroups.clear();
        endResetModel();
    }

    [[nodiscard]] int indexOf(Type *item) const
    {
        for (const auto &[index, entry] : std::ranges::views::enumerate(mItems)) {
            if (entry == item) {
                return index;
            }
        }
        return -1;
    }

    [[nodiscard]] Type *at(int row) const
    {
        if (row < 0 || row >= mItems.size()) {
            return nullptr;
        }
        return mItems[row];
    }
};

#endif // KTAILCTL_PROPERTY_LIST_MODEL_HPP