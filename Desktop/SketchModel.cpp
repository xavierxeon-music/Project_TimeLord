#include "SketchModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include "Target.h"

Sketch::Model::Model(QObject* parent, Target* target)
   : QStandardItemModel(parent)
   , Core::Interface()
   , currentState()
   , stateList()
   , bank(nullptr)

{
   connect(target, &Target::signalNewState, this, &Model::slotNewState);

   setHorizontalHeaderLabels({"number", "start pos", "time"});

   Core::Identifier dummy;
   bank = getBank(dummy);
}

void Sketch::Model::loadFromFile(const QString& fileName)
{
   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray data = file.readAll();
   file.close();

   stateList.clear();
   currentState.map.clear();

   clear();
   setHorizontalHeaderLabels({"number", "start pos", "time"});

   QJsonDocument doc = QJsonDocument::fromJson(data);

   QJsonArray stateArray = doc.array();
   for (const QJsonValue& stateValue : stateArray)
   {
      QJsonObject stateObject = stateValue.toObject();

      currentState.position = stateObject["position"].toInt();

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

void Sketch::Model::saveToFile(const QString& fileName)
{
   QJsonArray stateArray;
   for (const State& state : stateList)
   {
      QJsonObject stateObject;
      stateObject["position"] = static_cast<qint64>(state.position);
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

   QJsonDocument doc(stateArray);
   const QByteArray data = doc.toJson();

   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly))
      return;

   file.write(data);
   file.close();
}

void Sketch::Model::applyToBanks()
{
   if (stateList.count() < 2)
      return;

   for (size_t stateIndex = 1; stateIndex < stateList.count(); stateIndex++)
   {
      const State state1 = stateList.at(stateIndex - 1);
      const State state2 = stateList.at(stateIndex);

      const uint8_t stageLength = state2.position - state1.position;

      Core::Identifier identifier;
      for (identifier.bankIndex = 0; identifier.bankIndex < getBankCount(); identifier.bankIndex++)
      {
         for (identifier.rampIndex = 0; identifier.rampIndex < 8; identifier.rampIndex++)
         {
            PolyRamp* polyRamp = getPolyRamp(identifier);
            if (1 == stateIndex)
            {
               polyRamp->clear();
               const uint32_t rampLength = stateList.last().position;
               polyRamp->setStepSize(Tempo::Bar);
               polyRamp->setLength(rampLength);
               polyRamp->setLooping(false); // ???
               polyRamp->addStage(0, stateList.count() - 1);
            }

            const uint8_t stageIndex = stateIndex - 1;
            const uint8_t startHeight = state1.map.at(identifier.bankIndex).at(identifier.rampIndex);
            const uint8_t endHeight = state2.map.at(identifier.bankIndex).at(identifier.rampIndex);

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

   QStandardItem* timeItem = item(row, index.column() + 1);
   const uint32_t position = value.toInt();
   const QString timeText = compileTime(bank, Tempo::Bar, position);
   timeItem->setText(timeText);

   stateList[row].position = position;

   return result;
}

void Sketch::Model::createCurrentStateItems()
{
   const uint16_t index = invisibleRootItem()->rowCount();

   QStandardItem* numberItem = new QStandardItem();
   numberItem->setText(QString::number(index));
   numberItem->setEditable(false);

   QStandardItem* posItem = new QStandardItem();
   posItem->setText(QString::number(currentState.position));

   const uint32_t position = currentState.position;
   const QString timeText = compileTime(bank, Tempo::Bar, position);

   QStandardItem* timeItem = new QStandardItem();
   timeItem->setText(timeText);
   timeItem->setEditable(false);

   invisibleRootItem()->appendRow({numberItem, posItem, timeItem});
}
