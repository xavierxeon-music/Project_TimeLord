#include "DelegateSpinBox.h"

#include <QSpinBox>

Delegate::SpinBox::SpinBox(QObject* parent, const uint32_t& maxValue)
   : QStyledItemDelegate(parent)
   , Core::Interface()
   , maxValue(maxValue)
{
}

QWidget* Delegate::SpinBox::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QSpinBox* spinBox = new QSpinBox(parent);
   spinBox->setFrame(false);
   spinBox->setMinimum(0);
   spinBox->setMaximum(maxValue);

   return spinBox;
}

void Delegate::SpinBox::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   int value = index.model()->data(index, Qt::EditRole).toInt();

   QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
   spinBox->setValue(value);
}

void Delegate::SpinBox::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
   spinBox->interpretText();
   const uint32_t value = spinBox->value();

   model->setData(index, value, Qt::EditRole);
}

void Delegate::SpinBox::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
