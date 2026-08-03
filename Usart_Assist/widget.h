#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QEventLoop>
#include <QTimer>


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

    void on_Button_SendClear_clicked();

    void PlainTextEdit_ReceiveWindow_Show();

    void on_pushButton_FileOpen_clicked();

    void on_pushButton_FileSend_clicked();


signals:
    void USART_ACK_Received();
    void USART_NACK_Received();
    void USART_Info_Received();


private:
    Ui::Widget *ui;

    QSerialPort* m_pSerialPort = nullptr;
    QPlainTextEdit* m_pPlainTextEdit_ReceiveWindow = nullptr;

    QList<QString> m_PortName;

    QFile* m_pFile;
    QByteArray m_ReadData;

    uint8_t ack_flag;
    uint8_t nack_flag;


};
#endif // WIDGET_H
