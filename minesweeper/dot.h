#ifndef DOT_H
# define DOT_H
# include <QPushButton>

#include <QGridLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSettings>
#include <QSpinBox>
#include <QTimer>
#include <QToolBar>
#include <QTranslator>
#include <QApplication>
#include <QPainter>

class Dot : public QPushButton {
    Q_OBJECT
public:
    void highlight();
    Dot(int i, int j, QWidget *parent = 0);
    int i();
    int j();
    bool isMine();
    void setMine();
    void incValue();
    int value();
    void setOpen();
    bool isOpen();
    void swapFlag();
    bool isFlag();
    bool isQuestion();
    bool isHighligth();

    void setIsMine(bool isMine) {m_isMine = isMine; }
    void setIsOpen(bool isOpen) {m_isOpen = isOpen; }
    void setIsFlag(bool isFlag) {m_isFlag = isFlag; }
    void setValue(int value) {m_value = value; }
    void setI(int i) {m_i = i; }
    void setJ(int j) {m_j = j; }

signals:
    void clicked_left();
    void clicked_right();
    void clicked_middle();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    int m_i, m_j;
    int m_value;
    bool m_isMine;
    bool m_isOpen = false;
    bool m_isFlag = false;
    bool m_isQuestion = false;
    bool m_isHighligth;
};

#endif // DOT_H
