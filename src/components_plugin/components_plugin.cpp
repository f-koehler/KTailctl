#include <QQmlEngine>
#include <QQmlExtensionPlugin>

class ComponentsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    ComponentsPlugin() = default;
    ~ComponentsPlugin() override = default;
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.fkoehler.KTailctl.Components"));
        qmlRegisterModule(uri, 1, 0);
    }
};

#include "components_plugin.moc"
