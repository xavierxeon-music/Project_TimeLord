#ifndef SketchModelH
#define SketchModelH

#include "Core.h"
#include <QStandardItemModel>

#include <map>

#include <QJsonObject>

class Target;

namespace Sketch
{
   class Model : public QStandardItemModel, public Core::Interface
   {
      Q_OBJECT
   public:
      Model(QObject* parent, Target* target);

   signals:
      void signalSendState(const QJsonObject& stateObject);

   public:
      void loadFromFile();
      void saveToFile();
      void applyToBanks();
      QString compileInfo() const;
      void sendItemState(const QModelIndex& index);

   public slots:
      void slotNewState(const QJsonObject& stateObject);

   private:
      struct State
      {
         using Values = std::vector<uint8_t>;
         using Map = std::map<uint8_t, Values>; // bankIndex - stage values

         QString name;
         uint32_t position;
         Map map;

         using List = QList<State>;
      };

   private:
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
      void createCurrentStateItems();

   private:
      State currentState;
      State::List stateList;

      const Bank::Content* bank;
   };
} // namespace Sketch

#endif // NOT SketchModelH
