#pragma once

#include <cocos2d.h>

class PacToast : public cocos2d::LayerColor
{
public:
	static void makeText(cocos2d::Node* node, const std::string& msg, const float& time);//��̬������������ֱ�ӵ���  
	void removeToast(cocos2d::Node* node);
};
