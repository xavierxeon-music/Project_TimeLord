#ifndef BankContentH
#define BankContentH

#include <Remember.h>

#include <QJsonObject>

#include <Blocks/PolyRamp.h>

namespace Core
{
   class Interface;
}

namespace Bank
{
   class Content
   {
   public:
      Content();

   public:
      const uint8_t& getBeatsPerMinute() const;
      void setBeatsPerMinute(const uint8_t bpm);

      const QString& getName(const uint8_t index) const;
      void setName(const uint8_t index, const QString& name);

      QJsonObject writeNames() const;
      void readNames(const QJsonObject& data) const;

      QJsonObject writeRamps() const;
      void readRamps(const QJsonObject& data) const;

   private:
      friend class Core::Interface;

   private:
      uint8_t beatsPerMinute;
      PolyRamp polyRamps[8];
      QString rampNames[8];
   };
} // namespace Bank

#endif // NOT BankContentH
