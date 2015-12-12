#include "StartScene.h"
#include "Block.h"
#include <algorithm>

USING_NS_CC;

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
    if ( !LayerColor::initWithColor(Color4B(0xBB, 0xAA, 0xA0, 0xFF)) )
    {
        return false;
    }

	auto director = Director::getInstance();
	auto sz = director->getVisibleSize();

	int square_sz = std::min(sz.width, sz.height);
	int block_width = (square_sz - BLOCK_GAP*(BLOCK_NUMBER + 1)) / BLOCK_NUMBER;

	int diff_x = 0;
	if (sz.width > sz.height)
	{
		diff_x = sz.width - sz.height;
	}

	for (int i = 0; i < BLOCK_NUMBER; ++i)
	{
		int x = i * (BLOCK_GAP + block_width) + BLOCK_GAP + block_width / 2 + diff_x;
		for (int j = 0; j < BLOCK_NUMBER; ++j)
		{
			auto b = Block::create(2, block_width, block_width);
			int y = j * (BLOCK_GAP + block_width) + BLOCK_GAP + block_width / 2;
			b->setPosition(x, y);
			addChild(b, 1, i + BLOCK_NUMBER*j);
			log("tag%d", i + BLOCK_NUMBER*j);
		}
	}

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(StartScene::HandleKeyPressed, this);
	keyListener->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event* event) { log("Key %d released.", keyCode); };

	// 添加监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    return true;
}

void StartScene::HandleKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key %d pressed.", keyCode);
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		MoveLeft();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		MoveRight();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		MoveUp();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		MoveDown();
	}
}

bool StartScene::MoveLeft()
{
	log("left");

	for (int y = 0; y < BLOCK_NUMBER; ++y)
	{
		int left = -1;
		for (int x = 1; x < BLOCK_NUMBER; ++x)
		{
			auto curr = getChildByTag<Block*>(x + y*BLOCK_NUMBER);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int left_prev = left;
			for (int i = x - 1; i > left; --i)
			{
				auto tmp = getChildByTag<Block*>(i + y*BLOCK_NUMBER);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					tmp->setValue(tmp_value * 2);
					curr->setValue(0);
					left = i;
					break;
				}
				else
				{
					if (i == x - 1)
					{
						left = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(i + y*BLOCK_NUMBER);
						tmp->setValue(curr_value);
						curr->setValue(0);
						left = i;
						break;
					}
				}
			}

			if (left == left_prev && left + 1 != x)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(left + 1 + y*BLOCK_NUMBER);
				tmp->setValue(curr_value);
				curr->setValue(0);
			}
		}
	}
	return true;
}

bool StartScene::MoveRight()
{
	log("right");

	for (int y = 0; y < BLOCK_NUMBER; ++y)
	{
		int right = BLOCK_NUMBER;
		for (int x = BLOCK_NUMBER - 2; x >= 0; --x)
		{
			auto curr = getChildByTag<Block*>(x + y*BLOCK_NUMBER);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int right_prev = right;
			for (int i = x + 1; i < right; ++i)
			{
				auto tmp = getChildByTag<Block*>(i + y*BLOCK_NUMBER);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					tmp->setValue(tmp_value * 2);
					curr->setValue(0);
					right = i;
					break;
				}
				else
				{
					if (i == x + 1)
					{
						right = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(i + y*BLOCK_NUMBER);
						tmp->setValue(curr_value);
						curr->setValue(0);
						right = i;
						break;
					}
				}
			}

			if (right == right_prev && right - 1 != x)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(right - 1 + y*BLOCK_NUMBER);
				tmp->setValue(curr_value);
				curr->setValue(0);
			}
		}
	}
	return true;
}

bool StartScene::MoveUp()
{
	log("up");

	for (int x = 0; x < BLOCK_NUMBER; ++x)
	{
		int up = BLOCK_NUMBER;
		for (int y = BLOCK_NUMBER - 2; y >= 0; --y)
		{
			auto curr = getChildByTag<Block*>(x + y*BLOCK_NUMBER);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int up_prev = up;
			for (int i = y + 1; i < up; ++i)
			{
				auto tmp = getChildByTag<Block*>(x + i*BLOCK_NUMBER);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					tmp->setValue(tmp_value * 2);
					curr->setValue(0);
					up = i;
					break;
				}
				else
				{
					if (i == y + 1)
					{
						up = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(x + i*BLOCK_NUMBER);
						tmp->setValue(curr_value);
						curr->setValue(0);
						up = i;
						break;
					}
				}
			}

			if (up == up_prev && up - 1 != y)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(x + (up-1)*BLOCK_NUMBER);
				tmp->setValue(curr_value);
				curr->setValue(0);
			}
		}
	}
	return true;
}

bool StartScene::MoveDown()
{
	log("down");

	for (int x = 0; x < BLOCK_NUMBER; ++x)
	{
		int down = -1;
		for (int y = 1; y < BLOCK_NUMBER; ++y)
		{
			auto curr = getChildByTag<Block*>(x + y*BLOCK_NUMBER);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int down_prev = down;
			for (int i = y - 1; i > down; --i)
			{
				auto tmp = getChildByTag<Block*>(x + i*BLOCK_NUMBER);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					tmp->setValue(tmp_value * 2);
					curr->setValue(0);
					down = i;
					break;
				}
				else
				{
					if (i == y - 1)
					{
						down = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(x + i*BLOCK_NUMBER);
						tmp->setValue(curr_value);
						curr->setValue(0);
						down = i;
						break;
					}
				}
			}

			if (down == down_prev && down + 1 != y)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(x + (down + 1)*BLOCK_NUMBER);
				tmp->setValue(curr_value);
				curr->setValue(0);
			}
		}
	}
	return true;
}
