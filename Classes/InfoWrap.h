#pragma once

#include "cocos2d.h"

class InfoWrap: public cocos2d::Node
{
private:
	int width;
	int height;
public:
	InfoWrap(int width, int height);
	~InfoWrap();
	virtual bool init();
	static InfoWrap* create(int width, int height);
};

