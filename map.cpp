#include "map.h"


bool Wall::checkPosHasWall(Pos pos){
    for(auto wall: walls){
        if(wall->getPos().x == pos.x && wall->getPos().y == pos.y){
            return true;
        }
    }
    return false;
}

void Wall::newWall(int x, int y){
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

QJsonArray Wall::jsonWall(){
    QJsonArray array;
    for(auto wall: walls){
        array.append(wall->jsonPos());
    }
    return array;
}

void Wall::fromJson(const QJsonArray& array){
    clear();
    for(auto item: array){
        QJsonArray pos = item.toArray();
        walls.push_back(new WallNode(Pos(pos[0].toInt(), pos[1].toInt()), parent));
    }
}

void Wall::init(){
    for(int x = 0; x < MAP_WIDTH; x++){
        for(int y = 0; y < MAP_HEIGHT; y++){
            if(x == 0 || y == 0 || x == MAP_WIDTH-1 || y==MAP_HEIGHT-1){
                walls.push_back(new WallNode(posByGrid(x, y), parent));
            }
        }
    }
}

void Wall::draw(){
    for(auto wall: walls){
        wall->draw();
    }
}

std::list<WallNode*> Wall::getWall(){
    return walls;
}

void Wall::clear(){
    for(auto wall: walls){
        wall->remove();
    }
    walls.clear();

}

QJsonArray Snake::jsonSnake(){
    QJsonArray array;
    for(auto snake: snakes){
        array.append(snake->jsonPos());
    }
    return array;
}

void Snake::fromJson(const QJsonArray& array){
    clear();
    for(auto item= array.begin(); item != --array.end(); item++){
        QJsonArray pos = item->toArray();
        snakes.push_back(new SnakeBodyNode(Pos(pos[0].toInt(), pos[1].toInt()), parent));
    }
    snakes.push_back(new SnakeHeadNode(Pos(array.last()[0].toInt(), array.last()[1].toInt())));
}

void Snake::init(std::list<Pos>& pos_list){
    for(auto pos = pos_list.begin(); pos != --pos_list.end(); pos++){
        snakes.push_back(new SnakeBodyNode((*pos), parent));
    }
    snakes.push_back(new SnakeHeadNode(pos_list.back(), parent));
}

void Snake::clear(){
    for(auto snake: snakes){
        snake->remove();
    }
    snakes.clear();
}

void Snake::draw(){  //draw the snake
    for(SnakeNode* snake: snakes){
        snake->draw();
    }
}

Pos Snake::move(){  //move the snake normally
    SnakeNode* front = snakes.front();
    SnakeNode* tail_node = snakes.back();
    Pos new_pos = calNewPos(tail_node->getPos(), dir);
    front->move(tail_node->getPos());
    tail_node->move(new_pos);
    snakes.pop_front();
    snakes.insert(--snakes.end(), front);
    return new_pos;
}

Pos Snake::extend(){  //extend the snake when it eat food
    SnakeNode* tail_node = snakes.back();
    Pos new_pos = calNewPos(tail_node->getPos(), dir);
    snakes.insert(--snakes.end(), new SnakeBodyNode(tail_node->getPos(), parent));
    tail_node->move(new_pos);
    return new_pos;
}

bool Snake::borderCollisionDetect(Pos new_pos, Wall* _wall){
    for(auto wall: _wall->getWall()){
        if(new_pos.x == wall->getPos().x && new_pos.y == wall->getPos().y){
            emit crashed();
            return true;
        }
    }

    return false;
}

bool Snake::selfCollisionDetect(){
    SnakeNode* tail_node = snakes.back();
    Pos new_pos = calNewPos(tail_node->getPos(), dir);
    for (auto snake: snakes){
        if(snake->getPos().x == new_pos.x && snake->getPos().y == new_pos.y) {
            emit crashed();
            return true;
        }
    }
    return false;
}

bool Snake::checkPosHasSnake(Pos pos){
    for(auto snake: snakes){
        if(snake->getPos().x == pos.x && snake->getPos().y == pos.y) return true;
    }
    return false;
}

QString Snake::dirToString(){
    if(dir == left_dir) return "left";
    if(dir == right_dir) return "right";
    if(dir == up_dir) return "up";
    if(dir == down_dir) return "down";
    return "null";
}

void Snake::loadDir(QString direction){
    if(direction == "left") dir = left_dir;
    if(direction == "right") dir = right_dir;
    if(direction == "up") dir = up_dir;
    if(direction == "down") dir = down_dir;
}

void Snake::setDir(Direction direction){ //set direction of the snake
    if(dir == left_dir && direction == right_dir) return;
    if(dir == right_dir && direction == left_dir) return;
    if(dir == up_dir && direction == down_dir) return;
    if(dir == down_dir && direction == up_dir) return;
    dir = direction;
}


Map::Map(QWidget* _parent): parent(_parent), snake(new Snake(parent)), wall(new Wall(parent)){
        setParent(_parent);
    };

    void Map::save(QJsonObject& job){
        job.insert("inited", inited);
        job.insert("numToExtend", numToExtend);
        job.insert("snake", snake->jsonSnake());
        job.insert("wall", wall->jsonWall());
        job.insert("food", food->jsonPos());
        job.insert("direction", snake->dirToString());
    }

    void Map::load(QJsonObject& job){
        inited = job["inited"].toBool();
        numToExtend = job["numToExtend"].toInt();
        snake->fromJson(job["snake"].toArray());
        wall->fromJson(job["wall"].toArray());
        food->fromJson(job["food"].toArray());
        snake->loadDir(job["direction"].toString());
        foodPos = food->getPos();
        snake->draw();
        wall->draw();
        food->draw();
    }

    Snake* Map::getSnake(){
        return snake;
    }

    Wall* Map::getWall(){
        return wall;
    }

    void Map::generateFood(){
        srand(time(0));
        int x, y;
        do{
            x = rand() % MAP_WIDTH * GRID_WIDTH + X_OFFSET;
            y = rand() % MAP_HEIGHT * GRID_HEIGHT + Y_OFFSET;
        }while(wall->checkPosHasWall(Pos(x, y)) || snake->checkPosHasSnake(Pos(x, y)));

        foodPos = Pos(x, y);
        if(!inited){
            food = new FoodNode(foodPos, parent);
            inited = true;
        }
        else{
            food->move(foodPos);
        }

    }

    void Map::reset(){
        snake->clear();
        wall->clear();
        inited = false;
        food->remove();
        init();
    }

    void Map::init(){
         //temporary
        snake->init(initial_pos);
        wall->init();
        generateFood();
        snake->draw();
        food->draw();
        wall->draw();
    }

    void Map::update(){
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
                emit eatFood();
                numToExtend += 3;
                generateFood();
                food->draw();
            }
            if(!snake->borderCollisionDetect(newPos, wall)){
                snake->draw();
            }
        }
    };
