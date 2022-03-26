#include "SpinBoxDelegate.h"

#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject* parent, MainWidget* mainWidget, const Model::Target& target)
   : QStyledItemDelegate(parent)
   , DataCore(mainWidget)
   , target(target)
{
}

QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QSpinBox* spinBox = new QSpinBox(parent);
   spinBox->setFrame(false);
   spinBox->setMinimum(0);
   spinBox->setMaximum(100);

   return spinBox;
}

void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   int value = index.model()->data(index, Qt::EditRole).toInt();

   QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
   spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
   spinBox->interpretText();
   const uint8_t value = spinBox->value();


   const QVariant itemDataProvider = index.model()->data(index, Model::Role::Provider);
   const Model::Provider provider = itemDataProvider.value<Model::Provider>();
   const int graphIndex = index.model()->data(index, Model::Role::GraphIndex).toInt();

   SpinBoxDelegate* unConstMe = const_cast<SpinBoxDelegate*>(this);
   model->setData(index, value, Qt::EditRole);

   if (Model::Target::GraphLength == target)
   {
      Graph* graph = unConstMe->getGraph(provider, graphIndex);
      graph->setLength(value);
   }
}

void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
