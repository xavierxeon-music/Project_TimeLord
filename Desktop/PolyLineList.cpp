#include "PolyLineList.h"

PolyLine::List::List()
   : stageList()
{
}

PolyLine::List::~List()
{
   clear();
}

void PolyLine::List::compile(PolyRamp* polyRamp)
{
   clear();
   if (0 == polyRamp->getStageCount())
      return;

   auto addAnchorStage = [&](bool first)
   {
      Stage* stage = new Stage;
      stage->startPosition = first ? 0 : polyRamp->getLength();
      stage->endHeight = polyRamp->getStageStartHeight(0);
      stage->type = first ? Data::Type::StartAnchor : Data::Type::EndAnchor;

      stageList.append(stage);
   };

   addAnchorStage(true);

   uint32_t startPos = 0;
   for (uint8_t stageIndex = 0; stageIndex < polyRamp->getStageCount(); stageIndex++)
   {
      const uint8_t startHeight = polyRamp->getStageStartHeight(stageIndex);
      const uint8_t endHeight = polyRamp->getStageEndHeight(stageIndex);
      const uint8_t length = polyRamp->getStageLength(stageIndex);

      Stage* stage = new Stage;
      stage->startPosition = startPos;
      stage->startHeight = startHeight;
      stage->endHeight = endHeight;

      if (0 == length)
         stage->type = Data::Type::Step;
      else if (startHeight < endHeight)
         stage->type = Data::Type::Rise;
      else if (startHeight > endHeight)
         stage->type = Data::Type::Fall;
      else
         stage->type = Data::Type::Stable;

      stageList.append(stage);

      startPos += polyRamp->getStageLength(stageIndex);
      if (stageIndex + 1 == polyRamp->getStageCount())
         addAnchorStage(false);
   }
}

void PolyLine::List::apply(PolyRamp* polyRamp)
{
}

void PolyLine::List::clear()
{
   for (Stage* stage : stageList)
      delete stage;

   stageList.clear();
}

uint8_t PolyLine::List::getStageCount() const
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
