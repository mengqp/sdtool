#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include "Serial.h"
#include "Display.h"


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

    // 显示界面
    bool DisplayUiInit( void );
    void DisplayUiExit( void );

  private:
    // 数据初始化
    bool DataInit( void );
    void DataExit( void );

  signals:
    void SignalReadData(QString str);

  private slots:
    void SlotButtonLinkReleased( void );
    void SlotReadData(QString str);

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

	//显示数据
	QTextEdit *m_textDisplay;

    // 连接断开
    QPushButton *m_buttonLink;

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
