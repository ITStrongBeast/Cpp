#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "field.h"
#include "newgamedialog.h"
#include "clock.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(bool p, QWidget *parent = nullptr);
    void printCount(int x);

signals:
    void save();
    void startTimer();

public slots:
    void setCount(int x);
    void newGame();
    void newCstomGame();
    void translateToEng();
    void translateToRu();

protected:
    void closeEvent(QCloseEvent *event) override { m_field->saveGame(m_isRus); }
    void resizeEvent(QResizeEvent *event) override;

private:
    bool m_peek;
    bool m_isRus;
    Field *m_field;
    Clock *m_time;
    QLabel *m_numberOfMines;
    QAction *m_newCustomGame, *m_newGame, *m_peeping,
    *m_englishTranslation, *m_russianTranslation, *m_leftHanded;
    QSpinBox *m_n, *n_m, *m_mineNumber;
    QSettings m_settings;
    QVBoxLayout *m_screen;
    QHBoxLayout *m_toolLayout, *m_timeLayout;
    QMenu *m_menu;
    QToolBar *m_toolBar;
    QMenuBar *m_menuBar;
    QTranslator m_translator;
};

#endif	  // MAINWINDOW_H
