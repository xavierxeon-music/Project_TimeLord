#ifndef TempoWidgetH
#define TempoWidgetH

#include <QWidget>

#include <QLabel>

#include <Music/Tempo.h>

class TempoWidget : public QWidget
{
   Q_OBJECT
public:
   TempoWidget(QWidget* parent, const Tempo* tempo);

public slots:
   void slotStatusUpdate();

private:
   const Tempo* tempo;
   QLabel* bpmInfo;
};

#endif // NOT TempoWidgetH
