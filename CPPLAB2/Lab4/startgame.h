#ifndef STARTGAME_H
#define STARTGAME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QtWidgets>
#include <QVBoxLayout>


#include <QDialog>

class StartGame : public QDialog
{
    Q_OBJECT
    QVBoxLayout *screen;
    QPushButton *NG, *NcG;
public:
    StartGame();

signals:
    void mode(int8_t i);

public slots:
    void NewGame() {
        emit mode(0);
    }
    void NewcastomGame() {
        emit mode(1);
    }
};

#endif // STARTGAME_H
