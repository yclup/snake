#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QTimer>
#include <QAction>
#include <QLCDNumber>
#include <QFileDialog>
#include <QSlider>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include "map.h"
#include "configure.h"
#include <QPushButton>
#include <QMessageBox>
#include <QtMath>

class Controller : public QWidget
{
    Map* map;
    Q_OBJECT
    QTimer timer;
    Status status;
    QPushButton* start_btn;
    QPushButton* continue_btn;
    QPushButton* load_btn;
    QPushButton* restart_btn;
    QPushButton* pause_btn;
    QPushButton* save_btn;
    QPushButton* exit_btn;

    QAction* start_action;
    QAction* continue_action;
    QAction* load_action;
    QAction* restart_action;
    QAction* pause_action;
    QAction* save_action;
    QAction* exit_action;

    QLCDNumber* time;
    QLCDNumber* score;

    QSlider* speed_slider;

    int time_val;
    int score_val;

    bool key_pressed;
    int interval = 100;

    void setEnabled(bool a, bool b, bool c, bool d, bool e, bool f, bool g);



public:
    explicit Controller(QWidget *parent = nullptr);

    void save();
    void load();
    void receiveKeyPress(QKeyEvent* event);
    void setStatus();
    void setButton(QPushButton* start, QPushButton* _continue_btn, QPushButton* load, QPushButton* restart, QPushButton* pause, QPushButton* save, QPushButton* exit);
    void setAction(QAction* start, QAction* _continue, QAction* load, QAction* restart, QAction* pause, QAction* save, QAction* exit);
    void setOthers(QLCDNumber* time_lcd, QLCDNumber* score_lcd, QSlider* slider);
    void start();
    void reset();
    void pause();
    void restart();
    void changeSpeed(int val);

public slots:
    void eatFood();
    void update();
    void end();
    void newWall(int x, int y);
signals:

};

#endif // CONTROLLER_H
