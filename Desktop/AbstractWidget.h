#ifndef AbstractWidgetH
#define AbstractWidgetH

#include "Core.h"
#include <QWidget>

#include <QToolBar>
#include <QVBoxLayout>

class QItemSelectionModel;
class QStandardItemModel;
class QTreeView;

class MainWidget;

namespace Abstract
{
   class Widget : public QWidget, public Core::Interface
   {
      Q_OBJECT
   public:
      Widget(MainWidget* mainWidget);

   protected:
      QTreeView* addTreeView(QStandardItemModel* model);
      void addPayload(QWidget* widget);
      void setSelection(const uint& row);

   protected:
      QToolBar* toolBar;
      QTreeView* treeView;
      QItemSelectionModel* selectionModel;

   private slots:
      void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

   private:
      QStandardItemModel* model;
      QVBoxLayout* masterLayout;
      Core::Identifier selectionIdentifier;
   };
} // namespace Abstract

#endif // AbstractWidgetH
