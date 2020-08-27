#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include "configure.h"
#include <QFrame>
#include <QDebug>
#include <QJsonArray>

class Node : public QWidget
{
    Q_OBJECT
public:
    explicit Node(Pos pos, QString styleSheet, QWidget *parent = nullptr);
    void draw();
    void move(Pos newPos);
    Pos getPos();
    void remove();
    QJsonArray jsonPos();
    void fromJson(QJsonArray array);
private:
        QString styleSheet;
        Pos pos;
        QFrame* frame;
signals:

};

class SnakeNode: public Node{
    using Node::Node;
};

class SnakeHeadNode: public SnakeNode{
public:
    SnakeHeadNode(Pos pos, QWidget *parent = nullptr): SnakeNode(pos, QString("background-color: blue; border: 1px solid white"), parent){
    };
};

class SnakeBodyNode: public SnakeNode{
public:
    SnakeBodyNode(Pos pos, QWidget *parent = nullptr): SnakeNode(pos, QString("background-color: green; border: 1px solid white"), parent){
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
