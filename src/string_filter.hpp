#ifndef KTAILCTL_STRING_FILTER_HPP
#define KTAILCTL_STRING_FILTER_HPP

#include <QList>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

class StringFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QStringList filterRoleNames READ filterRoleNames WRITE setFilterRoleNames NOTIFY filterRoleNamesChanged FINAL)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged FINAL)
    Q_PROPERTY(bool inverted READ inverted WRITE setInverted NOTIFY invertedChanged FINAL)

    QStringList m_filterRoleNames;
    QString m_filterString;
    bool m_inverted = false;

    [[nodiscard]] auto resolveRoles() const -> QList<int>;
    [[nodiscard]] auto valueMatches(const QVariant &value) const -> bool;

public:
    explicit StringFilter(QObject *parent = nullptr);

    [[nodiscard]] auto filterRoleNames() const -> const QStringList &;
    void setFilterRoleNames(const QStringList &names);

    [[nodiscard]] auto filterString() const -> const QString &;
    void setFilterString(const QString &str);

    [[nodiscard]] auto inverted() const -> bool;
    void setInverted(bool inverted);

protected:
    [[nodiscard]] auto filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const -> bool override;

Q_SIGNALS:
    void filterRoleNamesChanged();
    void filterStringChanged();
    void invertedChanged();
};

#endif // KTAILCTL_STRING_FILTER_HPP
