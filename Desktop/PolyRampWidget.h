#ifndef GraphWidgetH
#define GraphWidgetH

#include "AbstractWidget.h"

#include <QStackedWidget>

#include "GraphEditDivision.h"
#include "GraphEditLength.h"
#include "GraphEditLoop.h"

class GraphModel;

class GraphWidget : public AbstractWidget
{
   Q_OBJECT
public:
   GraphWidget(MainWidget* mainWidget, GraphModel* graphModel);

signals:
   void signalGraphSelected(const Model::Provider provider, const uint8_t& index);

public:
   void hideEditStack();

private slots:
   void slotTrimCurrentGraph();
   void slotSetLengthAllGraphs();
   void slotSetDivisionAllGraphs();
   void slotSetLoopAllGraphs();
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   GraphModel* graphModel;
   QStackedWidget* editStack;
   GraphEdit::Length* lengthEdit;
   GraphEdit::Division* divisionEdit;
   GraphEdit::Loop* loopEdit;
   Model::Provider selectedProvider;
   int selectedGraphIndex;
};

#endif // GraphWidgetH
