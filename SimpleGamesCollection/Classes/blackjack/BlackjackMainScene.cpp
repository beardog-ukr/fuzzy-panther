#include "blackjack/BlackjackMainScene.h"
#include "blackjack/BlackboardNode.h"
#include "blackjack/BlackjackCommonUtils.h"
#include "blackjack/GameTableNode.h"
#include "blackjack/GameStateKeeper.h"
#include "main_menu/BackgroundNode.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;
#include <cmath>



// static const int gameWindowWidth = 640;
// static const int gameWindowHeight = 576;

// static const int mapSectionWidth = 192;
// static const int mapSectionHeight = 576;

// static const float mapSectionMoveDuration = 3.0;
// static const int mapSectionsCount = 4;

// static const int sectionsForVictoryRequirement = 10;


// enum ActionTagsBirdMainScene {
//   BMSAT_main =1
// };

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackjackMainScene::BlackjackMainScene() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackjackMainScene::~BlackjackMainScene() {
  GameStateKeeper::unloadCardsFromCache();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* BlackjackMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  BlackjackMainScene *pRet = new(std::nothrow) BlackjackMainScene();
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

bool BlackjackMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initBoard()) {
    return false;
  }

  if (!initMenu()) {
    return false;
  }

  if (!initGameTable()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  if (!initGameStateKeeper()) {
    return false;
  }

  startGame();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initBackground() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, ZO_scene_background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initBoard() {
  const Size cs = getContentSize();
  blackboardNode = BlackboardNode::create(c6);

  blackboardNode->setAnchorPoint(Vec2(1, 1));
  blackboardNode->setPosition(cs.width, cs.height);
  addChild(blackboardNode, ZO_scene_elements);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initGameTable() {
  const Size cs = getContentSize();
  Size gtes;
  gtes.width = (cs.width/3)*2;
  gtes.height = (cs.height/3)*2;

  gameTableNode = GameTableNode::create(gtes, c6);

  gameTableNode->setAnchorPoint(Vec2(0.5,0.5));
  gameTableNode->setPosition(cs.width/3, (cs.height/3)*2);
  addChild(gameTableNode, ZO_cards);

  return true;  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initGameStateKeeper() {
  gameStateKeeper = make_unique<GameStateKeeper>();
  gameStateKeeper->setLogger(c6);

  GameStateKeeper::loadCardsToCache();

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(BlackjackMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initMenu() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  Menu* menu = Menu::create();

  const int itemsCount = 2;
  string captions[itemsCount] = {"Hit", "Stand"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(BlackjackMainScene::mcHit, this),
                                     CC_CALLBACK_1(BlackjackMainScene::mcStand, this)
  };

  const Size currentWindowSize = getContentSize();
  Vec2 positions[itemsCount] = {
    {.x = currentWindowSize.width/8, .y = currentWindowSize.height/6},
    {.x = (currentWindowSize.width/8)*3, .y = currentWindowSize.height/6}
  };

  for (int i = 0; i< itemsCount; i++) {
    // MenuItemImage* item = MenuItemImage::create();
    // item->setNormalSpriteFrame(sfc->getSpriteFrameByName("menu_panel_main.png"));
    // item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("menu_panel_sec.png"));
    // item->setCallback(mcbs[i]);

    MenuItemImage* item = MenuItemImage::create(
      "blackjack/menu_panel_short_main.png", "blackjack/menu_panel_short_sec.png", mcbs[i]);

    const Size itemSize = item->getContentSize();
    item->setAnchorPoint(Vec2(0.5,0.5));
    item->setPosition(positions[i]);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }

  menu->addChild(prepareMainMenuButton());

  menu->setPosition(Vec2::ZERO);

  addChild(menu, ZO_scene_elements);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* BlackjackMainScene::prepareMainMenuButton() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  MenuItemImage* bmItem = MenuItemImage::create();
  bmItem->setNormalSpriteFrame(sfc->getSpriteFrameByName("menu_panel_main.png"));
  bmItem->setSelectedSpriteFrame(sfc->getSpriteFrameByName("menu_panel_sec.png"));
  bmItem->setCallback(CC_CALLBACK_1(BlackjackMainScene::mcBackToMain, this));

  const Size currentWindowSize = getContentSize();
  bmItem->setPosition( {.x = (currentWindowSize.width/4)*3, .y = currentWindowSize.height/6});

  const Size itemSize = bmItem->getContentSize();
  bmItem->setAnchorPoint(Vec2(0.5,0.5));

  Label* label = Label::createWithTTF("Back to main menu", "fonts/Marker Felt.ttf", 32);
  label->setTextColor(Color4B(160,82,45,255));
  label->setAnchorPoint(Vec2(0.5,0.5));
  label->setPosition(itemSize.width/2, itemSize.height/2);
  bmItem->addChild(label);

  return bmItem;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
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

void BlackjackMainScene::mcBackToMain(cocos2d::Ref *pSender) {
  Director::getInstance()->popScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::mcHit(cocos2d::Ref *pSender) {
  gameTableNode->dealToPlayer("sss");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::mcStand(cocos2d::Ref *pSender) {
  gameTableNode->revealFirst("zzz");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::startGame() {
  list<string> initialCards = gameStateKeeper->prepareInitialDeal();

  if (initialCards.size()<4) {
    C6_C1(c6, "bad logic");
    return;
  }

  gameTableNode->dealToDealer(initialCards.front());
  initialCards.pop_front();
  gameTableNode->dealToDealer(initialCards.front());
  initialCards.pop_front();

  gameTableNode->dealToPlayer(initialCards.front());
  initialCards.pop_front();
  gameTableNode->dealToPlayer(initialCards.front());
  initialCards.pop_front();

  blackboardNode->setDealerCount(gameStateKeeper->getDealerCount());
  blackboardNode->setPlayerCount(gameStateKeeper->getPlayerCount());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
