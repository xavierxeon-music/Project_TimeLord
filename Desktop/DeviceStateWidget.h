#ifndef DeviceStateWidgetH
#define DeviceStateWidgetH

#include "ui_DeviceStateWidget.h"
#include <QDialog>

namespace DeviceState
{
   class Model;

   class Widget : public QDialog, private Ui::DeviceStateWidget
   {
      Q_OBJECT
   public:
      Widget(QWidget* widget, Model* model);
   };
} // namespace DeviceState

#endif // NOT DeviceStateWidgetH
