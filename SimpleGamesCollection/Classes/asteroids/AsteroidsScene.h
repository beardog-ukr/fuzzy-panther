#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

class AsteroidsScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  virtual bool init();

  // implement the "static create()" method manually
  CREATE_FUNC(AsteroidsScene);

protected:
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
};


