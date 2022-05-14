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
      void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

   private:
      void setSelection(const uint& stageIndex);
      void polyRampSelected(Core::Identifier newIdentifier) override;

   private:
      Model* stageModel;
      QItemSelectionModel* selectionModel;

      Core::Identifier identifier;
      uint8_t selectedStageIndex;
   };
} // namespace Stage

#endif // StageWidgetH
