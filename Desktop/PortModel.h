#ifndef PortModelH
#define PortModelH

#include "DataCore.h"
#include <QStandardItemModel>

class PortModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   PortModel(MainWidget* mainWidget);

public:
   void init();
};

#endif // PortModelH
