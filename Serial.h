/*******************************************************************************
 * 文件名:Serial.h
 * 文件描述:关于串口的一些操作
 * 创建日期:2018/03/28 19:18:30
 * 版本：Ver1.0
 *
 * Copyright © 2018 - 2018 mengqp.
 *
 * 历史版本：
 * 修改人                修改时间                修改版本        修改内容
 *
 *
 ******************************************************************************/

#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include "win_qextserialport.h"

class MainWindow;

/*******************************************************************************
 *功能描述:
 *******************************************************************************/
class CSerial : public QObject
{
    Q_OBJECT

  public:
    /* constructor */
    explicit CSerial( QMainWindow *pMainWindow );
    /* distructor */
    virtual ~CSerial(void);

  public:
    // 串口初始化及退出
    bool Init( void );
    void Exit( void );

    // 获取串口属性内容
    QString GetPort( void );
    enum BaudRateType GetBaudRate( void );
    enum DataBitsType GetDataBits( void );
    enum ParityType GetParity( void );
    enum StopBitsType GetStopBits( void );

  private slots:
    // 接收串口数据
    void SlotReadData( void );

  private:
    MainWindow *m_pMainWindow;
    Win_QextSerialPort *m_Serial;
    QTimer *m_timerRead;




};


#endif /* SERIAL_H */

// This file is set to c + + mode when you set the following content to the bottom and reopen the file
// Local Variables:
// mode: c++
// End:
