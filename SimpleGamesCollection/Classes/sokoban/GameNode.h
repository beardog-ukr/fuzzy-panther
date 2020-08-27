#pragma once

#include <memory>
#include <list>
#include <utility> // std::pair

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class ActorNode;
class BoxNode;
class TargetNode;

class GameNode : public cocos2d::Node, virtual public SixCatsLoggerLoggable {
public:
  static GameNode* create(std::shared_ptr<SixCatsLogger> c6);

  static bool loadSpriteCache(std::shared_ptr<SixCatsLogger> c6);
  static void unloadSpriteCache();

  bool processKey(cocos2d::EventKeyboard::KeyCode keyCode);

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

  struct TargetInfo {
    int gameX;
    int gameY;
    TargetNode* node;
  };
  std::list<TargetInfo> targetsInfo;

  std::pair<int, int> personInfo;

  cocos2d::TMXTiledMap* mapNode;

  bool doMoveBox(const int boxX, const int boxY, const int diffX, const int diffY);
  void processMoveRequest(const int diffX, const int diffY);

  void processActionEnd(float dt);
  void processAttackEnd(float dt);
  void reevaluateTargets();

  bool checkForEndGame();

  ActorNode* actor;

  bool actionInProcess;
  bool hasBufferedSomething;
  cocos2d::EventKeyboard::KeyCode bufferedKeyCode;


  bool initActorNode();
  bool initBoxNodes();
  bool initMapNode();
  bool initTargetNodes();
  bool initSelf();
};

}
