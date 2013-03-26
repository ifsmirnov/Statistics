#include "drawer.hpp"

#include <QPainter>

#include <iostream>

Drawer::Drawer(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(600, 600);
    setMaximumSize(600, 600);
}

void Drawer::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());

    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);

    for (auto i: pts) {
        painter.drawEllipse(i, 2, 2);
    }
}

void Drawer::addPoint(QPoint p) {
    pts.push_back(p + QPoint(300, 300));
}

void Drawer::addPoint(int x, int y) {
    addPoint(QPoint(x, y));
}

void Drawer::reset() {
    pts.clear();
}
