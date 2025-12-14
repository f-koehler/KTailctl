#ifndef KTAILCTL_PROPERTY_LIST_MODEL_HPP
#define KTAILCTL_PROPERTY_LIST_MODEL_HPP

#include <QByteArray>
#include <QHash>
#include <QMetaObject>
#include <QMetaProperty>
#include <QModelIndex>
#include <QPointer>
#include <QPropertyNotifier>
#include <QVariant>

enum class PropertyListModelOwnership : bool {
    Owning,
    External,
};

template<typename T, PropertyListModelOwnership O = PropertyListModelOwnership::Owning>
class PropertyListModel : public QAbstractListModel
{
public:
    using Type = T;
    static constexpr PropertyListModelOwnership Ownership = O;
    static constexpr int SelfRole = Qt::UserRole;

    static_assert(std::is_base_of_v<QObject, T>, "PropertyListModel<T> requires T to derive from QObject");

private:
    QVector<QPointer<Type>> mItems;
    QHash<int, int> mRoleToPropertyIndex;
    QHash<int, QByteArray> mRoleNames;

public:
    explicit PropertyListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        setupMetaRoles();
    }

    [[nodiscard]] int rowCount([[maybe_unused]] const QModelIndex &parent) const noexcept override
    {
        return mItems.size();
    }

    [[nodiscard]] QVariant data(const QModelIndex &index, const int role) const override
    {
        if (!index.isValid()) {
            return {};
        }

        const int row = index.row();
        if (row < 0 || row >= mItems.size()) {
            return {};
        }

        const QPointer<Type> &item = mItems[row];
        if (!item) {
            return {};
        }

        if (role == SelfRole) {
            return QVariant::fromValue(static_cast<QObject *>(item.data()));
        }

        const int propertyIndex = mRoleToPropertyIndex.value(role, -1);
        if (propertyIndex < 0) {
            return {};
        }

        const QMetaProperty property = item->metaObject()->property(propertyIndex);

        return property.read(item);
    }

    [[nodiscard]] QHash<int, QByteArray> roleNames() const noexcept override
    {
        return mRoleNames;
    }

    // --- API ----------------------------------------------------------------

    int addItem(Type *item)
    {
        if (item == nullptr) {
            return -1;
        }

        const int row = mItems.size();
        beginInsertRows({}, row, row);

        if constexpr (Ownership == PropertyListModelOwnership::Owning) {
            if (!item->parent()) {
                item->setParent(this);
            }
        }

        mItems.append(item);

        // Remove row automatically if item is destroyed externally
        connect(item, &QObject::destroyed, this, [this, item]() {
            if (const int idx = indexOf(item); idx >= 0) {
                beginRemoveRows({}, idx, idx);
                mItems.removeAt(idx);
                endRemoveRows();
            }
        });

        endInsertRows();
        return row;
    }

    bool removeItem(int row)
    {
        if (row < 0 || row >= mItems.size()) {
            return false;
        }

        beginRemoveRows({}, row, row);

        if constexpr (Ownership == PropertyListModelOwnership::Owning) {
            if (mItems[row]) {
                mItems[row]->deleteLater();
            }
        }

        mItems.removeAt(row);
        endRemoveRows();
        return true;
    }

    void clear()
    {
        beginResetModel();

        if constexpr (Ownership == PropertyListModelOwnership::Owning) {
            for (const auto &item : mItems) {
                if (item) {
                    item->deleteLater();
                }
            }
        }

        mItems.clear();
        endResetModel();
    }

    Type *at(int row) const
    {
        if (row < 0 || row >= mItems.size()) {
            return nullptr;
        }
        return mItems[row];
    }

    int indexOf(Type *item) const
    {
        return mItems.indexOf(item);
    }

private:
    void setupMetaRoles()
    {
        const QMetaObject &metaObject = T::staticMetaObject;
        int role = Qt::UserRole;

        mRoleNames.insert(role, "self");
        mRoleToPropertyIndex.insert(role, -1);
        ++role;

        for (int i = metaObject.propertyOffset(); i < metaObject.propertyCount(); ++i) {
            const QMetaProperty property = metaObject.property(i);

            if (!property.isValid()) {
                continue;
            }

            mRoleToPropertyIndex.insert(role, i);
            mRoleNames.insert(role, property.name());
            ++role;
        }
    }
};

#endif // KTAILCTL_PROPERTY_LIST_MODEL_HPP
