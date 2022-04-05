#ifndef TempoWidgetH
#define TempoWidgetH

#include <QWidget>

#include <QLabel>

#include <Music/Tempo.h>

class TempoWidget : public QWidget
{
   Q_OBJECT
public:
   TempoWidget(QWidget* parent, Tempo* tempo);

public slots:
   void slotStatusUpdate();

private:
   Tempo* tempo;
   QLabel* bpmInfo;
};

#endif // NOT TempoWidgetH
