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
    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);
    void on_horizontalSlider_actionTriggered(int action);

    void on_pushButton_5_clicked();

    void on_horizontalSlider_2_actionTriggered(int action);

private:
    Ui::MainWindow *ui;
    QTimer *screenRefresh;
    QTimer *framebufferUpdater;
    int refreshRate;
    int framebufferRate;
    Helper *helper;
};

#endif // MAINWINDOW_H
