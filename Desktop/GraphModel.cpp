#include "GraphModel.h"

#include "MainWidget.h"

GraphModel::GraphModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
{
}

void GraphModel::update()
{
   clear();
   setHorizontalHeaderLabels({"name", "length", "division", "countt"});

   const PoviderNameMap& nameMap = getProviderNames();
   for (PoviderNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      const Model::Provider provider = it.key();

      QStandardItem* parentItem = new QStandardItem(it.value());
      parentItem->setEditable(false);

      QStandardItem* blankItem1 = new QStandardItem("");
      blankItem1->setEditable(false);

      QStandardItem* blankItem2 = new QStandardItem("");
      blankItem2->setEditable(false);

      invisibleRootItem()->appendRow({parentItem, blankItem1, blankItem2});

      for (uint8_t graphIndex = 0; graphIndex < 16; graphIndex++)
      {
         Graph* graph = getGraph(provider, graphIndex);

         QStandardItem* nameItem = new QStandardItem();
         {
            QString name = QString::number(graphIndex + 1);
            if (1 == name.length())
               name = QString("0") + name;

            nameItem->setText("Port " + name);
            nameItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            nameItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
            nameItem->setEditable(false);
         }

         QStandardItem* lengthItem = new QStandardItem();
         {
            const QString length = QString::number(graph->getLength());
            lengthItem->setText(length);
            lengthItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            lengthItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         }

         QStandardItem* stepSizeItem = new QStandardItem();
         {
            const std::string stepSize = Tempo::getName(graph->getStepSize());
            stepSizeItem->setText(QString::fromStdString(stepSize));
            stepSizeItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            stepSizeItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
            stepSizeItem->setData(QVariant::fromValue(graph->getStepSize()), Model::Role::Data);
         }

         QStandardItem* countItem = new QStandardItem();
         {
            const QString count = QString::number(graph->stageCount());
            countItem->setText(count);
            countItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            countItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
            countItem->setEditable(false);
         }

         parentItem->appendRow({nameItem, lengthItem, stepSizeItem, countItem});
      }
   }
}
