#ifndef GraphWidgetH
#define GraphWidgetH

#include "AbstractWidget.h"

class GraphModel;

class GraphWidget : public AbstractWidget
{
   Q_OBJECT
public:
   GraphWidget(MainWidget* mainWidget, GraphModel* graphModel);

signals:
   void signalGraphSelected(const Model::Provider provider, const uint8_t& index);

private slots:
   void slotTrimCurrentGraph();
   void slotSetLengthAllGraphs();
   void slotSetDivisionAllGraphs();
   void slotSetLoopAllGraphs();
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   GraphModel* graphModel;
   Model::Provider selectedProvider;
   int selectedGraphIndex;
};

#endif // GraphWidgetH
