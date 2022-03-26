#ifndef StageModelH
#define StageModelH

#include "DataCore.h"
#include <QStandardItemModel>

class StageModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   StageModel(MainWidget* mainWidget);

public slots:
   void slotPortChanged(const Model::Provider newProvider, const uint8_t& newGraphIndex);
   void slotInsertPoint();
   void slotRemovePoint();
   void slotMoveBack();
   void slotMoveForward();
   void slotPointSelected(const uint8_t& index);

public:
   void update();

private:
   Model::Provider provider;
   uint8_t graphIndex;
   uint8_t selectedStageIndex;
};

#endif // StageModelH
