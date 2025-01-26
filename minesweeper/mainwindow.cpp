#include "mainwindow.h"

MainWindow::MainWindow(bool p, QWidget *parent) : m_peek(p), QWidget(parent)
{
    m_screen = new QVBoxLayout;

    QSettings settings("save.ini", QSettings::IniFormat);

    settings.beginGroup("Field");
    bool notSaveFlag = settings.value("flag", true).toBool();
    bool isRus = settings.value("isRus", false).toBool();
    settings.endGroup();

    if (notSaveFlag)
    {
        m_field = new Field(10, 10, 10, this);
    }
    else
    {
        m_field = new Field("save.ini", this);
    }

    connect(m_field, &Field::getCount, this, &MainWindow::setCount);

    setWindowTitle("Minesweeper");

    m_newGame = new QAction(this);
    m_newGame->setText("New Game");
    m_newGame->setStatusTip("New Game state");
    connect(m_newGame, &QAction::triggered, this, &MainWindow::newGame);

    m_newCustomGame = new QAction(this);
    m_newCustomGame->setText("New Custome Game");
    m_newCustomGame->setStatusTip("New Custome Game state");
    connect(m_newCustomGame, &QAction::triggered, this, &MainWindow::newCstomGame);

    m_englishTranslation = new QAction(this);
    m_englishTranslation->setText("Translate To English");
    connect(m_englishTranslation, &QAction::triggered, this, &MainWindow::translateToEng);

    m_russianTranslation = new QAction(this);
    m_russianTranslation->setText("Перевод на русский");
    connect(m_russianTranslation, &QAction::triggered, this, &MainWindow::translateToRu);

    m_leftHanded = new QAction(this);
    m_leftHanded->setText("Left-Handed");
    connect(m_leftHanded, &QAction::triggered, m_field, &Field::setLeftHand);

    m_menu = new QMenu(this);
    m_menu->setTitle(QString("Settings"));
    m_menu->addAction(m_newGame);
    m_menu->addAction(m_newCustomGame);
    m_menu->addAction(m_leftHanded);

    m_toolBar = new QToolBar(this);
    m_toolBar->addAction(m_newGame);
    m_toolBar->addAction(m_newCustomGame);
    m_toolBar->addAction(m_leftHanded);

    if (m_peek)
    {
        m_peeping = new QAction(this);
        m_peeping->setText("Peek");
        connect(m_peeping, &QAction::triggered, m_field, &Field::lookMines);

        m_menu->addAction(m_peeping);
        m_toolBar->addAction(m_peeping);
    }

    m_menu->addAction(m_englishTranslation);
    m_menu->addAction(m_russianTranslation);

    m_menuBar = new QMenuBar(this);
    m_menuBar->addMenu(m_menu);

    m_toolLayout = new QHBoxLayout;
    m_toolLayout->addWidget(m_menuBar);
    m_toolLayout->addWidget(m_toolBar);

    if (isRus) translateToRu();

    m_timeLayout = new QHBoxLayout;

    m_numberOfMines = new QLabel(tr("The number of mines not found: ") + QString::number(m_field->getM()));

    m_time = new Clock(this);
    connect(m_field, &Field::finished, m_time, &Clock::stop);
    connect(this, &MainWindow::startTimer, m_time, &Clock::start);
    emit startTimer();

    m_timeLayout->addWidget(m_time);
    m_timeLayout->addWidget(m_numberOfMines);

    m_screen->addLayout(m_toolLayout);
    m_screen->addLayout(m_timeLayout);
    m_screen->addWidget(m_field);
    m_field->show();

    setLayout(m_screen);
}

void MainWindow::setCount(int x){
    delete m_numberOfMines;
    m_numberOfMines = new QLabel(tr("The number of mines not found: ") + QString::number(x));
    m_timeLayout->addWidget(m_numberOfMines);
}

void MainWindow::newGame()
{
    int32_t x = m_field->getX();
    int32_t y = m_field->getY();
    int32_t m = m_field->getM();

    delete m_numberOfMines;
    m_numberOfMines = new QLabel(tr("The number of mines not found: ") + QString::number(m));
    m_timeLayout->addWidget(m_numberOfMines);

    delete m_field;
    m_field = new Field(x, y, m, this);

    connect(m_field, &Field::getCount, this, &MainWindow::setCount);
    connect(m_peeping, &QAction::triggered, m_field, &Field::lookMines);
    connect(m_leftHanded, &QAction::triggered, m_field, &Field::setLeftHand);

    emit startTimer();

    layout()->addWidget(m_field);
}

void MainWindow::newCstomGame()
{
    NewGameDialog dialog;
    dialog.exec();
    int32_t m = dialog.getM();

    delete m_numberOfMines;
    m_numberOfMines = new QLabel(tr("The number of mines not found: ") + QString::number(m));
    m_timeLayout->addWidget(m_numberOfMines);

    delete m_field;
    m_field = new Field(dialog.getX(),  dialog.getY(), m, this);

    connect(m_field, &Field::getCount, this, &MainWindow::setCount);
    connect(m_peeping, &QAction::triggered, m_field, &Field::lookMines);
    connect(m_leftHanded, &QAction::triggered, m_field, &Field::setLeftHand);

    emit startTimer();

    layout()->addWidget(m_field);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int32_t newSide = qMax(newSize.width(), newSize.height());
    resize(newSide, newSide);
    QWidget::resizeEvent(event);
}

void MainWindow::translateToEng()
{
  m_translator.load(":/ENGtranslation.qm");
  qApp->installTranslator(&m_translator);

  m_isRus = false;
  m_field->setSave();

  setWindowTitle(tr("Minesweeper"));

  m_menu->setTitle(QString("Settings"));
  m_newGame->setText("New Game");
  m_newCustomGame->setText("New Custome Game");
  m_leftHanded->setText("Left-Handed");

  if (m_peek) {
      m_peeping->setText("Peek");
  }
}

void MainWindow::translateToRu()
{
  m_translator.load(":/RUtranslation.qm");
  qApp->installTranslator(&m_translator);

  m_isRus = true;
  m_field->setSave();

  setWindowTitle("Сапёр");

  m_menu->setTitle(QString("Настройки"));
  m_newGame->setText("Новая игра");
  m_newCustomGame->setText("Новая игра с выбором настроек");
  m_leftHanded->setText("Режим левши");

  if (m_peek) {
      m_peeping->setText("Подсмотреть мины");
  }
}
