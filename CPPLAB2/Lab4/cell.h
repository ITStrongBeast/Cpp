#ifndef CELL_H
#define CELL_H

#include <QMainWindow>

class Cell
{ 
public:
    bool isMine = false;
    bool view = false;
    bool isFlag = false;
    int16_t countMine = 0;
    int32_t x, y;

    Cell();

    void leftClick();
    void midClick();
    void rightClick();
};

#endif // CELL_H
