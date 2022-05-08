#ifndef BankH
#define BankH

#include <Remember.h>

#include <QJsonObject>

#include <Blocks/PolyRamp.h>

namespace Data
{
   class Core;
}

class Bank : public Remember::Root
{
public:
   Bank();

public:
   QJsonObject compileRamps() const;

private:
   friend class Data::Core;
   using PolyRampArray_ = Remember::RefArray<PolyRamp, 8>;

private:
   PolyRampArray_ polyRamps;
};

#endif // NOT BankH
