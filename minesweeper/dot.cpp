#include "dot.h"
#include <QPainter>
#include <QMouseEvent>

Dot::Dot(int i, int j, QWidget *parent)
  : QPushButton(parent), m_i(i), m_j(j), m_value(0), m_isMine(false), m_isHighligth(false)
  , m_isOpen(false), m_isFlag(false) {
    setStyleSheet("font-size: 20px; padding: 0px");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(32, 32);
}

int Dot::i() { return m_i; }
int Dot::j() { return m_j; }

int Dot::value() { return m_value; }
void Dot::incValue() { ++m_value; }

bool Dot::isMine() {return m_isMine; }
void Dot::setMine() { m_isMine = true; repaint(); }

bool Dot::isFlag() { return m_isFlag; }
bool Dot::isQuestion() { return m_isQuestion; }

bool Dot::isOpen() { return m_isOpen; }
void Dot::setOpen() { m_isOpen = !m_isOpen; repaint(); }

void Dot::swapFlag() {
    if (m_isFlag) {
        m_isQuestion = true;
        m_isFlag = false;
        this->setText("?");
        this->setStyleSheet("background-color: rgb(50, 200, 0); font-size: 20px; padding: 0px");
    } else if (m_isQuestion) {
        m_isQuestion = false;
        m_isFlag = false;
        this->setText("");
        this->setStyleSheet("font-size: 20px; padding: 0px");
    } else {
        m_isFlag = true;
    }
    repaint();
}

bool Dot::isHighligth() { return m_isHighligth; }
void Dot::highlight()
{
    m_isHighligth = !m_isHighligth;
    repaint();
}

/*virtual*/ void Dot::paintEvent(QPaintEvent *event) {
  if (isOpen() && isMine() == false && 0 == m_value)
    return;
  QPushButton::paintEvent(event);
  QPainter p(this);
  if (isOpen()) {
    if (isMine()) {
      p.setBrush(QBrush(Qt::red, Qt::SolidPattern));
      p.drawEllipse(2, 2, width() - 4, height() - 4);
      return;
    }
    setText(QString::number(m_value));
    return;
  }
  if (m_isFlag) {
    p.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    p.drawEllipse(2, 2, width() - 4, height() - 4);
  }
}

/*virtual*/ void Dot::mouseReleaseEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) emit clicked_left();
  if (e->button() == Qt::RightButton) emit clicked_right();
  if (e->button() == Qt::MiddleButton) emit clicked_middle();
}
