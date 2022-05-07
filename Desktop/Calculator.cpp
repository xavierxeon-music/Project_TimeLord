#include "Calculator.h"

#include <Music/Tempo.h>

#include "DataCore.h"
#include "ModelDivision.h"

Calculator::Calculator(QWidget* parent)
   : QDialog(parent)
{
   setupUi(this);

   tempoSpin->setValue(120);
   connect(tempoSpin, &QSpinBox::valueChanged, this, &Calculator::slotUpdate);

   divisionCombo->setModel(new Model::Division(this));
   divisionCombo->setCurrentIndex(3);
   connect(divisionCombo, &QComboBox::currentIndexChanged, this, &Calculator::slotUpdate);

   connect(minutesSpin, &QSpinBox::valueChanged, this, &Calculator::slotUpdate);
   connect(secondsSpin, &QSpinBox::valueChanged, this, &Calculator::slotUpdate);

   connect(closeButton, &QAbstractButton::clicked, this, &QDialog::accept);
}

void Calculator::slotUpdate()
{
   const float beatsPerMinute = tempoSpin->value(); // == quarter / min
   const float sixteenthPerSecond = (4 * beatsPerMinute) / 60.0;

   const Tempo::Division division = divisionCombo->currentData(Data::Role::Data).value<Tempo::Division>();
   const float sixteenthPerDivision = static_cast<float>(division);

   const float divisionsPerSecond = sixteenthPerSecond / sixteenthPerDivision;
   const float seconds = (60 * minutesSpin->value()) + secondsSpin->value();

   const float divisionCount = seconds * divisionsPerSecond;

   const QString text = QString::number(static_cast<uint32_t>(divisionCount));
   resultEdit->setText(text);
}
