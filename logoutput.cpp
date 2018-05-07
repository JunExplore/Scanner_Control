#include "logoutput.h"

LogOutPut::LogOutPut(QWidget *parent)
    :QTextBrowser(parent)
{
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}
LogOutPut::~LogOutPut()
{

}

void LogOutPut::outputProcessMsg(QString str)
{
    append(str);
}



