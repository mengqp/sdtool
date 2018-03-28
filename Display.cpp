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

/*******************************************************************************
 * 功能描述:构造函数
 ******************************************************************************/
CDisplay::CDisplay ( QMainWindow *pMainWindow )
{
    if ( NULL != pMainWindow )
    {
        m_pMainWindow = (MainWindow *) pMainWindow;
    }

}   /*-------- end 构造函数 -------- */

/*******************************************************************************
 * 功能描述:析构函数
 ******************************************************************************/
CDisplay::~CDisplay (void)
{

}   /*-------- end 析构函数 -------- */

/*******************************************************************************
 * 类:CDisplay
 * 函数名:Convert
 * 功能描述:转化报文为直接打印的形式
 * 参数:QString str
 * 被调用:
 * 返回值:QString
 ******************************************************************************/
QString CDisplay::Convert(QString str )
{

    return str;
}   /*-------- end class CDisplay method Convert -------- */