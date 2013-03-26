#include "optimizer.hpp"

#include <QVector>
#include <QPushButton>
#include <QBoxLayout>
#include <QString>
#include <QTextEdit>
#include <cmath>
#include <iostream>
#include <QLabel>

Optimizer::Optimizer(const QVector<QPoint> &pts,
                     QVector<double> factor,
                     QVector<double> lbound,
                     QWidget *parent) :
     QWidget(parent), pts(pts), factor(factor), lbound(lbound)
{
    reset();

    iterateButton = new QPushButton("Iterate", this);
    recalcButton = new QPushButton("Recalc", this);
    resetButton = new QPushButton("Reset", this);
    curVal = new QLineEdit(this);
    resX = new QLineEdit(this);
    resY = new QLineEdit(this);
    resR = new QLineEdit(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(recalcButton);
    layout->addWidget(curVal);
    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    layout->addWidget(frame);
    layout->addWidget(iterateButton);
    layout->addWidget(new QLabel("X"));
    layout->addWidget(resX);
    layout->addWidget(new QLabel("Y"));
    layout->addWidget(resY);
    layout->addWidget(new QLabel("R"));
    layout->addWidget(resR);
    layout->addWidget(resetButton);

    setLayout(layout);

    QObject::connect(iterateButton, SIGNAL(clicked()), this, SLOT(iterate()));
    QObject::connect(recalcButton, SIGNAL(clicked()), this, SLOT(useInputValues()));
    QObject::connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    updateValues();
}

std::pair<double, double> Optimizer::guessNormal(const QVector<double> &a) const {
    double s = 0, s2 = 0;
    for (auto i: a) {
        s += i;
        s2 += i*i;
    }
    s /= a.size();
    s2 /= a.size();
    return std::make_pair(s, s2 - s*s);
}

double Optimizer::likelihood(double x, double y, double r) {
    QVector<double> a;
    double coeff = -1;
    double sx = 0, sy = 0;
    double s2 = 0;
    for (auto i: pts) {
        sx += i.x();
        sy += i.y();
        double dx = i.x() - x;
        double dy = i.y() - y;
        double d = dx*dx + dy*dy;
        s2 += d/(r*r);

        a.push_back(sqrt(d) * coeff);
        coeff *= -1;
    }
    ///------
    return likelihoodDim1(a, r);
    ///------
    s2 /= pts.size();
    sx /= pts.size();
    sy /= pts.size();
    double d = ((sx-x)*(sx-x) + (sy-y)*(sy-y)) / (r*r);

    double p2 = s2 - d;
    return d + (p2-1)*(p2-1);

    /*std::pair<double, double> guessed = guessNormal(a);
    return guessed.first * guessed.first + (guessed.second - 1) * (guessed.second - 1);*/
}

double Optimizer::likelihoodDim1(const QVector<double> &values, double r) {
    double s = values.size() * 0.5 * log(2 * M_PI * r * r);
    for (auto i: values) {
        s += log(exp(i*i / (2*r*r)));
    }
    return s;
}

double Optimizer::likelihood(const QVector<double> &values) {
    return likelihood(toRealValue(0, values[0]),
            toRealValue(1, values[1]),
            toRealValue(2, values[2]));
}

double Optimizer::fromRealValue(int i, double x) const {
    return (x - lbound[i]) / factor[i];
}

double Optimizer::toRealValue(int i, double x) const {
    return x * factor[i] + lbound[i];
}

void Optimizer::iterate() {
    while (step > 1e-15) {
    double bestval = likelihood(value);
    QVector<double> bestans = value;
    for (int i = 0; i < value.size(); i++) {
        for (double cf = -1; cf < 2; cf += 2) {
            QVector<double> t = value;
            t[i] += cf * step;
            if (t[i] > 1 || t[i] < 0)
                continue;
            double curval = likelihood(t);
            if (curval < bestval) {
                bestval = curval;
                bestans = t;
            }
        }
    }
    value = bestans;
    step *= 0.9;
    updateValues();
    }
}

void Optimizer::updateValues() {
    double optX = toRealValue(0, value[0]);
    double optY = toRealValue(1, value[1]);
    double optR = toRealValue(2, value[2]);
    curVal->setText(QString::number(likelihood(optX, optY, optR)));
    resX->setText(QString::number(optX));
    resY->setText(QString::number(optY));
    resR->setText(QString::number(optR));
}
void Optimizer::useInputValues() {
    value[0] = fromRealValue(0, resX->text().toDouble());
    value[1] = fromRealValue(1, resY->text().toDouble());
    value[2] = fromRealValue(2, resR->text().toDouble());
    updateValues();
}

void Optimizer::reset() {
    value = QVector<double>(factor.size(), 0.5);
    step = 0.4;
}
