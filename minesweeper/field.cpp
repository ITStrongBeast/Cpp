#include "field.h"
#include "dot.h"
#include <QGridLayout>
#include <QMessageBox>

bool Field::isValidCoord(int i, int j) {
    return i >= 0 && j >= 0 && i < m_n && j < n_m;
}

bool Field::isWin() {
    int n = m_n * n_m - m_mineNumber;
    for (int i = 0; i < m_n; ++i){
        for (int j = 0; j < n_m; ++j){
            n -= m_field[i][j]->isOpen();
        }
    }
    return 0 == n;
}

void Field::openDot(int i, int j) {
    if (false == isValidCoord(i, j)) return;
    Dot *p = m_field[i][j];
    if (p->isOpen())
        return;
    p->setOpen();
    if (p->value()) return;
    openDot(i - 1, j); openDot(i + 1, j);
    openDot(i, j - 1); openDot(i, j + 1);
    openDot(i - 1, j - 1); openDot(i - 1, j + 1);
    openDot(i + 1, j - 1); openDot(i + 1, j + 1);
}

void Field::on_DotClickedRight() {
    if (m_leftHand) {
        m_leftHand = false;
        on_DotClickedLeft();
        m_leftHand = true;
        return;
    }
    if (false == m_isGameActive) return;
    Dot *t = reinterpret_cast<Dot*>(sender());
    if (t->isOpen()) return;
    t->swapFlag();
    if (t->isFlag()){
        --m_count;
    } else if (t->isQuestion()) {
        ++m_count;
    }
    emit getCount(m_count);
    if(isWin()) {
        m_isGameActive = false;
        m_saved = true;
        emit finished();
        QMessageBox::information(0, tr("WIN"), tr("You have won!"));
        openAll();
    }
}

void Field::openAll() {
    emit finished();
    for (int i = 0; i < m_n; ++i)
      for (int j = 0; j < n_m; ++j)
         m_field[i][j]->setOpen();
}

void Field::setLeftHand() {
    m_leftHand = !m_leftHand;
}

void Field::on_DotClickedLeft() {
    if(m_leftHand) {
        m_leftHand = false;
        on_DotClickedRight();
        m_leftHand = true;
        return;
    }
    if (false == m_isGameActive)
        return;
    Dot *t = reinterpret_cast<Dot*>(sender());
    if (t->isFlag() || t->isQuestion()) {
        return;
    }
    if (false == one_M) {
        one_M = true;
        mine_Setter(t->i(), t->j());
        openDot(t->i(), t->j());
        if (isWin()) {
            m_isGameActive = false;
            m_saved = true;
            emit finished();
            QMessageBox::information(0, tr("WIN"), tr("You have won!"));
            openAll();
        }
        return;
    }
    if (t->isOpen()) return;
    if (t->isMine()) {
        m_isGameActive = false;
        m_saved = true;
        emit finished();
        t->setStyleSheet("background-color: rgb(200, 0, 0);");
        QMessageBox::warning(nullptr, tr("LOSE"), tr("You've lost"));
        openAll();
        return;
    }
    openDot(t->i(), t->j());
    if (isWin()) {
        m_isGameActive = false;
        m_saved = true;
        emit finished();
        QMessageBox::information(0, tr("WIN"), tr("You have won!"));
        openAll();
    }
}

void Field::mine_Setter(int a, int b) {
    for (int i = 0; i < m_mineNumber;) {
      int x = QRandomGenerator::global()->bounded(m_n);
      int y = QRandomGenerator::global()->bounded(n_m);
      Dot *p = m_field[x][y];
      if (p->isMine())
        continue;
      if (x == a && y == b)
        continue;
      else {
        p->setMine();
        ++i;
      }
    }

    for (int i = 0; i < m_n; ++i) {
      for (int j = 0; j < m_n; ++j) {
        if (m_field[i][j]->isMine()) {
          if (isValidCoord(i - 1, j - 1)) m_field[i - 1][j - 1]->incValue();
          if (isValidCoord(i - 1, j)) m_field[i - 1][j]->incValue();
          if (isValidCoord(i - 1, j + 1)) m_field[i - 1][j + 1]->incValue();
          if (isValidCoord(i, j - 1)) m_field[i][j - 1]->incValue();
          if (isValidCoord(i, j + 1)) m_field[i][j + 1]->incValue();
          if (isValidCoord(i + 1, j - 1)) m_field[i + 1][j - 1]->incValue();
          if (isValidCoord(i + 1, j)) m_field[i + 1][j]->incValue();
          if (isValidCoord(i + 1, j + 1)) m_field[i + 1][j + 1]->incValue();
        }
      }
    }
}

void Field::on_DotClickedMiddle()
{
    if (!m_isGameActive) return;
    Dot *t = qobject_cast< Dot * >(sender());
    if (!t->isOpen()) return;
    int32_t i = t->i();
    int32_t j = t->j();
    int32_t counter = t->value();
    for (int32_t x = i - 1; x <= i + 1; ++x)
    {
        for (int32_t y = j - 1; y <= j + 1; ++y)
        {
            if (isValidCoord(x, y))
            {
                if (m_field[x][y]->isFlag()) {
                    --counter;
                }
            }
        }
    }
    if (counter != 0) {
        openingRadius(t);
        return;
    }
    for (int32_t x = i - 1; x <= i + 1; ++x)
    {
        for (int32_t y = j - 1; y <= j + 1; ++y)
        {
            if (isValidCoord(x, y) && !m_field[x][y]->isFlag()) {
                openDot(x, y);
                if (m_field[x][y]->isMine()) {
                    m_saved = true;
                    m_isGameActive = false;
                    emit finished();
                    t->setStyleSheet("background-color: rgb(200, 0, 0);");
                    QMessageBox::warning(nullptr, tr("LOSE"), tr("You've lost"));
                    openAll();
                    return;
                }
            }
        }
    }
    if(isWin()) {
        m_saved = true;
        m_isGameActive = false;
        emit finished();
        QMessageBox::information(0, tr("WIN"), tr("You have won!"));
        openAll();
    }
}

void Field::openingRadius(Dot *button)
{
    int32_t i = button->i();
    int32_t j = button->j();
    QVector< Dot * > close;
    for (int8_t di = -1; di < 2; di++)
    {
        for (int8_t dj = -1; dj < 2; dj++)
        {
            if (0 <= i + di && i + di < m_n && 0 <= j + dj && j + dj < n_m &&
                !(m_field[i + di][j + dj]->isOpen() || m_field[i + di][j + dj]->isFlag()))
            {
                m_field[i + di][j + dj]->setStyleSheet("border: 2px solid green; font-size: 20px; padding: 0px");
                close.push_back(m_field[i + di][j + dj]);
            }
        }
    }
    QTimer::singleShot(
        500,
        this,
        [close]()
        {
            for (Dot *b : close)
            {
                b->setText("");
                b->setStyleSheet("font-size: 20px; padding: 0px;");
            }
        });
}

void Field::createField(){
    m_field = new Dot **[m_n];
    for (int32_t i = 0; i < m_n; ++i)
    {
        m_field[i] = new Dot *[n_m];
        for (int32_t j = 0; j < n_m; ++j)
        {
            m_field[i][j] = new Dot(i, j, this);
        }
    }

    layout = new QGridLayout(this);
    layout->setSpacing(0);

    for (int32_t i = 0; i < m_n; ++i)
    {
        for (int32_t j = 0; j < n_m; ++j)
        {
            layout->addWidget(m_field[i][j], i, j, 1, 1);
            connect(m_field[i][j], SIGNAL(clicked_left()), this, SLOT(on_DotClickedLeft()));
            connect(m_field[i][j], SIGNAL(clicked_right()), this, SLOT(on_DotClickedRight()));
            connect(m_field[i][j], SIGNAL(clicked_middle()), this, SLOT(on_DotClickedMiddle()));
        }
    }
}

Field::Field(int rows, int columns, int mines, QWidget *parent)
  : QWidget(parent) {
    m_isGameActive = true;
    m_n = rows;
    n_m = columns;
    m_mineNumber = mines;
    m_count = mines;
    one_M = false;

    createField();
}

Field::Field(const QString &fileName, QWidget *parent) : QWidget(parent) {
    m_isGameActive = true;
    one_M = true;

    QSettings settings(fileName, QSettings::IniFormat);

    settings.beginGroup("Field");
    m_n = settings.value("m_n").toInt();
    n_m = settings.value("n_m").toInt();
    m_mineNumber = settings.value("m_mineNumber").toInt();
    settings.endGroup();

    createField();

    for (int i = 0; i < m_n; ++i)
    {
        for (int j = 0; j < n_m; ++j)
        {
            Dot *dot = m_field[i][j];
            settings.beginGroup(QString("Dot_%1_%2").arg(i).arg(j));
            dot->setIsMine(settings.value("isMine").toBool());
            dot->setIsOpen(settings.value("isOpen").toBool());
            dot->setIsFlag(settings.value("isFlag").toBool());
            dot->setValue(settings.value("value").toInt());
            settings.endGroup();
            dot->setEnabled(true);
        }
    }

    setLayout(layout);
}

void Field::setParameters(int rows, int columns, int mines) {
    m_n = rows;
    n_m = columns;
    m_mineNumber = mines;
}

void Field::lookMines()
{
    if (m_saved) {
        return;
    }
    for (int32_t i = 0; i < m_n; ++i)
    {
        for (int32_t j = 0; j < n_m; ++j)
        {
            if (m_field[i][j]->isMine() && !(m_field[i][j]->isFlag())) {
                if (m_field[i][j]->isOpen()) {
                    m_field[i][j]->setStyleSheet("font-size: 20px; padding: 0px");
                } else {
                    m_field[i][j]->setStyleSheet("background-color: rgb(200, 0, 0); font-size: 20px; padding: 0px");
                }
                m_field[i][j]->setOpen();
            }
        }
    }
}

void Field::saveGame(bool isRus)
{
    QSettings settings("save.ini", QSettings::IniFormat);

    settings.beginGroup("Field");
    settings.setValue("flag", false);
    settings.setValue("m_n", m_n);
    settings.setValue("n_m", n_m);
    settings.setValue("m_mineNumber", m_mineNumber);
    if (isRus)
    {
        settings.setValue("isRus", true);
    }
    else
    {
        settings.setValue("isRus", false);
    }
    settings.endGroup();

    for (int i = 0; i < m_n; ++i)
    {
        for (int j = 0; j < n_m; ++j)
        {
            settings.beginGroup(QString("Dot_%1_%2").arg(i).arg(j));
            settings.setValue("isMine", m_field[i][j]->isMine());
            settings.setValue("isOpen", m_field[i][j]->isOpen());
            settings.setValue("isFlag", m_field[i][j]->isFlag());
            settings.setValue("value", m_field[i][j]->value());
            settings.setValue("i", m_field[i][j]->i());
            settings.setValue("j", m_field[i][j]->j());
            settings.endGroup();
        }
    }

    if (m_saved)
        settings.clear();
}
