#include "PolyLineList.h"

#include <Blocks/PolyRamp.h>

PolyLine::List::List()
   : Data::Core()
   , stageList()
{
}

PolyLine::List::~List()
{
   clear();
}

void PolyLine::List::compile(const Data::Identifier& identifier)
{
   clear();
   PolyRamp* polyRamp = getPolyRamp(identifier);

   auto addAnchorStage = [&](bool first)
   {
      Stage* stage = new Stage;
      stage->startPosition = first ? 0 : polyRamp->stageCount();
      stage->endHeight = polyRamp->getStageStartHeight(0);
      stage->type = first ? Data::Type::StartAnchor : Data::Type::EndAnchor;

      stageList.append(stage);
   };

   addAnchorStage(true);

   uint32_t startPos = 0;
   for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
   {
      const uint8_t nextIndex = (stageIndex + 1 < polyRamp->stageCount()) ? stageIndex + 1 : 0;
      const uint8_t startHeight = polyRamp->getStageStartHeight(stageIndex);
      const uint8_t endHeight = polyRamp->getStageStartHeight(nextIndex);
      const uint8_t length = polyRamp->getStageLength(stageIndex);

      Stage* stage = new Stage;
      stage->startPosition = startPos;
      stage->endHeight = polyRamp->getStageStartHeight(nextIndex);

      if (0 == length && 0 != nextIndex)
         stage->type = Data::Type::Step;
      else if (startHeight < endHeight)
         stage->type = Data::Type::Rise;
      else if (startHeight > endHeight)
         stage->type = Data::Type::Fall;
      else
         stage->type = Data::Type::Stable;

      stageList.append(stage);

      startPos += polyRamp->getStageLength(stageIndex);
      if (stageIndex + 1 == polyRamp->stageCount())
         addAnchorStage(false);
   }
}

void PolyLine::List::apply(const Data::Identifier& identifier)
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
}

void PolyLine::List::clear()
{
   for (Stage* stage : stageList)
      delete stage;

   stageList.clear();
}

uint8_t PolyLine::List::stageCount() const
{
   return stageList.count();
}

PolyLine::Stage* PolyLine::List::getStage(const uint8_t index)
{
   return stageList.at(index);
}

PolyLine::Stage* PolyLine::List::insertStage(const uint8_t afterIndex, const Stage& stage)
{
   return nullptr;
}
