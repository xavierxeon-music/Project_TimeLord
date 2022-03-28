#include "TempoWidget.h"

#include <QHBoxLayout>

#include <QTimer>

TempoWidget::TempoWidget(QWidget* parent, const Tempo* tempo)
   : QWidget(parent)
   , tempo(tempo)
   , bpmInfo(nullptr)
{
   bpmInfo = new QLabel("??? bpm", this);

   QHBoxLayout* masterLayout = new QHBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(bpmInfo);

   QTimer* statusUpdateTimer = new QTimer(this);
   connect(statusUpdateTimer, &QTimer::timeout, this, &TempoWidget::slotStatusUpdate);
   statusUpdateTimer->start(1000);
}

void TempoWidget::slotStatusUpdate()
{
   const Tempo::RunState& runState = tempo->getRunState();
   const uint8_t& beatsPerMinute = tempo->getBeatsPerMinute();

   QString statusText;
   if (Tempo::RunState::Running == runState)
   {
      statusText += QChar(5125);
      statusText += QString(" ");
      statusText += QString::number(beatsPerMinute);
      statusText += " bpm";
   }
   else
      statusText += "stoped";
   bpmInfo->setText(statusText);
}
