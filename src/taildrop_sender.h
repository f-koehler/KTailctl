#ifndef KTAILCTL_TAILDROP_SENDER_H
#define KTAILCTL_TAILDROP_SENDER_H

#include <KJob>
#include <QPointer>
#include <QThread>

class TaildropSendThread : public QThread
{
    Q_OBJECT

private:
    QString mTarget;
    QStringList mFiles;

public:
    TaildropSendThread(const QString &target, const QStringList &files, QObject *parent = nullptr);

protected:
    void run() override;
};

class TaildropSendJob : public KJob
{
    Q_OBJECT

private:
    QPointer<TaildropSendThread> mThread;

public:
    TaildropSendJob(const QString &target, const QStringList &files, QObject *parent = nullptr);

    static TaildropSendJob *selectAndSendFiles(const QString &target);

public slots:
    void start() override;
};

class QmlTaildropSender : public QObject
{
    Q_OBJECT
public:
    explicit QmlTaildropSender(QObject *parent = nullptr);
    Q_INVOKABLE void selectAndSendFiles(const QString &target);
};

#endif
