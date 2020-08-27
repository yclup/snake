#include "controller.h"


Controller::Controller(QWidget *parent) : QWidget(parent), map(new Map(parent)), key_pressed(false)
{
    status = before_start;
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    QObject::connect(map->getSnake(), SIGNAL(crashed()), this, SLOT(end()));
    QObject::connect(parent, SIGNAL(newWall(int, int)), this, SLOT(newWall(int, int)));
    QObject::connect(map, SIGNAL(eatFood()), this, SLOT(eatFood()));
    map->init();
}

void Controller::setEnabled(bool a, bool b, bool c, bool d, bool e, bool f, bool g){
    start_btn->setEnabled(a);
    continue_btn->setEnabled(b);
    load_btn->setEnabled(c);
    restart_btn->setEnabled(d);
    pause_btn->setEnabled(e);
    save_btn->setEnabled(f);
    exit_btn->setEnabled(g);

    start_action->setEnabled(a);
    continue_action->setEnabled(b);
    load_action->setEnabled(c);
    restart_action->setEnabled(d);
    pause_action->setEnabled(e);
    save_action->setEnabled(f);
    exit_action->setEnabled(g);
}

void Controller::save(){
    QString path = QFileDialog::getSaveFileName(this, "Save", ".", "JSON(*.json)");
        if (!path.isEmpty()) {
            QFile file(path);
            if (file.open(QIODevice::WriteOnly)) {
                QJsonObject job;

                map->save(job);
                job.insert("time", time_val);
                job.insert("score", score_val);
                job.insert("interval", interval);

                QJsonDocument jd;
                jd.setObject(job);
                file.write(jd.toJson());
                file.close();
            }
        }
}

void Controller::load(){
    QString path = QFileDialog::getOpenFileName(this, "Open", ".", "JSON(*.json)");
        if (!path.isEmpty()) {
            QFile file(path);
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray allData = file.readAll();
                file.close();
                QJsonParseError json_error;
                QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
                if (json_error.error != QJsonParseError::NoError) {
                    QMessageBox::warning(this, "Error", "Json Error!");
                    return;
                }
                QJsonObject job = jsonDoc.object();
                map->load(job);
                time_val = job["time"].toInt();
                score_val = job["score"].toInt();
                interval = job["interval"].toInt();
                timer.setInterval(interval);
                speed_slider->setValue((1010-interval)/10);
                time->display(time_val);
                score->display(score_val);
//                key_pressed = true;
                this->status = Status::paused;
                setStatus();
            }
        }
}

void Controller::receiveKeyPress(QKeyEvent* event){
    switch (event->key()) {
    case Qt::Key_Left:
        map->getSnake()->setDir(left_dir);
        break;
    case Qt::Key_Right:
        map->getSnake()->setDir(right_dir);
        break;
    case Qt::Key_Up:
        if(!key_pressed) return;
        map->getSnake()->setDir(up_dir);
        break;
    case Qt::Key_Down:
        map->getSnake()->setDir(down_dir);
        break;
    default:
        break;
    }
    if(!key_pressed){
        key_pressed = true;
        timer.start(interval);
    }
}

void Controller::setStatus(){
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

void Controller::setButton(QPushButton* start, QPushButton* _continue_btn, QPushButton* load, QPushButton* restart, QPushButton* pause, QPushButton* save, QPushButton* exit){
    start_btn = start;
    continue_btn = _continue_btn;
    load_btn = load;
    restart_btn = restart;
    pause_btn = pause;
    save_btn = save;
    exit_btn = exit;
}

void Controller::setAction(QAction* start, QAction* _continue, QAction* load, QAction* restart, QAction* pause, QAction* save, QAction* exit){
    start_action = start;
    continue_action = _continue;
    load_action = load;
    restart_action = restart;
    pause_action = pause;
    save_action = save;
    exit_action = exit;
}

void Controller::setOthers(QLCDNumber* time_lcd, QLCDNumber* score_lcd, QSlider* slider){
    time = time_lcd;
    score = score_lcd;
    speed_slider = slider;
}

void Controller::start(){
    status = active;
    setStatus();
}

void Controller::reset(){
    time_val = 0;
    time->display(time_val);
    score_val = 0;
    score->display(score_val);
    status = before_start;
    key_pressed = false;
    map->reset();
    setStatus();
}

void Controller::pause(){

        timer.stop();
        status = paused;
        setStatus();
}

void Controller::restart(){

        timer.start();
        status = active;
        setStatus();
}

void Controller::eatFood(){
    score_val = score->intValue();
    score_val += 1;
    score->display(score_val);
}

void Controller::update(){
    time_val = time->intValue();
    time_val += 1;
    time->display(time_val);
    map->update();
}

void Controller::end(){
    QMessageBox::information(this, "提示", "游戏已结束！");
    timer.stop();
    status = finished;
    setStatus();
}



void Controller::newWall(int x, int y){
    if(status == before_start)
    map->getWall()->newWall(x, y);
}

void Controller::changeSpeed(int val){
    interval = 1010 - val*10;
    timer.setInterval(interval);
}
