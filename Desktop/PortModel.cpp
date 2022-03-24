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

   const PoviderNameMap& nameMap = getProviderNames();
   for (PoviderNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      const Provider provider = it.key();

      QStandardItem* parentItem = new QStandardItem(it.value());
      parentItem->setEditable(false);
      invisibleRootItem()->appendRow(parentItem);

      for (uint8_t index = 0; index < 16; index++)
      {
         const QString name = "Port " + QString::number(index + 1);
         QStandardItem* item = new QStandardItem(name);
         item->setData(QVariant::fromValue(provider));
         item->setEditable(false);
         parentItem->appendRow(item);
      }
   }
}
