#include "bird/MapSectionNode.h"
#include "bird/CommonUtils.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace bird;
static int objectIdGenerator = 0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MapSectionNode::MapSectionNode() {
  objectId = objectIdGenerator++;

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MapSectionNode::~MapSectionNode() {
  // ntdh
  C6_D2(c6, "here for #", objectId);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


MapSectionNode* MapSectionNode::create(const int obstacleType,
                                       std::shared_ptr<SixCatsLogger> inc6) {
  MapSectionNode *result = new (std::nothrow) MapSectionNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }

  if (!result->init(obstacleType)) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

string MapSectionNode::getFilenameForObstacleType(const int obstacleType) const {
  const string msNames[4] = {
    "bird/module01.png", "bird/module02.png", "bird/module03.png", "bird/module04.png"
  };

  string result = msNames[0];

  switch(obstacleType) {
  case 7:
    result = msNames[1];
    break;
  case 8:
    result = msNames[2];
    break;
  case 9:
    result = msNames[3];
    break;
    // no default section here
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MapSectionNode::init(const int inObstacleType) {
  obstacleType = inObstacleType;

  const string fn = getFilenameForObstacleType(obstacleType);
  if (!initWithFile(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;    //
  }

  if (!initPhysicsBody()) {
    return false;
  }

  setOpacity(20);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MapSectionNode::initHighPhysicsBody() {
  const int unitSize = 32;

  const Size sectionSize = getContentSize();

  Vec2 highPoints[4];
  // up left
  highPoints[0].x = (-1)* unitSize;
  highPoints[0].y = sectionSize.height/2;

  // up right
  highPoints[1].x = unitSize;
  highPoints[1].y = sectionSize.height/2;

  // down left
  highPoints[2].x = (-1)* unitSize;
  highPoints[2].y = obstacleType*unitSize + 2*unitSize -  (sectionSize.height/2);

  // down right
  highPoints[3].x = unitSize;
  highPoints[3].y = obstacleType*unitSize + 2*unitSize -  (sectionSize.height/2);

  PhysicsBody* physicsBody = PhysicsBody::createPolygon(&(highPoints[0]), 4,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_cliff);
  physicsBody->setCollisionBitmask(CM_wizard);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  Node* highNode = Node::create();
  highNode->setPosition(sectionSize.width/2, sectionSize.height/2);
  highNode->addComponent(physicsBody);
  addChild(highNode);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MapSectionNode::initLowPhysicsBody() {
  const int unitSize = 32;

  const Size sectionSize = getContentSize();

  Vec2 lowPoints[4];
  // up left
  lowPoints[0].x = (-1)* unitSize;
  lowPoints[0].y = obstacleType*unitSize - 2*unitSize - (sectionSize.height/2);

  // up right
  lowPoints[1].x = unitSize;
  lowPoints[1].y = obstacleType*unitSize - 2*unitSize - (sectionSize.height/2);

  // down left
  lowPoints[2].x = (-1)* unitSize;
  lowPoints[2].y = (-1) * (sectionSize.height/2);

  // down right
  lowPoints[3].x = unitSize;
  lowPoints[3].y = (-1) * (sectionSize.height/2);


  PhysicsBody* physicsBody = PhysicsBody::createPolygon(&(lowPoints[0]), 4,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_cliff);
  physicsBody->setCollisionBitmask(CM_wizard);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  Node* lowNode = Node::create();
  lowNode->setPosition(sectionSize.width/2, sectionSize.height/2);
  lowNode->addComponent(physicsBody);

  addChild(lowNode);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MapSectionNode::initPhysicsBody() {
  if (!initHighPhysicsBody()) {
    return false;
  }

  if (!initLowPhysicsBody()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

