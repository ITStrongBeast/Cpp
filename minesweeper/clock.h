#ifndef CLOCK_H
# define CLOCK_H
# include <QLCDNumber>
# include <QTime>

class Clock :public QLCDNumber {
    Q_OBJECT
public:
  Clock(QWidget *parent = 0);

protected:
  QTime m_time;
  QTimer *m_timer;

public slots:
  void on_tick();
  void stop();
  void start();
};

#endif // CLOCK_H
