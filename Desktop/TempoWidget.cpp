#include "TempoWidget.h"

#include <QHBoxLayout>

TempoWidget::TempoWidget(QWidget* parent)
   : QWidget(parent)
   , bpmInfo(nullptr)
{
   bpmInfo = new QLabel("??? bpm", this);

   QHBoxLayout* masterLayout = new QHBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(bpmInfo);
}

void TempoWidget::slotStatusUpdate(const Tempo::RunState& runState, const uint8_t& beatsPerMinute)
{
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
