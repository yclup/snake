#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QTimer>
#include "map.h"
#include "configure.h"
#include <QPushButton>

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

    void setEnabled(bool a, bool b, bool c, bool d, bool e, bool f, bool g){
        start_btn->setEnabled(a);
        continue_btn->setEnabled(b);
        load_btn->setEnabled(c);
        restart_btn->setEnabled(d);
        pause_btn->setEnabled(e);
        save_btn->setEnabled(f);
        exit_btn->setEnabled(g);
    }



public:
    void setButtonStatus(){
        if(status == before_start){
            setEnabled(true, false, true, false, false, false, true);
        }
        else if(status == active){
            setEnabled(false, false, false, false, true, false, true);
        }
        else if(status == paused){
            setEnabled(false, true, false, true, false, true, true);
        }
        else if(status == finished){
            setEnabled(false, false, false, true, false, false, true);
        }
    }

    void setButton(QPushButton* start, QPushButton* _continue_btn, QPushButton* load, QPushButton* restart, QPushButton* pause, QPushButton* save, QPushButton* exit){
        start_btn = start;
        continue_btn = _continue_btn;
        load_btn = load;
        restart_btn = restart;
        pause_btn = pause;
        save_btn = save;
        exit_btn = exit;
    }

    explicit Controller( Map* _map, QWidget *parent = nullptr);

    void start(){
        timer.start(100);
        status = active;
        setButtonStatus();
    }

    void reset(){
        status = before_start;
        map->reset();
        setButtonStatus();
    }
public slots:
    void update(){
        map->update();
    }

    void end(){
        timer.stop();
        status = finished;
        setButtonStatus();
    }

    void pause(){

            timer.stop();
            status = paused;
            setButtonStatus();
    }

    void restart(){

            timer.start();
            status = active;
            setButtonStatus();
    }

    void newWall(int x, int y){
        if(status == before_start)
        map->getWall()->newWall(x, y);
    }
signals:

};

#endif // CONTROLLER_H
