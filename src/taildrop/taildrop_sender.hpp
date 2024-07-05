#ifndef KTAILCTL_TAILDROP_TAILDROP_SENDER_HPP
#define KTAILCTL_TAILDROP_TAILDROP_SENDER_HPP

#include <KJob>
#include <QLoggingCategory>
#include <QPointer>
#include <QThread>

Q_DECLARE_LOGGING_CATEGORY(logcat_taildrop_sender)

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

#endif /* KTAILCTL_TAILDROP_TAILDROP_SENDER_HPP */
