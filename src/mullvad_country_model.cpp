#include "mullvad_country_model.hpp"

MullvadCountryModel::MullvadCountryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MullvadCountryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mCountries.size();
}
QHash<int, QByteArray> MullvadCountryModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{CountryCode, "countryCode"}, {CountryName, "countryName"}};
    return roles;
}
QVariant MullvadCountryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QStringLiteral("Invalid index");
    }
    switch (role) {
    case CountryCode:
        return mCountries.at(index.row()).first;
    case CountryName:
        return mCountries.at(index.row()).second;
    default:
        return QStringLiteral("Invalid role");
    }
}

void MullvadCountryModel::update(const QMap<QString, QString> &countries)
{
    QList<QPair<QString, QString>> newCountries;
    for (auto it = countries.cbegin(); it != countries.cend(); ++it) {
        if (it->isEmpty()) {
            continue;
        }
        newCountries.append({it.key(), it.value()});
    }

    if (newCountries == mCountries) {
        return;
    }

    beginResetModel();
    mCountries = newCountries;
    endResetModel();
}