#ifndef GAME_H
#define GAME_H

#include "customgame.h"
#include "board.h"

class Game : public QWidget
{
	Q_OBJECT
	bool peek;
    bool isRus;
	Board *board;
    QAction *newCustomGame, *newGame, *peeping, *toEng, *toRus;
	QSpinBox *size_x, *size_y, *number_mines;
	QSettings set;
	QVBoxLayout *screen;
	QHBoxLayout *tool;
	QMenu *m;
	QToolBar *t;
    QMenuBar *menu;
    QTranslator translator;

  protected:
    void closeEvent(QCloseEvent *event) override { board->saveGame(isRus); }
    void resizeEvent(QResizeEvent *event) override;

  public:
	Game(bool p, QWidget *parent = nullptr);
    ~Game();

  signals:
	void save();

  public slots:
    void NG();
    void NCG();
    void translateToEng();
    void translateToRu();
};

#endif	  // GAME_H
