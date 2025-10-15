#ifndef CHATPLUGIN_H
#define CHATPLUGIN_H

#include <QObject>
#include <VeyonMaster/Feature.h>
#include <VeyonCore/Plugin.h>

class MasterChatDialog;

// This class is the entry point for the plugin.
// It implements the Feature interface to add a button to the Veyon Master toolbar.
class ChatPlugin : public QObject,
                   public Veyon::Core::Plugin,
                   public Veyon::Master::Feature
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VeyonPlugin_iid FILE "plugin.json")
    Q_INTERFACES(Veyon::Core::Plugin Veyon::Master::Feature)

public:
    ChatPlugin();
    ~ChatPlugin() override;

    // Veyon::Core::Plugin interface
    void initialize() override;
    void cleanup() override;

    // Veyon::Master::Feature interface
    QString id() const override;
    QString name() const override;
    QString description() const override;
    QIcon icon() const override;
    QString category() const override;
    FeatureActivation activation() const override;

public slots:
    // This slot is called when the user clicks the "Chat" button in Veyon Master.
    void handleFeatureActivation(const Veyon::Master::FeatureActivationContext& context) override;

private:
    MasterChatDialog* m_masterChatDialog;
};

#endif // CHATPLUGIN_H