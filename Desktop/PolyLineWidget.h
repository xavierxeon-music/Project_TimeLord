#ifndef PolyLineWidgetH
#define PolyLineWidgetH

#include "AbstractWidget.h"

class QItemSelectionModel;

namespace PolyLine
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
      void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

   private:
      void setSelection(const uint& stageIndex);
      void polyRampSelected(Data::Identifier newIdentifier) override;
      void modelHasChanged(Data::Identifier identifier) override;

   private:
      Model* polyLineModel;
      QItemSelectionModel* selectionModel;

      Data::Identifier identifier;
      uint8_t selectedStageIndex;
   };
} // namespace PolyLine

#endif // NOT PolyLineWidgetH
