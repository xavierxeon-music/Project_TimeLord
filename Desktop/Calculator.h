#ifndef CalculatorH
#define CalculatorH

#include "ui_Calculator.h"
#include <QWidget>

class Calculator : public QDialog, private Ui::Calculator
{
   Q_OBJECT

public:
   Calculator(QWidget* parent);

private slots:
   void slotUpdate();
};

#endif // CalculatorH
