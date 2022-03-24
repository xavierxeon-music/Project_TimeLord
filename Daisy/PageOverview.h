#ifndef PageOverviewH
#define PageOverviewH

#include <Abstract/AbstractPageBase.h>

#include <Music/Tempo.h>

class Main;

class PageOverview : public Abstract::Page::Base
{
public:
   PageOverview(Main* main, const Tempo& tempo);

private:
   void render(OledDisplay* display) override;
   //State forward() override;
   //State back() override;
   //State clicked() override;

private:
   Main* main;
   const Tempo& tempo;
};

#endif // PageOverviewH
