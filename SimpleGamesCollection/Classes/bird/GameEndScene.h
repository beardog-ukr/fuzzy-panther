#pragma once

#include <memory>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace bird {

class GameEndScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  static cocos2d::Scene* createScene(const bool victory, std::shared_ptr<SixCatsLogger> c6);

protected:
  GameEndScene(const bool victory);
  virtual ~GameEndScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initMenu();
  bool initMenuBackground();

  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

  bool victory;
};

}

