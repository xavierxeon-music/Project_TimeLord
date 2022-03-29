#ifndef StageWidgetH
#define StageWidgetH

#include "AbstractWidget.h"

class StageModel;
class QItemSelectionModel;

class StageWidget : public AbstractWidget
{
   Q_OBJECT
public:
   StageWidget(MainWidget* mainWidget, StageModel* stageModel);

public slots:
   void slotGraphSelected(const Data::Provider& newProvider, const uint8_t& newGraphIndex);

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
   StageModel* stageModel;
   QItemSelectionModel* selectionModel;

   Data::Provider provider;
   uint8_t graphIndex;
   uint8_t selectedStageIndex;
};

#endif // StageWidgetH
