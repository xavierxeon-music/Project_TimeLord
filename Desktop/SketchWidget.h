#ifndef SketchWidgetH
#define SketchWidgetH

#include <QDialog>

#include <QTreeView>

namespace Sketch
{
   class Model;

   class Widget : public QDialog
   {
      Q_OBJECT
   public:
      Widget(QWidget* widget, Model* model);

   private slots:
      void slotLoadFromFile();
      void slotSaveNewFile();
      void slotItemDoubleClicked(const QModelIndex& index);

   private:
      Model* model;
      QTreeView* stateTree;
   };
} // namespace Sketch

#endif // NOT SketchWidgetH
