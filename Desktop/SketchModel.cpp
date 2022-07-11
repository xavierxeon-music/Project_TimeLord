#include "SketchModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include <FileSettings.h>

#include "Target.h"

// state

Sketch::Model::State::State()
   : name()
   , position(0)
   , map()
{
}

// mdoel

Sketch::Model::Model(QObject* parent, Target* target)
   : QStandardItemModel(parent)
   , Core::Interface()
   , currentState()
   , stateList()
   , bank(nullptr)

{
   connect(target, &Target::signalNewState, this, &Model::slotNewState);
   connect(this, &Model::signalSendState, target, &Target::slotSendState);

   setHorizontalHeaderLabels({"number", "name", "start pos", "time"});

   Core::Identifier dummy;
   bank = getBank(dummy);
}

void Sketch::Model::loadFromFile()
{
   FileSettings settings;
   const QJsonArray stateArray = settings.array("sketch");

   for (const QJsonValue& stateValue : stateArray)
   {
      QJsonObject stateObject = stateValue.toObject();

      currentState.position = stateObject["position"].toInt();
      currentState.name = stateObject["name"].toString();

      uint8_t counter = 0;
      QString key = QString::number(counter);
      while (stateObject.contains(key))
      {
         State::Values values;
         QJsonArray valueArray = stateObject[key].toArray();
         for (const QJsonValueRef& value : valueArray)
            values.push_back(value.toInt());

         currentState.map[counter] = values;

         counter++;
         key = QString::number(counter);
      }

      stateList.append(currentState);
      createCurrentStateItems();
      currentState.map.clear();
   }
}

void Sketch::Model::saveToFile()
{
   QJsonArray stateArray;
   for (const State& state : stateList)
   {
      QJsonObject stateObject;
      stateObject["position"] = static_cast<qint64>(state.position);
      stateObject["name"] = state.name;

      for (State::Map::const_iterator it = state.map.cbegin(); it != state.map.cend(); it++)
      {
         QJsonArray valueArray;
         for (uint8_t value : it->second)
            valueArray.append(value);

         const QString key = QString::number(it->first);
         stateObject[key] = valueArray;
      }

      stateArray.append(stateObject);
   }

   FileSettings settings;
   settings.write("sketch", stateArray);
}

void Sketch::Model::applyToBanks()
{
   if (stateList.count() < 2)
      return;

   const size_t stateCount = static_cast<size_t>(stateList.count());

   struct Ramp
   {
      size_t startIndex;
      size_t endIndex;

      using List = QList<Ramp>;
   };

   Ramp::List rampList;
   for (size_t stateIndex = 1; stateIndex < stateCount; stateIndex++)
   {
      const State state1 = stateList.at(stateIndex - 1);
      const State state2 = stateList.at(stateIndex);

      const int32_t stageLength = state2.position - state1.position;
      if (stageLength > 0)
      {
         rampList.append({stateIndex - 1, stateIndex});
      }
   }

   const size_t stageCount = rampList.count();
   for (size_t stageIndex = 0; stageIndex < stageCount; stageIndex++)
   {
      const State state1 = stateList.at(rampList.at(stageIndex).startIndex);
      const State state2 = stateList.at(rampList.at(stageIndex).endIndex);

      const uint8_t stageLength = state2.position - state1.position;
      Core::Identifier identifier;
      for (identifier.bankIndex = 0; identifier.bankIndex < getBankCount(); identifier.bankIndex++)
      {
         for (identifier.rampIndex = 0; identifier.rampIndex < 8; identifier.rampIndex++)
         {
            PolyRamp* polyRamp = getPolyRamp(identifier);
            if (0 == stageIndex)
            {
               polyRamp->clear();
               const uint32_t rampLength = stateList.last().position;
               polyRamp->setStepSize(Tempo::Bar);
               polyRamp->setLength(rampLength);
               polyRamp->setLooping(false); // ???
               polyRamp->addStage(0, stageCount);
            }

            const uint8_t startHeight = state1.map.at(identifier.bankIndex).at(identifier.rampIndex);
            const uint8_t endHeight = state2.map.at(identifier.bankIndex).at(identifier.rampIndex);

            //qDebug() << stageIndex << stageLength << startHeight << endHeight;
            polyRamp->setStageLength(stageIndex, stageLength);
            polyRamp->setStageStartHeight(stageIndex, startHeight);
            polyRamp->setStageEndHeight(stageIndex, endHeight);
         }
      }
   }

   callOnAllInstances(&Interface::rebuildModel, Core::Identifier());
}

QString Sketch::Model::compileInfo() const
{
   QString info;
   info += QString::number(getBankCount()) + " banks";
   info += " @ " + QString::number(bank->getBeatsPerMinute()) + " bpm";

   return info;
}

void Sketch::Model::sendItemState(const QModelIndex& index)
{
   const uint16_t row = index.row();
   const State state = stateList.at(row);

   for (State::Map::const_iterator it = state.map.cbegin(); it != state.map.cend(); it++)
   {
      QJsonArray valueArray;
      for (uint8_t value : it->second)
         valueArray.append(value);

      QJsonObject stateObject;
      stateObject["bankIndex"] = it->first;
      stateObject["values"] = valueArray;

      emit signalSendState(stateObject);
   }
}

void Sketch::Model::slotNewState(const QJsonObject& stateObject)
{
   uint8_t bankIndex = stateObject["bankIndex"].toInt();
   if (bankIndex < 0 || bankIndex >= getBankCount())
      return;

   State::Values values;
   const QJsonArray valueArray = stateObject["values"].toArray();
   for (const QJsonValue& value : valueArray)
      values.push_back(value.toInt());

   currentState.map[bankIndex] = values;
   if (currentState.map.size() < getBankCount())
      return;

   if (!stateList.isEmpty())
      currentState.position = stateList.last().position + 1;

   stateList.append(currentState);
   createCurrentStateItems();

   currentState.map.clear();
}

bool Sketch::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole != role && Qt::CheckStateRole != role)
      return result;

   const uint16_t row = index.row();
   const uint8_t column = index.column();

   if (1 == column) // name
   {
      const QString name = value.toString();
      stateList[row].name = name;
   }
   else if (2 == column) // position
   {
      const uint32_t position = value.toInt();
      stateList[row].position = position;

      const QString timeText = compileTime(bank, Tempo::Bar, position);
      QStandardItem* timeItem = item(row, column + 1);
      timeItem->setText(timeText);
   }
   return result;
}

void Sketch::Model::createCurrentStateItems()
{
   const uint16_t index = invisibleRootItem()->rowCount();

   QStandardItem* numberItem = new QStandardItem();
   numberItem->setText(QString::number(index));
   numberItem->setEditable(false);

   QStandardItem* nameItem = new QStandardItem();
   nameItem->setText(currentState.name);

   QStandardItem* posItem = new QStandardItem();
   const uint32_t position = currentState.position;
   posItem->setText(QString::number(position));

   QStandardItem* timeItem = new QStandardItem();
   const QString timeText = compileTime(bank, Tempo::Bar, position);
   timeItem->setText(timeText);
   timeItem->setEditable(false);

   invisibleRootItem()->appendRow({numberItem, nameItem, posItem, timeItem});
}
