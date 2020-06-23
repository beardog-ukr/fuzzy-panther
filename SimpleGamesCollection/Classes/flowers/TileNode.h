#pragma once

//#include <memory>
#include <vector>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace flowers {

enum TileType : int;

class TileNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static TileNode* create(std::shared_ptr<SixCatsLogger> c6);

  void setMineCounter(const int value);
  void setTileType(const TileType newTileType);

  void switchRedFlag();

  void reset();

protected:
  TileNode();
  virtual ~TileNode();

  cocos2d::Sprite* redFlag;
  cocos2d::Label* mineCounter;

  bool initMineCounter();
  bool initRedFlag();
  bool initSelf();



};

}
