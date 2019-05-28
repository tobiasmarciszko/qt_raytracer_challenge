#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define UNUSED_PARAM(param) (void) param

class QTimer;
class Helper;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() final;

    Helper* getHelper();

private slots:
    void on_pushButton_5_clicked();
    void on_horizontalSlider_2_actionTriggered(int action);

    void on_doubleSpinBoxFromX_valueChanged(double arg1);

    void on_doubleSpinBoxFromY_valueChanged(double arg1);

    void on_doubleSpinBoxFromZ_valueChanged(double arg1);

    void on_doubleSpinBoxToX_valueChanged(double arg1);

    void on_doubleSpinBoxToY_valueChanged(double arg1);

    void on_doubleSpinBoxToZ_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QTimer *framebufferUpdater;
    int framebufferRate;
    Helper *helper;
};

#endif // MAINWINDOW_H
