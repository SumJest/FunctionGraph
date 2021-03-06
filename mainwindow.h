#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_checkBox_3_stateChanged(int arg1);

    void on_action_triggered();

private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *e);
};


#endif // MAINWINDOW_H
