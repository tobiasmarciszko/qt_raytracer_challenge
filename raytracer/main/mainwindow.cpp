#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "helper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    helper(new Helper(this))
{
    ui->setupUi(this);

    refreshRate = 60; // FPS
    framebufferRate = 60; // FPS

    // Framebuffer update ticks
    framebufferUpdater = new QTimer(this);
    connect(framebufferUpdater, &QTimer::timeout, helper, &Helper::tick);
    framebufferUpdater->start(1000/framebufferRate);
    ui->fpsLabel_2->setText(QString::number(framebufferRate) + " FPS");

    // Framebuffer to screen updates
    screenRefresh = new QTimer(this);
    connect(screenRefresh, &QTimer::timeout, ui->openGLWidget, &GLWidget::animate);
    screenRefresh->start(1000/refreshRate);
    ui->fpsLabel->setText(QString::number(refreshRate) + " FPS");


}

MainWindow::~MainWindow()
{
    delete ui;
    delete helper;
}

void MainWindow::on_pushButton_clicked()
{
    if (screenRefresh->isActive()) {
        screenRefresh->stop();
        ui->pushButton->setText("Play");
    } else {
        screenRefresh->start(1000/refreshRate);
        ui->pushButton->setText("Pause");
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    UNUSED_PARAM(position);
//    fps = ui->horizontalSlider->value();
//    timer->setInterval(1000/fps);
//    ui->fpsLabel->setText(QString::number(fps) + " FPS");
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    UNUSED_PARAM(action);
    refreshRate = ui->horizontalSlider->value();
    screenRefresh->setInterval(1000/refreshRate);
    ui->fpsLabel->setText(QString::number(refreshRate) + " FPS");
}

Helper* MainWindow::getHelper()
{
    return helper;
}

void MainWindow::on_pushButton_5_clicked()
{
    if (framebufferUpdater->isActive()) {
        framebufferUpdater->stop();
        ui->pushButton_5->setText("Play");
    } else {
        framebufferUpdater->start(1000/framebufferRate);
        ui->pushButton_5->setText("Pause");
    }
}

void MainWindow::on_horizontalSlider_2_actionTriggered(int action)
{
    UNUSED_PARAM(action);
    framebufferRate = ui->horizontalSlider_2->value();
    framebufferUpdater->setInterval(1000/framebufferRate);
    ui->fpsLabel_2->setText(QString::number(framebufferRate) + " FPS");
}


