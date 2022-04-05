#include "TempoWidget.h"

#include <QHBoxLayout>

#include <QTimer>

const uint16_t callbackRate = 50;

TempoWidget::TempoWidget(QWidget* parent, Tempo* tempo)
   : QWidget(parent)
   , tempo(tempo)
   , bpmInfo(nullptr)
{
   bpmInfo = new QLabel("??? bpm", this);

   QHBoxLayout* masterLayout = new QHBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(bpmInfo);

   QTimer* statusUpdateTimer = new QTimer(this);
   statusUpdateTimer->setTimerType(Qt::PreciseTimer);
   connect(statusUpdateTimer, &QTimer::timeout, this, &TempoWidget::slotStatusUpdate);
   statusUpdateTimer->start(callbackRate);
}

void TempoWidget::slotStatusUpdate()
{
   tempo->advance(1000.0 / callbackRate);

   const Tempo::RunState& runState = tempo->getRunState();

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
