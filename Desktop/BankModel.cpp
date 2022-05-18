#include "BankModel.h"

#include "BankContent.h"

Bank::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Core::Interface()
{
   setHorizontalHeaderLabels({"number", "bpm"});
}

void Bank::Model::rebuildModel(Core::Identifier identifier)
{
   Q_UNUSED(identifier)

   clear();
   setHorizontalHeaderLabels({"number", "bpm"});

   for (uint8_t bankIndex = 0; bankIndex < getBankCount(); bankIndex++)
   {
      Core::Identifier identifier(bankIndex);
      const Bank::Content* bankContent = getBank(identifier);

      QStandardItem* nameItem = new QStandardItem();
      {
         QString name = QString::number(bankIndex + 1);
         if (1 == name.length())
            name = QString("0") + name;

         nameItem->setText(name);
         nameItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         nameItem->setData(QVariant::fromValue(Core::Target::BankName), Core::Role::Target);
      }

      QStandardItem* tempoItem = new QStandardItem();
      {
         const QString tempoText = QString::number(bankContent->getBeatsPerMinute());
         tempoItem->setText(tempoText);

         tempoItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         tempoItem->setData(QVariant::fromValue(Core::Target::BankTempo), Core::Role::Target);
      }

      invisibleRootItem()->appendRow({nameItem, tempoItem});
   }
}

bool Bank::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   if (Qt::EditRole != role)
      return QStandardItemModel::setData(index, value, role);

   const QVariant targetData = data(index, Core::Role::Target);
   if (targetData.isNull())
      return QStandardItemModel::setData(index, value, role);

   const Core::Identifier identifier = data(index, Core::Role::Identifier).value<Core::Identifier>();
   Bank::Content* bankContent = getBank(identifier);

   QVariant targeValue = value;
   const Core::Target::Value target = targetData.value<Core::Target::Value>();

   if (Core::Target::BankTempo == target)
   {
      const uint8_t bpm = value.toInt();
      bankContent->setBeatsPerMinute(bpm);
      callOnAllInstances(&Interface::modelHasChanged, identifier);

      setModified();
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   return result;
}
