#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace bird {

class ModuleNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static ModuleNode* create(const std::string& filename, std::shared_ptr<SixCatsLogger> c6);

protected:
  ModuleNode();
  virtual ~ModuleNode();

  int objectId;


};

}