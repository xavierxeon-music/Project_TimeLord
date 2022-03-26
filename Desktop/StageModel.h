#ifndef StageModelH
#define StageModelH

#include "DataCore.h"
#include <QStandardItemModel>

class StageModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   StageModel(MainWidget* mainWidget);

signals:
   void signalGraphLengthChanged(uint8_t graphIndex);

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
   bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

private:
   Model::Provider provider;
   uint8_t graphIndex;
   uint8_t selectedStageIndex;
};

#endif // StageModelH
