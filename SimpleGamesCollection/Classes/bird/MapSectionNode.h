#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace bird {

class MapSectionNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static MapSectionNode* create(const std::string& filename, std::shared_ptr<SixCatsLogger> c6);

protected:
  MapSectionNode();
  virtual ~MapSectionNode();

  int objectId;


};

}