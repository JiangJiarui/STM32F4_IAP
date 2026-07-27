#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QFile>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    void WidgetInit(void);


    ~Widget();

private slots:
    void on_Button_Open_clicked();

    void on_Button_Close_clicked();

    void on_Button_Send_clicked();

    void on_pushButton_SendClear_clicked();

    void on_PlainTextEdit_ReceiveWindow_Show();

    void on_pushButton_FileOpen_clicked();

    void on_pushButton_FileSend_clicked();


private:
    Ui::Widget *ui;

    QSerialPort* m_pSerialPort = nullptr;
    QPlainTextEdit* m_pPlainTextEdit_ReceiveWindow = nullptr;

    QList<QString> m_PortName;

    QFile* m_pFile;
    QByteArray m_ReadData;

};
#endif // WIDGET_H
