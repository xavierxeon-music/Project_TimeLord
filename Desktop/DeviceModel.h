#ifndef DeviceModelH
#define DeviceModelH

#include "DataCore.h"
#include <QStandardItemModel>

class DeviceModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   DeviceModel(MainWidget* mainWidget);

public:
   void init();
};

#endif // DeviceModelH
