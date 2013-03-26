#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QPushButton>
#include <QLineEdit>

#include <cmath>
#include <cstdlib>

#include "drawer.hpp"
#include "optimizer.hpp"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = 0);

public:
    void addPoint(int x, int y);
    void addPoint(QPoint p);
    void addRandomPoint(int x, int y, int r);

public slots:
    void addPointSlot();
    void reset();

private:
    static double normRandom();

private:
    Drawer *drawer;
    Optimizer *optimizer;
    QVector<QPoint> pts;

    QPushButton *newPointButton, *resetButton;
    QLineEdit *numNewPoints, *newPtX, *newPtY, *newPtR;
};

#endif // WINDOW_HPP
