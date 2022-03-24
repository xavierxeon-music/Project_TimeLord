#include "PageOverview.h"

#include <Tools/Convert.h>

#include "Main.h"

PageOverview::PageOverview(Main* main, const Tempo& tempo)
   : Abstract::Page::Base()
   , main(main)
   , tempo(tempo)
{
}

void PageOverview::render(OledDisplay* display)
{
   static const uint8_t yText = 50;

   const uint8_t bar = tempo.getCounter(Tempo::Division::Bar) % 1000;
   const uint8_t quarter = tempo.getCounter(Tempo::Division::Quarter);

   const std::string counterText = Convert::text(bar + 1) + ":" + Convert::text(quarter + 1);
   const uint8_t counterX = compileLeftX(60, counterText, Font_7x10, Alignment::Right);
   write(display, counterX, yText, counterText, Font_7x10, White);

   const uint8_t bpm = tempo.getBeatsPerMinute();
   const std::string bpmText = Convert::text(bpm);

   const uint8_t bpmX = compileLeftX(95, bpmText, Font_7x10, Alignment::Right);
   write(display, bpmX, yText, bpmText, Font_7x10, White);
   write(display, 95, yText, " bpm", Font_7x10, White);
}
