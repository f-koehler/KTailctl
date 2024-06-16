#ifndef KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP
#define KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP

#include <QByteArray>
#include <QLoggingCategory>
#include <QMutex>
#include <QQueue>
#include <QString>
#include <QUrl>
#include <atomic>

#include <KJob>

Q_DECLARE_LOGGING_CATEGORY(logcat_taildrop_send_job)

class TaildropSendJob : public KJob
{
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)

private:
    const QByteArray mTarget;
    QQueue<QUrl> mFiles;
    QMutex mMutex;
    std::atomic<bool> mRunning = false;

    void sendFiles();

    explicit TaildropSendJob(const QString &target, QObject *parent = nullptr);

public:
    Q_INVOKABLE TaildropSendJob *addFiles(const QList<QUrl> &files);
    void start() override;
    bool isRunning() const;

    static TaildropSendJob *getJob(const QString &target);

public slots:
    void clear();

signals:
    void isRunningChanged(bool running);
};

#endif /* KTAILCTL_TAILDROP_TAILDROP_SEND_JOB_HPP */
