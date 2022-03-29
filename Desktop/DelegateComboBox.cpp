#include "DelegateComboBox.h"

#include <QComboBox>
#include <QStandardItemModel>

Delegate::ComboBox::ComboBox(QObject* parent, MainWidget* mainWidget, QStandardItemModel* model)
   : QStyledItemDelegate(parent)
   , Data::Core(mainWidget)
   , model(model)
{
}

QWidget* Delegate::ComboBox::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(option)
   Q_UNUSED(index)

   QComboBox* comboBox = new QComboBox(parent);
   comboBox->setModel(model);

   return comboBox;
}

void Delegate::ComboBox::setEditorData(QWidget* editor, const QModelIndex& index) const
{
   QComboBox* comboBox = static_cast<QComboBox*>(editor);
   const QVariant& itemData = index.model()->data(index, Data::Role::Data);

   const int comboIndex = comboBox->findData(itemData, Data::Role::Data);
   comboBox->setCurrentIndex(comboIndex);
}

void Delegate::ComboBox::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
   QComboBox* comboBox = static_cast<QComboBox*>(editor);
   const QVariant data = comboBox->currentData(Data::Role::Data);

   model->setData(index, data, Qt::EditRole);
}

void Delegate::ComboBox::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   Q_UNUSED(index)

   editor->setGeometry(option.rect);
}
