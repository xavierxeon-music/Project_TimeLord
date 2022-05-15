#ifndef RampModelH
#define RampModelH

#include "Core.h"
#include <QStandardItemModel>

namespace Ramp
{
   class Model : public QStandardItemModel, public Core::Interface
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   public:
      void modelHasChanged(Core::Identifier identifier) override;

   private:
      friend class Widget;

   private:
      void rebuildModel(Core::Identifier identifier) override;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Ramp

#endif // RampModelH
