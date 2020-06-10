#include "AsteroidsScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "asteroids/GameManager.h"
#include "asteroids/ZOrderValues.h"

USING_NS_CC;
using namespace std;

using namespace asteroids;

static const string plistFilename = "asteroids/asteroids.plist";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsScene::AsteroidsScene() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsScene::~AsteroidsScene() {
  C6_D1(c6, "here");
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistFilename);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* AsteroidsScene::createScene() {
  // printf("%s: here\n", __func__);
  Scene* result = AsteroidsScene::create();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool AsteroidsScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::initWithPhysics() ) {
    return false;
  }

  // getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

  c6 = make_shared<SixCatsLogger>(SixCatsLogger::Debug);

  if (!initSpriteCache()) {
    return false;
  }

  gameManager = make_unique<asteroids::GameManager>();
  gameManager->setLogger(c6);

  Node* gameNode = initGameNode();
  if (gameNode == nullptr) {
    return false;
  }

  if (!initGameNodeBackground(gameNode->getContentSize())) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  auto contactListener = EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(AsteroidsScene::onContactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

  gameManager->startGame();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initGameNodeBackground(const cocos2d::Size& gameNodeSize) {


  if (!addBorderCorners(gameNodeSize)) {
    return false;
  }

  if (!addBorderFrames(gameNodeSize)) {
    return false;
  }

  if (!addBorderCenters(gameNodeSize)) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::addBorderCenters(const cocos2d::Size& gameNodeSize) {
  Vec2 aps[4] = {
    {.x = 0.5, .y = 0}, //up
    {.x = 0.5, .y = 1}, //down
    {.x = 1, .y = 0.5}, // left
    {.x = 0, .y = 0.5} // right
  };

  const Size cs = getContentSize();
  const float halfSceneWidth = cs.width/2;
  const float halfSceneHeight = cs.height/2;
  const float halfGNWidth = gameNodeSize.width/2;
  const float halfGNHeight = gameNodeSize.height/2;
  Vec2 positions[4] = {
    {.x = halfSceneWidth, .y = halfSceneHeight+halfGNHeight}, //up
    {.x = halfSceneWidth, .y = halfSceneHeight-halfGNHeight}, //down
    {.x = halfSceneWidth - halfGNWidth, .y = halfSceneHeight}, //left
    {.x = halfSceneWidth + halfGNWidth, .y = halfSceneHeight}, //right
  };

  const char filename[] = "asteroids/border_center.png";

  for (int i = 0; i<4; i++) {
    Sprite* sprite = Sprite::create(filename);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filename);
      return false;
    }

    sprite->setAnchorPoint(aps[i]);
    sprite->setPosition(positions[i]);

    addChild(sprite, ZO_scene_border_center);
  }

  return true;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::addBorderCorners(const cocos2d::Size& gameNodeSize) {
  Vec2 aps[4] = {
    {.x = 1, .y = 0}, //up left
    {.x = 0, .y = 0}, //up right
    {.x = 1, .y = 1}, //down left
    {.x = 0, .y = 1} //down right
  };

  const Size cs = getContentSize();
  const float halfSceneWidth = cs.width/2;
  const float halfSceneHeight = cs.height/2;
  const float halfGNWidth = gameNodeSize.width/2;
  const float halfGNHeight = gameNodeSize.height/2;
  Vec2 positions[4] = {
    {.x = halfSceneWidth - halfGNWidth, .y = halfSceneHeight+halfGNHeight}, //up left
    {.x = halfSceneWidth + halfGNWidth, .y = halfSceneHeight+halfGNHeight}, //up right
    {.x = halfSceneWidth - halfGNWidth, .y = halfSceneHeight-halfGNHeight}, //down left
    {.x = halfSceneWidth + halfGNWidth, .y = halfSceneHeight-halfGNHeight}, //down right
  };

  const char filename[] = "asteroids/border_corner.png";

  for (int i = 0; i<4; i++) {
    Sprite* sprite = Sprite::create(filename);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filename);
      return false;
    }

    sprite->setAnchorPoint(aps[i]);
    sprite->setPosition(positions[i]);

    addChild(sprite, ZO_scene_border_corner);
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::addBorderFrames(const cocos2d::Size& gameNodeSize) {
  const Size cs = getContentSize();
  const float halfSceneWidth = cs.width/2;
  const float halfSceneHeight = cs.height/2;
  const float halfGNWidth = gameNodeSize.width/2;
  const float halfGNHeight = gameNodeSize.height/2;

  const char filenameH[] = "asteroids/border_frame_horizontal.png";
  const char filenameV[] = "asteroids/border_frame_vertical.png";

  // Vec2 nextPos;
  float stopPos;
  float acc;

  //up left to right
  stopPos = halfSceneWidth;
  acc = halfSceneWidth - halfGNWidth;
  while (acc < stopPos) {
    Sprite* sprite = Sprite::create(filenameH);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameH);
      return false;
    }

    sprite->setAnchorPoint({.x = 0, .y = 0});
    sprite->setPosition({.x = acc, .y = halfSceneHeight+halfGNHeight});

    acc = acc + sprite->getContentSize().width;

    addChild(sprite, ZO_scene_border_frame);
  }

  // const char filename2[] = "asteroids/border_center.png";

  //up right to left
  stopPos = halfSceneWidth;
  acc = halfSceneWidth + halfGNWidth;
  while (acc > stopPos) {
    Sprite* sprite = Sprite::create(filenameH);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameH);
      return false;
    }

    sprite->setAnchorPoint({.x = 1, .y = 0});
    sprite->setPosition({.x = acc, .y = halfSceneHeight+halfGNHeight});

    acc = acc - sprite->getContentSize().width;

    addChild(sprite, ZO_scene_border_frame);
  }


  ////////////////////
  //down left to right
  stopPos = halfSceneWidth;
  acc = halfSceneWidth - halfGNWidth;
  while (acc < stopPos) {
    Sprite* sprite = Sprite::create(filenameH);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameH);
      return false;
    }

    sprite->setAnchorPoint({.x = 0, .y = 1});
    sprite->setPosition({.x = acc, .y = halfSceneHeight-halfGNHeight});

    acc = acc + sprite->getContentSize().width;

    addChild(sprite, ZO_scene_border_frame);
  }

  //down right to left
  stopPos = halfSceneWidth;
  acc = halfSceneWidth + halfGNWidth;
  while (acc > stopPos) {
    Sprite* sprite = Sprite::create(filenameH);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameH);
      return false;
    }

    sprite->setAnchorPoint({.x = 1, .y = 1});
    sprite->setPosition({.x = acc, .y = halfSceneHeight-halfGNHeight});

    acc = acc - sprite->getContentSize().width;

    addChild(sprite, ZO_scene_border_frame);
  }

  //left : bottom to up
  stopPos = halfSceneHeight;
  acc = halfSceneHeight - halfGNHeight;
  while (acc < stopPos) {
    Sprite* sprite = Sprite::create(filenameV);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameV);
      return false;
    }

    sprite->setAnchorPoint({.x = 1, .y = 0});
    sprite->setPosition({.x = halfSceneWidth - halfGNWidth, .y = acc});

    acc = acc + sprite->getContentSize().height;

    addChild(sprite, ZO_scene_border_frame);
  }

  //left: up to bottom
  stopPos = halfSceneHeight;
  acc = halfSceneHeight + halfGNHeight;
  while (acc > stopPos) {
    Sprite* sprite = Sprite::create(filenameV);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameV);
      return false;
    }

    sprite->setAnchorPoint({.x = 1, .y = 1});
    sprite->setPosition({.x = halfSceneWidth - halfGNWidth, .y = acc});

    acc = acc - sprite->getContentSize().height;

    addChild(sprite, ZO_scene_border_frame);
  }

  //right : bottom to up
  stopPos = halfSceneHeight;
  acc = halfSceneHeight - halfGNHeight;
  while (acc < stopPos) {
    Sprite* sprite = Sprite::create(filenameV);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameV);
      return false;
    }

    sprite->setAnchorPoint({.x = 0, .y = 0});
    sprite->setPosition({.x = halfSceneWidth + halfGNWidth, .y = acc});

    acc = acc + sprite->getContentSize().height;

    addChild(sprite, ZO_scene_border_frame);
  }


  //right : up to bottom
  stopPos = halfSceneHeight;
  acc = halfSceneHeight + halfGNHeight;
  while (acc > stopPos) {
    Sprite* sprite = Sprite::create(filenameV);
    if (sprite == nullptr) {
      C6_C2(c6, "Failed to load: ", filenameV);
      return false;
    }

    sprite->setAnchorPoint({.x = 0, .y = 1});
    sprite->setPosition({.x = halfSceneWidth + halfGNWidth, .y = acc});

    acc = acc - sprite->getContentSize().height;

    addChild(sprite, ZO_scene_border_frame);
  }


  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* AsteroidsScene::initGameNode() {
  Node * gameNode = gameManager->prepareGameNode();
  if (gameNode == nullptr) {
    return nullptr;
  }

  const Vec2 backupAP = gameNode->getAnchorPoint();

  gameNode->setAnchorPoint(Vec2(0.5,0.5));
  const Size cs = getContentSize();
  gameNode->setPosition(cs.width/2, cs.height/2);
  addChild(gameNode, ZO_game_background);

  gameNode->setAnchorPoint(backupAP);

  return gameNode;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(AsteroidsScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initSpriteCache() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidsScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (gameManager->processKeyCode(keyCode)) {
    return;
  }

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

bool AsteroidsScene::onContactBegin(PhysicsContact& contact) {
  printf("%s: here\n", __func__);

  return gameManager->processContact(contact);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
