#pragma once

#include <memory>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class GameManager;
// #include "GameManager.h"

class AsteroidsScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  virtual bool init();

  // implement the "static create()" method manually
  CREATE_FUNC(AsteroidsScene);

protected:
  AsteroidsScene();
  virtual ~AsteroidsScene();

  bool initBackground();
  bool initGameNode();
  // bool initSettingsMenu();
  // bool initNewGameMenu();

  // void mcExit(cocos2d::Ref *pSender);

  // cocos2d::Menu* mainMenu;

  // cocos2d::Sprite* settingsMenu; // TODO: implement real menu instead of sprite stub
  // cocos2d::Menu* newGameMenu;
  // cocos2d::Node* currentSideMenu;
  // void switchSideMenu(cocos2d::Node* newMenu);

  std::unique_ptr<asteroids::GameManager> gameManager;
};

}

