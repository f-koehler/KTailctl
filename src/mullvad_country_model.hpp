#ifndef KTAILCTL_TAILSCALE_MULLVAD_COUNTRY_MODEL_HPP
#define KTAILCTL_TAILSCALE_MULLVAD_COUNTRY_MODEL_HPP

#include <QAbstractListModel>
#include <QMap>
#include <QString>
#include <QVector>

class MullvadCountryModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<QPair<QString, QString>> mCountries;

public:
    enum Roles : int {
        CountryCode = Qt::UserRole + 1,
        CountryName,
    };

    explicit MullvadCountryModel(QObject *parent = nullptr);
    virtual ~MullvadCountryModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void update(const QMap<QString, QString> &countries);
};

#endif /* KTAILCTL_TAILSCALE_MULLVAD_COUNTRY_MODEL_HPP */
