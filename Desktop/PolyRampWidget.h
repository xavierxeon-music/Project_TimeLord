#ifndef PolyRampWidgetH
#define PolyRampWidgetH

#include "AbstractWidget.h"

#include <QStackedWidget>

#include "EditDivision.h"
#include "EditLength.h"
#include "EditLoop.h"

class PolyRampModel;

class PolyRampWidget : public AbstractWidget
{
   Q_OBJECT
public:
   PolyRampWidget(MainWidget* mainWidget, PolyRampModel* polyRampModel);

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
   PolyRampModel* polyRampModel;
   QStackedWidget* editStack;
   Edit::Length* lengthEdit;
   Edit::Division* divisionEdit;
   Edit::Loop* loopEdit;
   Model::Provider selectedProvider;
   int selectedGraphIndex;
};

#endif // PolyRampWidgetH
