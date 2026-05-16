#include "string_filter.hpp"
#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsortfilterproxymodel.h>
#include <qstringview.h>
#include <qtmetamacros.h>

StringFilter::StringFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

auto StringFilter::resolveRole() const -> int
{
    if ((sourceModel() == nullptr) || m_filterRoleName.isEmpty()) {
        return -1;
    }
    const QByteArray roleUtf8 = m_filterRoleName.toUtf8();
    const auto roleNames = sourceModel()->roleNames();
    for (auto it = roleNames.cbegin(); it != roleNames.cend(); ++it) {
        if (it.value() == roleUtf8) {
            return it.key();
        }
    }
    return -1;
}

auto StringFilter::filterRoleName() const -> const QString &
{
    return m_filterRoleName;
}

void StringFilter::setFilterRoleName(const QString &name)
{
    if (m_filterRoleName == name) {
        return;
    }
    m_filterRoleName = name;
    beginFilterChange();
    endFilterChange();
    Q_EMIT filterRoleNameChanged();
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

auto StringFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const -> bool
{
    if (m_filterString.isEmpty()) {
        return true;
    }
    const int role = resolveRole();
    if (role < 0) {
        return true;
    }
    const QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
    return sourceModel()->data(idx, role).toString().contains(m_filterString, Qt::CaseInsensitive);
}
