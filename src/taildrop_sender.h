#ifndef KTAILCTL_TAILDROP_SENDER_H
#define KTAILCTL_TAILDROP_SENDER_H

#include <QThread>

class TaildropSendWorker;

class TaildropSender : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int workerCount READ workerCount NOTIFY workerCountChanged)

private:
    QString mTarget;
    QList<TaildropSendWorker *> mWorkers;

public:
    TaildropSender(const QString &target, QObject *parent = nullptr);
    int workerCount();

signals:
    void workerCountChanged(int);

public slots:
    void pruneWorkers();
    Q_INVOKABLE void sendFiles(const QStringList &);
};

class TaildropSendWorker : public QThread
{
    Q_OBJECT

private:
    QString mTarget;
    QStringList mFiles;

protected:
    void run() override;

public:
    TaildropSendWorker(const QString &target, const QStringList &files, QObject *parent = nullptr);
};

#endif /* KTAILCTL_TAILDROP_SENDER_H */
