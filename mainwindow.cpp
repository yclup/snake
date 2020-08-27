#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "node.h"
#include "map.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    controller = new Controller(this);
    ui->setupUi(this);
    this->setFixedSize(1000,850);
    this->setWindowTitle("贪吃蛇");
    controller->setButton(ui->start, ui->continue_2,ui->load, ui->restart,ui->pause, ui->save, ui->quit);
    controller->setAction(ui->start_2, ui->continue_action, ui->load_action, ui->restart_action, ui->pause_action, ui->save_action, ui->quit_action);
    controller->setOthers(ui->time, ui->score,ui->speed_slider);
    controller->setStatus();
    this->setFocusPolicy(Qt::TabFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    controller->receiveKeyPress(event);
}

void MainWindow::mousePressEvent(QMouseEvent* e){
    QPointF point = e->localPos();
    int x = point.toPoint().x();
    int y = point.toPoint().y();
    emit newWall(x, y);
}

void MainWindow::on_start_clicked()
{
    controller->start();
}

void MainWindow::on_start_2_triggered()
{
    controller->start();
}

void MainWindow::on_pause_clicked()
{
    controller->pause();
}

void MainWindow::on_continue_2_clicked()
{
    controller->restart();
}

void MainWindow::on_restart_clicked()
{
    controller->reset();
}

void MainWindow::on_quit_clicked()
{
    exit(1);
}

void MainWindow::on_pause_action_triggered()
{
    controller->pause();
}

void MainWindow::on_continue_action_triggered()
{
    controller->restart();
}

void MainWindow::on_restart_action_triggered()
{
    controller->reset();
}

void MainWindow::on_quit_action_triggered()
{
    exit(1);
}

void MainWindow::on_save_clicked()
{
    controller->save();
}

void MainWindow::on_load_clicked()
{
    controller->load();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    controller->changeSpeed(value);
}

void MainWindow::on_save_action_triggered()
{
    controller->save();
}

void MainWindow::on_load_action_triggered()
{
    controller->load();
}
