#include "game.h"

Game::Game(bool p, QWidget *parent) : peek(p), QWidget(parent)
{
	screen = new QVBoxLayout;

    QSettings settings("save.ini", QSettings::IniFormat);

    settings.beginGroup("Game");
    bool flag = settings.value("flag", true).toBool();
    bool isRus = settings.value("isRus", false).toBool();
    settings.endGroup();

    if (flag)
    {
        board = new Board(10, 10, 10);
    }
    else
    {
        board = new Board("save.ini");
    }

    setWindowTitle("Minesweeper");

    newGame = new QAction(this);
    newGame->setText("New Game");
    newGame->setStatusTip("New Game state");
    connect(newGame, &QAction::triggered, this, &Game::NG);

    newCustomGame = new QAction(this);
    newCustomGame->setText("New Custome Game");
    newCustomGame->setStatusTip("New Custome Game state");
    connect(newCustomGame, &QAction::triggered, this, &Game::NCG);

    toEng = new QAction(this);
    toEng->setText("Translate To English");
    connect(toEng, &QAction::triggered, this, &Game::translateToEng);

    toRus = new QAction(this);
    toRus->setText("Перевод на русский");
    connect(toRus, &QAction::triggered, this, &Game::translateToRu);

    m = new QMenu(this);
    m->setTitle(QString("Settings"));
    m->addAction(newGame);
    m->addAction(newCustomGame);

    t = new QToolBar(this);
    t->addAction(newGame);
    t->addAction(newCustomGame);

    if (peek)
    {
        peeping = new QAction(this);
        peeping->setText("Peek");
        peeping->setStatusTip("Peek state");
        connect(peeping, &QAction::triggered, board, &Board::lookMines);

        m->addAction(peeping);
        t->addAction(peeping);
    }

    m->addAction(toEng);
    m->addAction(toRus);

    menu = new QMenuBar(this);
    menu->addMenu(m);

    tool = new QHBoxLayout;
    tool->addWidget(menu);
    tool->addWidget(t);

    if (isRus)
    {
        translateToRu();
    }

    screen->addLayout(tool);
	screen->addWidget(board);
	board->show();

	setLayout(screen);
}

Game::~Game()
{
    delete board;
    delete newCustomGame;
    delete newGame;
    delete peeping;
    delete toEng;
    delete toRus;
    delete m;
    delete t;
    delete menu;
    delete tool;
    delete screen;
}

void Game::NG()
{
    int32_t x = board->sizeRow;
    int32_t y = board->sizeColumn;
    int32_t m = board->numberMines;
    delete board;
    board = new Board(x, y, m);
    connect(peeping, &QAction::triggered, board, &Board::lookMines);
    layout()->addWidget(board);
}

void Game::NCG()
{
    CustomGame dialog;
    dialog.exec();

    int32_t x = dialog.getX();
    int32_t y = dialog.getY();
    int32_t m = dialog.getM();
    delete board;
    board = new Board(x, y, m);
    connect(peeping, &QAction::triggered, board, &Board::lookMines);
    layout()->addWidget(board);
}

void Game::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int32_t newSide = qMin(newSize.width(), newSize.height());
    resize(newSide, newSide);
    QWidget::resizeEvent(event);
}

void Game::translateToEng()
{
  translator.load(":/translation_eng.qm");
  qApp->installTranslator(&translator);

  isRus = false;
  board->set_save();

  setWindowTitle(tr("Minesweeper"));

  m->setTitle(QString("Settings"));
  newGame->setText("New Game");
  newCustomGame->setText("New Custome Game");
  peeping->setText("Peek");
}

void Game::translateToRu()
{
  translator.load(":/translation_ru.qm");
  qApp->installTranslator(&translator);

  isRus = true;
  board->set_save();

  setWindowTitle("Сапёр");

  m->setTitle(QString("Настройки"));
  newGame->setText("Новая игра");
  newCustomGame->setText("Новая игра с выбором настроек");
  peeping->setText("Подсмотреть мины");
}
