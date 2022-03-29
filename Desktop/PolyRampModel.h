#ifndef PolyRampModelH
#define PolyRampModelH

#include "DataCore.h"
#include <QStandardItemModel>

class PolyRampModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   PolyRampModel(MainWidget* mainWidget);

public slots:
   void slotGraphLengthChanged(const Model::Provider& provider, const uint8_t& graphIndex);

public:
   void rebuild();

private:
   bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};

#endif // PolyRampModelH
