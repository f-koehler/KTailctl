#ifndef KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP
#define KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP

#include <KJob>
#include <QList>
#include <QLoggingCategory>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QUrl>

Q_DECLARE_LOGGING_CATEGORY(logcat_taildrop_send_job)

class TaildropSendJob : public KJob
{
    Q_OBJECT

private:
    QString mTarget;
    QList<QUrl> mFiles;

    void sendFiles();

public:
    explicit TaildropSendJob(const QString &target, const QList<QUrl> &files, QObject *parent = nullptr);
    void start() override;

    bool isFinished() const
    {
        return KJob::isFinished();
    }
};

class TaildropSendJobs : public QObject
{
    Q_OBJECT

private:
    QMap<QString, QList<TaildropSendJob *>> mPeerJobs;
    QMutex mMutex;

    explicit TaildropSendJobs(QObject *parent = nullptr);

public:
    void sendFiles(const QString &target, const QList<QUrl> &files);
    Q_INVOKABLE void selectAndSendFiles(const QString &target);

    static TaildropSendJobs *instance();

public slots:
    void cleanupJobs();
};

#endif /* KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP */
