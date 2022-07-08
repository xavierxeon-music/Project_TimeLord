#ifndef DeviceStateModelH
#define DeviceStateModelH

#include "Core.h"
#include <QStandardItemModel>

#include <map>

#include <QJsonObject>

class Target;

namespace DeviceState
{
   class Model : public QStandardItemModel, public Core::Interface
   {
      Q_OBJECT
   public:
      Model(QObject* parent, Target* target);

   public:
      void applyToBanks();
      QString compileInfo() const;

   public slots:
      void slotNewState(const QJsonObject& stateObject);

   private:
      struct Stage
      {
         using Map = std::map<uint8_t, QJsonObject>;

         uint32_t position;
         Map map;

         using List = QList<Stage>;
      };

   private:
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

   private:
      Stage currentStage;
      Stage::List stageList;

      const Bank::Content* bank;
   };
} // namespace DeviceState

#endif // NOT DeviceStateModelH
