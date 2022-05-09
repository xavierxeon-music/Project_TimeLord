#include "BankModel.h"

Bank::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()
{
   setHorizontalHeaderLabels({"number", "bpm"});
}

void Bank::Model::modelHasChanged(Data::Identifier identifier)
{
}
