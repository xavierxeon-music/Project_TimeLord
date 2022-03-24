#ifndef TimeLordH
#define TimeLordH

#include <Remember.h>

#include <Blocks/Graph.h>

class DataCore;

class TimeLord
{
public:
   TimeLord(Remember::Root* root);

public: // things to remeber
   using GraphList_ = Remember::RefArray<Graph, 16>;

protected:
   GraphList_ graphs;

private:
   friend class DataCore;
};

#endif // TimeLordH
