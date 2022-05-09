#ifndef BankDataH
#define BankDataH

#include <Remember.h>

#include <QJsonObject>

#include <Blocks/PolyRamp.h>

namespace Data
{
   class Core;
}

namespace Bank
{
   class Data
   {
   public:
      Data();

   public:
      QJsonObject write() const;
      void read(const QJsonObject& data) const;

   private:
      friend class ::Data::Core;

   private:
      PolyRamp polyRamps[8];
   };
} // namespace Bank

#endif // NOT BankDataH
