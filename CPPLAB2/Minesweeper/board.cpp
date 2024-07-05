#include "board.h"

void Board::createBoard()
{
	board.resize(sizeRow);
	for (uint16_t i = 0; i < sizeRow; i++)
	{
		board[i].resize(sizeColumn);
	}

	table = new QGridLayout;

    table->setSpacing(0);

	for (uint16_t i = 0; i < sizeRow; i++)
	{
		for (uint16_t j = 0; j < sizeColumn; j++)
		{
			Cell *b = new Cell(i, j, this);
			board[i][j] = b;
			table->addWidget(b, i, j);
			QObject::connect(b, &Cell::pressLeft, this, &Board::leftClick);
			QObject::connect(b, &Cell::pressRight, this, &Board::rightClick);
			QObject::connect(b, &Cell::pressMid, this, &Board::midClick);
		}
	}
}

void Board::recurOpen(uint16_t x, uint16_t y)
{
	Cell *b = board[x][y];
	b->view = true;
	countOpen++;
	if (b->countMine > 0)
	{
		b->setText(QString::number(b->countMine));
		b->setStyleSheet("color: darkblue;");
		QFont font = b->font();
		font.setPointSize(16);
		font.setBold(true);
		b->setFont(font);
	}
	else
	{
		b->setStyleSheet("background-color: rgb(100, 100, 100);");
		b->setText("");
		b->setEnabled(false);
		for (int8_t i = -1; i < 2; i++)
		{
			for (int8_t j = -1; j < 2; j++)
			{
				if (0 <= x + i && x + i < sizeRow && 0 <= y + j && y + j < sizeColumn && !(board[x + i][y + j]->view))
				{
					recurOpen(x + i, y + j);
				}
			}
		}
	}
}

void Board::startBoard()
{
	int32_t countMine = 0;

	while (countMine != numberMines)
	{
		int32_t i = QRandomGenerator::global()->bounded(sizeRow);
		int32_t j = QRandomGenerator::global()->bounded(sizeColumn);
		if (!(board[i][j]->isMine) && !(board[i][j]->view))
		{
			board[i][j]->isMine = true;
			countMine++;
		}
	}

	for (int32_t i = 0; i < sizeRow; i++)
	{
		for (int32_t j = 0; j < sizeColumn; j++)
		{
			if (board[i][j]->isMine)
			{
				continue;
			}
			int32_t count = 0;
			for (int8_t di = -1; di < 2; di++)
			{
				for (int8_t dj = -1; dj < 2; dj++)
				{
					if (0 <= i + di && i + di < sizeRow && 0 <= j + dj && j + dj < sizeColumn && board[i + di][j + dj]->isMine)
					{
						count++;
					}
				}
			}
			board[i][j]->countMine = count;
		}
	}
}

void Board::win()
{
	for (int32_t i = 0; i < sizeRow; i++)
	{
		for (int32_t j = 0; j < sizeColumn; j++)
		{
			board[i][j]->setEnabled(false);
			if (!(board[i][j]->view))
			{
                board[i][j]->setText("|>");
				board[i][j]->setStyleSheet("color: red;");
				QFont font = board[i][j]->font();
                font.setPointSize(12);
				font.setBold(true);
				board[i][j]->setFont(font);
			}
		}
	}
	saved = true;
    QMessageBox::information(nullptr, tr("WIN"), tr("You have won!!!"));
}

void Board::lose(Cell *button)
{
	for (int32_t i = 0; i < sizeRow; i++)
	{
		for (int32_t j = 0; j < sizeColumn; j++)
		{
			board[i][j]->setEnabled(false);
			if (board[i][j]->isMine)
			{
                board[i][j]->setText("M");
				QFont font = board[i][j]->font();
                font.setPointSize(12);
				font.setBold(true);
				board[i][j]->setFont(font);
				board[i][j]->setStyleSheet("background-color: rgb(150, 0, 0);");
			}
			else if (board[i][j]->countMine > 0)
			{
				board[i][j]->setText(QString::number(board[i][j]->countMine));
				QFont font = board[i][j]->font();
				font.setPointSize(16);
				font.setBold(true);
				board[i][j]->setFont(font);
				board[i][j]->setStyleSheet("color: darkblue; background-color: rgb(100, 100, 100);");
			}
			else
			{
				board[i][j]->setText("");
				board[i][j]->setStyleSheet("background-color: rgb(100, 100, 100);");
			}
		}
	}
	saved = true;
	button->setStyleSheet("background-color: rgb(200, 0, 0);");
    QMessageBox::warning(nullptr, tr("LOSE"), tr("You've lost"));
}

void Board::leftClick()
{
	Cell *button = qobject_cast< Cell * >(sender());
	if (!(button->isFlag) && button->view == false)
	{
		button->view = true;
		countOpen++;
		if (firstClick)
		{
			startBoard();
			firstClick = false;
		}
		else if (button->isMine)
		{
			lose(button);
			return;
		}
		if (button->countMine == 0)
		{
			countOpen--;
			recurOpen(button->x, button->y);
		}
		else
		{
			button->setText(QString::number(button->countMine));
			button->setStyleSheet("color: darkblue;");
			QFont font = button->font();
			font.setPointSize(16);
			font.setBold(true);
			button->setFont(font);
		}
		if (sizeRow * sizeColumn - countOpen == numberMines)
		{
			win();
		}
	}
}

void Board::rightClick()
{
	Cell *button = qobject_cast< Cell * >(sender());
	if (!(button->view) && countOpen != 0)
	{
		button->isFlag = !(button->isFlag);
		if (button->look)
		{
			return;
		}
		if (button->isFlag)
		{
            button->setText("|>");
			button->setStyleSheet("color: red;");
			QFont font = button->font();
            font.setPointSize(12);
			font.setBold(true);
			button->setFont(font);
		}
		else
		{
			button->setText("");
			button->setStyleSheet("background-color: rgb(189, 189, 189);");
		}
	}
}

void Board::openingRadius(Cell *button)
{
	int32_t i = button->x;
	int32_t j = button->y;
	QVector< Cell * > close;
	for (int8_t di = -1; di < 2; di++)
	{
		for (int8_t dj = -1; dj < 2; dj++)
		{
			if (0 <= i + di && i + di < sizeRow && 0 <= j + dj && j + dj < sizeColumn &&
				!(board[i + di][j + dj]->view || board[i + di][j + dj]->isFlag))
			{
				board[i + di][j + dj]->setStyleSheet("border: 2px solid lightgreen;");
				close.push_back(board[i + di][j + dj]);
			}
		}
	}
	QTimer::singleShot(
		500,
		this,
		[close]()
		{
			for (Cell *b : close)
			{
                if (b->look) {
                    b->setText("M");
                    QFont font = b->font();
                    font.setPointSize(12);
                    font.setBold(true);
                    b->setFont(font);
                    b->setStyleSheet("background-color: rgb(200, 0, 0);");
                } else {
                    b->setText("");
                    b->setStyleSheet("background-color: rgb(189, 189, 189);");
                }
			}
		});
}

void Board::midClick()
{
	Cell *button = qobject_cast< Cell * >(sender());
	if (button->view == true && button->countMine > 0)
	{
		int32_t count = 0;
		uint16_t i = button->x;
		uint16_t j = button->y;
		for (int8_t di = -1; di < 2; di++)
		{
			for (int8_t dj = -1; dj < 2; dj++)
			{
				if (0 <= i + di && i + di < sizeRow && 0 <= j + dj && j + dj < sizeColumn && board[i + di][j + dj]->isFlag)
				{
					count++;
				}
			}
		}
		if (count != button->countMine)
		{
			openingRadius(button);
			return;
		}
		for (int8_t di = -1; di < 2; di++)
		{
			for (int8_t dj = -1; dj < 2; dj++)
			{
				if (!(0 <= i + di && i + di < sizeRow && 0 <= j + dj && j + dj < sizeColumn && board[i + di][j + dj]->view == false))
				{
					continue;
				}
				Cell *b = board[i + di][j + dj];
				if (b->isMine)
				{
					if (b->isFlag)
					{
						continue;
					}
					else
					{
						countOpen = -1;
						lose(b);
					}
				}
				b->view = true;
				if (b->countMine > 0)
				{
					b->setText(QString::number(b->countMine));
					b->setStyleSheet("color: darkblue;");
					QFont font = b->font();
					font.setPointSize(16);
					font.setBold(true);
					b->setFont(font);
					countOpen++;
				}
				else
				{
					recurOpen(b->x, b->y);
				}
			}
		}
		if (sizeRow * sizeColumn - countOpen == numberMines)
		{
			win();
		}
	}
}

void Board::lookMines()
{
    if (saved) {
        return;
    }
	for (int32_t i = 0; i < sizeRow; i++)
	{
		for (int32_t j = 0; j < sizeColumn; j++)
		{
			if (board[i][j]->isMine)
			{
				if (board[i][j]->look)
				{
					if (board[i][j]->isFlag)
					{
                        board[i][j]->setText("|>");
						board[i][j]->setStyleSheet("color: red;");
						QFont font = board[i][j]->font();
                        font.setPointSize(12);
						font.setBold(true);
						board[i][j]->setFont(font);
					}
					else
					{
						board[i][j]->setText("");
						board[i][j]->setStyleSheet("background-color: rgb(189, 189, 189);");
					}
				}
				else
				{
                    board[i][j]->setText("M");
					QFont font = board[i][j]->font();
                    font.setPointSize(12);
					font.setBold(true);
					board[i][j]->setFont(font);
					board[i][j]->setStyleSheet("background-color: rgb(200, 0, 0);");
				}
				board[i][j]->look = !(board[i][j]->look);
			}
		}
	}
}

Board::Board(int32_t x, int32_t y, int32_t m, QWidget *parent) :
	sizeRow(x), sizeColumn(y), numberMines(m), QWidget(parent)
{
	createBoard();
	setLayout(table);
}

Board::Board(const QString &fileName)
{
	QSettings settings(fileName, QSettings::IniFormat);

	settings.beginGroup("Game");
	countOpen = settings.value("countOpen").toInt();
	firstClick = settings.value("firstClick").toBool();
	sizeRow = settings.value("sizeRow").toInt();
	sizeColumn = settings.value("sizeColumn").toInt();
	numberMines = settings.value("numberMines").toInt();
	settings.endGroup();

	createBoard();

	for (int i = 0; i < sizeRow; ++i)
	{
		for (int j = 0; j < sizeColumn; ++j)
		{
			Cell *button = board[i][j];
			settings.beginGroup(QString("Cell_%1_%2").arg(i).arg(j));
			button->isMine = settings.value("isMine").toBool();
			button->view = settings.value("view").toBool();
			button->isFlag = settings.value("isFlag").toBool();
			button->countMine = settings.value("countMine").toInt();
			button->x = settings.value("x").toInt();
			button->y = settings.value("y").toInt();
			settings.endGroup();

			if (button->view)
			{
				if (button->countMine > 0)
				{
					button->setText(QString::number(button->countMine));
					QFont font = button->font();
					font.setPointSize(16);
					font.setBold(true);
					button->setFont(font);
					button->setStyleSheet("color: darkblue; background-color: rgb(100, 100, 100);");
				}
				else
				{
					button->setText("");
					button->setStyleSheet("background-color: rgb(100, 100, 100);");
				}
			}
			else if (button->isFlag)
			{
                button->setText("|>");
				button->setStyleSheet("color: red;");
				QFont font = button->font();
                font.setPointSize(12);
				font.setBold(true);
				button->setFont(font);
			}
			else
			{
				button->setText("");
				button->setStyleSheet("background-color: rgb(189, 189, 189);");
			}
		}
	}

	setLayout(table);
}

void Board::saveGame(bool isRus)
{
	QSettings settings("save.ini", QSettings::IniFormat);

	settings.beginGroup("Game");
	settings.setValue("flag", false);
	settings.setValue("countOpen", countOpen);
	settings.setValue("firstClick", firstClick);
	settings.setValue("sizeRow", sizeRow);
	settings.setValue("sizeColumn", sizeColumn);
	settings.setValue("numberMines", numberMines);
    if (isRus)
    {
        settings.setValue("isRus", true);
    }
    else
    {
        settings.setValue("isRus", false);
    }
	settings.endGroup();

	for (int i = 0; i < sizeRow; ++i)
	{
		for (int j = 0; j < sizeColumn; ++j)
		{
			settings.beginGroup(QString("Cell_%1_%2").arg(i).arg(j));
			settings.setValue("isMine", board[i][j]->isMine);
			settings.setValue("view", board[i][j]->view);
			settings.setValue("isFlag", board[i][j]->isFlag);
			settings.setValue("countMine", board[i][j]->countMine);
			settings.setValue("x", board[i][j]->x);
			settings.setValue("y", board[i][j]->y);
			settings.endGroup();
		}
	}

	if (saved)
	{
		settings.clear();
	}
}

Board::~Board()
{
    for (QVector< Cell * > buf : board)
    {
        for (Cell *button : buf)
        {
            delete button;
        }
    }
    delete table;
}
