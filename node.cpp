#include "node.h"



Node::Node(Pos _pos, QString _styleSheet, QWidget *parent) :QWidget(parent), styleSheet(_styleSheet), pos(_pos)
{
    frame = new QFrame(parent);
}

void Node::draw(){
    frame->setStyleSheet(styleSheet);
    frame->setGeometry(pos.x, pos.y, GRID_WIDTH, GRID_HEIGHT);
    frame->show();
}

void Node::move(Pos newPos){
    pos = newPos;
}

Pos Node::getPos(){
    return pos;
}
void Node::remove(){
    delete frame;
}

QJsonArray Node::jsonPos(){
    QJsonArray array;
    array.append(pos.x);
    array.append(pos.y);
    return array;
}

void Node::fromJson(QJsonArray array){
    pos = Pos(array[0].toInt(), array[1].toInt());
}


