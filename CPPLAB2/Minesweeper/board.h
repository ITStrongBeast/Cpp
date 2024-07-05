#ifndef BOARD_H
#define BOARD_H

#include "cell.h"

class Board : public QWidget
{
	Q_OBJECT
	int32_t countOpen = 0;
	bool firstClick = true;
    bool saved = false;
	QVector< QVector< Cell * > > board;
	QGridLayout *table;

  public:
	int32_t sizeRow;
	int32_t sizeColumn;
	int32_t numberMines;

	Board(int32_t x, int32_t y, int32_t m, QWidget *parent = nullptr);
	Board(const QString &fileName);
    ~Board();

	void createBoard();
	void recurOpen(uint16_t x, uint16_t y);
	void startBoard();
	void win();
	void lose(Cell *button);
	void openingRadius(Cell *button);
    void set_save() {saved = false;}

  signals:

  public slots:
	void lookMines();
	void leftClick();
	void rightClick();
	void midClick();
    void saveGame(bool isRus);
};

#endif	  // BOARD_H
