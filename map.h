#ifndef MAP_H
#define MAP_H

#include "node.h"
#include "functions.h"
#include <list>
#include <QKeyEvent>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMessageBox>
#include "ctime"
#include <stdlib.h>
#include <QTimer>


class Wall: public QWidget
{
    Q_OBJECT
private:
    QWidget* parent;
    std::list<WallNode*> walls;

public slots:
    bool checkPosHasWall(Pos pos);
    void newWall(int x, int y);

public:
    Wall(QWidget* _parent = nullptr):parent(_parent){};

    QJsonArray jsonWall();
    void fromJson(const QJsonArray& array);
    void init();
    void draw();
    std::list<WallNode*> getWall();
    void clear();
};


class Snake: public QWidget
{
    Q_OBJECT
signals:
    void crashed();

public:
    Snake(QWidget* _parent = nullptr): parent(_parent), dir(INITIAL_DIRECTION){}

    QJsonArray jsonSnake();
    void fromJson(const QJsonArray& array);
    void init(std::list<Pos>& pos_list);
    void clear();
    void draw();
    Pos move();
    Pos extend();
    bool borderCollisionDetect(Pos new_pos, Wall* _wall);
    bool selfCollisionDetect();
    bool checkPosHasSnake(Pos pos);
    QString dirToString();
    void loadDir(QString direction);
    void setDir(Direction direction);

private:
    QWidget* parent;
    std::list<SnakeNode*> snakes;
    Direction dir;
};


class Map: public QWidget
{
    Q_OBJECT
signals:
    void eatFood();

private:
    QWidget* parent;
    Snake* snake;
    Pos foodPos;
    FoodNode* food;
    Wall* wall;
    bool inited = false;
    int numToExtend = 0;
    std::list<Pos> initial_pos = { posByGrid(20,20), posByGrid(20,21)};

public:
    explicit Map(QWidget* _parent);

    void save(QJsonObject& job);
    void load(QJsonObject& job);
    Snake* getSnake();
    Wall* getWall();
    void generateFood();
    void reset();
    void init();



public slots:
    void update();
};
#endif // MAP_H
