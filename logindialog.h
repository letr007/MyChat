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
    // ��¼�ɹ��ź�,�����û���
    void loginSuccess(const QString &username);

private slots:
    void on_loginButton_clicked();

private:
    Ui::loginDialog *ui; // ָ�� UI �ļ���ָ��
};

