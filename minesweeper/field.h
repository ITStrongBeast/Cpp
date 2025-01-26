#ifndef FIELD_H
# define FIELD_H
# include <QWidget>
#include <QRandomGenerator>
#include <dot.h>
class Dot;

//! игровое поле
class Field :public QWidget {
  Q_OBJECT
public:
    Field(int rows, int columns, int mines, QWidget *parent = 0);
    Field(const QString &fileName, QWidget *parent = 0);

    void setParameters(int rows, int columns, int mines);
    void setSave() {m_saved = false;}

    int getX() {return m_n; };
    int getY() {return n_m; };
    int getM() {return m_mineNumber; };

protected:
    void openingRadius(Dot *button);
    void openAll();
    bool isValidCoord(int i, int j);
    void openDot(int i, int j);
    bool isWin();
    bool m_isGameActive;
    void mine_Setter(int i, int j);
    bool one_M;

public slots:
    void setLeftHand();
    void lookMines();
    void saveGame(bool isRus);
    void on_DotClickedLeft();
    void on_DotClickedRight();
    void on_DotClickedMiddle();

signals:
    void finished();
    void getCount(int x);

private:
    int m_n;
    int n_m;
    int m_mineNumber;
    int m_count;
    bool m_saved = false;
    bool m_leftHand = false;
    QLabel *m_numberOfMines;
    QVBoxLayout *m_screen;
    QGridLayout *layout;
    Dot*** m_field;
    void createField();
};

#endif // FIELD_H
