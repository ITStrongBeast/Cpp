#include "castomgame.h"

CastomGame::CastomGame()
{
    screen = new QVBoxLayout;
    screen->addWidget(new QLabel("PARAMETERS"));
    screen->addWidget(new QLabel("FIELD SIZE"));

    size_x = new QSpinBox;
    size_x->setRange(1, 200);
    Sx = new QHBoxLayout;
    Sx->addWidget(new QLabel("Size Column: "));
    Sx->addWidget(size_x);
    resX = size_x->text().toInt();

    size_y = new QSpinBox;
    size_y->setRange(1, 200);
    Sy = new QHBoxLayout;
    Sy->addWidget(new QLabel("Size Row: "));
    Sy->addWidget(size_y);
    resY = size_y->text().toInt();

    screen->addLayout(Sx);
    screen->addLayout(Sy);

    number_mines = new QSpinBox;
    number_mines->setRange(1, resX * resY - 1); // сделать ограничение по количеству клеток
    screen->addWidget(new QLabel("NUMBER OF MINES"));
    screen->addWidget(number_mines);
    resM = number_mines->text().toInt();

    QPushButton *end = new QPushButton("Start Game");
    screen->addWidget(end);

    QObject::connect(end, &QPushButton::clicked, this, &CastomGame::size);
    QObject::connect(end, &QPushButton::clicked, this, &CastomGame::close);
    setLayout(screen);
}

