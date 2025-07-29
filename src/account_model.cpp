#include "account_model.hpp"
#include "account_data.hpp"

AccountModel::AccountModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AccountModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mAccounts.size();
}

QHash<int, QByteArray> AccountModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{
        {ControlUrlRole, "controlUrl"},
        {IdRole, "id"},
        {KeyRole, "key"},
        {NameRole, "name"},
        {DomainNameRole, "domainName"},
        {MagicDnsNameRole, "magicDnsName"},
        {UserIdRole, "userId"},
        {UserNameRole, "userName"},
        {LoginNameRole, "loginName"},
    };
    return roles;
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid inded");
    }
    switch (role) {
    case ControlUrlRole:
        return mAccounts.at(index.row()).controlUrl;
    case IdRole:
        return mAccounts.at(index.row()).id;
    case KeyRole:
        return mAccounts.at(index.row()).key;
    case NameRole:
        return mAccounts.at(index.row()).name;
    case DomainNameRole:
        return mAccounts.at(index.row()).domainName;
    case MagicDnsNameRole:
        return mAccounts.at(index.row()).magicDnsName;
    case UserIdRole:
        return mAccounts.at(index.row()).userId;
    case UserNameRole:
        return mAccounts.at(index.row()).userName;
    case LoginNameRole:
        return mAccounts.at(index.row()).loginName;
    default:
        return QStringLiteral("Invalid role");
    }
}

const QVector<AccountData> &AccountModel::accounts() const
{
    return mAccounts;
}

void AccountModel::update(const QVector<AccountData> &accounts)
{
    if (mAccounts.size() > accounts.size()) {
        beginRemoveRows(QModelIndex(), accounts.size(), mAccounts.size() - 1);
        mAccounts.erase(mAccounts.begin() + accounts.size(), mAccounts.end());
        endRemoveRows();
    }
    for (int i = 0; i < mAccounts.size(); ++i) {
        const QList<int> updatedRoles = updateRow(i, accounts[i]);
        if (!updatedRoles.isEmpty()) {
            emit dataChanged(index(i), index(i), updatedRoles);
        }
    }
    if (mAccounts.size() < accounts.size()) {
        beginInsertRows(QModelIndex(), mAccounts.size(), accounts.size() - 1);
        for (int i = mAccounts.size(); i < accounts.size(); ++i) {
            mAccounts.append(accounts[i]);
        }
        endInsertRows();
    }
}

QList<int> AccountModel::updateRow(int row, const AccountData &account)
{
    QList<int> result;
    AccountData &current = mAccounts[row];
    if (current.controlUrl != account.controlUrl) {
        current.controlUrl = account.controlUrl;
        result.append(ControlUrlRole);
    }
    if (current.id != account.id) {
        current.id = account.id;
        result.append(IdRole);
    }
    if (current.key != account.key) {
        current.key = account.key;
        result.append(KeyRole);
    }
    if (current.name != account.name) {
        current.name = account.name;
        result.append(NameRole);
    }
    if (current.domainName != account.domainName) {
        current.domainName = account.domainName;
        result.append(DomainNameRole);
    }
    if (current.magicDnsName != account.magicDnsName) {
        current.magicDnsName = account.magicDnsName;
        result.append(MagicDnsNameRole);
    }
    if (current.userName != account.userName) {
        current.userName = account.userName;
        result.append(UserNameRole);
    }
    if (current.userId != account.userId) {
        current.userId = account.userId;
        result.append(UserIdRole);
    }
    if (current.loginName != account.loginName) {
        current.loginName = account.loginName;
        result.append(LoginNameRole);
    }
    return result;
}
