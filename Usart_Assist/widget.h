#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>

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
    void PortNameInit(void);


    ~Widget();

private slots:
    void on_Button_Open_clicked();

private:
    Ui::Widget *ui;

    QSerialPort* m_pSerialPort;

    QList<QString> m_PortName;

};
#endif // WIDGET_H
