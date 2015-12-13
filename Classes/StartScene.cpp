#include "StartScene.h"
#include "Block.h"
#include <algorithm>
#include <cstdlib>
#include "Toast.h"

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

	InitBlocks();

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

	bool result = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		result = MoveLeft();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		result = MoveRight();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		result = MoveUp();
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		result = MoveDown();
	}
	else
	{
		log("usage: left, right, up, down");
	}

	std::vector<int> blocks;
	for (int i = 0; i < BLOCK_NUMBER*BLOCK_NUMBER; ++i)
	{
		auto child = getChildByTag<Block*>(i);
		int value = child->getValue();
		blocks.push_back(value);
	}

	bool is_win = false;
	for (auto it : blocks)
	{
		if (it >= 2048)
		{
			is_win = true;
			PacToast::toast(this, "you win", 1.0f);
			break;
		}
	}

	if (result)
	{
		auto b = RandomNewBlock();
		if (b != nullptr)
		{
			// check over
			if (free_blocks.empty() && !MoveLeft(blocks) && !MoveRight(blocks) && !MoveDown(blocks) && !MoveDown(blocks))
			{
				if (!is_win)
				{
					PacToast::toast(this, "you lose", 1.0f);
				}
			}
		}
		else
		{
			PacToast::toast(this, "something error", 1.0f);
		}
	}
	else if (free_blocks.empty() && !is_win)
	{
		if (!MoveLeft(blocks) && !MoveRight(blocks) && !MoveDown(blocks) && !MoveDown(blocks))
		{
			PacToast::toast(this, "you lose", 1.0f);
		}
	}
}

bool StartScene::MoveLeft()
{
	log("left");
	bool success = false;
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
					CombineBlocks(curr, tmp);
					left = i;
					success = true;
					break;
				}
				else
				{
					if (i + 1 == x)
					{
						left = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(i + 1 + y*BLOCK_NUMBER);
						CombineBlocks(curr, tmp);
						left = i;
						success = true;
						break;
					}
				}
			}

			if (left == left_prev && left + 1 != x)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(left + 1 + y*BLOCK_NUMBER);
				CombineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveLeft(std::vector<int>& blocks)
{
	log("test left");
	bool success = false;
	for (int y = 0; y < BLOCK_NUMBER; ++y)
	{
		int left = -1;
		for (int x = 1; x < BLOCK_NUMBER; ++x)
		{
			int curr_tag = x + y*BLOCK_NUMBER;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int left_prev = left;
			for (int i = x - 1; i > left; --i)
			{
				int tmp_tag = i + y*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					blocks[curr_tag] = 0;
					blocks[tmp_tag] = curr_value + tmp_value;
					left = i;
					success = true;
					break;
				}
				else
				{
					if (i + 1 == x)		// 相邻未移动
					{
						left = i;
						break;
					}
					else
					{
						int tmp_tag = i + 1 + y*BLOCK_NUMBER;
						int tmp_value = blocks[tmp_tag];
						blocks[curr_tag] = 0;
						blocks[tmp_tag] = curr_value + tmp_value;
						left = i;
						success = true;
						break;
					}
				}
			}

			if (left == left_prev && left + 1 != x)	// 未移动, 全部是free
			{
				int tmp_tag = left + 1 + y*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveRight()
{
	log("right");
	bool success = false;
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
					CombineBlocks(curr, tmp);
					right = i;
					success = true;
					break;
				}
				else
				{
					if (i - 1 == x)		// 相邻
					{
						right = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(i - 1 + y*BLOCK_NUMBER);
						CombineBlocks(curr, tmp);
						right = i;
						success = true;
						break;
					}
				}
			}

			if (right == right_prev && right - 1 != x)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(right - 1 + y*BLOCK_NUMBER);
				CombineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveRight(std::vector<int>& blocks)
{
	log("test right");
	bool success = false;
	for (int y = 0; y < BLOCK_NUMBER; ++y)
	{
		int right = BLOCK_NUMBER;
		for (int x = BLOCK_NUMBER - 2; x >= 0; --x)
		{
			int curr_tag = x + y*BLOCK_NUMBER;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int right_prev = right;
			for (int i = x + 1; i < right; ++i)
			{
				int tmp_tag = i + y*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					blocks[curr_tag] = 0;
					blocks[tmp_tag] = curr_value + tmp_value;
					right = i;
					success = true;
					break;
				}
				else
				{
					if (i - 1 == x)		// 相邻未移动
					{
						right = i;
						break;
					}
					else
					{
						int tmp_tag = i - 1 + y*BLOCK_NUMBER;
						int tmp_value = blocks[tmp_tag];
						blocks[curr_tag] = 0;
						blocks[tmp_tag] = curr_value + tmp_value;
						right = i;
						success = true;
						break;
					}
				}
			}

			if (right == right_prev && right - 1 != x)	// 未移动设置
			{
				int tmp_tag = right - 1 + y*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveUp()
{
	log("up");
	bool success = false;
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
					CombineBlocks(curr, tmp);
					up = i;
					success = true;
					break;
				}
				else
				{
					if (i - 1 == y)
					{
						up = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(x + (i - 1)*BLOCK_NUMBER);
						CombineBlocks(curr, tmp);
						up = i;
						success = true;
						break;
					}
				}
			}

			if (up == up_prev && up - 1 != y)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(x + (up-1)*BLOCK_NUMBER);
				CombineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveUp(std::vector<int>& blocks)
{
	log("test up");
	bool success = false;
	for (int x = 0; x < BLOCK_NUMBER; ++x)
	{
		int up = BLOCK_NUMBER;
		for (int y = BLOCK_NUMBER - 2; y >= 0; --y)
		{
			int curr_tag = x + y*BLOCK_NUMBER;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int up_prev = up;
			for (int i = y + 1; i < up; ++i)
			{
				int tmp_tag = x + i*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					blocks[curr_tag] = 0;
					blocks[tmp_tag] = curr_value + tmp_value;
					up = i;
					success = true;
					break;
				}
				else
				{
					if (i - 1 == y)		// 相邻未移动
					{
						up = i;
						break;
					}
					else
					{
						int tmp_tag = x + (i - 1)*BLOCK_NUMBER;
						int tmp_value = blocks[tmp_tag];
						blocks[curr_tag] = 0;
						blocks[tmp_tag] = curr_value + tmp_value;
						up = i;
						success = true;
						break;
					}
				}
			}

			if (up == up_prev && up - 1 != y)	// 未移动设置
			{
				int tmp_tag = x + (up - 1)*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveDown()
{
	log("down");
	bool success = false;
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
					CombineBlocks(curr, tmp);
					down = i;
					success = true;
					break;
				}
				else
				{
					if (i + 1 == y)
					{
						down = i;
						break;
					}
					else
					{
						auto tmp = getChildByTag<Block*>(x + (i + 1)*BLOCK_NUMBER);
						CombineBlocks(curr, tmp);
						down = i;
						success = true;
						break;
					}
				}
			}

			if (down == down_prev && down + 1 != y)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(x + (down + 1)*BLOCK_NUMBER);
				CombineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool StartScene::MoveDown(std::vector<int>& blocks)
{
	log("test down");
	bool success = false;
	for (int x = 0; x < BLOCK_NUMBER; ++x)
	{
		int down = -1;
		for (int y = 1; y < BLOCK_NUMBER; ++y)
		{
			int curr_tag = x + y*BLOCK_NUMBER;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int down_prev = down;
			for (int i = y - 1; i > down; --i)
			{
				int tmp_tag = x + i*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					blocks[curr_tag] = 0;
					blocks[tmp_tag] = curr_value + tmp_value;
					down = i;
					success = true;
					break;
				}
				else
				{
					if (i + 1 == y)		// 相邻未移动
					{
						down = i;
						break;
					}
					else
					{
						int tmp_tag = x + (i + 1)*BLOCK_NUMBER;
						int tmp_value = blocks[tmp_tag];
						blocks[curr_tag] = 0;
						blocks[tmp_tag] = curr_value + tmp_value;
						down = i;
						success = true;
						break;
					}
				}
			}

			if (down == down_prev && down + 1 != y)	// 未移动设置
			{
				int tmp_tag = x + (down + 1)*BLOCK_NUMBER;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

int StartScene::GetFreeBlock()
{
	assert(!free_blocks.empty());

	int index = rand() % free_blocks.size();
	auto it = free_blocks.begin();
	while (index-- > 0)
	{
		++it;
	}
	int tag = *it;
	free_blocks.erase(it);
	return tag;
}

void StartScene::AddFreeBlock(int tag)
{
	free_blocks.insert(tag);
}

bool StartScene::InitBlocks()
{
	srand(time(NULL));
	for (int i = 0; i < BLOCK_NUMBER*BLOCK_NUMBER; ++i)
	{
		free_blocks.insert(i);
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

	int block1 = GetFreeBlock();
	int block2 = GetFreeBlock();

	for (int x = 0; x < BLOCK_NUMBER; ++x)
	{
		int _x = x * (BLOCK_GAP + block_width) + BLOCK_GAP + block_width / 2 + diff_x;
		for (int y = 0; y < BLOCK_NUMBER; ++y)
		{
			int tag = x + BLOCK_NUMBER*y;
			int value = 0;
			if (tag == block1 || tag == block2)
			{
				value = 2;
			}
			auto b = Block::create(value, block_width, block_width);
			int _y = y * (BLOCK_GAP + block_width) + BLOCK_GAP + block_width / 2;
			b->setPosition(_x, _y);
			addChild(b, 1, tag);
			log("tag%d", tag);
		}
	}
	return true;
}

Block* StartScene::RandomNewBlock()
{
	if (free_blocks.empty())
	{
		log("no free block exist, maybe win");
		return nullptr;
	}
	int tag = GetFreeBlock();
	auto b = getChildByTag<Block*>(tag);
	b->setValue(2);
	return b;
}

bool StartScene::CombineBlocks(Block* b1, Block* b2)
{
	int tag1 = b1->getTag();
	int tag2 = b2->getTag();
	int value1 = b1->getValue();
	int value2 = b2->getValue();
	AddFreeBlock(tag1);
	free_blocks.erase(tag2);
	b1->setValue(0);
	b2->setValue(value1 + value2);
	return true;
}
