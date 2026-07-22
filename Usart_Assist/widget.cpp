#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    Widget::PortNameInit();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::PortNameInit(void)
{
    //const QSeriaPortInfo &port = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &ports , QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(ports);
        if(serial.open(QIODevice::ReadOnly))
        {
            m_PortName.append(serial.portName());
            serial.close();
        }
    }

    for(qsizetype i = 0; i < m_PortName.size(); ++i)
    {
        ui->comboBox_PortName->addItem(m_PortName.at(i));
    }
}

void Widget::on_Button_Open_clicked()
{
    m_pSerialPort = new QSerialPort(this);
    m_pSerialPort->setPortName(ui->comboBox_PortName->currentText());
    m_pSerialPort->setBaudRate(ui->comboBox_Baudrate->currentText().toInt()); //convert string to int
    //set Databits
    switch(ui->comboBox_Databits->currentIndex())
    {
        case 0:
            m_pSerialPort->setDataBits(QSerialPort::Data5);
            break;
        case 1:
            m_pSerialPort->setDataBits(QSerialPort::Data6);
            break;
        case 2:
            m_pSerialPort->setDataBits(QSerialPort::Data7);
            break;
        case 3:
            m_pSerialPort->setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
    }
    //set Stopbits
    switch(ui->comboBox_Stopbits->currentIndex())
    {
        case 0:
            m_pSerialPort->setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            m_pSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
            break;
        case 2:
            m_pSerialPort->setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
    }
    //set Checkbits
    switch(ui->comboBox_CheckBits->currentIndex())
    {
        case 0:
        m_pSerialPort->

    }


    ui->label_PortState->setText("Serial Port Opened!");
}

