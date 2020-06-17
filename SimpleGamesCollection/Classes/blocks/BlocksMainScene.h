#pragma once

#include <memory>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class BlocksGameNode;


class BlocksMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  BlocksMainScene();
  virtual ~BlocksMainScene();

  bool init();
  bool initBackground();
//  bool initBoard();
  bool initKeyboardProcessing();
//  bool initMenu();
  bool initGameNode();

  void startGame();
  void doNextTick();
  BlocksGameNode* gameNode;

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

//  std::unique_ptr<GameStateKeeper> gameStateKeeper;
};

}

