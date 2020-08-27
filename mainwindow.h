#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPoint>
#include "map.h"
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Controller* controller;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent* e) override;

signals:
    void newWall(int x, int y);

private slots:
    void on_start_clicked();
    void on_start_2_triggered();
    void on_pause_clicked();
    void on_continue_2_clicked();
    void on_restart_clicked();
    void on_quit_clicked();
    void on_pause_action_triggered();
    void on_continue_action_triggered();
    void on_restart_action_triggered();
    void on_quit_action_triggered();
    void on_save_clicked();
    void on_load_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_save_action_triggered();
    void on_load_action_triggered();
};
#endif // MAINWINDOW_H
