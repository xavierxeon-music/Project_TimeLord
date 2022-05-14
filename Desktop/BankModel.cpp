#include "BankModel.h"

#include "BankContent.h"

Bank::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Core::Interface()
{
   setHorizontalHeaderLabels({"number", "bpm"});
}

void Bank::Model::modelHasChanged(Core::Identifier identifier)
{
}

void Bank::Model::rebuildModel(Core::Identifier)
{
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
