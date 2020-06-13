#include "bird/GameEndScene.h"
#include "main_menu/BackgroundNode.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace bird;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameEndScene::GameEndScene(const bool inVictory) {
  victory = inVictory;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameEndScene::~GameEndScene() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* GameEndScene::createScene(const bool victory, std::shared_ptr<SixCatsLogger> inC6) {

  GameEndScene *pRet = new(std::nothrow) GameEndScene(victory);
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

bool GameEndScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initMenuBackground()) {
    return false;
  }

  if (!initMenu()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameEndScene::initBackground() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode);//, ZO_scene_background);

  const char filename[] = "bird/bird_game_end.png";
  Sprite* sprite = Sprite::create(filename);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  sprite->setAnchorPoint(Vec2(0.5,0.5));
  const Size cs = getContentSize();
  sprite->setPosition(cs.width/2, cs.height/2);
  addChild(sprite);//, ZO_scene_background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameEndScene::initMenu() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  Menu* menu = Menu::create();

  const int itemsCount = 2;
  string captions[itemsCount] = {"Back to main menu", "Try again"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(GameEndScene::mcBackToMain, this),
                                     CC_CALLBACK_1(GameEndScene::mcTryAgain, this)
  };

  const Size currentWindowSize = getContentSize();

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create();
    item->setNormalSpriteFrame(sfc->getSpriteFrameByName("menu_panel_main.png"));
    item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("menu_panel_sec.png"));
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

  addChild(menu);//, ZO_menu_elements);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameEndScene::initMenuBackground() {
  // --- menu background (half-transparent image)
  const string menuBackgroundFileName = "asteroids/game_end_background.png";
  Sprite* const menuBackground = Sprite::create(menuBackgroundFileName);
  if (menuBackground == nullptr) {
    C6_C2(c6, "Failed to load ", menuBackgroundFileName);
    return false;
  }

  menuBackground->setAnchorPoint(Vec2(0, 0));
  menuBackground->setPosition(Vec2(0, 0));

  addChild(menuBackground);//, ZO_menu_background);

  // --- spaceman picture (either dead or funny)
  const string failImageName = "bird/grave.png";
  const string victoryImageName = "bird/tree.png";
  const string imageName = victory ? victoryImageName : failImageName;
  Sprite* const menuPicture = Sprite::create(imageName);
  if (menuPicture == nullptr) {
    C6_C2(c6, "Failed to load ", imageName);
    return false;
  }

  menuPicture->setAnchorPoint(Vec2(0, 0));
  menuPicture->setPosition(Vec2(0, 0));

  addChild(menuPicture);//, ZO_menu_elements);

  // --- label with game ending status
  const string failText = "You failed :(";
  const string victoryText = "Victory!!!";
  const string labelText = victory ? victoryText : failText;
  Label* label = Label::createWithTTF(labelText, "fonts/Marker Felt.ttf", 64);
  label->setTextColor(Color4B(160,82,45,255));
  label->setAnchorPoint(Vec2(0.5,0.5));

  const Size currentWindowSize = getContentSize();
  label->setPosition(currentWindowSize.width/2,
                     currentWindowSize.height/2 + currentWindowSize.height/4);
  addChild(label);//, ZO_menu_elements);


  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameEndScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(GameEndScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameEndScene::mcBackToMain(cocos2d::Ref *pSender) {
  Director::getInstance()->popToRootScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameEndScene::mcTryAgain(cocos2d::Ref *pSender) {
  Director::getInstance()->popScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameEndScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
