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
#include <QDebug>
#include "datatype.h"

/*******************************************************************************
 * 功能描述:构造函数
 ******************************************************************************/
CDisplay::CDisplay(QMainWindow *pMainWindow)
{
    if (NULL != pMainWindow) {
        m_pMainWindow = (MainWindow *)pMainWindow;
    }

    m_recv_head = 0;
    m_recv_tail = 0;

    DataInit();

    // char buf[] = { 0x21, 0x03, 0x04, 0x06, 0x00, 0x16, 0x22, 0x25 };
    // int len = 8;

    // if (IsCrcBuf(buf, len))
    //     qDebug() << 11111;
    // else
    //     qDebug() << 22222;

} /*-------- end 构造函数 -------- */

/*******************************************************************************
 * 功能描述:析构函数
 ******************************************************************************/
CDisplay::~CDisplay(void)
{
    DataExit();
} /*-------- end 析构函数 -------- */

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
    m_pFileNormal = new QFile("normal.txt");
    m_pFileNormal->open(QIODevice::WriteOnly | QIODevice::Append);

    m_pFilePrivate = new QFile("error.txt");
    m_pFilePrivate->open(QIODevice::WriteOnly | QIODevice::Append);

    return true;
} /*-------- end class CDisplay method DataInit -------- */

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
    if (NULL != m_pFileNormal) {
        if (m_pFileNormal->isOpen()) {
            m_pFileNormal->close();
        }
        delete m_pFileNormal;
        m_pFileNormal = NULL;
    }

    if (NULL != m_pFilePrivate) {
        if (m_pFilePrivate->isOpen()) {
            m_pFilePrivate->close();
        }
        delete m_pFilePrivate;
        m_pFilePrivate = NULL;
    }

} /*-------- end class CDisplay method DataExit -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:Convert
 * 功能描述:转化报文为直接打印的形式
 * 参数:QString str
 * 被调用:
 * 返回值:QString
 ******************************************************************************/
QString CDisplay::Convert(QByteArray ba)
{
    QString str = "";

    if (m_pMainWindow->m_boxPause->isChecked()) {
        return str;
    }

    char RecvBuf[10240];
    unsigned int RecvLen = 10240;
    if (ba.size() >= (int)RecvLen) {
        RecvLen = 10240;
    }
    else {
        RecvLen = ba.size();
    }

    memcpy(RecvBuf, ba.data(), RecvLen);

    if (m_pMainWindow->m_boxTime->isChecked()) {
        str += GetTime();
        str += "--";
    }
    if (m_pMainWindow->m_boxLen->isChecked()) {
        str += "len=" + QString("%1").arg(RecvLen, 4, 10, QChar('0')) + "--";
    }
    if (m_pMainWindow->m_boxHex->isChecked()) {
        str += GetHex(RecvBuf, RecvLen);
    }
    else {
        str += QString(ba);
    }

    if (m_pMainWindow->m_boxSave->isChecked()) {
        SaveNormal(str);

        switch (m_pMainWindow->m_comboMode->currentIndex()) {
        case 1: {
            if (IsPrivateBuf(RecvBuf, RecvLen)) {
                str += "\n------------------------------error";
                SavePrivate(str);
            }
            break;
        }
        case 2: {
            if (!IsCrcBuf(RecvBuf, RecvLen)) {
                str += "\n------------------------------error";
                SavePrivate(str);
            }
            break;
        }
        default:
            break;
        }
    }

    // bool ok;
    // str.toUInt( &ok, 16);
    // QByteArray ByteHex;
    // = QString2Hex( str );

    return str;
} /*-------- end class CDisplay method Convert -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:GetHex
 * 功能描述:获取16进制string
 * 参数:char *buf
 * 被调用:
 * 返回值:QString
 ******************************************************************************/
QString CDisplay::GetHex(char *buf, unsigned int len)
{
    QString strHex;
    for (unsigned int i = 0; i < len; i++) {
        QString str = QString("%1").arg(buf[i] & 0xFF, 2, 16, QLatin1Char('0'));
        strHex += str;
        strHex += " ";
    }

    return strHex;
} /*-------- end class CDisplay method GetHex -------- */

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
} /*-------- end class CDisplay method GetTime -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:SaveNormal
 * 功能描述:保存正常报文
 * 参数:void
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::SaveNormal(QString str)
{
    if (!m_pFileNormal->isOpen()) {
        m_pFileNormal->open(QIODevice::WriteOnly | QIODevice::Append);
    }

    QTextStream normal(m_pFileNormal);
    normal << str << endl;
    return true;
} /*-------- end class CDisplay method SaveNormal -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:SaveNormal
 * 功能描述:保存正常报文
 * 参数:void
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::SavePrivate(QString str)
{
    if (!m_pFilePrivate->isOpen()) {
        m_pFilePrivate->open(QIODevice::WriteOnly | QIODevice::Append);
    }
    QTextStream pv(m_pFilePrivate);
    pv << str << endl;

    return true;
} /*-------- end class CDisplay method SaveNormal -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:IsPrivateBuf
 * 功能描述:所怍到的报文是否是定制报文
 * 参数:char *buf
 * 参数:unsigned int len
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::IsPrivateBuf(char *buf, unsigned int len)
{
    int init_len = 0;
    char tmp[256];
    if (len < 100) {
        return false;
    }
    // sprintf(tmp, "11111 %.2x %.2x %.2x %.2x\n",
    //         buf[1], buf[2], buf[3],buf[4]);
    // qDebug() << FROMLOCAL( tmp );

    if (0xa3 == (unsigned char)buf[1] && 0x20 == (unsigned char)buf[2] &&
        0x01 == (unsigned char)buf[3] && 0x30 == (unsigned char)buf[4]) {
        init_len = buf[3] + 5;
    }

    // sprintf(tmp, "22222int_len=%d %.2x %.2x %.2x %.2x\n", init_len,
    //         buf[1+init_len], buf[2+init_len], buf[3+init_len],buf[4+init_len]);
    // qDebug() << FROMLOCAL( tmp );

    if (0xa5 == (unsigned char)buf[1 + init_len] &&
        0x20 == (unsigned char)buf[2 + init_len] &&
        0x9f == (unsigned char)buf[3 + init_len] &&
        0x30 == (unsigned char)buf[4 + init_len]) {
        for (int i = 0; i < 32; i++) {
            unsigned int val =
                ((unsigned char)buf[35 + 2 * i + init_len] << 8) |
                ((unsigned char)buf[36 + 2 * i + init_len]);

            // sprintf(tmp, "3333  i=%d   %.2x %.2x %u\n", i,
            //         (unsigned char)buf[35 + 2 * i + init_len ] & 0xff,
            //         (unsigned char)buf[36 + 2 * i + init_len] & 0xff, val);
            // qDebug() << FROMLOCAL( tmp );

            if (10000 <= val) {
                return true;
            }
        }
    }

    return false;
} /*-------- end class CDisplay method IsPrivateBuf -------- */

unsigned short CDisplay::GetCRC(unsigned char *buf, unsigned short len)
{
    unsigned short Genpoly = 0xA001;
    unsigned short CRC = 0xFFFF;
    unsigned short index;
    while (len--) {
        CRC = CRC ^ (unsigned short)*buf++;
        for (index = 0; index < 8; index++) {
            if ((CRC & 0x0001) == 1)
                CRC = (CRC >> 1) ^ Genpoly;
            else
                CRC = CRC >> 1;
        }
    }

    return CRC;
}

/*******************************************************************************
 * 类:CDisplay
 * 函数名:IsPrivateBuf
 * 功能描述:所怍到的报文是否是定制报文
 * 参数:char *buf
 * 参数:unsigned int len
 * 被调用:
 * 返回值:bool
 ******************************************************************************/
bool CDisplay::IsCrcBuf(char *buf, unsigned int len)
{
    int base = 0;
    int i = 0;
    unsigned char tmpbuf[1024];
    unsigned int tmplen;
    unsigned short crc;

    for (i = 0; i < len; i++) {
        m_recvbuf[m_recv_tail] = buf[i];
        m_recv_tail++;
        if (m_recv_tail >= MAX_RECV_LEN)
            m_recv_tail = 0;
    }

    while (m_recv_tail != m_recv_head) {
        unsigned int recvlen =
            (m_recv_tail + MAX_RECV_LEN - m_recv_head) % MAX_RECV_LEN;

		tmplen =0;
		memset(tmpbuf, 0 , 1024);

        if (m_recvbuf[m_recv_head% MAX_RECV_LEN] == 0x21 &&
            m_recvbuf[(m_recv_head + 1) % MAX_RECV_LEN] == 0x03) {
            for (i = 0; i < 1024; i++) {
                if (i >= recvlen) {
					// 不判断最后一帧
					return true;
                }
                if (i > 0) {
                    if (m_recvbuf[(m_recv_head + i) % MAX_RECV_LEN] == 0x21 &&
                        m_recvbuf[(m_recv_head + i + 1) % MAX_RECV_LEN] == 0x03) {
                        tmplen = i;
                        for (i = 0; i < tmplen; i++) {
                            tmpbuf[i] = m_recvbuf[m_recv_head];
                            m_recv_head++;
                            if (m_recv_head >= MAX_RECV_LEN)
                                m_recv_head = 0;
                        }
                        break;
                    }
                }
            }

            if (2 >= tmplen)
                return false;

            crc = GetCRC((unsigned char *)tmpbuf, tmplen - 2);

            if ((crc & 0xff) != (unsigned char)tmpbuf[tmplen - 2] ||
                ((crc & 0xff00) >> 8) != (unsigned char)tmpbuf[tmplen - 1])
                return false;
        }
        else {
            m_recv_head++;
            if (m_recv_head >= MAX_RECV_LEN)
                m_recv_head = 0;
        }

        if (m_recv_head >= MAX_RECV_LEN)
            m_recv_head = 0;
    }

    // while (m_recvbuf[base] == 0x21 && buf[base + 1] == 0x03) {
    //     for (i = 0; i < 1024; i++) {
    //         if (base + i >= len) {
    //             break;
    //         }
    //         if (i > 0) {
    //             if (buf[base + i] == 0x21 && buf[base + i + 1] == 0x03)
    //                 break;
    //         }
    //     }

    //     printf("i=%d\n", i);
    //     if (2 >= i)
    //         return false;

    //     base += i;
    //     if (base > len)
    //         return false;

    //     crc = GetCRC((unsigned char *)buf + base - i, i - 2);

    //     if ((crc & 0xff) != (unsigned char)buf[base - 2] ||
    //         ((crc & 0xff00) >> 8) != (unsigned char)buf[base - 1])
    //         return false;

    //     if (base == len)
    //         break;
    // }

    // qDebug() << crc << (unsigned char)buf[base - 2]
    // 		 << (unsigned char)buf[base - 1];
    // unsigned short local =
    //     (unsigned short)((buf[len - 2] & 0xff) |
    //                      ((unsigned short)(buf[len - 1] << 8) & 0xff00));

    return true;
} /*-------- end class CDisplay method IsPrivateBuf -------- */
