#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "node.h"
#include "map.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1000,850);
    Map* main_map = new Map(this);
    map = main_map;
    controller = new Controller(map, this);
    controller->setButton(ui->start, ui->continue_2,ui->load, ui->restart,ui->pause, ui->save, ui->quit);
    controller->setButtonStatus();
    this->setFocusPolicy(Qt::TabFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
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
