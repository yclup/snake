#ifndef CONFIGURE_H
#define CONFIGURE_H


struct Pos
{
    int x;
    int y;
    Pos(int _x, int _y) {x = _x; y = _y;}
    Pos() {}
};

enum Direction{
    right_dir,
    left_dir,
    up_dir,
    down_dir
};

enum Status{
    before_start,
    active,
    paused,
    finished
};

const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 40;
const int X_OFFSET = 0;
const int Y_OFFSET = 50;
const Direction INITIAL_DIRECTION = right_dir;


#endif // CONFIGURE_H
