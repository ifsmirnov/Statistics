#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

#include <QPushButton>
#include <QVector>
#include <QLineEdit>

#include <QWidget>

class Optimizer : public QWidget
{
    Q_OBJECT
public:
    explicit Optimizer(const QVector<QPoint> &pts,
                       QVector<double> factor,
                       QVector<double> lbound,
                       QWidget *parent = 0);

public slots:
    void iterate();
    void updateValues();
    void useInputValues();
    void reset();

private:
    double likelihood(double x, double y, double r);
    double likelihood(const QVector<double> &values);
    double likelihoodDim1(const QVector<double> &values, double r);
    std::pair<double, double> guessNormal(const QVector<double> &a) const;
    double onePointLikelihood(double pointX, double pointY, double guessX, double guessY, double guessR);

    double fromRealValue(int i, double x) const;
    double toRealValue(int i, double x) const;

private:
    const QVector<QPoint> &pts;

    QPushButton *recalcButton;
    QPushButton *iterateButton;
    QPushButton *resetButton;
    QLineEdit *curVal, *resX, *resY, *resR;
    
    QVector<double> value;
    QVector<double> factor;
    QVector<double> lbound;
    double step;
};

#endif // OPTIMIZER_HPP
