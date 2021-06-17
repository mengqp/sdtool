/*******************************************************************************
 * 文件名:Display.h
 * 文件描述:显示数据处理
 * 创建日期:2018/03/28 20:46:07
 * 版本：Ver1.0
 *
 * Copyright © 2018 - 2018 mengqp.
 *
 * 历史版本：
 * 修改人                修改时间                修改版本        修改内容
 *
 *
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QByteArray>
#include <QString>
#include <QFile>
class MainWindow;

/*******************************************************************************
 *功能描述:
 *******************************************************************************/
class CDisplay
{
  public:
    /* constructor */
    explicit CDisplay(QMainWindow *pMainWindow);
    /* distructor */
    virtual ~CDisplay(void);

  private:
    // 数据
    bool DataInit( void );
    void DataExit( void );

  public:
    QString Convert( QByteArray ba );

  private:
    // 转化为16进岓
    QString GetHex( char *buf, unsigned int len );
    // 获取报文长度
    QString GetLen( void );
    // 获取时间
    QString GetTime( void );
    // 保存正常报文
    bool SaveNormal( QString str );
    // 保存定制报文
    bool SavePrivate( QString str );

    // 是否是定制报文
    bool IsPrivateBuf( char *buf, unsigned int len );
    // 获取错误报文

	// crc 报文
	bool IsCrcBuf(char *buf , unsigned int len );
	unsigned short GetCRC(unsigned char *buf, unsigned short len);

  private:
    MainWindow *m_pMainWindow;

    QFile *m_pFileNormal,  *m_pFilePrivate;


};



#endif /* DISPLAY_H */

// This file is set to c + + mode when you set the following content to the bottom and reopen the file
// Local Variables:
// mode: c++
// End:
