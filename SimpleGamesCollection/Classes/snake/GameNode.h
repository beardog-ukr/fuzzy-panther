#pragma once

#include <memory>
#include <vector>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace snake {

enum MoveResultType : int;
class BodyPartsFactory;
// class DigitNode;
class GameStateKeeper;

class GameNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static GameNode* create(std::shared_ptr<SixCatsLogger> c6);

  static bool loadSpriteCache(std::shared_ptr<SixCatsLogger> c6);
  static void unloadSpriteCache();

  void processKey(cocos2d::EventKeyboard::KeyCode keyCode);

  typedef std::function<void (bool)> SceneCallback;
  void startNewGame(SceneCallback sceneCallback);

  static const int kCellSize;

protected:
  GameNode();
  virtual ~GameNode();

  bool initSelf();
  // bool initDigitNodes();
  // bool initWhiteRocket();

  void doOneTick(float dt);
  std::list<cocos2d::Node*> previousSnakeNodes;

  void startGame();

  SceneCallback sceneCallback;

//  int currentBarrelGameX;
//  int currentBarrelGameY;
  cocos2d::Node* currentBarrelNode;
  void processBarrel(const MoveResultType moveResult);

  std::unique_ptr<BodyPartsFactory> bodyPartsFactory;
  std::unique_ptr<GameStateKeeper> gameStateKeeper;
};

}
