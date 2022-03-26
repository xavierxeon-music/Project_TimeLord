#ifndef GraphModelH
#define GraphModelH

#include "DataCore.h"
#include <QStandardItemModel>

class GraphModel : public QStandardItemModel, public DataCore
{
   Q_OBJECT
public:
   GraphModel(MainWidget* mainWidget);

public:
   void update();

private:
   bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
};

#endif // GraphModelH
