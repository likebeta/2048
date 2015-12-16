#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Block;

class StartScene : public cocos2d::LayerColor
{
public:
	StartScene();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(StartScene);
private:
	void HandleTouch(cocos2d::Touch* touch, cocos2d::Event* event);
	void HandleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void HandleProcess(const std::string& direction);
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
	void SetAnimation(bool ing);
private:
	const int BLOCK_NUMBER = 4;
	float matrix_width;
	float block_gap;
	float block_width;
	std::set<int> free_blocks;
	bool is_animation;
};

#endif // __HELLOWORLD_SCENE_H__
