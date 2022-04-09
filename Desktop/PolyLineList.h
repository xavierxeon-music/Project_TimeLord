#ifndef PolyLineListH
#define PolyLineListH

#include "DataCore.h"

namespace PolyLine
{
   struct Stage
   {
      uint8_t startPosition;
      uint8_t endHeight;
      Data::Type::Value type;
   };

   class List : public Data::Core
   {
   public:
      List();
      ~List();

   public:
      void compile(const Data::Identifier& identifier);
      void apply(const Data::Identifier& identifier);

      void clear();
      uint8_t stageCount() const;
      Stage* getStage(const uint8_t index);
      Stage* insertStage(const uint8_t afterIndex, const Stage& stage);

   private:
      QList<Stage*> stageList;
   };
} // namespace PolyLine

#endif // NOT PolyLineListH
