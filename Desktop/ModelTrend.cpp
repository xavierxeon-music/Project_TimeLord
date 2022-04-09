#include "ModelTrend.h"

#include "DataCore.h"

Model::Trend::Trend(QObject* parent)
   : QStandardItemModel(parent)
{
   auto addTrendItem = [&](const Data::Type::Value& type)
   {
      QStandardItem* item = new QStandardItem();

      item->setText(Data::Type::getName(type));
      item->setIcon(Data::Type::getIcon(type));
      item->setData(QVariant::fromValue(type), Data::Role::Data);

      invisibleRootItem()->appendRow(item);
   };

   addTrendItem(Data::Type::Anchor);
   addTrendItem(Data::Type::Rise);
   addTrendItem(Data::Type::Fall);
   addTrendItem(Data::Type::Stable);
   addTrendItem(Data::Type::Step);
}

