#include "newgamedialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Custom Game"));

    m_screen = new QVBoxLayout(this);


    m_label1 = new QLabel(tr("Size Column: "));
    m_screen->addWidget(m_label1);

    m_n = new QSpinBox(this);
    m_n->setRange(2, 100);
    m_screen->addWidget(m_n);


    m_label2 = new QLabel(tr("Size Row: "));
    m_screen->addWidget(m_label2);

    n_m = new QSpinBox(this);
    n_m->setRange(2,100);
    m_screen->addWidget(n_m);


    m_label3 = new QLabel(tr("Number of Mines: "));
    m_screen->addWidget(m_label3);

    m_numberOfMines = new QSpinBox(this);
    m_numberOfMines->setRange(1, calculateMaxMines(m_n->value(), n_m->value()));
    m_screen->addWidget(m_numberOfMines);


    m_end = new QPushButton(tr("Start"));
    m_screen->addWidget(m_end);

    QObject::connect(m_n, QOverload< int >::of(&QSpinBox::valueChanged), this, &NewGameDialog::updateMineRange);
    QObject::connect(n_m, QOverload< int >::of(&QSpinBox::valueChanged), this, &NewGameDialog::updateMineRange);
    QObject::connect(m_end, &QPushButton::clicked, this, &QDialog::accept);
}

void NewGameDialog::updateMineRange()
{
    int32_t maxMines = calculateMaxMines(m_n->value(), n_m->value());
    m_numberOfMines->setMaximum(maxMines);
}
