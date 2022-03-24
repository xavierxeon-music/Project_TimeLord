#include "PortModel.h"

#include "MainWidget.h"

PortModel::PortModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
{
}

void PortModel::init()
{
   clear();

   const TimeLord::GraphList_& graphList = getGraphList();
   for (uint8_t index = 0; index < graphList.getSize(); index++)
   {
      const QString name = "Port " + QString::number(index + 1);
      QStandardItem* item = new QStandardItem(name);
      item->setEditable(false);
      invisibleRootItem()->appendRow(item);
   }
}
