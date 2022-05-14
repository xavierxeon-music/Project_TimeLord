#include "ModelDivision.h"

#include "Core.h"

Model::Division::Division(QObject* parent)
   : QStandardItemModel(parent)
{
   auto addDivisionItem = [&](const Tempo::Division& division)
   {
      QStandardItem* item = new QStandardItem();

      const std::string name = Tempo::getName(division);
      item->setText(QString::fromStdString(name));
      item->setData(QVariant::fromValue(division), Core::Role::Data);

      invisibleRootItem()->appendRow(item);
   };

   addDivisionItem(Tempo::Division::Sixteenth);
   addDivisionItem(Tempo::Division::Eigth);
   addDivisionItem(Tempo::Division::Quarter);
   addDivisionItem(Tempo::Division::Bar);
}
