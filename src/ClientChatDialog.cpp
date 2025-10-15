#include "ClientChatDialog.h"
#include "ui_ClientChatDialog.h" // Generated from a .ui file
#include <QShortcut>
#include <QKeyEvent>
#include <QDateTime>
#include <QComputerInfo>
#include <QDebug>

// Placeholder for UI definition
namespace Ui {
    class ClientChatDialog {
    public:
        QTextEdit* chatHistoryTextEdit;
        QLineEdit* messageLineEdit;
        QPushButton* sendButton;
        QVBoxLayout* mainLayout;

        void setupUi(QWidget *ClientChatDialog) {
            mainLayout = new QVBoxLayout(ClientChatDialog);
            chatHistoryTextEdit = new QTextEdit(ClientChatDialog);
            chatHistoryTextEdit->setReadOnly(true);
            messageLineEdit = new QLineEdit(ClientChatDialog);
            messageLineEdit->setPlaceholderText("Reply to Master...");
            sendButton = new QPushButton("Send", ClientChatDialog);

            QHBoxLayout* bottomLayout = new QHBoxLayout();
            bottomLayout->addWidget(messageLineEdit);
            bottomLayout->addWidget(sendButton);

            mainLayout->addWidget(new QLabel("Chat with Master:"));
            mainLayout->addWidget(chatHistoryTextEdit);
            mainLayout->addLayout(bottomLayout);

            ClientChatDialog->setLayout(mainLayout);
            ClientChatDialog->setWindowTitle("Veyon Client Chat");
        }
    };
}

ClientChatDialog::ClientChatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientChatDialog)
{
    ui->setupUi(this);
    setupShortcuts();

    // Make the window stay on top
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);

    connect(ui->sendButton, &QPushButton::clicked, this, &ClientChatDialog::onSendButtonClicked);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &ClientChatDialog::onSendButtonClicked);
}

ClientChatDialog::~ClientChatDialog()
{
    delete ui;
}

void ClientChatDialog::setupShortcuts()
{
    // F10 to show/hide. Since this will be triggered by Veyon's service,
    // we need to register it globally or handle the RPC to show the window.
    m_f10Shortcut = new QShortcut(QKeySequence(Qt::Key_F10), this);
    connect(m_f10Shortcut, &QShortcut::activated, this, &ClientChatDialog::toggleWindowVisibility);
}

void ClientChatDialog::toggleWindowVisibility()
{
    if (isVisible()) {
        hide();
    } else {
        show();
        activateWindow();
    }
}

void ClientChatDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hide();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void ClientChatDialog::displayMessage(const QString& sender, const QString& message, const QString& priority)
{
    // Make the window pop up for urgent messages
    if(priority == "Urgent" || priority == "Announcement") {
        show();
        activateWindow();
    }

    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formattedMessage = QString("[%1] %2: %3").arg(timestamp, sender, message);

    // Style the message based on priority
    if(priority == "Urgent") {
        ui->chatHistoryTextEdit->append(QString("<b style='color:red;'>%1</b>").arg(formattedMessage));
    } else if (priority == "Announcement") {
        ui->chatHistoryTextEdit->append(QString("<b style='color:blue;'>%1</b>").arg(formattedMessage));
    } else {
        ui->chatHistoryTextEdit->append(formattedMessage);
    }
}

void ClientChatDialog::onSendButtonClicked()
{
    QString message = ui->messageLineEdit->text().trimmed();
    if(message.isEmpty()) {
        return;
    }

    QString clientName = QComputerInfo::machineHostName();

    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formattedMessage = QString("[%1] %2 (You): %3").arg(timestamp, clientName, message);
    ui->chatHistoryTextEdit->append(formattedMessage);

    sendMessageToMaster(message);

    ui->messageLineEdit->clear();
}


void ClientChatDialog::sendMessageToMaster(const QString& message)
{
    // This is the other half of the communication logic.
    qInfo() << "Sending message to master:" << message;

    // PSEUDO-CODE for Veyon communication
    /*
    Veyon::Core::RemoteProcedureCall rpc("VeyonChat", "receiveClientMessage");
    rpc.addArgument("message", message);
    rpc.addArgument("senderId", Veyon::Core::Application::instance()->client()->id());
    Veyon::Client::ClientCore::instance()->serviceConnection()->executeRemoteProcedureCall(rpc);
    */
}
