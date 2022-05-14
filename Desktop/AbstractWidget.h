#ifndef AbstractWidgetH
#define AbstractWidgetH

#include "Core.h"
#include <QWidget>

#include <QToolBar>
#include <QVBoxLayout>

class MainWidget;

namespace Abstract
{
   class Widget : public QWidget, public Core::Interface
   {
      Q_OBJECT
   public:
      Widget(MainWidget* mainWidget);

   protected:
      void addPayload(QWidget* widget);

   protected:
      QToolBar* toolBar;

   private:
      QVBoxLayout* masterLayout;
   };
} // namespace Abstract

#endif // AbstractWidgetH
