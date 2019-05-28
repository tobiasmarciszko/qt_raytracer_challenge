#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDesktopWidget>
#include <QScreen>
#include "helper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    helper(new Helper(this))
{
    ui->setupUi(this);

    framebufferRate = 30; // FPS

    // Framebuffer update ticks
    framebufferUpdater = new QTimer(this);
    connect(framebufferUpdater, &QTimer::timeout, helper, &Helper::tick);
    connect(framebufferUpdater, &QTimer::timeout, ui->openGLWidget, &GLWidget::animate);

    framebufferUpdater->start(1000/framebufferRate);
    ui->fpsLabel_2->setText(QString::number(framebufferRate) + " FPS");

    // Center application window on current screen
    const QScreen *screen = QApplication::screens()[0];
    auto geo = geometry();
    geo.moveCenter(screen->geometry().center());
    setGeometry(geo);

    // Pause as initial state:
    on_pushButton_5_clicked();
    helper->tick();

    // Connect the renderer to the widget
    auto effect = helper->effect;
    connect(effect, &IEffect::rendererReady, ui->openGLWidget, &GLWidget::animate);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete helper;
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

void MainWindow::on_doubleSpinBoxFromX_valueChanged(double arg1)
{
    helper->fromX(arg1);
    helper->tick();
}

void MainWindow::on_doubleSpinBoxFromY_valueChanged(double arg1)
{
    helper->fromY(arg1);
    helper->tick();
}

void MainWindow::on_doubleSpinBoxFromZ_valueChanged(double arg1)
{
    helper->fromZ(arg1);
    helper->tick();
}

void MainWindow::on_doubleSpinBoxToX_valueChanged(double arg1)
{
    helper->toX(arg1);
    helper->tick();
}


void MainWindow::on_doubleSpinBoxToY_valueChanged(double arg1)
{
    helper->toY(arg1);
    helper->tick();
}


void MainWindow::on_doubleSpinBoxToZ_valueChanged(double arg1)
{
    helper->toZ(arg1);
    helper->tick();
}
