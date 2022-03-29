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
      Widget(MainWidget* mainWidget, Model* polyLineModel);

   public slots:
      void slotGraphSelected(const Data::Provider& newProvider, const uint8_t& newGraphIndex);

   private slots:
      void slotInsertPoint();
      void slotRemovePoint();
      void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

   private:
      void setSelection(const uint& stageIndex);

   private:
      Model* polyLineModel;
      QItemSelectionModel* selectionModel;

      Data::Provider provider;
      uint8_t graphIndex;
      uint8_t selectedStageIndex;
   };
} // namespace PolyLine

#endif // NOT PolyLineWidgetH
