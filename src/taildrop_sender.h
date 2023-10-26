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
    quint64 mBytesSent;
    quint64 mBytesTotal;
    quint64 mCurrentFileBytesSent;

public:
    TaildropSendThread(QString target, const QStringList &files, QObject *parent = nullptr);

protected:
    void run() override;
};

class TaildropSendJob : public KJob
{
    Q_OBJECT

private:
    QPointer<TaildropSendThread> mThread;

public:
    explicit TaildropSendJob(const QString &target, const QStringList &files, QObject *parent = nullptr);
    virtual ~TaildropSendJob() = default;

    static TaildropSendJob *selectAndSendFiles(const QString &target);
    static TaildropSendJob *sendFiles(const QString &target, const QStringList &files);

public slots:
    void start() override;
};

class TaildropSender : public QObject
{
    Q_OBJECT
public:
    explicit TaildropSender(QObject *parent = nullptr);
    Q_INVOKABLE static void selectAndSendFiles(const QString &target);
    Q_INVOKABLE static void sendFiles(const QString &target, const QStringList &files);
};

#endif
