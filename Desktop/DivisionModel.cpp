#include "DivisionModel.h"

#include <Music/Tempo.h>

#include "DataCore.h"

DivisionModel::DivisionModel(QObject* parent)
   : QStandardItemModel(parent)
{
   auto addDivisionItem = [&](const Tempo::Division& division)
   {
      QStandardItem* item = new QStandardItem();

      const std::string name = Tempo::getName(division);
      item->setText(QString::fromStdString(name));
      item->setData(QVariant::fromValue(division), Data::Role::Data);

      invisibleRootItem()->appendRow(item);
   };

   addDivisionItem(Tempo::Division::Sixteenth);
   addDivisionItem(Tempo::Division::Eigth);
   addDivisionItem(Tempo::Division::Quarter);
   addDivisionItem(Tempo::Division::Bar);
}
