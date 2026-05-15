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

    [[nodiscard]] int resolveRole() const;

public:
    explicit StringFilter(QObject *parent = nullptr);

    [[nodiscard]] QString filterRoleName() const;
    void setFilterRoleName(const QString &name);

    [[nodiscard]] QString filterString() const;
    void setFilterString(const QString &str);

protected:
    [[nodiscard]] bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

Q_SIGNALS:
    void filterRoleNameChanged();
    void filterStringChanged();
};

#endif // KTAILCTL_STRING_FILTER_HPP
