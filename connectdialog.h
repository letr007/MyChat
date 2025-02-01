#include <QDialog>
#include "ui_connectDialog.h"
#include <QString>
#include <QMessageBox>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class connectDialog; }
QT_END_NAMESPACE

class ConnectDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget* parent = nullptr);
    ~ConnectDialog();

signals:
    void serverAddress(const QString &address);

private slots:
    void on_connectButton_clicked();

private:
    Ui::connectDialog* ui; // 指向 UI 文件的指针
};

