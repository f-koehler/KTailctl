#include "tailscale_client.h"

TailscaleClient::TailscaleClient(QObject *parent)
    : QObject(parent)
    , m_action_manage_tailscale("org.fkoehler.tailctl.managetailscale")
{
    m_action_manage_tailscale.setHelperId("org.fkoehler.tailctl");
}

void TailscaleClient::setTailscaleExecutable(const QString &executable)
{
    QVariantMap args;
    args["operation"] = "setTailscaleExecutable";
    args["executable"] = executable;
    m_action_manage_tailscale.setArguments(args);
    auto job = m_action_manage_tailscale.execute();
    if (!job->exec()) {
        qWarning() << "KAuth error" << job->error() << ": " << job->errorText();
    }
}
void TailscaleClient::setTaildropEnabled(bool enabled)
{
    QVariantMap args;
    args["operation"] = "setTaildropEnabled";
    args["enabled"] = enabled;
    m_action_manage_tailscale.setArguments(args);
    m_action_manage_tailscale.execute()->exec();
}
void TailscaleClient::setTaildropDirectory(const QString &directory)
{
    QVariantMap args;
    args["operation"] = "setTaildropDirectory";
    args["directory"] = directory;
    m_action_manage_tailscale.setArguments(args);
    m_action_manage_tailscale.execute()->exec();
}
void TailscaleClient::setTaildropStrategy(const TailctlConfig::EnumTaildropStrategy::type &strategy)
{
    QVariantMap args;
    args["operation"] = "setTaildropStrategy";
    switch (strategy) {
    case TailctlConfig::EnumTaildropStrategy::Overwrite:
        args["strategy"] = "overwrite";
        break;
    case TailctlConfig::EnumTaildropStrategy::Skip:
        args["strategy"] = "skip";
        break;
    default:
        args["strategy"] = "rename";
        break;
    }
    m_action_manage_tailscale.setArguments(args);
    m_action_manage_tailscale.execute()->exec();
}