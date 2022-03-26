#include "ComboBoxDelegate.h"

#include <QComboBox>
#include <QStandardItemModel>

#include <Music/Tempo.h>

ComboBoxDelegate::ComboBoxDelegate(QObject* parent, MainWidget* mainWidget, const Model::Target& target)
   : QStyledItemDelegate(parent)
   , DataCore(mainWidget)
   , target(target)
{
   model = new QStandardItemModel(this);

   if (Model::Target::GraphStepSize == target)
   {
      auto addDivisionItem = [&](const Tempo::Division& division)
      {
         QStandardItem* item = new QStandardItem();

         const std::string name = Tempo::getName(division);
         item->setText(QString::fromStdString(name));
         item->setData(QVariant::fromValue(division), Model::Role::Data);

         model->invisibleRootItem()->appendRow(item);
      };

      addDivisionItem(Tempo::Division::Sixteenth);
      addDivisionItem(Tempo::Division::Eigth);
      addDivisionItem(Tempo::Division::Quarter);
      addDivisionItem(Tempo::Division::Bar);
   }
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QComboBox* comboBox = new QComboBox(parent);
   comboBox->setModel(model);

   return comboBox;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   int value = index.model()->data(index, Qt::EditRole).toInt();

   QComboBox* comboBox = static_cast<QComboBox*>(editor);
   const QVariant& itemData = index.model()->data(index, Model::Role::Data);

   const int comboIndex = comboBox->findData(itemData, Model::Role::Data);
   comboBox->setCurrentIndex(comboIndex);
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QComboBox* comboBox = static_cast<QComboBox*>(editor);
   const QVariant data = comboBox->currentData(Model::Role::Data);

   // TODO get value?

   const QVariant itemDataProvider = index.model()->data(index, Model::Role::Provider);
   const Model::Provider provider = itemDataProvider.value<Model::Provider>();
   const int graphIndex = index.model()->data(index, Model::Role::GraphIndex).toInt();

   ComboBoxDelegate* unConstMe = const_cast<ComboBoxDelegate*>(this);

   if (Model::Target::GraphStepSize == target)
   {
      const Tempo::Division stepSize = data.value<Tempo::Division>();
      const std::string name = Tempo::getName(stepSize);
      model->setData(index, QString::fromStdString(name), Qt::EditRole);

      Graph* graph = unConstMe->getGraph(provider, graphIndex);
      graph->setStepSize(stepSize);
   }
}

void ComboBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
