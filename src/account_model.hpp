#ifndef KTAILCTL_ACCOUNT_MODEL_HPP
#define KTAILCTL_ACCOUNT_MODEL_HPP

#include "account_data.hpp"
#include <QAbstractListModel>
#include <QtCore/qnamespace.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringview.h>

class AccountModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<AccountData> mAccounts;

    QList<int> updateRow(int row, const AccountData &account);

public:
    enum Roles : int {
        ControlUrlRole = Qt::UserRole + 1,
        IdRole,
        KeyRole,
        NameRole,
        DomainNameRole,
        MagicDnsNameRole,
        UserIdRole,
        UserNameRole,
        LoginNameRole,
        ProfilePictureUrl
    };

    explicit AccountModel(QObject *parent = nullptr);
    virtual ~AccountModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int rolw) const override;

    const QVector<AccountData> &accounts() const;

public slots:
    void update(const QVector<AccountData> &accounts);
};

#endif
