#ifndef StageModelH
#define StageModelH

#include "DataCore.h"
#include <QStandardItemModel>

class StageModel : public QStandardItemModel, public Data::Core
{
   Q_OBJECT
public:
   StageModel(MainWidget* mainWidget);

signals:
   void signalGraphLengthChanged(const Data::Provider& provider, const uint8_t& graphIndex);

public:
   void rebuild(const Data::Provider provider, const uint8_t& graphIndex, bool lengthChanged);

private:
   void update(PolyRamp* polyRamp, const uint8_t& stageIndex);
   bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};

#endif // StageModelH
