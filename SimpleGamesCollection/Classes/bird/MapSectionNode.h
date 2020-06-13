#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

// #include <

namespace bird {

class MapSectionNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static MapSectionNode* create(const int obstacleType, std::shared_ptr<SixCatsLogger> c6);

  static bool loadCached();
  static void unloadCached();


protected:
  MapSectionNode();
  virtual ~MapSectionNode();

  virtual bool init(const int obstacleType);
  bool initPhysicsBody();
  bool initHighPhysicsBody();
  bool initLowPhysicsBody();


  int objectId;

  int obstacleType;

  std::string getFilenameForObstacleType(const int obstacleType) const;



};

}