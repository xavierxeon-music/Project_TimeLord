#ifndef GraphWidgetH
#define GraphWidgetH

#include "AbstractWidget.h"

class GraphModel;

class GraphWidget : public AbstractWidget
{
   Q_OBJECT
public:
   GraphWidget(MainWidget* mainWidget, QToolBar* toolBar, GraphModel* graphModel);

signals:
   void signalPortChanged(const Model::Provider provider, const uint8_t& index);

private slots:
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   GraphModel* graphModel;
};

#endif // GraphWidgetH
