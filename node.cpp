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



