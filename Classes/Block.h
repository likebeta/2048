#pragma once
#include "cocos2d.h"

class Block: public cocos2d::LayerColor
{
private:
	int value;
	int fake_value;
public:
	Block(int value);
	~Block();
	static Block* create(int value, int width, int height);
	static cocos2d::Color4B getBkColorByValue(int value);
	static cocos2d::Color4B getTextColorByValue(int value);
	static int getFontSizeByValue(int value);
	void setValue(int value);
	void setFakeValue(int value);
	int getValue();
	bool initWithValue(int value, int width, int height);
	Block* clone();
};

