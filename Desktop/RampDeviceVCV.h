#ifndef RampDeviceVCVH
#define RampDeviceVCVH

#include <Remember.h>
#include <QObject>

#include <QAction>
#include <QJsonObject>

#include <Blocks/PolyRamp.h>
#include <Midi/MidiPhysicalOutput.h>

namespace Data
{
   class Core;
}

namespace RampDevice
{
   class VCV : public QObject, public Remember::Root
   {
      Q_OBJECT
   public:
      struct ServerActions
      {
         QAction* connectToServer;
         QAction* bankUp;
         QAction* pushToServer;
      };

   public:
      VCV(QObject* parent);

   public:
      const ServerActions& getServerActions() const;
      void updateBankIcon();

   private:
      friend class Data::Core;
      using PolyRampArray_ = Remember::RefArray<PolyRamp, 8>;
      using Index_ = Remember::Value<uint8_t>;

   private slots:
      void slotConnectToServer(bool connect);
      void slotBankUp();
      void slotPushToServer();

   private:
      QJsonObject compileRamps() const;

   private:
      PolyRampArray_ polyRamps;
      Midi::Physical::Output output;
      Index_ bankIndex;

      ServerActions actions;
      QMap<uint8_t, QIcon> iconBuffer;
   };
} // namespace RampDevice

#endif // NOT RampDeviceVCVH
