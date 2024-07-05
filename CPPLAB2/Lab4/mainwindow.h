#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cell.h"

#include <cstdlib>

#include <QMainWindow>
#include <QWidget>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMouseEvent>
#include <QMessageBox>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QCheckBox>
#include <QSpinBox>

class MainWindow : public QWidget
{
    Q_OBJECT
    int8_t mode = 0;
    int8_t peep = 0;
    int32_t sizeColumn = 10;
    int32_t sizeRow = 10;
    int32_t numberMines = 10;
    Cell **board;
public:
    MainWindow(QWidget *parent = nullptr);
    void peeping() {peep = 1;}
    void createBoard();

public slots:
    void getNCG(int32_t size_x, int32_t size_y, int32_t number_mines) {
        sizeColumn = size_x;
        sizeRow = size_y;
        numberMines = number_mines;
    }
    void setNG(){
        mode = 0;
    }
    void setNCG(){
        mode = 1;
    }
};


#endif // MAINWINDOW_H
