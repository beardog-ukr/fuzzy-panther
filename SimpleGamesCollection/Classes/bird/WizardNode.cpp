#include "bird/WizardNode.h"
#include "bird/CommonUtils.h"
using namespace bird;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;



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
  // const Size sectionSize = getContentSize();

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

