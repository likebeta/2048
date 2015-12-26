#pragma once
#include "cocos2d.h"

class Block;

class BlockManager: public cocos2d::LayerColor
{
private:
	int _block_number = 4;
	float _block_gap;
	float _block_width;
	std::set<int> _free_blocks;
	bool _is_animation;
public:
	BlockManager(int block_number);
	~BlockManager();
	static BlockManager* create(GLfloat width, GLfloat height, int block_number);
	bool initBlocks();
	int getFreeBlock();
	Block* randomNewBlock();
	bool combineBlocks(Block* b1, Block* b2);
	void addFreeBlock(int tag);
	void setAnimation(bool ing);
	void handleAction(const std::string& direction);
	bool isAnimation();
	GLfloat getBlockGap();
private:
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	bool moveLeft(std::vector<int>& blocks);
	bool moveRight(std::vector<int>& blocks);
	bool moveUp(std::vector<int>& blocks);
	bool moveDown(std::vector<int>& blocks);
};
