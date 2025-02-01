#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_mychat.h"
#include <QtWebSockets/QWebSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MyChat; }
QT_END_NAMESPACE


class MyChat : public QMainWindow
{
    Q_OBJECT

public:
    MyChat(QWidget *parent = nullptr);
    ~MyChat();

private slots:
    void onConnected();
    void onTextMessageReceived(const QString& message);
    void onSendButtonClicked();
    void login();
    void logout();
    void handleLoginSuccess(const QString &username);
    void handleServerAddress(const QString &address);
    void connnectToServer();
    void disconnectFromServer();

    

private:
    Ui::MyChatClass *ui;
    QWebSocket *socket;
    QString username = "匿名";
    QDialog *loginDialog;
    QDialog *connectDialog;

    void connect_server(QString address);
    QString encodeBase64(const QString& input);
    QString decodeBase64(const QString& input);
};
