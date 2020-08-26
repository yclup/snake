#include "configure.h"
#include <QDebug>

Pos calNewPos(Pos old_pos, Direction direction){
    int x = old_pos.x;
    int y = old_pos.y;
    switch (direction) {
    case right_dir:
        return Pos(x+GRID_WIDTH, y);
        break;
    case left_dir:
        return Pos(x-GRID_WIDTH, y);
    case up_dir:
        return Pos(x, y-GRID_HEIGHT);
    case down_dir:
        return Pos(x, y+GRID_HEIGHT);
    default:
        break;
    }
}

Pos posByGrid(int x, int y){
    return Pos(x*GRID_WIDTH + X_OFFSET, y*GRID_HEIGHT+ Y_OFFSET);

}
