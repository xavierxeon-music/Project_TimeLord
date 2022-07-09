#ifndef SketchWidgetH
#define SketchWidgetH

#include "ui_SketchWidget.h"
#include <QDialog>

namespace Sketch
{
   class Model;

   class Widget : public QDialog, private Ui::SketchWidget
   {
      Q_OBJECT
   public:
      Widget(QWidget* widget, Model* model);
   };
} // namespace Sketch

#endif // NOT SketchWidgetH
