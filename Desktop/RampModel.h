#ifndef RampModelH
#define RampModelH

#include "DataCore.h"
#include <QStandardItemModel>

namespace Ramp
{
   class Model : public QStandardItemModel, public Data::Core
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   public:
      void modelHasChanged(const Data::Identifier& identifier) override;

   private:
      friend class Widget;

   private:
      void rebuildModel(const Data::Identifier& identifier = Data::Identifier()) override;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Ramp

#endif // RampModelH
