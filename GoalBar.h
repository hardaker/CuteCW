#ifndef GOALBAR_H
#define GOALBAR_H

#include <QProgressBar>

class GoalBar : public QProgressBar
{
public:
    GoalBar(QWidget *parent = 0);
    GoalBar(QWidget *parent = 0, int goalBarLevel = -1);

    void setGoalBarLevel(int newlevel);

protected:
    virtual void 	paintEvent ( QPaintEvent *event );

private:
    int m_goalBarLevel;
};

#endif // GOALBAR_H
