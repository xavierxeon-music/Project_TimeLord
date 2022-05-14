#ifndef DelegateSpinBoxH
#define DelegateSpinBoxH

#include <QStyledItemDelegate>
#include "Core.h"

namespace Delegate
{
   class SpinBox : public QStyledItemDelegate, public Core::Interface
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
