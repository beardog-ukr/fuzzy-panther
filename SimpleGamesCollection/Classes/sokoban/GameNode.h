#pragma once

#include <memory>
#include <list>
#include <utility> // std::pair

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class ActorNode;
class BoxNode;

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

  struct BoxInfo {
    int gameX;
    int gameY;
    BoxNode* node;
  };
  std::list<BoxInfo> boxesInfo;

  std::list<std::pair<int, int> > targetsInfo;
  std::pair<int, int> personInfo;

  cocos2d::TMXTiledMap* mapNode;

  void doActorAttack(const int diffX, const int diffY);
  bool doMoveBox(const int boxX, const int boxY, const int diffX, const int diffY);
  void processMoveRequest(const int diffX, const int diffY);

  void processActionEnd(float dt);

  ActorNode* actor;

  bool actionInProcess;
  bool hasBufferedSomething;
  cocos2d::EventKeyboard::KeyCode bufferedKeyCode;


  bool initActorNode();
  bool initBoxNodes();
  bool initMapNode();
  bool initSelf();
};

}
