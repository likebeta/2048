#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Block;

class StartScene : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(StartScene);
private:
	void HandleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	bool MoveLeft();
	bool MoveRight();
	bool MoveUp();
	bool MoveDown();
	bool MoveLeft(std::vector<int>& blocks);
	bool MoveRight(std::vector<int>& blocks);
	bool MoveUp(std::vector<int>& blocks);
	bool MoveDown(std::vector<int>& blocks);
	int GetFreeBlock();
	void AddFreeBlock(int tag);
	bool InitBlocks();
	Block* RandomNewBlock();
	bool CombineBlocks(Block* b1, Block* b2);
private:
	const int BLOCK_NUMBER = 4;
	const int BLOCK_GAP = 10;
	std::set<int> free_blocks;
};

#endif // __HELLOWORLD_SCENE_H__
