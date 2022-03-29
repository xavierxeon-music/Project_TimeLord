#ifndef PolyRampModelH
#define PolyRampModelH

#include "DataCore.h"
#include <QStandardItemModel>

class PolyRampModel : public QStandardItemModel, public Data::Core
{
   Q_OBJECT
public:
   PolyRampModel(MainWidget* mainWidget);

public slots:
   void slotGraphLengthChanged(const Data::Provider& provider, const uint8_t& graphIndex);

public:
   void rebuild();

private:
   bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};

#endif // PolyRampModelH
