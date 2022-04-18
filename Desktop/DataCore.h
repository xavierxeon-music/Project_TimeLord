#ifndef DataCoreH
#define DataCoreH

#include <Blocks/PolyRamp.h>

class MainWidget;

namespace RampDevice
{
   class VCV;
};

namespace Data
{
   Q_NAMESPACE

   struct Target
   {
      enum Value : uint8_t
      {
         None,
         PolyRampName,
         PolyRampLength,
         PolyRampStepSize,
         PolyRampLoop,
         PolyRampStageCount,
         StageStartHeight,
         StageEndHeight,
         StageLength,
         StageType,
         StageStartPosition

      };
   };

   struct Identifier
   {
      uint8_t rampIndex;
      uint8_t stageIndex;

      Identifier(const uint8_t& rampIndex = 0, const uint8_t& stageIndex = 255);
      bool hasStage() const;
   };

   struct Role
   {
      static constexpr int Identifier = Qt::UserRole + 11; // the identifier
      static constexpr int Target = Qt::UserRole + 12;     // the target enum
      static constexpr int Data = Qt::UserRole + 13;       // non integer data in original format
   };

   struct Type
   {
      enum Value : uint8_t
      {
         Unknown,
         StartAnchor,
         EndAnchor,
         Rise,
         Fall,
         Stable,
         Step
      };

      static QString getName(const Value& type);
      static QIcon getIcon(const Value& type);
   };

   // all graph data access and manipulation should happen via this class
   class Core
   {      
   public:
      Core();

   public:
      virtual void modelHasChanged(Identifier identifier);
      virtual void polyRampSelected(Identifier identifier);
      virtual void rebuildModel(Identifier identifier);
      virtual void saveSettings();

   public:
      static const QString keys;

   protected:
      PolyRamp* getPolyRamp(const Identifier& identifier); // ignores stage index
      const PolyRamp* getPolyRamp(const Identifier& identifier) const; // ignores stage index

      template <typename... Args>
      void callOnAllInstances(void (Core::*function)(Args...), Args... args);

      void setLockGraphSize(bool locked);
      void toggleLockGraphSize();
      bool getLockGraphSize() const;

      void setModified();


   private:
      friend class ::MainWidget;

   private:
      void createRampDevice(QObject* parent);
      void unsetModified();

   private:
      static bool isModified;
      static bool lockGraphSize;
      static RampDevice::VCV* device;
      static QList<Core*> instanceList;
   };

} // namespace Data

#ifndef DataCoreHPP
#include "DataCore.hpp"
#endif // NOT DataCoreHPP

#endif // NOT DataCoreH
