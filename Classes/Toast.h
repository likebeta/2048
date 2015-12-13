#pragma once

#include <cocos2d.h>

class PacToast: public cocos2d::LayerColor
{
public:
	static bool toast(cocos2d::Node* node, const std::string& msg, const float& time);
public:
	void removeToast(cocos2d::Node* node);
};
