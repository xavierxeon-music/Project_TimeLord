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

      QJsonObject write() const;
      void read(const QJsonObject& data) const;

   private:
      friend class Core::Interface;

   private:
      uint8_t beatsPerMinute;
      PolyRamp polyRamps[8];
   };
} // namespace Bank

#endif // NOT BankContentH
