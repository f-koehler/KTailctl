#ifndef KTAILCTL_LOCATION_H
#define KTAILCTL_LOCATION_H

#include "location_data.h"

class Location : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString country READ country NOTIFY countryChanged)
    Q_PROPERTY(QString countryCode READ countryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(QString cityCode READ cityCode NOTIFY cityCodeChanged)
    Q_PROPERTY(int priority READ priority NOTIFY priorityChanged)

private:
    LocationData mData;

public:
    explicit Location(QObject *parent = nullptr);
    virtual ~Location() = default;

    const QString &country() const;
    const QString &countryCode() const;
    const QString &city() const;
    const QString &cityCode() const;
    int priority() const;
    const LocationData &locationData() const;

signals:
    void countryChanged(const QString &country);
    void countryCodeChanged(const QString &countryCode);
    void cityChanged(const QString &city);
    void cityCodeChanged(const QString &cityCode);
    void priorityChanged(int priority);

public slots:
    void update(LocationData &newData);
};

#endif /* KTAILCTL_LOCATION_H */
