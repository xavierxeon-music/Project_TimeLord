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
      Model(MainWidget* mainWidget);

   signals:
      void signalGraphLengthChanged(const Data::Identifier& identifier);

   public:
      void rebuild(const Data::Identifier& identifier, bool lengthChanged);

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

   private:
      void update(PolyRamp* polyRamp, const uint8_t& stageIndex);
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Stage

#endif // StageModelH
