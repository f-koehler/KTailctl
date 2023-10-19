#include <QClipboard>
#include <QMimeData>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>
#include <ksystemclipboard.h>

class Util : public QObject
{
    Q_OBJECT
public:
    void setClipboardText(const QString &text)
    {
        auto *data = new QMimeData();
        data->setData(QStringLiteral("text/plain"), text.toUtf8());
        KSystemClipboard::instance()->setMimeData(data, QClipboard::Clipboard);
    }
};

class ComponentsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    ComponentsPlugin() = default;
    ~ComponentsPlugin() = default;
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.fkoehler.KTailctl.Components"));
        qmlRegisterModule(uri, 1, 0);
        // Util util;
        // qmlRegisterSingletonInstance("org.fkoehler.KTailctl.Components.Util", 1, 0, "Util", &util);
        // qmlRegisterSingletonInstance("org.fkoehler.KTailctl.Components.Util", 1, 0, "Util", &util);
    }
};

#include "componentsplugin.moc"