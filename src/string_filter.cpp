#include "string_filter.hpp"

#include <QAbstractItemModel>
#include <QByteArray>
#include <QHash>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaType>
#include <QModelIndex>
#include <QVariant>
#include <Qt>
#include <algorithm>
#include <qtmetamacros.h>

StringFilter::StringFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

auto StringFilter::resolveRoles() const -> QList<int>
{
    QList<int> roles;
    if ((sourceModel() == nullptr) || m_filterRoleNames.isEmpty()) {
        return roles;
    }
    const auto roleNames = sourceModel()->roleNames();
    for (const QString &name : m_filterRoleNames) {
        const QByteArray roleUtf8 = name.toUtf8();
        for (auto it = roleNames.cbegin(); it != roleNames.cend(); ++it) {
            if (it.value() == roleUtf8) {
                roles.append(it.key());
                break;
            }
        }
    }
    return roles;
}

auto StringFilter::valueMatches(const QVariant &value) const -> bool
{
    // Nested object roles (e.g. a peer's location) carry their searchable text
    // in string sub-properties, so match against those rather than the object
    // itself. This lets a search hit e.g. a Mullvad node's city/country name.
    if (auto *object = value.value<QObject *>(); object != nullptr) {
        const QMetaObject *meta = object->metaObject();
        for (int i = 0; i < meta->propertyCount(); ++i) {
            const QVariant property = meta->property(i).read(object);
            if (property.typeId() == QMetaType::QString && property.toString().contains(m_filterString, Qt::CaseInsensitive)) {
                return true;
            }
        }
        return false;
    }
    // QStringList roles (e.g. tags): match if any element contains the filter string.
    if (value.typeId() == QMetaType::QStringList) {
        const QStringList list = value.toStringList();
        return std::ranges::any_of(list, [&](const QString &item) {
            return item.contains(m_filterString, Qt::CaseInsensitive);
        });
    }
    return value.toString().contains(m_filterString, Qt::CaseInsensitive);
}

auto StringFilter::filterRoleNames() const -> const QStringList &
{
    return m_filterRoleNames;
}

void StringFilter::setFilterRoleNames(const QStringList &names)
{
    if (m_filterRoleNames == names) {
        return;
    }
    m_filterRoleNames = names;
    beginFilterChange();
    endFilterChange();
    Q_EMIT filterRoleNamesChanged();
}

auto StringFilter::filterString() const -> const QString &
{
    return m_filterString;
}

void StringFilter::setFilterString(const QString &str)
{
    if (m_filterString == str) {
        return;
    }
    m_filterString = str;
    beginFilterChange();
    endFilterChange();
    Q_EMIT filterStringChanged();
}

auto StringFilter::inverted() const -> bool
{
    return m_inverted;
}

void StringFilter::setInverted(bool inverted)
{
    if (m_inverted == inverted) {
        return;
    }
    m_inverted = inverted;
    beginFilterChange();
    endFilterChange();
    Q_EMIT invertedChanged();
}

auto StringFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const -> bool
{
    if (m_filterString.isEmpty()) {
        return true;
    }
    const QList<int> roles = resolveRoles();
    if (roles.isEmpty()) {
        return true;
    }
    const QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
    const bool matches = std::ranges::any_of(roles, [&](const int role) -> bool {
        return valueMatches(sourceModel()->data(idx, role));
    });
    return m_inverted ? !matches : matches;
}
