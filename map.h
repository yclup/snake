#ifndef MAP_H
#define MAP_H

#include "node.h"
#include "functions.h"
#include <list>
#include <QKeyEvent>
#include "ctime"
#include <stdlib.h>
#include <QTimer>


class Wall: public QWidget
{
    Q_OBJECT
public slots:
    bool checkPosHasWall(Pos pos){
        for(auto wall: walls){
            if(wall->getPos().x == pos.x && wall->getPos().y == pos.y){
                return true;
            }
        }
        return false;
    }

    void newWall(int x, int y){
        int x_grid = (x-X_OFFSET)/GRID_WIDTH;
        int y_grid = (y-Y_OFFSET)/GRID_HEIGHT;
        if(x_grid >= MAP_WIDTH -1 || y_grid >= MAP_HEIGHT -1 || x_grid <= 0 || y_grid <= 0) return;
        Pos wallPos = posByGrid(x_grid, y_grid);
        if(checkPosHasWall(wallPos)){
            for(auto wall: walls){
                if(wall->getPos().x == wallPos.x && wall->getPos().y == wallPos.y){
                    walls.remove(wall);
                    wall->remove();
                    delete wall;
                }
            }
        }
        else{
            walls.push_back(new WallNode(wallPos, parent));
        }
        draw();
    }
private:
    QWidget* parent;
    std::list<WallNode*> walls;
public:
    Wall(QWidget* _parent = nullptr):parent(_parent){
        //init wall of four sides
    };

    void init(){
        for(int x = 0; x < MAP_WIDTH; x++){
            for(int y = 0; y < MAP_HEIGHT; y++){
                if(x == 0 || y == 0 || x == MAP_WIDTH-1 || y==MAP_HEIGHT-1){
                    walls.push_back(new WallNode(posByGrid(x, y), parent));
                }
            }
        }
    }

    void draw(){
        for(auto wall: walls){
            wall->draw();
        }
    }

    std::list<WallNode*> getWall(){
        return walls;
    }

    void clear(){
        for(auto wall: walls){
            wall->remove();
        }
        walls.clear();

    }
};

class Snake: public QWidget
{
    Q_OBJECT
signals:
    void crashed();

public:
    Snake(QWidget* _parent = nullptr): parent(_parent){}
    void init(std::list<Pos>& pos_list){
        dir = INITIAL_DIRECTION;
        for(Pos pos: pos_list){
            snakes.push_back(new SnakeNode(pos, parent));
        }
    }

    void clear(){
        for(auto snake: snakes){
            snake->remove();
        }
        snakes.clear();
    }

    void draw(){  //draw the snake
        for(SnakeNode* snake: snakes){
            snake->draw();
        }
    }

    Pos move(){  //move the snake normally
        SnakeNode* front = snakes.front();
        SnakeNode* tail_node = snakes.back();
        Pos new_pos = calNewPos(tail_node->getPos(), dir);
        front->move(new_pos);
        snakes.push_back(front);
        snakes.pop_front();
        return new_pos;
    }

    Pos extend(){  //extend the snake when it eat food
        SnakeNode* tail_node = snakes.back();
        Pos new_pos = calNewPos(tail_node->getPos(), dir);
        snakes.push_back(new SnakeNode(new_pos, parent));
        return new_pos;
    }

    bool borderCollisionDetect(Pos new_pos, Wall* _wall){
        for(auto wall: _wall->getWall()){
            if(new_pos.x == wall->getPos().x && new_pos.y == wall->getPos().y){
                //TODO
                qDebug() << "crash to the wall" << endl;
                emit crashed();
                return true;
            }
        }

        return false;
    }

    bool selfCollisionDetect(){
        SnakeNode* tail_node = snakes.back();
        Pos new_pos = calNewPos(tail_node->getPos(), dir);
        for (auto snake: snakes){
            if(snake->getPos().x == new_pos.x && snake->getPos().y == new_pos.y) return true;
        }
        return false;
    }

    bool checkPosHasSnake(Pos pos){
        for(auto snake: snakes){
            if(snake->getPos().x == pos.x && snake->getPos().y == pos.y) return true;
        }
        return false;
    }

    void setDir(Direction direction){ //set direction of the snake
        if(dir == left_dir && direction == right_dir) return;
        if(dir == right_dir && direction == left_dir) return;
        if(dir == up_dir && direction == down_dir) return;
        if(dir == down_dir && direction == up_dir) return;
        dir = direction;
    }
private:
    QWidget* parent;
    std::list<SnakeNode*> snakes;
    Direction dir;
};


class Map: public QWidget
{
    Q_OBJECT
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
    explicit Map(QWidget* _parent): parent(_parent), snake(new Snake(parent)), wall(new Wall(parent)){

        setParent(_parent);
//        QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
//        timer.start(200);
        //generate first food
    };

    Snake* getSnake(){
        return snake;
    }

    Wall* getWall(){
        return wall;
    }

    void generateFood(){
        srand(time(0));
        int x, y;
        do{
            x = rand() % MAP_WIDTH * GRID_WIDTH + X_OFFSET;
            y = rand() % MAP_HEIGHT * GRID_HEIGHT + Y_OFFSET;
        }while(wall->checkPosHasWall(Pos(x, y)) || snake->checkPosHasSnake(Pos(x, y)));
        qDebug() << x << y;
        foodPos = Pos(x, y);
        if(!inited){
            food = new FoodNode(foodPos, parent);
            inited = true;
        }
        else{
            food->move(foodPos);
        }

    }

    void reset(){
        snake->clear();
        wall->clear();
        inited = false;
        food->remove();
        init();
    }

    void init(){
         //temporary
        snake->init(initial_pos);
        wall->init();
        snake->draw();
        generateFood();
        food->draw();
        wall->draw();
    }

    void receiveKeyPress(QKeyEvent* event){
        switch (event->key()) {
        case Qt::Key_Left:
            snake->setDir(left_dir);
            break;
        case Qt::Key_Right:
            snake->setDir(right_dir);
            break;
        case Qt::Key_Up:
            snake->setDir(up_dir);
            break;
        case Qt::Key_Down:
            snake->setDir(down_dir);
            break;
        default:
            break;
        }
    }


public slots:
    void update(){
        Pos newPos;
        if(!snake->selfCollisionDetect()){
            if(numToExtend != 0){
                newPos = snake->extend();
                if(!snake->borderCollisionDetect(newPos, wall)){
                    snake->draw();
                }
                numToExtend -= 1;
            }
            else newPos = snake->move();
            if(newPos.x == foodPos.x && newPos.y == foodPos.y){
                numToExtend += 3;
                generateFood();
                food->draw();
            }
            if(!snake->borderCollisionDetect(newPos, wall)){
                snake->draw();
            }
        }
    };
};
#endif // MAP_H
