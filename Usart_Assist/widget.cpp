#include "widget.h"
#include "ui_widget.h"

#include "UserFile.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    Widget::WidgetInit();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::WidgetInit(void)
{
    /* TextLabel Init */
    ui->label_PortState->setText("Serial Port Closed");
    ui->label_FileState->setText("choose an bin File");

    /* Portname Init  */
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

    /* ReceiveWindow Init */
    if(!ui->plainTextEdit_ReceiveWindow->isReadOnly())
    {
        ui->plainTextEdit_ReceiveWindow->setReadOnly(true);
    }
    ui->plainTextEdit_ReceiveWindow->setLineWrapMode(QPlainTextEdit::NoWrap);

    ui->label_IPaddr->clear();
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
    switch(ui->comboBox_ParityBits->currentIndex())
    {
        case 0:
            m_pSerialPort->setParity(QSerialPort::NoParity);
            break;
        case 1:
            m_pSerialPort->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            m_pSerialPort->setParity(QSerialPort::OddParity);
            break;
        case 3:
            m_pSerialPort->setParity(QSerialPort::SpaceParity);
            break;
        case 4:
            m_pSerialPort->setParity(QSerialPort::MarkParity);
            break;
        default:
            break;
    }

    switch(ui->comboBox_Flowcontrol->currentIndex())
    {
        case 0:
            m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
            break;
        case 1:
            m_pSerialPort->setFlowControl(QSerialPort::HardwareControl);
            break;
        case 2:
            m_pSerialPort->setFlowControl(QSerialPort::SoftwareControl);
            break;
        default:
            break;
    }


    connect(m_pSerialPort, SIGNAL(readyRead()), this,
            SLOT(PlainTextEdit_ReceiveWindow_Show()));

    if(m_pSerialPort->open(QIODeviceBase::ReadWrite))
    {
        ui->label_PortState->setText("Serial Port Opened");
        ui->Button_Open->setEnabled(false);
        ui->Button_Close->setEnabled(true);
        ui->comboBox_Baudrate->setEnabled(false);
        ui->comboBox_Databits->setEnabled(false);
        ui->comboBox_Flowcontrol->setEnabled(false);
        ui->comboBox_ParityBits->setEnabled(false);
        ui->comboBox_Stopbits->setEnabled(false);
        ui->comboBox_PortName->setEnabled(false);
    }
    else
    {
        ui->label_PortState->setText("Serial Port Opened Failed!");
    }


}


void Widget::on_Button_Close_clicked()
{
    if(m_pSerialPort == nullptr)
    {
        return;
    }
    m_pSerialPort->close();
    delete m_pSerialPort;
    m_pSerialPort = nullptr;

    ui->label_PortState->setText("Serial Port Closed");
    ui->Button_Close->setEnabled(false);
    ui->Button_Open->setEnabled(true);
    ui->comboBox_Baudrate->setEnabled(true);
    ui->comboBox_Databits->setEnabled(true);
    ui->comboBox_Flowcontrol->setEnabled(true);
    ui->comboBox_ParityBits->setEnabled(true);
    ui->comboBox_Stopbits->setEnabled(true);
    ui->comboBox_PortName->setEnabled(true);

}



void Widget::on_Button_Send_clicked()
{
    if((m_pSerialPort != nullptr) && (m_pSerialPort->isOpen()))
    {
        QString StringToSend = ui->plainTextEdit_SendWindow->toPlainText();
        QByteArray BytesToSend = StringToSend.toUtf8();

        if(!m_pSerialPort->write(BytesToSend))
        {
            qDebug("SerialPort Send Error");
            return;
        }

    }
}



void Widget::on_Button_SendClear_clicked()
{
    if(!ui->plainTextEdit_SendWindow->toPlainText().isEmpty())
    {
        ui->plainTextEdit_SendWindow->clear();
    }
    else
    {
        return;
    }
}



void Widget::PlainTextEdit_ReceiveWindow_Show()
{
    QByteArray ReceivedData;
    QString strBuf;
    ReceivedData = m_pSerialPort->readAll();
    if(!ReceivedData.isEmpty())
    {
        if(ReceivedData.at(0) == (char)0xaa)
        {
            qDebug() << "ACK Received";
            ack_flag = 1;
            emit USART_ACK_Received();
            return;
        }
        else if(ReceivedData.at(0) == (char)0xff)
        {
            qDebug() << "NACK Received";
            nack_flag = 1;
            emit USART_ACK_Received();
            return;
        }
        else
        {
            strBuf += QString(ReceivedData);
        }
    }
    else
    {
        return;
    }
    ui->plainTextEdit_ReceiveWindow->insertPlainText(strBuf);
}

void Widget::on_pushButton_clicked()
{
    ui->plainTextEdit_ReceiveWindow->clear();
}



/**************************** File Transmit Function ***********************************/
void Widget::on_pushButton_FileOpen_clicked()
{
    QString Filename = QFileDialog::getOpenFileName(this, "选择bin文件",
                                                    "/",
                                                    "*.bin");
    if(!Filename.isEmpty())
    {
        ui->label_FileState->setText(Filename);
        m_pFile = new QFile(Filename);

        if(!m_pFile->open(QIODeviceBase::ReadOnly))
        {
            qDebug("File Open Error");
        }
        m_ReadData = m_pFile->readAll();
        qDebug("File Read Success");
        m_pFile->close();
    }
    else
    {
        return;
    }
}

void Widget::on_pushButton_FileSend_clicked()
{
    QByteArray framearray;
    QEventLoop  WaitforACKloop;
    QEventLoop timer;

    connect(this, &Widget::USART_ACK_Received, &WaitforACKloop, &QEventLoop::quit);

    if(m_pSerialPort == nullptr)
    {
        ui->label_FileState->setText("Serial not Opened!");
        return;
    }

    if(m_ReadData.isEmpty())
    {
        qDebug("Empty File!");
        return;
    }


    UserFrame* frame = new UserFrame(0xa5cd, 0, nullptr);
    framearray = QByteArray::fromRawData((const char*)frame->frame(), (uint8_t)(frame->total_length()));
    while(1)
    {
        qDebug() << framearray.toHex(' ');
        m_pSerialPort->write(framearray);
        WaitforACKloop.exec();
        if(ack_flag)
        {
            ack_flag = 0;
            break;
        }
        else if(nack_flag)
        {
            nack_flag = 0;
        }
    }

    if(m_pFile != nullptr)
    {
        uint32_t i = m_ReadData.length() / MAX_BUFSIZE;
        uint32_t j = m_ReadData.length() % MAX_BUFSIZE;

        uint32_t k = 0;
        while(i--)
        {
            QByteArray buf = m_ReadData.mid(MAX_BUFSIZE*(k++), MAX_BUFSIZE);
            UserFrame* frame = new UserFrame(0xabcd, buf.length(), (uint8_t*)buf.data());
            framearray = QByteArray::fromRawData((const char*)frame->frame(), (uint8_t)(frame->total_length()));
            while(1)
            {
                qDebug() << framearray.toHex(' ');
                m_pSerialPort->write(framearray);
                WaitforACKloop.exec();
                if(ack_flag)
                {
                    ack_flag = 0;
                    break;
                }
                else if(nack_flag)
                {
                    nack_flag = 0;
                }
            }
            delete frame;
        }

        if(j != 0)
        {
            QByteArray buf = m_ReadData.mid(MAX_BUFSIZE * k, j);
            UserFrame* frame = new UserFrame(0xabcd, buf.length(), (uint8_t*)buf.data());
            framearray = QByteArray::fromRawData((const char*)frame->frame(), (uint8_t)(frame->total_length()));
            while(1)
            {
                qDebug() << framearray.toHex(' ');
                m_pSerialPort->write(framearray);
                WaitforACKloop.exec();
                if(ack_flag)
                {
                    ack_flag = 0;
                    break;
                }
                else if(nack_flag)
                {
                    nack_flag = 0;
                }
            }
            delete frame;
        }

        UserFrame* frame = new UserFrame(0xaecd, 0, nullptr);
        framearray = QByteArray::fromRawData((const char*)frame->frame(), (uint8_t)(frame->total_length()));
        while(1)
        {
            qDebug() << framearray.toHex(' ');
            m_pSerialPort->write(framearray);
            WaitforACKloop.exec();
            if(ack_flag)
            {
                ack_flag = 0;
                break;
            }
            else if(nack_flag)
            {
                nack_flag = 0;
            }
        }
        delete frame;
    }

    else
    {
        ui->label_FileState->setText("choose an bin File");
    }
}


/************************************************************************************************************/
/********************************** Network Communication Function **************************************************/

void Widget::on_pushButton_NetConnect_clicked()
{
    QString port;

    if(ui->lineEdit_Port->text().isEmpty())
    {
        QMessageBox::information(this, "Connection info", "Please input an valid port number");
        return;
    }
    else
    {
        port = ui->lineEdit_Port->text();
    }

    m_server = new QTcpServer;
    m_server->listen(QHostAddress::AnyIPv4, port.toShort());

    connect(m_server, &QTcpServer::newConnection, this, &Widget::newClientHandler);

    ui->pushButton_NetConnect->setEnabled(false);
}

void Widget::newClientHandler()
{
    if(m_server == nullptr)
    {
        return;
    }
    m_socket = m_server->nextPendingConnection();

    ui->label_IPaddr->setText(m_socket->peerAddress().toString());
}


void Widget::on_pushButton_NetDisconnect_clicked()
{
    if(m_server==nullptr)
    {
        return;
    }
    if(m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
    if(m_socket->state() != QAbstractSocket::UnconnectedState)
    {
        m_socket->waitForDisconnected(1000);
    }

    /* must delete class socket first, then class server */
    delete m_socket;
    m_socket = nullptr;
    delete m_server;
    m_server = nullptr;


    ui->pushButton_NetConnect->setEnabled(true);
    ui->label_IPaddr->clear();
}

void Widget::on_pushButton_NetClear_clicked()
{
    ui->plainTextEdit_Network->clear();
}


void Widget::on_pushButton_NetSend_clicked()
{
    if((m_socket != nullptr) && (m_socket->state() == QAbstractSocket::ConnectedState))
    {
        QString StringToSend = ui->plainTextEdit_Network->toPlainText();

        if(!m_socket->write(StringToSend.toUtf8()))
        {
            qDebug("SerialPort Send Error");
            return;
        }
    }
}

/************************************************************************************************************/











