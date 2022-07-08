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

         uint32_t positions;
         Map map;

         using List = QList<Stage>;
      };

      Stage currentStage;
      Stage::List stageList;
   };
} // namespace DeviceState

#endif // NOT DeviceStateModelH
