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
   void signalGraphLengthChanged(const Model::Provider& provider, const uint8_t& graphIndex);

public:
   void rebuild(const Model::Provider provider, const uint8_t& graphIndex, bool lengthChanged);

private:
   bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};

#endif // StageModelH
