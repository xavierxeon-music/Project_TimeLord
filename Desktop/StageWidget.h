#ifndef StageWidgetH
#define StageWidgetH

#include "AbstractWidget.h"

class QItemSelectionModel;

namespace Stage
{
   class Model;

   class Widget : public Abstract::Widget
   {
      Q_OBJECT
   public:
      Widget(MainWidget* mainWidget);

   private slots:
      void slotInsertPoint();
      void slotRemovePoint();
      void slotMoveBack();
      void slotMoveForward();
      void slotLockGraphSize();

   private:
      void selectionChanged(Core::Identifier newIdentifier) override;

   private:
      Model* stageModel;
      Core::Identifier identifier;
      uint8_t selectedStageIndex;
   };
} // namespace Stage

#endif // StageWidgetH
