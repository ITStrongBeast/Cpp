#include "customgame.h"

CustomGame::CustomGame(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Custom Game"));

    screen = new QVBoxLayout(this);

    label1 = new QLabel(tr("FIELD SIZE"));
    screen->addWidget(label1);

    size_x = new QSpinBox;
    size_x->setRange(2, 50);
    Sx = new QHBoxLayout;
    label2 = new QLabel(tr("Size Column: "));
    Sx->addWidget(label2);
    Sx->addWidget(size_x);

    size_y = new QSpinBox;
    size_y->setRange(2, 50);
    Sy = new QHBoxLayout;
    label3 = new QLabel(tr("Size Row: "));
    Sy->addWidget(label3);
    Sy->addWidget(size_y);

    screen->addLayout(Sx);
    screen->addLayout(Sy);

    number_mines = new QSpinBox;
    number_mines->setRange(1, calculateMaxMines(size_x->value(), size_y->value()));
    label4 = new QLabel(tr("NUMBER OF MINES"));
    screen->addWidget(label4);
    screen->addWidget(number_mines);

    end = new QPushButton(tr("Start Game"));
    screen->addWidget(end);

    QObject::connect(end, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(size_x, QOverload< int >::of(&QSpinBox::valueChanged), this, &CustomGame::updateMineRange);
    QObject::connect(size_y, QOverload< int >::of(&QSpinBox::valueChanged), this, &CustomGame::updateMineRange);
}

CustomGame::~CustomGame()
{
    delete size_x;
    delete size_y;
    delete number_mines;
    delete Sx;
    delete Sy;
    delete label1;
    delete label2;
    delete label3;
    delete label4;
    delete end;
}

void CustomGame::updateMineRange()
{
    int32_t maxMines = calculateMaxMines(size_x->value(), size_y->value());
    number_mines->setMaximum(maxMines);
}
