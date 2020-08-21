#include "snake/SnakeMainScene.h"
#include "snake/GameNode.h"
#include "snake/ZOrderValues.h"
#include "main_menu/BackgroundNode.h"

using namespace snake;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

static const int kGameWindowWidth = 1120;
static const int kGameWindowHeight = 640;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SnakeMainScene::SnakeMainScene() {
  // gameOver = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SnakeMainScene::~SnakeMainScene() {
  GameNode::unloadSpriteCache();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* SnakeMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  SnakeMainScene *pRet = new(std::nothrow) SnakeMainScene();
  if (pRet == nullptr) {
    return nullptr;
  }

  pRet->setLogger(inC6);

  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  }
  else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!GameNode::loadSpriteCache(c6)) {
    return false;
  }

  if (!initBackgroundBorder()) {
    return false;
  }

  if (!initGameNode()) {
    return false;
  }

  if (!initGameOverMenu()) {
    return false;
  }

  if (!initGameOverImage()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  //finally
  gameNode->startNewGame([this](bool gameResult) {
    this->processGameOver(gameResult);      //
  });

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::initBackground() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    C6_D2(c6, "Failed to init background with ", bFn);
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, kSceneBackgroundZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::initBackgroundBorder() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundSource = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundSource == nullptr) {
    return false;
  }

  backgroundSource->setAnchorPoint(Vec2(0,0));
  backgroundSource->setPosition(0,0);
  addChild(backgroundSource);

  const Size cs = getContentSize();
  const int rtH = (int)round(cs.height);
  const int rtW = (int)round(cs.width/2 - (kGameWindowWidth/2));

  RenderTexture* rtLeft = RenderTexture::create(rtW, rtH);
  rtLeft->begin();
  backgroundSource->visit();
  rtLeft->end();
  Director::getInstance()->getRenderer()->render();

  rtLeft->setPosition(rtW/2,rtH/2);
  addChild(rtLeft, kSceneBorderZOrder);

  RenderTexture* rtUp = RenderTexture::create(kGameWindowWidth, (cs.height-kGameWindowHeight)/2);

  backgroundSource->setPosition(0 - (cs.width/2 - (kGameWindowWidth/2)),
                                0-(kGameWindowHeight + (cs.height-kGameWindowHeight)/2));

  rtUp->begin();
  backgroundSource->visit();
  rtUp->end();
  Director::getInstance()->getRenderer()->render();

  rtUp->setPosition(cs.width/2,
                    kGameWindowHeight + (cs.height-kGameWindowHeight)/2 +
                    (cs.height-kGameWindowHeight)/4);
  addChild(rtUp, kSceneBorderZOrder);

  RenderTexture* rtDown = RenderTexture::create(kGameWindowWidth, (cs.height-kGameWindowHeight)/2);

  backgroundSource->setPosition(0 - (cs.width/2 - (kGameWindowWidth/2)), 0);

  rtDown->begin();
  backgroundSource->visit();
  rtDown->end();
  Director::getInstance()->getRenderer()->render();

  rtDown->setPosition(cs.width/2,(cs.height-kGameWindowHeight)/4);
  addChild(rtDown, kSceneBorderZOrder);

  RenderTexture* rtRight = RenderTexture::create(rtW, rtH);

  backgroundSource->setPosition(0 - cs.width/2 - (kGameWindowWidth/2), 0);

  rtRight->begin();
  backgroundSource->visit();
  rtRight->end();
  Director::getInstance()->getRenderer()->render();

  rtRight->setPosition(cs.width - rtW/2,rtH/2);
  addChild(rtRight, kSceneBorderZOrder);


  // --- finally remove background, it's not needed anymore
  backgroundSource->removeFromParent();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::initGameNode() {
  gameNode = GameNode::create(c6);
  if (gameNode == nullptr) {
    return false;
  }

  gameNode->setAnchorPoint(Vec2(0.5, 0.5));

  const Size cs = getContentSize();
  gameNode->setPosition(cs.width/2, cs.height/2);
  addChild(gameNode, kGameBackgroundZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::initGameOverImage() {
  // --- victory image
  const string fn = "repeat/victoryImage.png";
  victoryImage = Sprite::create(fn);
  if (victoryImage == nullptr) {
    C6_D2(c6, "Failed to find ", fn);
    return false;
  }

  const Size cs = getContentSize();
  victoryImage->setPosition(cs.width/2, cs.height/2);

  addChild(victoryImage, kGameOverImageZOrder);

  victoryImage->setVisible(false);

  // --- failure image
  const string fnf = "repeat/failureImage.png";
  failureImage = Sprite::create(fnf);
  if (failureImage == nullptr) {
    C6_D2(c6, "Failed to find ", fnf);
    return false;
  }

  failureImage->setPosition(cs.width/2, cs.height/2);

  addChild(failureImage, kGameOverImageZOrder);

  failureImage->setVisible(false);

  lastGameOverImage = nullptr;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::initGameOverMenu() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  Menu* menu = Menu::create();

  const int itemsCount = 2;
  string captions[itemsCount] = {"Back to main menu", "Try again"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(SnakeMainScene::mcBackToMain, this),
                                     CC_CALLBACK_1(SnakeMainScene::mcTryAgain, this)
  };

  const Size currentWindowSize = getContentSize();

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create();
    item->setNormalSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_main.png"));
    item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_sec.png"));
    item->setCallback(mcbs[i]);

    const Size itemSize = item->getContentSize();
    C6_D4(c6, "One item size is ", itemSize.width, "x", itemSize.height);
    Vec2 ip;
    ip.x = itemSize.width + itemSize.height/2;
    ip.x = currentWindowSize.width - (i+1)*ip.x;
    ip.x = ip.x + itemSize.width/2;
    ip.y = itemSize.height;

    C6_D4(c6, "Will put menu item at ", ip.x, ":", ip.y);

    item->setAnchorPoint(Vec2(0.5,0.5));
    item->setPosition(ip);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }

  menu->setPosition(Vec2::ZERO);

  addChild(menu, kGameOverMenuZOrder);

  menu->setVisible(false);

  gameOverMenu = menu;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SnakeMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(SnakeMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SnakeMainScene::mcBackToMain(cocos2d::Ref *pSender) {
  Director::getInstance()->popToRootScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SnakeMainScene::mcTryAgain(cocos2d::Ref *pSender) {
  gameNode->removeFromParentAndCleanup(true);
  initGameNode();

  gameOverMenu->setVisible(false);
  lastGameOverImage->setVisible(false);

  gameNode->startNewGame([this](bool gameResult) {
    this->processGameOver(gameResult);      //
  });
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SnakeMainScene::processGameOver(const bool gameEndedVictory) {
  C6_D1(c6, "here");

  if (gameEndedVictory) {
    lastGameOverImage = victoryImage;
  }
  else {
    lastGameOverImage = failureImage;
  }
  lastGameOverImage->setVisible(true);

  gameOverMenu->setVisible(true);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SnakeMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  // else if (EventKeyboard::KeyCode::KEY_SPACE == keyCode) {
  //   gameNode->startNewGame([this](bool gameResult) {
  //     this->processGameOver(gameResult);      //

  //   });
  // }
  else if ((EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode)||
           (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode)||
           (EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode)||
           (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode)) {
    gameNode->processKey(keyCode);
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
