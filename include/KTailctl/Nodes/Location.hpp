#ifndef KTAILCTL_NODES_LOCATION_HPP
#define KTAILCTL_NODES_LOCATION_HPP

#include <QObject>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class Location : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString countryCode READ getCountryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString countryName READ getCountryName NOTIFY countryNameChanged)
    Q_PROPERTY(QString cityCode READ getCountryCode NOTIFY cityCodeChanged)
    Q_PROPERTY(QString cityName READ getCountryName NOTIFY cityNameChanged)
    Q_PROPERTY(double longitude READ getLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(double latitude READ getLatitude NOTIFY latitudeChanged)
    QML_ELEMENT

private:
    QString mCountryCode;
    QString mCountryName;
    QString mCityCode;
    QString mCityName;
    double mLongitude;
    double mLatitude;

public:
    explicit Location(QObject *parent = nullptr);
    virtual ~Location() = default;

    const QString &getCountryCode() const;
    const QString &getCountryName() const;
    const QString &getCityCode() const;
    const QString &getCityName() const;
    double getLongitude() const;
    double getLatitude() const;

signals:
    void countryCodeChanged(const QString &);
    void countryNameCity(const QString &);
    void cityCodeChanged(const QString &);
    void cityNameCity(const QString &);
    void longitudeChanged(double);
    void latitudeChanged(double);
};

#endif /* KTAILCTL_KTAILCTL_NODES_LOCATION_HPP */
