#ifndef KTAILCTL_USER_H
#define KTAILCTL_USER_H

#include "user_data.h"
#include <QObject>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 id READ id NOTIFY idChanged)
    Q_PROPERTY(QString loginName READ loginName NOTIFY loginNameChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString profilePicUrl READ profilePicUrl NOTIFY profilePicUrlChanged)

private:
    UserData mData;

public:
    explicit User(QObject *parent = nullptr);

    const quint64 &id() const;
    const QString &loginName() const;
    const QString &displayName() const;
    const QString &profilePicUrl() const;

signals:
    void idChanged(const quint64 &id);
    void loginNameChanged(const QString &loginName);
    void displayNameChanged(const QString &displayName);
    void profilePicUrlChanged(const QString &profilePicUrl);

public slots:
    void update(UserData &newData);
};

#endif /* KTAILCTL_USER_H */
