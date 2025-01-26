#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include "field.h"

class NewGameDialog : public QDialog
{
    Q_OBJECT

  private:
    QSpinBox *m_n, *n_m, *m_numberOfMines;
    QVBoxLayout *m_screen;
    QLabel *m_label1, *m_label2, *m_label3;
    QPushButton *m_end;

    int32_t calculateMaxMines(int32_t x, int32_t y) const { return x * y - 2; }

  public:
    NewGameDialog(QWidget *parent = nullptr);

    int32_t getX() const { return m_n->value(); }

    int32_t getY() const { return n_m->value(); }

    int32_t getM() const { return m_numberOfMines->value(); }

  private slots:
    void updateMineRange();

};

#endif // NEWGAMEDIALOG_H
