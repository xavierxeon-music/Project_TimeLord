#ifndef TempoWidgetH
#define TempoWidgetH

#include <QWidget>

#include <QLabel>

#include <Music/Tempo.h>

class TempoWidget : public QWidget
{
   Q_OBJECT
public:
   TempoWidget(QWidget* parent);

public slots:
   void slotStatusUpdate(const Tempo::RunState& runState, const uint8_t& beatsPerMinute);

private:
   QLabel* bpmInfo;
};

#endif // NOT TempoWidgetH
