/*******************************************************************************
 * 文件名:Serial.cpp
 * 文件描述:同头文件
 * 创建日期:2018/03/28 19:34:43
 * 版本：Ver1.0
 *
 * Copyright © 2018 - 2018 mengqp.
 *
 * 历史版本：
 * 修改人                修改时间                修改版本        修改内容
 *
 *
 ******************************************************************************/

#include <QByteArray>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

#include "mainwindow.h"
#include "Serial.h"
#include "datatype.h"



/*******************************************************************************
 * 功能描述:构造函数
 ******************************************************************************/
CSerial::CSerial ( QMainWindow *pMainWindow )
        : QObject( pMainWindow )
{
    if ( NULL != pMainWindow )
    {
        m_pMainWindow = (MainWindow *) pMainWindow;
    }
}   /*-------- end 构造函数 -------- */

/*******************************************************************************
 * 功能描述:析构函数
 ******************************************************************************/
CSerial::~CSerial (void)
{
    Exit();
}   /*-------- end 析构函数 -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:Init
 * 功能描述:初始化串口
 * 参数:void
 * 被调用:外部调用
 * 返回值:bool
 ******************************************************************************/
bool CSerial::Init(void)
{
    QString Port = GetPort();
    // m_Serial =  new Win_QextSerialPort(Port,QextSerialBase::Polling);
    // m_Serial =  new QSerialPort(Port,QextSerialBase::Polling);
    m_Serial =  new QSerialPort(Port);

    m_Serial ->open(QIODevice::ReadWrite);     //以读写方式打开串口
    if (!m_Serial->isOpen())
    {
        return false;
    }

    m_Serial->setBaudRate( GetBaudRate() );      //波特率设置，我们设置为115200
    m_Serial->setDataBits( GetDataBits() );  //数据位设置，我们设置为8位数据位
    m_Serial->setParity( GetParity() );  //奇偶校验设置，我们设置为无校验
    m_Serial->setStopBits( GetStopBits() );  //停止位设置，我们设置为1位停止位
    m_Serial->setFlowControl( QSerialPort::NoFlowControl );    //数据流控制设置，我们设置为无数据流控制
    // m_Serial->setTimeout(200);     //延时设置，我们设置为延时200ms,如果设置为500ms的话，会造成程序无响应，原因未知

    m_timerRead = new QTimer(this);      //设置读取计时器
    m_timerRead->start(30);      //设置延时为100ms
    connect(m_timerRead,SIGNAL(timeout()),this,SLOT(SlotReadData()));
    // connect(m_Serial, SIGNAL(readyRead()), this, SLOT(SlotReadData()));

    return true;
}   /*-------- end class CSerial method Init -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:Exit
 * 功能描述:退出串口
 * 参数:void
 * 被调用:外部调用
 * 返回值:void
 ******************************************************************************/
void CSerial::Exit(void)
{
    if( NULL != m_timerRead )
    {
        disconnect(m_timerRead,SIGNAL(timeout()),this,SLOT(SlotReadData()));
        delete m_timerRead;
        m_timerRead = NULL;
    }
    if( NULL != m_Serial )
    {
        m_Serial->clear();
        // disconnect(m_Serial, SIGNAL(readyRead()), this, SLOT(SlotReadData()));
        // if(m_Serial->isOpen())
        {
            m_Serial->close();
        }

        delete m_Serial;
        m_Serial  = NULL;
    }

}   /*-------- end class CSerial method Exit -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:GetPort
 * 功能描述:获取串口string
 * 参数:void
 * 被调用:Init
 * 返回值:QString
 ******************************************************************************/
QString CSerial::GetPort(void)
{
    unsigned int uIndex = m_pMainWindow->m_comboPort->currentIndex() + 1;
    return (FROMLOCAL("COM") + QString::number( uIndex, 10) );
}   /*-------- end class CSerial method GetPort -------- */


/*******************************************************************************
 * 类:CSerial
 * 函数名:GetBaudRate
 * 功能描述:获取波特率
 * 参数:void
 * 被调用:Init
 * 返回值:enum BaudRateType
 ******************************************************************************/
// enum BaudRateType CSerial::GetBaudRate(void)
enum QSerialPort::BaudRate CSerial::GetBaudRate(void)
{
    bool ok;
    unsigned int uIndex = m_pMainWindow->m_comboBaudRate->currentText().toUInt( &ok, 10 );
    switch ( uIndex )
    {
        case 1200: // 1200
            {
                return QSerialPort::Baud1200;
            }
            break;

        case 2400: // 2400
            {
                return QSerialPort::Baud2400;
            }
            break;

        case 4800: // 4800
            {
                return QSerialPort::Baud4800;
            }
            break;

        case 9600: // 9600
            {
                return QSerialPort::Baud9600;
            }
            break;

        case 19200: // 19200
            {
                return QSerialPort::Baud19200;
            }
            break;

        case 38400: // 38400
            {
                return QSerialPort::Baud38400;
            }
            break;
        default:
            break;
    }

    return QSerialPort::Baud9600;

}   /*-------- end class CSerial method GetBaudRate -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:GetDataBits
 * 功能描述:获取数据位
 * 参数:void
 * 被调用:Init
 * 返回值:enum DataBitsType
 ******************************************************************************/
// enum DataBitsType CSerial::GetDataBits(void)
enum QSerialPort::DataBits CSerial::GetDataBits(void)
{
    bool ok;
    unsigned int uIndex = m_pMainWindow->m_comboDataBits->currentText().toUInt( &ok, 10 );

    switch ( uIndex )
    {
        case 8:
            {
                return QSerialPort::Data8;
            }
            break;

        case 7:
            {
                return QSerialPort::Data7;

            }
            break;

        case 6:
            {
                return QSerialPort::Data6;

            }
            break;

        case 5:
            {
                return QSerialPort::Data5;

            }
            break;
        default:
            break;
    }

    return QSerialPort::Data8;
}   /*-------- end class CSerial method GetDataBits -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:GetParity
 * 功能描述:获取校检位
 * 参数:void
 * 被调用:Init
 * 返回值:enum ParityType
 ******************************************************************************/
// enum ParityType CSerial::GetParity(void)
enum QSerialPort::Parity CSerial::GetParity(void)
{
    unsigned int uIndex = m_pMainWindow->m_comboStopBits->currentIndex();

    switch ( uIndex )
    {
        case 0: // 偶
            {
                return QSerialPort::EvenParity;
                // return PAR_EVEN;
            }
            break;

        case 1: // 奇
            {
                return QSerialPort::OddParity;
            }
            break;

        case 2: // 无
            {
                return QSerialPort::NoParity;
            }
            break;
        default:
            break;
    }

    return QSerialPort::EvenParity;
}   /*-------- end class CSerial method GetParity -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:GetStopBits()
 * 功能描述:获取停止位
 * 参数:void
 * 被调用:Init
 * 返回值:enum StopBitsType
 ******************************************************************************/
// enum StopBitsType CSerial::GetStopBits(void)
enum  QSerialPort::StopBits CSerial::GetStopBits(void)
{
    bool ok;
    unsigned int uIndex = m_pMainWindow->m_comboStopBits->currentText().toUInt( &ok, 10 );

    switch ( uIndex )
    {
        case 2:
            {
                return QSerialPort::TwoStop;
            }
            break;

        case 1:
            {
                return QSerialPort::OneStop;

            }
            break;

        default:
            break;
    }

    return QSerialPort::OneStop;

}   /*-------- end class CSerial method GetStopBits() -------- */

/*******************************************************************************
 * 类:CSerial
 * 函数名:SlotReadData
 * 功能描述:读数据
 * 参数:void
 * 被调用:
 * 返回值:void
 ******************************************************************************/
void CSerial::SlotReadData (void)
{
    m_timerRead->stop();
    QByteArray ba ;
    bool ok;
    ba.clear();

    while( 1 )
    {
        // QByteArray temp = m_Serial->readAll();         //读取串口缓冲区的所有数据给临时变量temp
        // QByteArray temp = m_Serial->readLine();        //读取串口缓冲区的所有数据给临时变量temp
        // m_Serial->waitForReadyRead(200);                      //读取串口缓冲区的所有数据给临时变量temp
        if ( NULL == m_Serial )
        {
            return;
        }

        QByteArray temp = m_Serial->readAll();                 //读取串口缓冲区的所有数据给临时变量temp
        // qDebug() << temp.toHex();
        if( !temp.isEmpty())
        {
            ba += temp;
        }
        else
        {
            if ( !ba.isEmpty())
            {
                emit m_pMainWindow->SignalReadData( ba );
                m_timerRead->start(m_pMainWindow->m_lineFrame->text().toInt(&ok, 10));
                return;
            }
        }

        // 帧与帧之间的延时
        QTime t;
        t.start();
        while(t.elapsed()< m_pMainWindow->m_lineFrame->text().toInt(&ok, 10))
        {
            QCoreApplication::processEvents();
        }
        // QEventLoop eventloop;
        // QTimer::singleShot(100, &eventloop, SLOT(quit()));
        // eventloop.exec();

    }

}   /*-------- end class CSerial method SlotReadData -------- */
