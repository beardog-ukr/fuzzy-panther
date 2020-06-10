#include "asteroids/SpaceshipNode.h"
using namespace asteroids;
#include "asteroids/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath> // round
#include <cstdlib> //div
static const double pi = acos(-1);

USING_NS_CC;
using namespace std;

enum SpaceshipNodeAnimationTags {
  SNAT_RotationClockwice,
  SNAT_RotationCounterClockwice,
  SNAT_Move
};

static const float rotationDurarionOneStep = 2;
static const float rotationValueOneStep = 85;
static const float moveDurarionOneStep = 1.0;

static double calculateNormalizedRotation(const double rotation) {
  double result = rotation;
  if (rotation>=360) {
    result = div((int)round(rotation), 360).rem;
  }
  else if (rotation<0) {
    result = 360 - div((int)round(abs(rotation)), 360).rem;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SpaceshipNode::SpaceshipNode() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SpaceshipNode::~SpaceshipNode() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SpaceshipNode* SpaceshipNode::create(shared_ptr<SixCatsLogger> c6) {

  SpaceshipNode *pRet = new(std::nothrow) SpaceshipNode();
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->init()) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

double SpaceshipNode::getAngle() const {
  float result = getRotation();
  if (result>360) {
    result -= 360;
  }
  else if (result <0) {
    result = result +360;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SpaceshipNode::init() {
  // const char frameName[] = "asteroids/playerShip2_blue.png";
  const char frameName[] = "ship_blue_2.png";
  baseSprite = Sprite::createWithSpriteFrameName(frameName);
  // baseSprite = Sprite::create(frameName);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", frameName);
    return false;
  }

  const int pointsCount = 12;
  const Vec2 pointsArr[pointsCount] = {
    {.x =   -56, .y =    -5  },
    {.x =   -14, .y =    18  },
    {.x =   -4, .y =    38  },
    {.x =   4, .y =    38  },
    {.x =   12, .y =    19  },
    {.x =   54, .y =    -5  },
    {.x =   36, .y =    -33 },
    {.x =   16, .y =    -29 },
    {.x =   11, .y =    -36 },
    {.x =   -13, .y =    -36 },
    {.x =   -17, .y =    -29 },
    {.x =   -37, .y =    -33 }
  };

  PhysicsBody* physicsBody = PhysicsBody::createPolygon(pointsArr, pointsCount,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_ship);
  physicsBody->setCollisionBitmask(CM_asteroid);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  baseSprite->addComponent(physicsBody);
  baseSprite->setTag(IT_ship);


  addChild(baseSprite, ZO_ss);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 SpaceshipNode::calculateNewForwardPosition() {
  const Vec2 currentPosition = getPosition();
  const float currentRotation = calculateNormalizedRotation(getRotation());

  Vec2 result = currentPosition;

  const float flightLength = 60;

  double angle;

  if ((currentRotation>=0)&&(currentRotation<90)) {
    angle = 90 - currentRotation;
    angle = (angle*pi)/180;

    result.x = currentPosition.x + flightLength*cos(angle);
    result.y = currentPosition.y + flightLength*sin(angle);
  }
  else if ((currentRotation>=90)&&(currentRotation<180)) {
    angle = currentRotation -90;
    angle = (angle*pi)/180;

    result.x = currentPosition.x + flightLength*cos(angle);
    result.y = currentPosition.y - flightLength*sin(angle);
  }
  else if ((currentRotation>=180)&&(currentRotation<270)) {
    angle = 270 - currentRotation;
    angle = (angle*pi)/180;

    result.x = currentPosition.x - flightLength*cos(angle);
    result.y = currentPosition.y - flightLength*sin(angle);
  }
  else if ((currentRotation>=270)&&(currentRotation<360)) {
    angle = currentRotation - 270;
    angle = (angle*pi)/180;

    result.x = currentPosition.x - flightLength*cos(angle);
    result.y = currentPosition.y + flightLength*sin(angle);
  }
  else {
    C6_D2(c6, "Bad rotation value ", currentRotation);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SpaceshipNode::moveForward() {
  stopAllActionsByTag(SNAT_RotationClockwice);
  stopAllActionsByTag(SNAT_RotationCounterClockwice);


  const int nra = getNumberOfRunningActionsByTag(SNAT_Move);
  if (nra>0) {
    C6_T3(c6, "Move skipped because it's already running ", nra, " times");
    return;
  }

  Vec2 newPos = calculateNewForwardPosition();


  MoveTo* ma = MoveTo::create(moveDurarionOneStep, newPos);
  ma->setTag(SNAT_Move);

  runAction(ma);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SpaceshipNode::rotateClockwice() {
  Action* ccAction = getActionByTag(SNAT_RotationCounterClockwice);
  if (ccAction!=nullptr) {
    stopAction(ccAction);
    return;
  }

  stopAllActionsByTag(SNAT_RotationClockwice);

  RotateBy* ra = RotateBy::create(rotationDurarionOneStep, rotationValueOneStep);
  ra->setTag(SNAT_RotationClockwice);
  runAction(ra);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SpaceshipNode::rotateCounterClockwice() {
  Action* ccAction = getActionByTag(SNAT_RotationClockwice);
  if (ccAction!=nullptr) {
    stopAction(ccAction);
    return;
  }

  stopAllActionsByTag(SNAT_RotationCounterClockwice);

  RotateBy* ra = RotateBy::create(rotationDurarionOneStep, (-1)*rotationValueOneStep);
  ra->setTag(SNAT_RotationCounterClockwice);
  runAction(ra);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .