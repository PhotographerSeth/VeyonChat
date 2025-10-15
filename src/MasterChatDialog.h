#include "ChatPlugin.h"
#include "MasterChatDialog.h"
#include <QAction>
#include <QDebug>

ChatPlugin::ChatPlugin()
    : m_masterChatDialog(nullptr)
{
}

ChatPlugin::~ChatPlugin()
{
}

// Called when the plugin is loaded by Veyon
void ChatPlugin::initialize()
{
    qInfo() << "VeyonChat plugin initialized.";
    // TODO: Initialize client-side components here, if necessary.
    // This part runs in both Master and Client processes.
    // You can check the application type:
    // if (Veyon::Core::Application::instance()->applicationType() == Veyon::Core::Application::Client) {
    //     // Create and setup client chat window
    // }
}

// Called when the plugin is unloaded
void ChatPlugin::cleanup()
{
    qInfo() << "VeyonChat plugin cleaned up.";
}

// --- Feature Implementation for Veyon Master ---

QString ChatPlugin::id() const
{
    return "VeyonChat.MasterFeature";
}

QString ChatPlugin::name() const
{
    return tr("Chat");
}

QString ChatPlugin::description() const
{
    return tr("Open a chat window to communicate with clients.");
}

QIcon ChatPlugin::icon() const
{
    // The icon is loaded from the Qt Resource file (resources.qrc)
    return QIcon(":/icon.svg");
}

QString ChatPlugin::category() const
{
    return tr("Communication");
}

// We want the button to be enabled when at least one client is selected.
Veyon::Master::Feature::FeatureActivation ChatPlugin::activation() const
{
    return Veyon::Master::Feature::FeatureActivation::Selection;
}

// This is the main logic when the user clicks the chat icon.
void ChatPlugin::handleFeatureActivation(const Veyon::Master::FeatureActivationContext& context)
{
    if (!m_masterChatDialog)
    {
        m_masterChatDialog = new MasterChatDialog();
    }

    // Pass the list of selected client IDs to the chat window
    m_masterChatDialog->setSelectedClients(context.selectedClients);
    m_masterChatDialog->show();
    m_masterChatDialog->raise();
    m_masterChatDialog->activateWindow();
}
