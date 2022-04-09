#ifndef StageModelH
#define StageModelH

#include "DataCore.h"
#include <QStandardItemModel>

namespace Stage
{
   class Model : public QStandardItemModel, public Data::Core
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   private:
      struct Items
      {
         QStandardItem* indexItem;
         QStandardItem* startPosItem;
         QStandardItem* lengthItem;
         QStandardItem* startHeigthItem;

         Items(Model* model, const Data::Identifier& identifier);
         Items(Model* model, const int& row);
      };
      friend class Widget;

   private:
      void rebuildModel(const Data::Identifier& identifier) override;
      void update(PolyRamp* polyRamp, const uint8_t& stageIndex);
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Stage

#endif // StageModelH
