#ifndef CASTOMGAME_H
#define CASTOMGAME_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QSettings>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>

class CastomGame : public QDialog
{
    Q_OBJECT
    int8_t modeG = 0;
    int32_t resX = 100, resY = 100, resM;
    QVBoxLayout *screen;
    QHBoxLayout *Sx, *Sy, *Sm;
    QSpinBox *size_x, *size_y, *number_mines;
    QPushButton *end;

public:
    CastomGame();
    void getSize();
signals:
    void mode(int32_t size_x, int32_t size_y, int32_t number_mines);

public slots:
    void size(int8_t i) {
        emit mode(resX, resY, resM);
    }
    void setRange() {
        resM = resX * resY - 1;
    }
};

#endif // CASTOMGAME_H
