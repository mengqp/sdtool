#include "mainwindow.h"
#include "datatype.h"

static QString BUTTON_LINK = FROMLOCAL("连接");
static QString BUTTON_UNLINK = FROMLOCAL("断开");
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if ( !UiInit() )
    {
        return;
    }

    if ( !DataInit() )
    {
        return ;
    }

}

MainWindow::~MainWindow()
{
    DataExit();
    UiExit();
}

/*******************************************************************************
 * 类:MainWindow
 * 函数名:UiInit
 * 功能描述:界面初始化
 * 参数:void
 * 被调用:构造涵数
 * 返回值:bool
 ******************************************************************************/
bool MainWindow::UiInit (void)
{
    QString name = SDTOOL_NAME + VERSION;
    this->setWindowTitle( name );
    this->setWindowFlags(this->windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(800,600);

    if ( !SerialUiInit() )
    {
        return false;
    }

    if ( !DisplayUiInit() )
    {
        return false;
    }

    return true;
}   /*-------- end class MainWindow method UiInit  -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:UiExit
 * 功能描述:界面删除
 * 参数:void
 * 被调用:析构
 * 返回值:void
 ******************************************************************************/
void MainWindow::UiExit (void)
{
    DisplayUiExit();
    SerialUiExit();
}   /*-------- end class MainWindow method UiExit  -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:SerialUiInit
 * 功能描述:串口ui初始化
 * 参数:void
 * 被调用:UiInit
 * 返回值:bool
 ******************************************************************************/
bool MainWindow ::SerialUiInit(void)
{
    m_labelPort = new QLabel( this );
    m_labelPort->setGeometry( 20, 20, 30, 20);
    m_labelPort->setText( FROMLOCAL("COM") );

    m_comboPort = new QComboBox( this );
    m_comboPort->setGeometry( 50, 20, 50, 20 );
    QStringList list = get_avail_sp_();
    for (int i=0; i < list.size(); i++)
    {
        m_comboPort->addItem( list.at(i) );
    }

    m_labelBaudRate = new QLabel( this );
    m_labelBaudRate->setGeometry( 110, 20, 50, 20);
    m_labelBaudRate->setText( FROMLOCAL("波特率:") );

    m_comboBaudRate = new QComboBox( this );
    m_comboBaudRate->setGeometry( 160, 20, 60, 20 );
    m_comboBaudRate->addItem( FROMLOCAL("1200") );
    m_comboBaudRate->addItem( FROMLOCAL("2400") );
    m_comboBaudRate->addItem( FROMLOCAL("4800") );
    m_comboBaudRate->addItem( FROMLOCAL("9600") );
    m_comboBaudRate->addItem( FROMLOCAL("19200") );
    m_comboBaudRate->addItem( FROMLOCAL("38400") );

    m_labelDataBits = new QLabel( this );
    m_labelDataBits->setGeometry( 230, 20, 50, 20);
    m_labelDataBits->setText( FROMLOCAL("数据位:") );

    m_comboDataBits = new QComboBox( this );
    m_comboDataBits->setGeometry( 280, 20, 30, 20 );
    m_comboDataBits->addItem( FROMLOCAL("8") );
    m_comboDataBits->addItem( FROMLOCAL("7") );
    m_comboDataBits->addItem( FROMLOCAL("6") );
    m_comboDataBits->addItem( FROMLOCAL("5") );

    m_labelParity = new QLabel( this );
    m_labelParity->setGeometry( 320, 20, 50, 20);
    m_labelParity->setText( FROMLOCAL("校验位:") );

    m_comboParity = new QComboBox( this );
    m_comboParity->setGeometry( 370, 20, 50, 20 );
    m_comboParity->addItem( FROMLOCAL("偶") );
    m_comboParity->addItem( FROMLOCAL("奇") );
    m_comboParity->addItem( FROMLOCAL("无") );

    m_labelStopBits = new QLabel( this );
    m_labelStopBits->setGeometry( 430, 20, 50, 20);
    m_labelStopBits->setText( FROMLOCAL("校验位:") );

    m_comboStopBits = new QComboBox( this );
    m_comboStopBits->setGeometry( 480, 20, 30, 20 );
    m_comboStopBits->addItem( FROMLOCAL("1") );
    m_comboStopBits->addItem( FROMLOCAL("2") );

    m_labelFrame = new QLabel( this );
    m_labelFrame->setGeometry( 520, 20, 60, 20);
    m_labelFrame->setText( FROMLOCAL("帧延时(ms):") );

    m_lineFrame = new QLineEdit( this );
    m_lineFrame->setGeometry( 580, 20, 30, 20 );
    m_lineFrame->setText( FROMLOCAL("10"));

    // 通断
    m_buttonLink = new QPushButton(this);
    m_buttonLink->setGeometry( 620, 20, 50, 20 );
    m_buttonLink->setText( BUTTON_LINK );
    connect(m_buttonLink, SIGNAL(released()), this, SLOT(SlotButtonLinkReleased()));


    return true;
}   /*-------- end class MainWindow  method SerialUiInit -------- */

/******************************************************************************
 * 类:MainWindow
 * 函数名:SerialUiExit
 * 功能描述:串口ui退出
 * 参数:void
 * 被调用:UiExit
 * 返回值:void
 ******************************************************************************/
void MainWindow ::SerialUiExit(void)
{
    connect(m_buttonLink, SIGNAL(released()), this, SLOT(SlotButtonLinkReleased()));
    if( NULL != m_buttonLink )
    {
        delete m_buttonLink;
        m_buttonLink = NULL;
    }

    if( NULL != m_comboStopBits )
    {
        delete m_comboStopBits;
        m_comboStopBits = NULL;
    }
    if( NULL != m_labelStopBits )
    {
        delete m_labelStopBits;
        m_labelStopBits = NULL;
    }

    if( NULL != m_comboDataBits )
    {
        delete m_comboDataBits;
        m_comboDataBits = NULL;
    }
    if( NULL != m_labelDataBits )
    {
        delete m_labelDataBits;
        m_labelDataBits = NULL;
    }

    if( NULL != m_comboBaudRate )
    {
        delete m_comboBaudRate;
        m_comboBaudRate = NULL;
    }
    if( NULL != m_labelBaudRate )
    {
        delete m_labelBaudRate;
        m_labelBaudRate = NULL;
    }

    if( NULL != m_comboPort )
    {
        delete m_comboPort;
        m_comboPort = NULL;
    }

    if( NULL != m_labelPort )
    {
        delete m_labelPort;
        m_labelPort = NULL;
    }
}   /*-------- end class MainWindow  method SerialUiExit -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:SerialUiSetEnabled
 * 功能描述:设置串口控件是否可用
 * 参数: bool b
 * 被调用:
 * 返回值:void
 ******************************************************************************/
void MainWindow ::SerialUiSetEnabled( bool b )
{
    m_comboPort->setEnabled( b );
    m_comboBaudRate->setEnabled( b );
    m_comboDataBits->setEnabled( b );
    m_comboParity->setEnabled( b );
    m_comboStopBits->setEnabled( b );
    m_lineFrame->setEnabled(b);

}   /*-------- end class MainWindow  method SerialUiSetEnabled -------- */


/*******************************************************************************
 * 类:MainWindow
 * 函数名:SlotButtonLinkReleased
 * 功能描述:连接与断开
 * 参数:void
 * 被调用:
 * 返回值:void
 ******************************************************************************/
void MainWindow ::SlotButtonLinkReleased(void)
{
    if ( BUTTON_LINK == m_buttonLink->text())
    {
        m_buttonLink ->setText( BUTTON_UNLINK );
        m_Serial->Init( );
        SerialUiSetEnabled( false );
    }
    else
    {
        m_buttonLink ->setText( BUTTON_LINK );
        m_Serial->Exit();
        SerialUiSetEnabled( true );
    }
}   /*-------- end class MainWindow              method SlotButtonLinkReleased -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:DisplayUiInit
 * 功能描述:初始化显示区域
 * 参数:void
 * 被调用:UiInit
 * 返回值:bool
 ******************************************************************************/
bool MainWindow ::DisplayUiInit(void)
{
    m_labelMode = new QLabel( this );
    m_labelMode->setGeometry( 20, 50, 30, 20);
    m_labelMode->setText( FROMLOCAL("显示:") );

    m_comboMode = new QComboBox( this );
    m_comboMode->setGeometry( 50, 50, 80, 20 );
    m_comboMode->addItem( FROMLOCAL("全部") );
    m_comboMode->addItem( FROMLOCAL("定制") );
    m_comboMode->addItem( FROMLOCAL("CRC21") );

    m_boxTime = new QCheckBox( FROMLOCAL("时间"), this );
    m_boxTime->setGeometry( 140, 50, 50, 20 );
    m_boxTime->setChecked( true );

    m_boxLen = new QCheckBox( FROMLOCAL("长度"), this );
    m_boxLen->setGeometry( 210, 50, 50, 20 );
    m_boxLen->setChecked( true );

    m_boxHex = new QCheckBox( FROMLOCAL("16进制"), this );
    m_boxHex->setGeometry( 260, 50, 60, 20 );
    m_boxHex->setChecked( true );

    m_boxPause = new QCheckBox( FROMLOCAL("暂停"), this );
    m_boxPause->setGeometry( 320, 50, 50, 20 );

    m_boxSave = new QCheckBox( FROMLOCAL("保存"), this );
    m_boxSave->setGeometry( 370, 50, 50, 20 );
    m_boxSave->setChecked( true );


    m_textDisplay = new QTextEdit( this );
    m_textDisplay->setGeometry(20, 80, 760, 500 );
    m_textDisplay->setReadOnly(true);
    m_textDisplay->document()->setMaximumBlockCount(10000);

    return true;
}   /*-------- end class MainWindow  method DisplayUiInit -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:DisplayUiExit
 * 功能描述:显示区退出
 * 参数:void
 * 被调用:UiExit
 * 返回值:void
 ******************************************************************************/
void MainWindow ::DisplayUiExit(void)
{
    if( NULL != m_boxSave )
    {
        delete m_boxSave;
        m_boxSave = NULL;
    }
    if( NULL != m_boxPause )
    {
        delete m_boxPause;
        m_boxPause = NULL;
    }

    if( NULL != m_boxHex )
    {
        delete m_boxHex;
        m_boxHex = NULL;
    }
    if( NULL != m_boxLen )
    {
        delete m_boxLen;
        m_boxLen = NULL;
    }

    if( NULL != m_boxTime )
    {
        delete m_boxTime;
        m_boxTime = NULL;
    }

    if( NULL != m_comboMode )
    {
        delete m_comboMode;
        m_comboMode = NULL;
    }
    if( NULL != m_labelMode )
    {
        delete m_labelMode;
        m_labelMode = NULL;
    }

    if( NULL != m_textDisplay )
    {
        delete m_textDisplay;
        m_textDisplay = NULL;
    }

}   /*-------- end class MainWindow  method DisplayUiExit -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:DataInit
 * 功能描述:数据初始化
 * 参数:void
 * 被调用:构造
 * 返回值:bool
 ******************************************************************************/
bool MainWindow ::DataInit(void)
{
    m_Serial = new CSerial( (QMainWindow *)this );
    m_Display = new CDisplay( (QMainWindow *)this );

    connect( this, SIGNAL(SignalReadData(QByteArray)),
             this, SLOT(SlotReadData(QByteArray)));

    return true;
}   /*-------- end class MainWindow  method DataInit -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:DataExit
 * 功能描述:数据退出
 * 参数:void
 * 被调用:析构
 * 返回值:void
 ******************************************************************************/
void MainWindow ::DataExit(void)
{
    disconnect( this, SIGNAL(SignalReadData(QByteArray)),
                this, SLOT(SlotReadData(QByteArray)));
    if( NULL != m_Display )
    {
        delete m_Display;
        m_Display = NULL;
    }

    if( NULL != m_Serial )
    {
        delete m_Serial;
        m_Serial = NULL;
    }
}   /*-------- end class MainWindow  method DataExit -------- */

/*******************************************************************************
 * 类:MainWindow
 * 函数名:SlotReadData
 * 功能描述:接收数据处理
 * 参数:QString str
 * 被调用:
 * 返回值:void
 ******************************************************************************/
void MainWindow ::SlotReadData(QByteArray ba )
{
    QString display = m_Display->Convert( ba );
    if ( !display.isEmpty() )
    {
        m_textDisplay->append( display );
    }
}   /*-------- end class MainWindow  method SlotReadData -------- */

QStringList MainWindow::get_avail_sp_()
{
    QStringList list_avail_sp;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            list_avail_sp.append(serial.portName());
            serial.close();
        }
    }

    return list_avail_sp;
}
