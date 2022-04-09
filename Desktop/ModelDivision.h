#ifndef ModelDivisionH
#define ModelDivisionH

#include <QStandardItemModel>

namespace Model
{
   class Division : public QStandardItemModel
   {
      Q_OBJECT
   public:
      Division(QObject* parent);
   };

} // namespace Model

#endif // NOT ModelDivisionH
