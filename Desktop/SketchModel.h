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

   public:
      void loadFromFile(const QString& fileName);
      void saveToFile(const QString& fileName);
      void applyToBanks();
      QString compileInfo() const;

   public slots:
      void slotNewState(const QJsonObject& stateObject);

   private:
      struct State
      {
         using Values = std::vector<uint8_t>;
         using Map = std::map<uint8_t, Values>; // bankIndex - stage values

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
