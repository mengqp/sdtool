#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include "Serial.h"
#include "Display.h"
#include <QLineEdit>
#include <QHBoxLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:
    // 界面初始化
    bool UiInit( void );
    // 界面退出
    void UiExit( void );

    // 串口界面
    bool SerialUiInit( void );
    void SerialUiExit( void );
    void SerialUiSetEnabled( bool b );  // 设置串口上控件是否可用

    // 显示界面
    bool DisplayUiInit( void );
    void DisplayUiExit( void );

  private:
    // 数据初始化
    bool DataInit( void );
    void DataExit( void );
    QStringList get_avail_sp_() ;

  signals:
    void SignalReadData(QByteArray ba);

  private slots:
    void SlotButtonLinkReleased( void );
    void SlotReadData(QByteArray ba);

  public:
    // 串口窗口
    QLabel *m_labelPort;
    QComboBox *m_comboPort;

    // 波特率
    QLabel *m_labelBaudRate;
    QComboBox *m_comboBaudRate;

    // 数据位
    QLabel *m_labelDataBits;
    QComboBox *m_comboDataBits;

    // 校验位
    QLabel *m_labelParity;
    QComboBox *m_comboParity;

    // 校验位
    QLabel *m_labelStopBits;
    QComboBox *m_comboStopBits;

    // 帧延时
    QLabel *m_labelFrame;
    QLineEdit *m_lineFrame;

    // 连接断开
    QPushButton *m_buttonLink;

	QHBoxLayout* m_playout_s;

  public:
    // 模式 显示所有报文或是特定保文
    QLabel *m_labelMode;
    QComboBox *m_comboMode;

    // 是不是显示时间
    QCheckBox *m_boxTime;

    // 是不是16进制显示
    QCheckBox *m_boxLen;

    // 是不是16进制显示
    QCheckBox *m_boxHex;

    // 暂停
    QCheckBox *m_boxPause;

    // 保存
    QCheckBox *m_boxSave;

	//显示数据
	QTextEdit *m_textDisplay;

	QHBoxLayout* m_playout_o;



  private:
    // 串口
    CSerial *m_Serial;

    // 显示数据
    CDisplay *m_Display;



};

#endif // MAINWINDOW_H

// This file is set to c + + mode when you set the following content to the bottom and reopen the file
// Local Variables:
// mode: c++
// End:
