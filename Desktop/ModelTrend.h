#ifndef ModelTrendH
#define ModelTrendH

#include <QStandardItemModel>

namespace Model
{
   class Trend : public QStandardItemModel
   {
      Q_OBJECT
   public:
      Trend(QObject* parent);
   };
} // namespace Model

#endif // NOT ModelTrendH
