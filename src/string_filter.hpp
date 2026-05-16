#ifndef KTAILCTL_STRING_FILTER_HPP
#define KTAILCTL_STRING_FILTER_HPP

#include <QSortFilterProxyModel>
#include <QtQmlIntegration/qqmlintegration.h>

class StringFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString filterRoleName READ filterRoleName WRITE setFilterRoleName NOTIFY filterRoleNameChanged FINAL)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged FINAL)

    QString m_filterRoleName;
    QString m_filterString;

    [[nodiscard]] auto resolveRole() const -> int;

public:
    explicit StringFilter(QObject *parent = nullptr);

    [[nodiscard]] auto filterRoleName() const -> QString;
    void setFilterRoleName(const QString &name);

    [[nodiscard]] auto filterString() const -> QString;
    void setFilterString(const QString &str);

protected:
    [[nodiscard]] auto filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const -> bool override;

Q_SIGNALS:
    void filterRoleNameChanged();
    void filterStringChanged();
};

#endif // KTAILCTL_STRING_FILTER_HPP
