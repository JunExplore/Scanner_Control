#ifndef LOGOUTPUT_H
#define LOGOUTPUT_H

#include <QTextBrowser>

class LogOutPut : public QTextBrowser{
    Q_OBJECT
public:
    LogOutPut();
    ~LogOutPut();

    void outputProcessMsg(QString str);
};

#endif // LOGOUTPUT_H
