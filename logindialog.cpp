#include "logindialog.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::loginDialog)
{
    ui->setupUi(this); // 加载 UI 文件

    connect(ui->pushButton, &QPushButton::clicked, this, &LoginDialog::on_loginButton_clicked);
}

LoginDialog::~LoginDialog() {
    delete ui; // 释放 UI
}

void LoginDialog::on_loginButton_clicked()
{
    QString username = ui->lineEdit->text();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名不能为空");
        return;
    }
    emit loginSuccess(username);
    //qDebug() << "signal emitted";
    close();
}