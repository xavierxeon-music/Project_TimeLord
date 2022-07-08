#include "DeviceStateWidget.h"

#include "DeviceStateModel.h"

DeviceState::Widget::Widget(QWidget* widget, Model* model)
   : QDialog(widget)
{
   setupUi(this);

   infoLabel->setText(model->compileInfo());

   stateTree->setModel(model);
   connect(discardButton, &QPushButton::clicked, this, &QDialog::reject);
   connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
}
