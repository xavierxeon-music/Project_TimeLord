#ifndef PointModelH
#define PointModelH

#include "DataCore.h"
#include <QStandardItemModel>

class PointModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   PointModel(MainWidget* mainWidget);

public slots:
   void slotPortChanged(const uint8_t& index);
   void slotInsertPoint();
   void slotRemovePoint();
   void slotMoveBack();
   void slotMoveForward();
   void slotPointSelected(const uint8_t& index);

private:
   void update();

private:
   uint8_t portIndex;
   uint8_t pointIndex;
};

#endif // PointModelH
