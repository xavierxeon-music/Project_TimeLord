#ifndef StageWidgetH
#define StageWidgetH

#include "AbstractWidget.h"

class StageModel;

class StageWidget : public AbstractWidget
{
   Q_OBJECT
public:
   StageWidget(MainWidget* mainWidget, StageModel* stageModel);

signals:
   void signalPointSelected(const uint8_t& index);

private slots:
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   StageModel* stageModel;
};

#endif // StageWidgetH
