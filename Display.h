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

  public:
    QString Convert( QString str );

  private:
    MainWindow *m_pMainWindow;


};



#endif /* DISPLAY_H */

// This file is set to c + + mode when you set the following content to the bottom and reopen the file
// Local Variables:
// mode: c++
// End:
