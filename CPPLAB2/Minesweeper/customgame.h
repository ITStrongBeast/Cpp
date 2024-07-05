#ifndef CUSTOMGAME_H
#define CUSTOMGAME_H

#include "board.h"

class CustomGame : public QDialog
{
	Q_OBJECT

  private:
	QSpinBox *size_x, *size_y, *number_mines;
	QVBoxLayout *screen;
	QLabel *label1, *label2, *label3, *label4;
	QPushButton *end;
	QHBoxLayout *Sx, *Sy;

    int32_t calculateMaxMines(int32_t x, int32_t y) const { return x * y - 1; }

  public:
    CustomGame(QWidget *parent = nullptr);
    ~CustomGame();

	int32_t getX() const { return size_x->value(); }

	int32_t getY() const { return size_y->value(); }

	int32_t getM() const { return number_mines->value(); }

  private slots:
    void updateMineRange();

};

#endif	  // CUSTOMGAME_H
