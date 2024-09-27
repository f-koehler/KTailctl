#ifndef KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP
#define KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP

#include <KJob>
#include <QList>
#include <QLoggingCategory>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QThread>
#include <QUrl>

Q_DECLARE_LOGGING_CATEGORY(logcat_taildrop_send_job)

class TaildropSendWorker : public QThread
{
    Q_OBJECT

private:
    const QString mTarget;
    const QList<QUrl> mFiles;

protected:
    void run() override;

public:
    explicit TaildropSendWorker(const QString &target, const QList<QUrl> &files, QObject *parent = nullptr);

signals:
    void sendingFinished();
    void filesSent(qsizetype filesSent);
};

class TaildropSendJob : public KJob
{
    Q_OBJECT

private:
    TaildropSendWorker *mWorker;

public:
    explicit TaildropSendJob(const QString &target, const QList<QUrl> &files, QObject *parent = nullptr);
    void start() override;
};

class TaildropSendJobFactory : public QObject
{
    Q_OBJECT
private:
    TaildropSendJobFactory() = default;

public:
    virtual ~TaildropSendJobFactory() = default;
    static TaildropSendJobFactory *instance();

    Q_INVOKABLE static TaildropSendJob *sendFiles(const QString &target, const QList<QUrl> &files);
    Q_INVOKABLE static TaildropSendJob *selectAndSendFiles(const QString &target);
};

#endif /* KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP */
