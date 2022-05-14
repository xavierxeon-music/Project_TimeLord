#ifndef DelegateComboBoxH
#define DelegateComboBoxH

#include "Core.h"
#include <QStyledItemDelegate>

class QStandardItemModel;

namespace Delegate
{
   class ComboBox : public QStyledItemDelegate, public Core::Interface
   {
      Q_OBJECT

   public:
      ComboBox(QObject* parent, QStandardItemModel* model);

   public:
      QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
      void setEditorData(QWidget* editor, const QModelIndex& index) const override;
      void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
      void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

   private:
      QStandardItemModel* model;
   };
} // namespace Delegate

#endif // NOT DelegateComboBoxH
