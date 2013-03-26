#include "window.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QFrame>
#include <iostream>

Window::Window(QWidget *parent) :
    QWidget(parent)
{
    drawer = new Drawer(this);
    std::cerr << "Drawer created" << std::endl;

    QVector<double> factor, lbound;
    factor.push_back(300); lbound.push_back(-150);
    factor.push_back(300); lbound.push_back(-150);
    factor.push_back(100); lbound.push_back(1);
    optimizer = new Optimizer(pts, factor, lbound, this);
    std::cerr << "Optimizer created" << std::endl;

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    layout->addWidget(drawer);

    QBoxLayout *innerLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    innerLayout->setAlignment(Qt::AlignTop);

    resetButton = new QPushButton("Reset", this);
    newPointButton = new QPushButton("New point", this);
    numNewPoints = new QLineEdit("10", this);
    newPtX = new QLineEdit("0", this);
    newPtY = new QLineEdit("0", this);
    newPtR = new QLineEdit("50", this);

    innerLayout->addWidget(newPointButton);
    innerLayout->addWidget(new QLabel("Number of points"));
    innerLayout->addWidget(numNewPoints);
    innerLayout->addWidget(new QLabel("X mean"));
    innerLayout->addWidget(newPtX);
    innerLayout->addWidget(new QLabel("Y mean"));
    innerLayout->addWidget(newPtY);
    innerLayout->addWidget(new QLabel("Standard deviation"));
    innerLayout->addWidget(newPtR);
    innerLayout->addWidget(resetButton);
    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    innerLayout->addWidget(frame);

    innerLayout->addWidget(optimizer);

    layout->addLayout(innerLayout);

    setLayout(layout);

    QObject::connect(newPointButton, SIGNAL(clicked()), this, SLOT(addPointSlot()));
    QObject::connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));

}

void Window::addPoint(int x, int y) {
    drawer->addPoint(x, y);
    pts.push_back(QPoint(x, y));
}
void Window::addPoint(QPoint p) {
    drawer->addPoint(p);
    pts.push_back(p);
}
void Window::addPointSlot() {
    int x = newPtX->text().toInt();
    int y = newPtY->text().toInt();
    int r = newPtR->text().toInt();
    int n = numNewPoints->text().toInt();
    for (int i = 0; i < n; i++) {
        addRandomPoint(x, y, r);
    }
    update();
}

void Window::addRandomPoint(int x, int y, int r) {
    double t = normRandom() * r;
    double a = (double)rand()/RAND_MAX*2.*M_PI;
    double sina = sin(a), cosa = cos(a);
    addPoint(x+round(cosa*t), y+round(sina*t));
}

double Window::normRandom() {
    double s = (double)(rand()+1)/RAND_MAX;
    double t = (double)(rand()+1)/RAND_MAX;
    return sin(2*M_PI*s) * sqrt(-2.*log(t));
}

void Window::reset() {
    pts.clear();
    drawer->reset();
    update();
}
