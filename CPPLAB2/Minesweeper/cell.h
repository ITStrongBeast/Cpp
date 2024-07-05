#ifndef CELL_H
#define CELL_H

#include <QGridLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSettings>
#include <QSpinBox>
#include <QTimer>
#include <QToolBar>
#include <QTranslator>
#include <QApplication>

class Cell : public QPushButton
{
    Q_OBJECT

  protected:
    void mousePressEvent(QMouseEvent *event);

  public:
    bool isMine = false;
    bool view = false;
    bool look = false;
    bool isFlag = false;
    uint16_t countMine = 0;
    uint16_t x, y;

    Cell(uint16_t x, uint16_t y, QWidget *parent = nullptr);
    ~Cell() {}

  signals:
    void pressLeft();
    void pressRight();
    void pressMid();

  public slots:
};

#endif // CELL_H
