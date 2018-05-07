#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "spidata.h"
#include <QMainWindow>
#include <QDebug>
#include <QHBoxLayout>
#include <QSplitter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void getX_position( );
    void getY_position( );
    void start_sendData( );

private:
    Ui::MainWindow *ui;

    //send data
    SPIData *data;
    SPIDev *spi_dev;
    uint32_t x_pos;
    uint32_t y_pos;
};

#endif // MAINWINDOW_H
