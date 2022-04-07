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

   public slots:
      void slotRampChanged(const Data::Identifier& identifier);

   public:
      void rebuild();

   private:
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Ramp

#endif // RampModelH
