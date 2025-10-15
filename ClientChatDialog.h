#ifndef CLIENTCHATDIALOG_H
#define CLIENTCHATDIALOG_H

#include <QWidget>

namespace Ui {
class ClientChatDialog;
}
class QShortcut;

class ClientChatDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatDialog(QWidget *parent = nullptr);
    ~ClientChatDialog();

public slots:
    void displayMessage(const QString& sender, const QString& message, const QString& priority);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onSendButtonClicked();
    void toggleWindowVisibility();

private:
    void setupShortcuts();
    void sendMessageToMaster(const QString& message);

    Ui::ClientChatDialog *ui;
    QShortcut* m_f10Shortcut;
};

#endif // CLIENTCHATDIALOG_H
