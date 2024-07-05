#include "mainwindow.h"
#include "castomgame.h"
#include "cell.h"

void MainWindow::createBoard() {
    board = new Cell*[sizeRow];
    for (int32_t i = 0; i < sizeRow; i++)
    {
        board[i] = new Cell[sizeColumn]{};
    }

    int32_t countMine = 0;

    while (countMine != numberMines) {
        int32_t i = rand() % sizeRow;
        int32_t j = rand() % sizeColumn;
        if (!board[i][j].isMine && board[i][j].view != 1)  // вторая проверка на то, что мы не в клетке которую открыли
        {
           board[i][j].isMine = true;
           countMine++;
        }
    }

    for (int32_t i = 0; i < sizeRow; i++) {
        for (int32_t j = 0; j < sizeColumn; j++) {
            if (board[i][j].isMine) {
                continue;
            }
            int32_t count = 0;
            for (int8_t di = -1; di < 2; di++) {
                for (int8_t dj = -1; dj < 2; dj++) {
                    if (0 <= i + di < sizeRow && 0 <= j + dj < sizeColumn &&
                            board[i + di][j + dj].isMine) {
                        count++;
                    }
                }
            }
            board[i][j].x = i;
            board[i][j].y = j;
            board[i][j].countMine = count;
        }
    }



}


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Minesweeper");

    QVBoxLayout *screen = new QVBoxLayout;
    screen->addWidget(new QLabel("ABA"));
    screen->addWidget(new QPushButton("BAB"));

    setLayout(screen);
}




//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//{
//    QDialog dialog;
//    dialog.setWindowTitle("Start Game");

//    QVBoxLayout *startScreen = new QVBoxLayout(&dialog);
//    QPushButton *NG = new QPushButton("Game");
//    QPushButton *NcG = new QPushButton("Castom Game");

//    startScreen->addWidget(new QLabel("Minesweeper"));
//    startScreen->addWidget(NG);
//    startScreen->addWidget(NcG);

//    QObject::connect(NG, &QPushButton::clicked, this, &MainWindow::setNG);
//    QObject::connect(NcG, &QPushButton::clicked, this, &MainWindow::setNCG);

//    QObject::connect(NG, &QPushButton::clicked, &dialog, &QDialog::accept);
//    QObject::connect(NcG, &QPushButton::clicked, &dialog, &QDialog::accept);

//    dialog.exec();

//    if (mode != 0) {
//        CastomGame castom;
//        castom.setModal(true);
//        QObject::connect(&castom, &CastomGame::mode, this, &MainWindow::getNCG);
//        castom.exec();
//    }

//    // не понимаю как создать новый интерфейс
//    //QVBoxLayout *screen = new QVBoxLayout;
//    //QMenuBar *head = new QMenuBar; // сделать меню и toolbar




//    //createBoard();



//    //setCentralWidget();
//    setLayout(startScreen);
//}

