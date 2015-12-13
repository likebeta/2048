#pragma once

#include <cocos2d.h>

class PacToast : public cocos2d::LayerColor
{
public:
	static void makeText(cocos2d::Node* node, const std::string& msg, const float& time);//静态函数，方便类直接调用  
	void removeToast(cocos2d::Node* node);
};
