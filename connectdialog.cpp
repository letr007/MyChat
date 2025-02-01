#include "connectdialog.h"

ConnectDialog::ConnectDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::connectDialog)
{
    ui->setupUi(this); // 加载 UI 文件

    connect(ui->pushButton, &QPushButton::clicked, this, &ConnectDialog::on_connectButton_clicked);
}

ConnectDialog::~ConnectDialog() {
    delete ui; // 释放 UI
}

void ConnectDialog::on_connectButton_clicked() {
    QString ip = ui->lineEdit->text();
    QString port = ui->lineEdit_2->text();

    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "错误", "IP或端口不能为空");
        return;
    }

    bool ok;
    int portNum = port.toInt(&ok);
    if (!ok || portNum <= 0 || portNum > 65535) {
        QMessageBox::warning(this, "错误", "端口格式错误");
        return;
    }

    QHostAddress addressTest;
    if (!addressTest.setAddress(ip)) {
        QMessageBox::warning(this, "错误", "IP地址格式错误");
        return;
    }

    QString formattedAddress;
    if (addressTest.protocol() == QAbstractSocket::IPv6Protocol) {
        formattedAddress = "[" + ip + "]:" + port;  // IPv6 需要加 []
    }
    else {
        formattedAddress = ip + ":" + port;  // IPv4 直接拼接
    }

    emit serverAddress(formattedAddress);
    qDebug() << "Signal emitted:" << formattedAddress;
    close();
}