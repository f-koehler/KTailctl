#ifndef TAILCTL_TAILDROP_HELPER_HPP
#define TAILCTL_TAILDROP_HELPER_HPP

#include <QLocalServer>
#include <QProcess>
#include <QtCore>

class TaildropHelper : public QObject
{
    Q_OBJECT

private:
    QLocalServer m_server;
    QProcess m_process;
    bool m_enabled;
    QString m_executable;
    QString m_directory;
    QString m_strategy;

public slots:
    void restartProcess();
    void stopProcess();
    void onNewConnection();

public:
    TaildropHelper(bool enabled, const QString &executable, const QString &directory, const QString &strategy, QObject *parent = nullptr);
};

#endif /* TAILCTL_TAILDROP_HELPER_HPP */
