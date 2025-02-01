#include <QDialog>
#include "ui_loginDialog.h"
#include <QString>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class loginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

signals:
    // 登录成功信号,传递用户名
    void loginSuccess(const QString &username);

private slots:
    void on_loginButton_clicked();

private:
    Ui::loginDialog *ui; // 指向 UI 文件的指针
};

