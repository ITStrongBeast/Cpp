#include "startgame.h"

StartGame::StartGame()
{

    setWindowTitle("Minesweeper");

    QVBoxLayout *screen = new QVBoxLayout;
    screen->addWidget(new QLabel("ABA"));
    screen->addWidget(new QPushButton("BAB"));


//    QDialog dialog;
//    dialog.setWindowTitle("Menu");

//    QVBoxLayout *screen = new QVBoxLayout;
//    QPushButton *NG = new QPushButton("New Game");
//    QPushButton *NcG = new QPushButton("New Castom Game");

//    screen->addWidget(new QLabel("Minesweeper"));
//    screen->addWidget(NG);
//    screen->addWidget(NcG);

//    QObject::connect(NG, &QPushButton::clicked, this, &StartGame::NewGame);
//    QObject::connect(NcG, &QPushButton::clicked, this, &StartGame::NewcastomGame);


//    dialog.setWindowTitle("Menu");

//    QVBoxLayout *layout = new QVBoxLayout(&dialog);

//    QLabel *label = new QLabel("What is your name?");
//    QLineEdit *lineEdit = new QLineEdit;
//    QPushButton *button = new QPushButton("OK");

//    layout->addWidget(label);
//    layout->addWidget(lineEdit);
//    layout->addWidget(button);

//    QObject::connect(button, &QPushButton::clicked, &dialog, &QDialog::accept);

//    if (dialog.exec()) { // exec() makes the dialog modal
//        QString name = lineEdit->text();
//        QMessageBox::information(nullptr, "Greeting", "Hello, " + name + "!");
//    }

    setLayout(screen);
}
