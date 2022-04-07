#ifndef DelegateSpinBoxH
#define DelegateSpinBoxH

#include <QStyledItemDelegate>
#include "DataCore.h"

namespace Delegate
{
   class SpinBox : public QStyledItemDelegate, public Data::Core
   {
      Q_OBJECT

   public:
      SpinBox(QObject* parent);

   public:
      QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
      void setEditorData(QWidget* editor, const QModelIndex& index) const override;
      void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
      void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

   };
} // namespace Delegate

#endif // NOT DelegateSpinBoxH
