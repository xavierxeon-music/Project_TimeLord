#ifndef ModelDivisionH
#define ModelDivisionH

#include <QStandardItemModel>

#include <Music/Tempo.h>

namespace Model
{
   class Division : public QStandardItemModel
   {
      Q_OBJECT
   public:
      Division(QObject* parent);
   };

} // namespace Model

Q_DECLARE_METATYPE(Tempo::Division);

#endif // NOT ModelDivisionH
