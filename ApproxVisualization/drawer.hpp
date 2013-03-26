#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <cmath>
#include <cstdlib>

class Drawer : public QWidget
{
    Q_OBJECT
public:
    explicit Drawer(QWidget *parent = 0);
        
public slots:
    void paintEvent(QPaintEvent *);
    void reset();

public:
    void addPoint(QPoint p);
    void addPoint(int x, int y);

private:
    QVector<QPoint> pts;
};

#endif // DRAWER_HPP
