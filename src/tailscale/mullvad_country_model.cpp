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
    if (countries.size() == mCountries.size()) {
        return;
    }
    beginResetModel();
    for (auto it = countries.cbegin(); it != countries.cend(); ++it) {
        mCountries.append({it.key(), it.value()});
    }
    endResetModel();
}

// void MullvadCountryModel::update(const QVector<Peer> &mullvadNodes)
// {
//     Location *location = nullptr;
//     for (Peer *node : mullvadNodes) {
//         location = node->location();
//         if (location == nullptr) {
//             continue;
//         }
//         mCountriesNew.insert(location->countryCode(), location->country());
//     }

//     if (mCountries.size() > mCountriesNew.size()) {
//         beginRemoveRows(QModelIndex(), mCountriesNew.size(), mCountries.size() - 1);
//         mCountries.erase(mCountries.begin() + mCountriesNew.size(), mCountries.end());
//         endRemoveRows();
//     }

//     if (!mCountries.isEmpty()) {
//         int index = 0;
//         for (auto it = mCountriesNew.cbegin(); it != mCountriesNew.cend(); ++it) {
//             if (mCountries[index].first != mCountriesNew[it.key()]) {
//                 mCountries[index].first = it.key();
//                 mCountries[index].second = it.value();
//                 emit dataChanged(createIndex(index, 0), createIndex(index, 0));
//             }
//             ++index;
//         }
//     }

//     if (mCountries.size() < mCountriesNew.size()) {
//         for (auto it = mCountriesNew.cbegin() + mCountries.size(); it != mCountriesNew.cend(); ++it) {
//             beginInsertRows(QModelIndex(), mCountries.size(), mCountries.size());
//             mCountries.append({it.key(), it.value()});
//             endInsertRows();
//         }
//     }
// }