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
    Map* map;
    Controller* controller;

protected:
    void keyPressEvent(QKeyEvent *event) override{
        map->receiveKeyPress(event);
    }

    void mousePressEvent(QMouseEvent* e) override{
        QPointF point = e->localPos();
        int x = point.toPoint().x();
        int y = point.toPoint().y();
        emit newWall(x, y);
    }

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
};
#endif // MAINWINDOW_H
