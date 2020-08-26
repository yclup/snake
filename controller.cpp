#include "controller.h"


Controller::Controller(Map* _map, QWidget *parent) : QWidget(parent), map(_map)
{
    status = before_start;
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    QObject::connect(map->getSnake(), SIGNAL(crashed()), this, SLOT(end()));
    QObject::connect(parent, SIGNAL(newWall(int, int)), this, SLOT(newWall(int, int)));
    map->init();
}
