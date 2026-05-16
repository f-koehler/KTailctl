#ifndef KTAILCTL_PROPERTY_LIST_MODEL_HPP
#define KTAILCTL_PROPERTY_LIST_MODEL_HPP

#include <QByteArray>
#include <QHash>
#include <QMetaObject>
#include <QMetaProperty>
#include <QModelIndex>
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
    QVector<Type *> mItems;
    QHash<int, int> mRoleToPropertyIndex;
    QHash<int, QByteArray> mRoleNames;

public:
    explicit PropertyListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        setupMetaRoles();
    }

    // NOLINTNEXTLINE(portability-template-virtual-member-function)
    [[nodiscard]] auto rowCount([[maybe_unused]] const QModelIndex &parent) const noexcept -> int override
    {
        return mItems.size();
    }

    // NOLINTNEXTLINE(portability-template-virtual-member-function)
    [[nodiscard]] auto data(const QModelIndex &index, const int role) const -> QVariant override
    {
        if (!index.isValid()) {
            return {};
        }

        const int row = index.row();
        if (row < 0 || row >= mItems.size()) {
            return {};
        }

        Type *const item = at(row);
        if (item == nullptr) {
            return {};
        }

        if (role == SelfRole) {
            return QVariant::fromValue(static_cast<QObject *>(item));
        }

        const int propertyIndex = mRoleToPropertyIndex.value(role, -1);
        if (propertyIndex < 0) {
            return {};
        }

        const QMetaProperty property = item->metaObject()->property(propertyIndex);

        return property.read(item);
    }

    // NOLINTNEXTLINE(portability-template-virtual-member-function)
    [[nodiscard]] auto roleNames() const noexcept -> QHash<int, QByteArray> override
    {
        return mRoleNames;
    }

    // --- API ----------------------------------------------------------------

    auto addItem(Type *item) -> int
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
        connect(item, &QObject::destroyed, this, [this, item] -> auto {
            if (const int idx = indexOf(item); idx >= 0) {
                beginRemoveRows({}, idx, idx);
                mItems.removeAt(idx);
                endRemoveRows();
            }
        });

        endInsertRows();
        return row;
    }

    auto removeItem(int row) -> bool
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

    [[nodiscard]] auto at(int row) const -> Type *
    {
        if (row < 0 || row >= mItems.size()) {
            return nullptr;
        }
        return mItems[row]; // NOLINT(cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
    }

    auto indexOf(Type *item) const -> int
    {
        return mItems.indexOf(item);
    }

    [[nodiscard]] auto roleIndexForProperty(const QByteArray &propertyName) const -> int
    {
        for (auto it = mRoleNames.begin(); it != mRoleNames.end(); ++it) {
            if (it.value() == propertyName) {
                return it.key();
            }
        }
        return -1;
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
