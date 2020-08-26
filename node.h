#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include "configure.h"
#include <QFrame>
#include <QDebug>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(Pos pos, QString styleSheet, QWidget *parent = nullptr);
    void draw();
    void move(Pos newPos);
    Pos getPos(){
        return pos;
    }
    void remove(){
        delete frame;
    }


private:
        QString styleSheet;
        Pos pos;
        QFrame* frame;
signals:

};

class SnakeNode: public Node{
public:
    SnakeNode(Pos pos, QWidget *parent = nullptr): Node(pos, QString("background-color: black"), parent){
    };
};

class WallNode: public Node{
public:
    WallNode(Pos pos, QWidget *parent = nullptr): Node(pos, QString("background-color: grey"), parent){};
};

class FoodNode: public Node{
public:
    FoodNode(Pos pos, QWidget *parent = nullptr): Node(pos, QString("background-color: red"), parent){};
};

#endif // NODE_H
