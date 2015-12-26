#include "BlockManager.h"
#include <algorithm>
#include <cstdlib>
#include "Block.h"
#include "Toast.h"


USING_NS_CC;

BlockManager::BlockManager(int block_number)
{
	_block_number = block_number;
	_is_animation = false;
	_ignoreAnchorPointForPosition = false;
}

BlockManager::~BlockManager()
{
}

BlockManager* BlockManager::create(GLfloat width, GLfloat height, int block_number)
{
	auto b = new BlockManager(block_number);
	if (b && b->initWithColor(Color4B(0, 0, 0, 0), width, height) && b->initBlocks())
	{
		b->autorelease();
		return b;
	}
	CC_SAFE_DELETE(b);
	return nullptr;
}

bool BlockManager::initBlocks()
{
	srand(time(NULL));
	for (int i = 0; i < _block_number*_block_number; ++i)
	{
		_free_blocks.insert(i);
	}

	/*
	* block_width = 14*gap_width
	* margin_width = gap_width/2 + gap_width/2
	* matrix_width = block_width*_block_number + gap_width*(_block_number+1) + margin_width
	*              = gap_width*(15*_block_number+2)
	*/
	auto sz = getContentSize();
	auto matrix_width = sz.width;
	_block_gap = float(matrix_width) / (15 * _block_number + 2);
	_block_width = _block_gap * 14;

	float diff_x = _block_gap / 2;
	float diff_y = _block_gap / 2;

	int block1 = getFreeBlock();
	int block2 = getFreeBlock();

	for (int x = 0; x < _block_number; ++x)
	{
		int _x = x * (_block_gap + _block_width) + _block_gap + _block_width / 2 + diff_x;
		for (int y = 0; y < _block_number; ++y)
		{
			int tag = x + _block_number*y;
			int value = 0;
			if (tag == block1 || tag == block2)
			{
				value = 2;
			}
			auto b = Block::create(value, _block_width, _block_width);
			int _y = y * (_block_gap + _block_width) + _block_gap + _block_width / 2 + diff_y;
			b->setPosition(_x, _y);
			addChild(b, 1, tag);
			log("tag%d", tag);
		}
	}
	return true;
}

int BlockManager::getFreeBlock()
{
	assert(!_free_blocks.empty());

	int index = rand() % _free_blocks.size();
	auto it = _free_blocks.begin();
	while (index-- > 0)
	{
		++it;
	}
	int tag = *it;
	_free_blocks.erase(it);
	return tag;
}

Block* BlockManager::randomNewBlock()
{
	if (_free_blocks.empty())
	{
		log("no free block exist, maybe win");
		return nullptr;
	}
	int tag = getFreeBlock();
	auto b = getChildByTag<Block*>(tag);
	auto tmp = b->clone();
	addChild(tmp, 99);
	auto value = (rand() % 100 <= 10) ? 4 : 2;
	b->setFakeValue(value);
	tmp->setValue(value);
	tmp->setTag(100 + tag);
	tmp->setScale(0.2f);
	_is_animation = true;
	auto zoom_in = CCScaleTo::create(0.2f, 1.0f);
	auto action_callback = CallFuncN::create([this, tmp, b, value](Node* node) {
		tmp->removeFromParentAndCleanup(true);
		b->setValue(value);
		_is_animation = false;
	});
	auto seq = Sequence::create(zoom_in, action_callback, nullptr);
	tmp->runAction(seq);
	return b;
}

bool BlockManager::combineBlocks(Block* b1, Block* b2)
{
	int tag1 = b1->getTag();
	int tag2 = b2->getTag();
	int value1 = b1->getValue();
	int value2 = b2->getValue();
	addFreeBlock(tag1);
	_free_blocks.erase(tag2);
	b1->setValue(0);
	b2->setValue(value1 + value2);
	return true;
}

void BlockManager::addFreeBlock(int tag)
{
	_free_blocks.insert(tag);
}

void BlockManager::setAnimation(bool ing)
{
	_is_animation = ing;
}

bool BlockManager::isAnimation()
{
	return _is_animation;
}

GLfloat BlockManager::getBlockGap()
{
	return _block_gap;
}

void BlockManager::handleAction(const std::string& direction)
{
	if (_is_animation)
	{
		log("is_animation ...");
		return;
	}
	bool result = false;
	if (direction == "left")
	{
		result = moveLeft();
	}
	else if (direction == "right")
	{
		result = moveRight();
	}
	else if (direction == "down")
	{
		result = moveDown();
	}
	else if (direction == "up")
	{
		result = moveUp();
	}

	std::vector<int> blocks;
	for (int i = 0; i < _block_number*_block_number; ++i)
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
		auto b = randomNewBlock();
		if (b != nullptr)
		{
			// check over
			blocks[b->getTag()] = b->getValue();
			if (_free_blocks.empty() && !moveLeft(blocks) && !moveRight(blocks) && !moveDown(blocks) && !moveUp(blocks))
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
	else if (_free_blocks.empty() && !is_win)
	{
		if (!moveLeft(blocks) && !moveRight(blocks) && !moveDown(blocks) && !moveUp(blocks))
		{
			PacToast::toast(this, "you lose", 1.0f);
		}
	}
}

bool BlockManager::moveLeft()
{
	log("left");
	bool success = false;
	for (int y = 0; y < _block_number; ++y)
	{
		int left = -1;
		for (int x = 1; x < _block_number; ++x)
		{
			auto curr = getChildByTag<Block*>(x + y*_block_number);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int left_prev = left;
			for (int i = x - 1; i > left; --i)
			{
				auto tmp = getChildByTag<Block*>(i + y*_block_number);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					combineBlocks(curr, tmp);
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
						auto tmp = getChildByTag<Block*>(i + 1 + y*_block_number);
						combineBlocks(curr, tmp);
						left = i;
						success = true;
						break;
					}
				}
			}

			if (left == left_prev && left + 1 != x)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(left + 1 + y*_block_number);
				combineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveLeft(std::vector<int>& blocks)
{
	log("test left");
	bool success = false;
	for (int y = 0; y < _block_number; ++y)
	{
		int left = -1;
		for (int x = 1; x < _block_number; ++x)
		{
			int curr_tag = x + y*_block_number;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int left_prev = left;
			for (int i = x - 1; i > left; --i)
			{
				int tmp_tag = i + y*_block_number;
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
						int tmp_tag = i + 1 + y*_block_number;
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
				int tmp_tag = left + 1 + y*_block_number;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveRight()
{
	log("right");
	bool success = false;
	for (int y = 0; y < _block_number; ++y)
	{
		int right = _block_number;
		for (int x = _block_number - 2; x >= 0; --x)
		{
			auto curr = getChildByTag<Block*>(x + y*_block_number);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int right_prev = right;
			for (int i = x + 1; i < right; ++i)
			{
				auto tmp = getChildByTag<Block*>(i + y*_block_number);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					combineBlocks(curr, tmp);
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
						auto tmp = getChildByTag<Block*>(i - 1 + y*_block_number);
						combineBlocks(curr, tmp);
						right = i;
						success = true;
						break;
					}
				}
			}

			if (right == right_prev && right - 1 != x)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(right - 1 + y*_block_number);
				combineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveRight(std::vector<int>& blocks)
{
	log("test right");
	bool success = false;
	for (int y = 0; y < _block_number; ++y)
	{
		int right = _block_number;
		for (int x = _block_number - 2; x >= 0; --x)
		{
			int curr_tag = x + y*_block_number;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int right_prev = right;
			for (int i = x + 1; i < right; ++i)
			{
				int tmp_tag = i + y*_block_number;
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
						int tmp_tag = i - 1 + y*_block_number;
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
				int tmp_tag = right - 1 + y*_block_number;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveUp()
{
	log("up");
	bool success = false;
	for (int x = 0; x < _block_number; ++x)
	{
		int up = _block_number;
		for (int y = _block_number - 2; y >= 0; --y)
		{
			auto curr = getChildByTag<Block*>(x + y*_block_number);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int up_prev = up;
			for (int i = y + 1; i < up; ++i)
			{
				auto tmp = getChildByTag<Block*>(x + i*_block_number);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					combineBlocks(curr, tmp);
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
						auto tmp = getChildByTag<Block*>(x + (i - 1)*_block_number);
						combineBlocks(curr, tmp);
						up = i;
						success = true;
						break;
					}
				}
			}

			if (up == up_prev && up - 1 != y)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(x + (up - 1)*_block_number);
				combineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveUp(std::vector<int>& blocks)
{
	log("test up");
	bool success = false;
	for (int x = 0; x < _block_number; ++x)
	{
		int up = _block_number;
		for (int y = _block_number - 2; y >= 0; --y)
		{
			int curr_tag = x + y*_block_number;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int up_prev = up;
			for (int i = y + 1; i < up; ++i)
			{
				int tmp_tag = x + i*_block_number;
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
						int tmp_tag = x + (i - 1)*_block_number;
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
				int tmp_tag = x + (up - 1)*_block_number;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveDown()
{
	log("down");
	bool success = false;
	for (int x = 0; x < _block_number; ++x)
	{
		int down = -1;
		for (int y = 1; y < _block_number; ++y)
		{
			auto curr = getChildByTag<Block*>(x + y*_block_number);
			int curr_value = curr->getValue();
			if (curr_value <= 0)
			{
				continue;
			}
			int down_prev = down;
			for (int i = y - 1; i > down; --i)
			{
				auto tmp = getChildByTag<Block*>(x + i*_block_number);
				int tmp_value = tmp->getValue();
				if (tmp_value <= 0)
				{
					continue;
				}
				else if (tmp_value == curr_value)	// 合并
				{
					combineBlocks(curr, tmp);
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
						auto tmp = getChildByTag<Block*>(x + (i + 1)*_block_number);
						combineBlocks(curr, tmp);
						down = i;
						success = true;
						break;
					}
				}
			}

			if (down == down_prev && down + 1 != y)	// 未移动设置
			{
				auto tmp = getChildByTag<Block*>(x + (down + 1)*_block_number);
				combineBlocks(curr, tmp);
				success = true;
			}
		}
	}
	return success;
}

bool BlockManager::moveDown(std::vector<int>& blocks)
{
	log("test down");
	bool success = false;
	for (int x = 0; x < _block_number; ++x)
	{
		int down = -1;
		for (int y = 1; y < _block_number; ++y)
		{
			int curr_tag = x + y*_block_number;
			int curr_value = blocks[curr_tag];
			if (curr_value <= 0)
			{
				continue;
			}
			int down_prev = down;
			for (int i = y - 1; i > down; --i)
			{
				int tmp_tag = x + i*_block_number;
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
						int tmp_tag = x + (i + 1)*_block_number;
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
				int tmp_tag = x + (down + 1)*_block_number;
				int tmp_value = blocks[tmp_tag];
				blocks[curr_tag] = 0;
				blocks[tmp_tag] = curr_value + tmp_value;
				success = true;
			}
		}
	}
	return success;
}