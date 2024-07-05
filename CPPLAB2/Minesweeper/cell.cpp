#include "cell.h"

void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit pressLeft();
    }
    else if (event->button() == Qt::RightButton)
    {
        emit pressRight();
    }
    else
    {
        emit pressMid();
    }
}

Cell::Cell(uint16_t x, uint16_t y, QWidget *parent) : QPushButton(parent), x(x), y(y)
{
    setStyleSheet("background-color: rgb(189, 189, 189);");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(40, 40);
}
