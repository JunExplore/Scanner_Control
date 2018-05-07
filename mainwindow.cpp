#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicdef.h"
#include "glwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GLWindow *glwindow = new GLWindow(this);
    ui->glWindow->setWidget(glwindow);
    ui->glWindow->setWidgetResizable(true);
    ui->glWindow->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    consoleOutput = new LogOutPut(this);
    QGroupBox *consoleGroup = new QGroupBox;
    QHBoxLayout *consoleLayout = new QHBoxLayout;
    consoleLayout->addWidget(consoleOutput);
    consoleGroup->setLayout(consoleLayout);

    QSplitter *rightSplitter = new QSplitter(Qt::Vertical);
    rightSplitter->addWidget(ui->xy_group);
    rightSplitter->addWidget(consoleGroup);
    rightSplitter->setStretchFactor(1, 3);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(ui->glWindow);
    mainSplitter->addWidget(rightSplitter);

    setCentralWidget(mainSplitter);
    data = new SPIData();
    spi_dev = new SPIDev();
    spi_dev->selectDevice("/dev/spi");
    spi_dev->openDevice();

    connect(ui->x_positon_lineedit, SIGNAL(returnPressed()), this, SLOT(getX_position()));
    connect(ui->y_positon_lineedit, SIGNAL(returnPressed()), this, SLOT(getY_position()));
    connect(ui->start_button, SIGNAL(clicked()), this, SLOT(start_sendData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getX_position()
{
    QString x_pos_val_s = ui->x_positon_lineedit->text();
    bool ok;
    x_pos = x_pos_val_s.toInt(&ok, 16);
    consoleOutput->outputProcessMsg("x pos is:" + x_pos_val_s);
}


void MainWindow::getY_position()
{
    QString y_pos_val_s = ui->y_positon_lineedit->text();
    bool ok;
    y_pos = y_pos_val_s.toInt(&ok, 16);
    consoleOutput->outputProcessMsg("x pos is:" + y_pos_val_s);
}

void MainWindow::start_sendData()
{
    consoleOutput->outputProcessMsg("ceshi shuchu");

    uint32_t tmp = x_pos << 16;
    uint32_t xy_pos = tmp + y_pos;
    data->fillBuffer(xy_pos);
    data->dataSend(spi_dev);
    consoleOutput->outputProcessMsg("x pos is:" + QString::number(xy_pos, 10));
    qDebug("%d", xy_pos);
}
