#include "StartScene.h"
#include "Block.h"
#include <algorithm>
#include <cstdlib>
#include "Toast.h"
#include "BlockManager.h"

USING_NS_CC;

StartScene::StartScene()
{
}

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(187, 170, 160, 255)) )
    {
        return false;
    }

	auto director = Director::getInstance();
	auto sz = director->getVisibleSize();

	auto matrix_width = std::min(sz.width, sz.height);
	auto bm = BlockManager::create(matrix_width, matrix_width, BLOCK_NUMBER);
	if (bm == nullptr)
	{
		return false;
	}

	bm->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	if (sz.width > sz.height)
	{
		bm->setPosition(sz.width - sz.height, 0);
	}
	else
	{
		bm->setPosition(0, 0);
	}

	addChild(bm, 1, "block_manager");
	// Ìí¼Ó¼üÅÌ¼àÌýÆ÷
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(StartScene::HandleKeyPressed, this);
	keyListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event) { log("Key %d released.", (int)keyCode); };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	// Ìí¼Ó´¥Ãþ¼àÌýÆ÷
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [](Touch *touch, Event *unused_event) {
		Vec2 location = touch->getLocation();
		log("sprite begin... x = %f, y = %f", location.x, location.y);
		return true;
	};

	touchListener->onTouchEnded = CC_CALLBACK_2(StartScene::HandleTouch, this);

	touchListener->onTouchMoved = [](Touch *touch, Event *unused_event) {
		Vec2 location = touch->getLocation();
		log("sprite move... x = %f, y = %f", location.x, location.y);
	};

	touchListener->onTouchCancelled = [](Touch *touch, Event *unused_event) {
		Vec2 location = touch->getLocation();
		log("sprite cancel... x = %f, y = %f", location.x, location.y);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}

void StartScene::HandleTouch(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Vec2 now_pt = touch->getLocation();
	Vec2 prev_pt = touch->getStartLocation();
	log("sprite end... (%f, %f) -> (%f, %f)", prev_pt.x, prev_pt.y, now_pt.x, now_pt.y);
	auto x_axis = now_pt.x - prev_pt.x;
	auto y_axis = now_pt.y - prev_pt.y;
	auto bm = getChildByName<BlockManager*>("block_manager");
	if (bm == nullptr)
	{
		return;
	}

	auto block_gap = bm->getBlockGap();
	if (std::abs(x_axis) >= std::abs(y_axis))
	{
		if (x_axis <= -block_gap)	// ×ó
		{
			bm->handleAction("left");
		}
		else if (x_axis >= block_gap)
		{
			bm->handleAction("right");
		}
	}
	else
	{
		if (y_axis <= -block_gap)	// ÏÂ
		{
			bm->handleAction("down");
		}
		else if (y_axis >= block_gap)
		{
			bm->handleAction("up");
		}
	}
}

void StartScene::HandleKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key %d pressed.", (int)keyCode);

	auto bm = getChildByName<BlockManager*>("block_manager");
	if (bm == nullptr)
	{
		return;
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		bm->handleAction("left");
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		bm->handleAction("right");
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		bm->handleAction("up");
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		bm->handleAction("down");
	}
	else
	{
		log("usage: left, right, up, down");
	}
}
