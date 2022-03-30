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
      Widget(MainWidget* mainWidget, Model* stageModel);

   public slots:
      void slotGraphSelected(const Data::Identifier& newIdentifier);

   private slots:
      void slotInsertPoint();
      void slotRemovePoint();
      void slotMoveBack();
      void slotMoveForward();
      void slotLockGraphSize();
      void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

   private:
      void setSelection(const uint& stageIndex);

   private:
      Model* stageModel;
      QItemSelectionModel* selectionModel;

      Data::Identifier identifier;
      uint8_t selectedStageIndex;
   };
} // namespace Stage

#endif // StageWidgetH
