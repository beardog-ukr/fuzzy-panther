#include "bird/WizardNode.h"
#include "bird/CommonUtils.h"
using namespace bird;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

static const float downStepDuration = 0.5; // time to go down for one step
static const float upStepDuration = 0.5; //
static const int stepSize = 32;


enum ActionTagsWizardNode {
  // WNAT_up,
  // WNAT_down,
  WNAT_vertical_move
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

WizardNode::WizardNode() {
  //ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

WizardNode::~WizardNode() {
  // ntdh
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


WizardNode* WizardNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  WizardNode *result = new (std::nothrow) WizardNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }

  if (!result->init()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::doDie(cocos2d::CallFunc *cf ) {
  stopAllActionsByTag(WNAT_vertical_move);
  DelayTime* dt = DelayTime::create(3.0);
  Sequence* seq = Sequence::create(dt, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::doGoDown() {
  stopAllActionsByTag(WNAT_vertical_move);

  ActionInterval* action = prepareDownAction();

  runAction(action);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::doGoUp() {
  stopAllActionsByTag(WNAT_vertical_move);

  ActionInterval* actionDown = prepareDownAction();
  ActionInterval* actionUp = prepareUpAction();
  Sequence* seq = Sequence::create(actionUp, actionDown, nullptr);
  seq->setTag(WNAT_vertical_move);

  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WizardNode::init() {
  const string fn = "bird/wizard_idle_00.png";
  // const string fn = "bird/wizard_fly_04.png";
  if (!initWithFile(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;    //
  }

  if (!initPhysicsBody()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WizardNode::initPhysicsBody() {
  Vec2 points[8] = {
    {.x =  0, .y = 25},
    {.x = 15, .y = 20},
    {.x = 20, .y = 0},
    {.x = 15, .y = -15},
    {.x = 0, .y = -20},
    {.x = -15,.y = -15},
    {.x = -20,.y =  0},
    {.x = -15,.y = 20}
  };

  PhysicsBody* physicsBody = PhysicsBody::createPolygon(&(points[0]), 8,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_wizard);
  physicsBody->setCollisionBitmask(CM_cliff);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  addComponent(physicsBody);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::setLowPoint(const Vec2& lp) {
  lowPoint = lp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::start() {
  doGoDown();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActionInterval* WizardNode::prepareDownAction() const {
  const Vec2 cp = getPosition();
  C6_D4(c6, "Will go from ", cp.x, ":", cp.y);
  C6_D4(c6, "Will go to   ", lowPoint.x, ":", lowPoint.y);
  const float duration = (fabs(cp.y + stepSize - lowPoint.y)/stepSize)*downStepDuration;
  C6_D3(c6, "Will go for ", duration, " seconds");

  MoveTo* mta = MoveTo::create(duration, lowPoint);//
  mta->setTag(WNAT_vertical_move);
  EaseIn* ea = EaseIn::create(mta, 3.0f);
  ea->setTag(WNAT_vertical_move);

  return ea;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActionInterval* WizardNode::prepareUpAction() const {
  Vec2 newPosition = getPosition();
  newPosition.y = newPosition.y + stepSize;

  MoveTo* mta = MoveTo::create(upStepDuration, newPosition);//
  // mta->setTag(WNAT_vertical_move);
  EaseIn* ea = EaseIn::create(mta, 3.0f);
  // ea->setTag(WNAT_vertical_move);

  return ea;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .