#include "GraphModel.h"

#include "MainWidget.h"

GraphModel::GraphModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
{
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});
}

void GraphModel::update()
{
   clear();
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});

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
            lengthItem->setData(QVariant::fromValue(Model::Target::GraphLength), Model::Role::Target);
         }

         QStandardItem* stepSizeItem = new QStandardItem();
         {
            const std::string stepSize = Tempo::getName(graph->getStepSize());
            stepSizeItem->setText(QString::fromStdString(stepSize));
            stepSizeItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            stepSizeItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
            stepSizeItem->setData(QVariant::fromValue(graph->getStepSize()), Model::Role::Data);
            stepSizeItem->setData(QVariant::fromValue(Model::Target::GraphStepSize), Model::Role::Target);
         }

         QStandardItem* loopItem = new QStandardItem();
         {
            loopItem->setCheckable(true);
            loopItem->setCheckState(graph->isLooping() ? Qt::Checked : Qt::Unchecked);
            loopItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            loopItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
            loopItem->setData(QVariant::fromValue(Model::Target::GraphLoop), Model::Role::Target);
         }

         QStandardItem* countItem = new QStandardItem();
         {
            const QString count = QString::number(graph->stageCount());
            countItem->setText(count);
            countItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
            countItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
            countItem->setEditable(false);
         }

         parentItem->appendRow({nameItem, lengthItem, stepSizeItem, loopItem, countItem});
      }
   }
}

bool GraphModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole != role && Qt::CheckStateRole != role)
      return result;

   const QVariant targetData = data(index, Model::Role::Target);
   if (targetData.isNull())
      return result;

   const Model::Provider provider = data(index, Model::Role::Provider).value<Model::Provider>();
   const uint8_t graphIndex = data(index, Model::Role::GraphIndex).value<uint8_t>();

   Graph* graph = getGraph(provider, graphIndex);

   const Model::Target target = targetData.value<Model::Target>();
   if (Model::Target::GraphLength == target)
   {
      const uint8_t length = value.toInt();
      graph->setLength(length);
   }
   else if (Model::Target::GraphStepSize == target)
   {
      const Tempo::Division stepSize = value.value<Tempo::Division>();
      const std::string name = Tempo::getName(stepSize);
      result = QStandardItemModel::setData(index, QString::fromStdString(name), Qt::EditRole);

      graph->setStepSize(stepSize);
   }
   else if (Model::Target::GraphLoop == target)
   {
      bool loopOn = value.toBool();
      graph->setLooping(loopOn);
   }

   return result;
}
