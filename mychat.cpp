#include "mychat.h"
#include "logindialog.h"
#include "connectdialog.h"
#include <QDebug>

MyChat::MyChat(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MyChatClass)
{
    ui->setupUi(this);

    // 初始化
    ui->sendMessageButton->setEnabled(false);
    ui->messageInput->setEnabled(false);
    //ui->textMessageReceived->setEnabled(false);
    ui->textMessageReceived->setReadOnly(true);
    ui->connectServerButton->setEnabled(false);
    ui->serverMembersList->clear();

    // 初始化 WebSocket
    socket = new QWebSocket;
    connect(socket, &QWebSocket::connected, this, &MyChat::onConnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &MyChat::onTextMessageReceived);

    // 连接服务器
    //socket->open(QUrl("ws://localhost:8080"));

    // 绑定按钮
    connect(ui->sendMessageButton, &QPushButton::clicked, this, &MyChat::onSendButtonClicked);
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &MyChat::onSendButtonClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &MyChat::login);
    connect(ui->connectServerButton, &QPushButton::clicked, this, &MyChat::connnectToServer);
}

MyChat::~MyChat()
{
    delete ui;
    socket->sendTextMessage("ACT:PUSH:LOGOUT:username:" + this->username);
    socket->close();
    socket->deleteLater();
}

void MyChat::onConnected()
{
    qDebug() << "Connected to server";
    ui->sendMessageButton->setEnabled(true);
    ui->messageInput->setEnabled(true);
    // 向服务器发送登录消息
    QString msg = "ACT:PUSH:LOGIN:username:" + this->username;
    socket->sendTextMessage(msg);
    // 从服务器获取信息
    socket->sendTextMessage("ACT:PULL:MEMBERS");
    socket->sendTextMessage("ACT:PULL:SERVERNAME");
}

void MyChat::onTextMessageReceived(const QString &message)
{
    // 显示接收到的消息
    ui->textMessageReceived->append("Server: " + message);
    QStringList list = message.split(":");
    if (list[0] == "ACT")
    {
        if (list[1] == "CBK")
        {
            if (list[2] == "MEMBERS")
            {
                ui->serverMembersList->clear();
                QStringList members = list[3].split("|");
                for (int i = 0; i < members.length(); i++)
                {
                    ui->serverMembersList->addItem(members[i]);
                }
            }
            if (list[2] == "SERVERNAME")
            {
                ui->label_ServerName->setText(list[3]);
            }
        }
        if (list[1] == "PUSH")
        {
            if (list[2] == "UPDATE_MEMBER")
            {
                // 重新拉取成员列表
                socket->sendTextMessage("ACT:PULL:MEMBERS");
                qDebug() << "Update members list";
            }
        }
    }
    if (list[0] == "MSG")
    {
        if (list[1] == "SER")
        {
            QString user = list[2];
            QString msg = decodeBase64(list[3]);
            ui->textMessageReceived->append(user + ":" + msg);
        }
        //ui->textMessageReceived->append(list[1] + ":" + list[2]);
    }
}

QString MyChat::encodeBase64(const QString& input) {
    // 将 QString 转换为 QByteArray
    QByteArray byteArray = input.toUtf8();

    // 对 QByteArray 进行 Base64 编码
    QByteArray base64 = byteArray.toBase64();

    // 将 QByteArray 转换为 QString
    return QString(base64);
}

QString MyChat::decodeBase64(const QString& input) {
    // 将 QString 转换为 QByteArray
    QByteArray base64 = input.toUtf8();

    // 对 QByteArray 进行 Base64 解码
    QByteArray byteArray = QByteArray::fromBase64(base64);

    // 将 QByteArray 转换为 QString
    return QString::fromUtf8(byteArray);
}


void MyChat::onSendButtonClicked()
{
    // 发送消息
    QString message = ui->messageInput->text();
    if (!message.isEmpty()) {
        // 对消息进行 Base64 编码
        message = encodeBase64(message);
        socket->sendTextMessage("MSG:"+ this->username + ":" + message);
        //ui->textMessageReceived->append("You: " + message);
        ui->messageInput->clear();
    }
}

void MyChat::login()
{
    // 初始化Dialog指针
    loginDialog = new LoginDialog(this);
    loginDialog->setWindowTitle("Login");
    loginDialog->setModal(true);
    //connect(loginDialog, &LoginDialog::loginSuccess, this, &MyChat::handleLoginSuccess);
    connect(loginDialog, SIGNAL(loginSuccess(QString)), this, SLOT(handleLoginSuccess(QString)));
    loginDialog->exec();
}

void MyChat::handleLoginSuccess(const QString &username)
{
    this->username = username;
    qDebug() << "Login as" << this->username;
    ui->connectServerButton->setEnabled(true);
    ui->loginButton->setText("登出");
    connect(ui->loginButton, &QPushButton::clicked, this, &MyChat::logout);
    disconnect(ui->loginButton, &QPushButton::clicked, this, &MyChat::login);
    ui->label_Username->setText(this->username);
}

void MyChat::logout()
{
    qDebug() << "Logout";
    ui->loginButton->setText("登录");
    disconnect(ui->loginButton, &QPushButton::clicked, this, &MyChat::logout);
    connect(ui->loginButton, &QPushButton::clicked, this, &MyChat::login);
    ui->label_Username->setText("");
    this->username = "";
    disconnectFromServer();
    ui->connectServerButton->setEnabled(false);
}

void MyChat::connnectToServer()
{
    //connect_server("127.0.0.1", 8080);
    connectDialog = new ConnectDialog(this);
    connectDialog->setWindowTitle("连接到服务器");
    connectDialog->setModal(true);
    connect(connectDialog, SIGNAL(serverAddress(QString)), this, SLOT(handleServerAddress(QString)));
    //connect(connectDialog, &ConnectDialog::serverAddress, this, &MyChat::handleServerAddress);
    connectDialog->exec();
    ui->connectServerButton->setText("断开连接");
    disconnect(ui->connectServerButton, &QPushButton::clicked, this, &MyChat::connnectToServer);
    connect(ui->connectServerButton, &QPushButton::clicked, this, &MyChat::disconnectFromServer);
}

void MyChat::handleServerAddress(const QString &address)
{
    connect_server(address);
}

void MyChat::connect_server(QString address)
{
    qDebug() << "Connecting to server" << address;
    socket->open(QUrl("ws://" + address));
}

void MyChat::disconnectFromServer()
{
    socket->sendTextMessage("ACT:PUSH:LOGOUT:username:" + this->username);
    socket->close();
    ui->connectServerButton->setText("连接服务器");
    connect(ui->connectServerButton, &QPushButton::clicked, this, &MyChat::connnectToServer);
    disconnect(ui->connectServerButton, &QPushButton::clicked, this, &MyChat::disconnectFromServer);
    ui->sendMessageButton->setEnabled(false);
    ui->messageInput->setEnabled(false);
    ui->messageInput->clear();
    ui->textMessageReceived->clear();
    ui->serverMembersList->clear();
    ui->label_ServerName->setText("");
}