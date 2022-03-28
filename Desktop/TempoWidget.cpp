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
   statusUpdateTimer->start(100);
}

void TempoWidget::slotStatusUpdate()
{
   const Tempo::RunState& runState = tempo->getRunState();
   const uint8_t& beatsPerMinute = tempo->getBeatsPerMinute();

   if (Tempo::RunState::Running == runState)
   {
      const uint8_t bar = tempo->getCounter(Tempo::Division::Bar);
      const uint8_t quarter = tempo->getCounter(Tempo::Division::Quarter);
      const uint8_t bpm = tempo->getBeatsPerMinute();
      QString statusText = QChar(5125) + QString(" %1:%2  %3 bpm");
      statusText = statusText.arg(bar);
      statusText = statusText.arg(quarter);
      statusText = statusText.arg(bpm);
      bpmInfo->setText(statusText);
   }
   else
      bpmInfo->setText("stoped");
}
