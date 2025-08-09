#include <KTailctl/Nodes/TailscaleNodeProperties.hpp>

TailscaleNodeProperties::TailscaleNodeProperties(const QString &id, QObject *parent)
    : NodeProperties(id, parent)
{
}
TailscaleNodeProperties::TailscaleNodeProperties(QString &&id, QObject *parent)
    : NodeProperties(id, parent)
{
}

const QStringList &TailscaleNodeProperties::getTags() const
{
    return mTags;
}
bool TailscaleNodeProperties::isExitNode() const
{
    return mIsExitNode;
}
const QString &TailscaleNodeProperties::getDnsName() const
{
    return mDnsName;
}
const QString &TailscaleNodeProperties::getOperatingSystem() const
{
    return mOperatingSystem;
}
const QDateTime &TailscaleNodeProperties::getLastSeenTime() const
{
    return mLastSeenTime;
}
const QDateTime &TailscaleNodeProperties::getCreationTime() const
{
    return mCreationTime;
}