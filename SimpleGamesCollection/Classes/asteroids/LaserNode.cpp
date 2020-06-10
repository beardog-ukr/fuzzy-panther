#include "asteroids/LaserNode.h"
using namespace asteroids;
#include "asteroids/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include <cmath>

USING_NS_CC;
using namespace std;

static const double pi = acos(-1);

const double laserSpeed = 3.0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static double calculateDistance(const Vec2& one, const Vec2& two) {
  double xDiff = one.x - two.x;
  double yDiff = one.y - two.y;
  double result = sqrt(xDiff*xDiff + yDiff*yDiff);

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

LaserNode::LaserNode(const double inAngle) {
  basicAngle = inAngle;

  baseSprite = nullptr;

  upMargin = 680+50;
  downMargin = -50;
  leftMargin = -50;
  rightMargin = 1200 + 50;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

LaserNode::~LaserNode() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 LaserNode::calculatePointA(const Vec2& currentPos, const double rotationAngle) const {
  Vec2 result;

  if ((rotationAngle>=0)&&(rotationAngle<pi/2)) {
    result = calculatePointAupright(currentPos, rotationAngle);
  }
  else if ((rotationAngle>=pi/2)&&(rotationAngle<pi)) {
    result = calculatePointAdownright(currentPos, rotationAngle);
  }
  else if ((rotationAngle>=pi)&&(rotationAngle<(pi*1.5))) {
    result = calculatePointAdownleft(currentPos, rotationAngle);
  }
  else if ((rotationAngle>=(pi*1.5))&&(rotationAngle<(2*pi))) {
    result = calculatePointAupleft(currentPos, rotationAngle);
  }
  else {
    C6_D2(c6, "Bad angle value: ", rotationAngle);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 LaserNode::calculatePointAupright(const cocos2d::Vec2& currentPos,
                                       const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = pi/2 - rotationAngle;

  Vec2 pointA1;
  pointA1.x = rightMargin;
  cathetus = rightMargin - currentPos.x;
  pointA1.y = currentPos.y + (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = upMargin - currentPos.y;
  pointA2.x = currentPos.x + (cathetus/tan(alpha));
  pointA2.y = upMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 LaserNode::calculatePointAdownright(const cocos2d::Vec2& currentPos,
                                         const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = rotationAngle - pi/2;

  Vec2 pointA1;
  pointA1.x = rightMargin;
  cathetus = rightMargin - currentPos.x;
  pointA1.y = currentPos.y - (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = currentPos.y - downMargin;
  pointA2.x = currentPos.x + (cathetus/tan(alpha));
  pointA2.y = downMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 LaserNode::calculatePointAdownleft(const cocos2d::Vec2& currentPos,
                                        const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = (1.5*pi) - rotationAngle;

  Vec2 pointA1;
  pointA1.x = leftMargin;
  cathetus = currentPos.x - leftMargin;
  pointA1.y = currentPos.y - (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = currentPos.y - downMargin;
  pointA2.x = currentPos.x - (cathetus/tan(alpha));
  pointA2.y = downMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 LaserNode::calculatePointAupleft(const cocos2d::Vec2& currentPos,
                                      const double rotationAngle) const {
  Vec2 pointA2;
  double alpha;
  double cathetus;
  Vec2 result;

  alpha = rotationAngle - (1.5*pi);

  Vec2 pointA1;
  pointA1.x = leftMargin;
  cathetus = currentPos.x - leftMargin;
  pointA1.y = currentPos.y + (cathetus*tan(alpha));

  //calculate A2 point
  cathetus = upMargin - currentPos.y;
  pointA2.x = currentPos.x - (cathetus/tan(alpha));
  pointA2.y = upMargin;

  if (calculateDistance(currentPos, pointA1) < calculateDistance(currentPos, pointA2)) {
    result = pointA1;
  }
  else {
    result = pointA2;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

LaserNode* LaserNode::create(const double angle, shared_ptr<SixCatsLogger> c6) {

  LaserNode *pRet = new(std::nothrow) LaserNode(angle);
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

bool LaserNode::init() {

  if (!initBaseSprite()) {
    return false;
  }

  if (!initPhysicsBody()) {
    return false;
  }

  setRotation(basicAngle);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool LaserNode::initBaseSprite() {
  const string frameName = "laser_blue_01.png";
  baseSprite = Sprite::createWithSpriteFrameName(frameName);
  if (baseSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", frameName);
    return false;
  }

  // baseSprite->setOpacity(60);

  addChild(baseSprite, ZO_laser);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool LaserNode::initPhysicsBody() {
  if (baseSprite == nullptr) {
    C6_C1(c6, "Bad call");
    return false;
  }

  PhysicsBody* physicsBody = PhysicsBody::createBox(baseSprite->getContentSize(),
                                                    PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_ship); // note laser uses same category as ships
  physicsBody->setCollisionBitmask(CM_asteroid);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  baseSprite->addComponent(physicsBody);
  baseSprite->setTag(IT_laser);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LaserNode::setAngle(const double rotationAngle) {
  double alpha = (rotationAngle*pi)/180;

  const Vec2 currentPos = getPosition();

  pointA = calculatePointA(currentPos, alpha);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LaserNode::start() {
  setAngle(basicAngle);// calculation is performed hee because only at this point
  //position of the node is defined
  resetMove();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LaserNode::resetMove() {

  stopAllActions();

  MoveTo* mt = MoveTo::create(laserSpeed, pointA);
  RemoveSelf* removeSelf = RemoveSelf::create();

  Sequence* seq = Sequence::create(mt, removeSelf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

