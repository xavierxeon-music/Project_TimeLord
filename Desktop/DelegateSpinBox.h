#ifndef SpinBoxDelegateH
#define SpinBoxDelegateH

#include <QStyledItemDelegate>
#include "DataCore.h"

class SpinBoxDelegate : public QStyledItemDelegate, public DataCore
{
   Q_OBJECT

public:
   SpinBoxDelegate(QObject* parent, MainWidget* mainWidget, const Model::Target& target);

public:
   QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
   void setEditorData(QWidget* editor, const QModelIndex& index) const override;
   void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
   void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
   const Model::Target target;
};

#endif // NOT SpinBoxDelegateH
