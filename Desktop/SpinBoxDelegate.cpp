#include "SpinBoxDelegate.h"

#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject* parent, MainWidget* mainWidget)
   : QStyledItemDelegate(parent)
   , DataCore(mainWidget)
{
}

QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QSpinBox* editor = new QSpinBox(parent);
   editor->setFrame(false);
   editor->setMinimum(0);
   editor->setMaximum(100);

   return editor;
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

   model->setData(index, value, Qt::EditRole);

   const QVariant itemDataProvider = index.model()->data(index, Model::Role::Provider);
   const Model::Provider provider = itemDataProvider.value<Model::Provider>();
   const int graphIndex = index.model()->data(index, Model::Role::GraphIndex).toInt();

   SpinBoxDelegate* unConstMe = const_cast<SpinBoxDelegate*>(this);

   const Model::Target target = index.model()->data(index, Model::Role::Target).value<Model::Target>();
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
