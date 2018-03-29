/*******************************************************************************
 * 文件名:Display.cpp
 * 文件描述:见头文件
 * 创建日期:2018/03/28 20:49:57
 * 版本：Ver1.0
 *
 * Copyright © 2018 - 2018 mengqp.
 *
 * 历史版本：
 * 修改人                修改时间                修改版本        修改内容
 *
 *
 ******************************************************************************/
#include "Display.h"
#include "mainwindow.h"
#include <QTextStream>
#include <QDateTime>

/*******************************************************************************
 * 功能描述:构造函数
 ******************************************************************************/
CDisplay::CDisplay ( QMainWindow *pMainWindow )
{
    if ( NULL != pMainWindow )
    {
        m_pMainWindow = (MainWindow *) pMainWindow;
    }

    DataInit();


}   /*-------- end 构造函数 -------- */

/*******************************************************************************
 * 功能描述:析构函数
 ******************************************************************************/
CDisplay::~CDisplay (void)
{
    DataExit();
}   /*-------- end 析构函数 -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:DataInit
 * 功能描述:数据初始化
 * 参数:void
 * 被调用:构造
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::DataInit(void)
{
    m_pFileNormal = new QFile( "normal.txt" );
    m_pFileNormal->open( QIODevice::WriteOnly | QIODevice::Append );

    m_pFilePrivate = new QFile( "private.txt" );
    m_pFilePrivate->open( QIODevice::WriteOnly | QIODevice::Append );

    return true;
}   /*-------- end class CDisplay method DataInit -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:DataExit
 * 功能描述:数据退出
 * 参数:void
 * 被调用:
 * 返回值:void
 ******************************************************************************/
void CDisplay::DataExit(void)
{
    if( NULL != m_pFileNormal )
    {
        if ( m_pFileNormal->isOpen() )
        {
            m_pFileNormal->close();
        }
        delete m_pFileNormal;
        m_pFileNormal = NULL;
    }

    if( NULL != m_pFilePrivate )
    {
        if ( m_pFilePrivate->isOpen() )
        {
            m_pFilePrivate->close();
        }
        delete m_pFilePrivate;
        m_pFilePrivate = NULL;
    }

}   /*-------- end class CDisplay method DataExit -------- */



/*******************************************************************************
 * 类:CDisplay
 * 函数名:Convert
 * 功能描述:转化报文为直接打印的形式
 * 参数:QString str
 * 被调用:
 * 返回值:QString
 ******************************************************************************/
QString CDisplay::Convert(QByteArray ba )
{
    QString str = "";

    if ( m_pMainWindow->m_boxPause->isChecked() )
    {
        return str;
    }

    char RecvBuf[10240];
    unsigned int RecvLen = 10240;
    if ( ba.size() >= (int)RecvLen )
    {
        RecvLen = 10240;
    }
    else
    {
        RecvLen = ba.size();
    }

    memcpy( RecvBuf, ba.data(), RecvLen );

    if ( m_pMainWindow->m_boxTime->isChecked() )
    {
        str += GetTime(  );
        str += "--";
    }
    if (m_pMainWindow->m_boxLen->isChecked() )
    {
        str += "len=" + QString("%1").arg(RecvLen, 4, 10, QChar('0')) + "--";
    }
    if ( m_pMainWindow->m_boxHex->isChecked() )
    {
        str += GetHex( RecvBuf, RecvLen );
    }
    else
    {
        str += QString( ba );
    }

    if ( m_pMainWindow->m_boxSave->isChecked() )
    {

        SaveNormal( str );
        if ( 1 == m_pMainWindow->m_comboMode->currentIndex() )
        {
            if ( IsPrivateBuf(RecvBuf, RecvLen ))
            {
                str += "------------------------------error";
                SavePrivate( str );
            }
        }
    }

    // bool ok;
    // str.toUInt( &ok, 16);
    // QByteArray ByteHex;
    // = QString2Hex( str );

    return str;
}   /*-------- end class CDisplay method Convert -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:GetHex
 * 功能描述:获取16进制string
 * 参数:char *buf
 * 被调用:
 * 返回值:QString
 ******************************************************************************/
QString CDisplay::GetHex( char *buf, unsigned int len)
{
    QString strHex;
    for(unsigned int i=0;i<len;i++)
    {
        QString str = QString("%1").arg(buf[i]&0xFF,2,16,QLatin1Char('0'));
        strHex += str;
        strHex += " ";
    }

    return strHex;
}   /*-------- end class CDisplay method GetHex -------- */

/*******************************************************************************
 * 类:CDisplay前
 * 函数名:GetTime
 * 功能描述:获取当前时间
 * 参数:void
 * 被调用:
 * 返回值:QString
 ******************************************************************************/
QString CDisplay::GetTime(void)
{
    return QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss:zzz");
}   /*-------- end class CDisplay method GetTime -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:SaveNormal
 * 功能描述:保存正常报文
 * 参数:void
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::SaveNormal( QString str )
{
    if ( !m_pFileNormal->isOpen() )
    {
        m_pFileNormal->open( QIODevice::WriteOnly | QIODevice::Append );
    }

    QTextStream normal(m_pFileNormal);
    normal << str << endl;
    return true;
}   /*-------- end class CDisplay method SaveNormal -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:SaveNormal
 * 功能描述:保存正常报文
 * 参数:void
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::SavePrivate( QString str )
{
    if ( !m_pFilePrivate->isOpen() )
    {
        m_pFilePrivate->open( QIODevice::WriteOnly | QIODevice::Append );
    }
    QTextStream pv(m_pFilePrivate);
    pv << str << endl;

    return true;
}   /*-------- end class CDisplay method SaveNormal -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:IsPrivateBuf
 * 功能描述:所怍到的报文是否是定制报文
 * 参数:char *buf
 * 参数:unsigned int len
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::IsPrivateBuf(char *buf , unsigned int len )
{
    int init_len = 0;
    if ( len < 100 )
    {
        return false;
    }

    if ( 0xa3 == (unsigned char)buf[1]
         && 0x20 == (unsigned char)buf[2]
         && 0x01 == (unsigned char)buf[3]
         && 0x30 == (unsigned char)buf[4])
    {
        init_len = buf[3] + 5;
    }

    if ( 0xa5 == (unsigned char)buf[1 + init_len]
         && 0x20 == (unsigned char)buf[2 + init_len]
         && 0x9f == (unsigned char)buf[3 + init_len]
         && 0x30 == (unsigned char)buf[4 + init_len] )
    {
        for (int i=0; i < 32; i++)
        {
            unsigned int val = ( buf[35 + i + init_len ] << 8)
                               | ( buf[36 + i + init_len]) ;
            if ( 10000 <= val )
            {
                return true;
            }
        }

    }

    return false;
}   /*-------- end class CDisplay method IsPrivateBuf -------- */
