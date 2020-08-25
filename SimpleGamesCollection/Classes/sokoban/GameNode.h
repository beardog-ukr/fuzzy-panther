#pragma once

#include <memory>
#include <list>
#include <utility> // std::pair

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class DigitNode;

class GameNode : public cocos2d::Node, virtual public SixCatsLoggerLoggable {
public:
  static GameNode* create(std::shared_ptr<SixCatsLogger> c6);

  static bool loadSpriteCache(std::shared_ptr<SixCatsLogger> c6);
  static void unloadSpriteCache();

  void processKey(cocos2d::EventKeyboard::KeyCode keyCode);

protected:
  GameNode();
  virtual ~GameNode();

  bool loadMetaInfo(cocos2d::TMXTiledMap* const mapNode,
                    const std::string& layerName, const int metaCode,
                    std::list<std::pair<int, int> >& data);
  std::list<std::pair<int, int> > obstaclesInfo;
  std::list<std::pair<int, int> > boxesInfo;
  std::list<std::pair<int, int> > targetsInfo;
  std::pair<int, int> personInfo;



  bool initSelf();
};

}
