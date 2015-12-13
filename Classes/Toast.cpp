#include "Toast.h"

USING_NS_CC;

bool PacToast::toast(cocos2d::Node* node, const std::string& msg, const float& time)
{
	log("toast: %s", msg.c_str());
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithSystemFont(msg.c_str(), "", 30, Size(0, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
	label->setColor(Color3B::WHITE);
	label->ignoreAnchorPointForPosition(false);
	label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	label->setOpacity(100);
	label->setPosition(10, 8);

	auto sz = label->getContentSize() + Size(20, 16);
	auto layer = (PacToast*)(PacToast::create(Color4B(130, 120, 120, 100), sz.width, sz.height));
	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	layer->setOpacity(100);
	layer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	auto action_in = FadeIn::create(time / 5);
	auto action_delay = DelayTime::create(time / 5 * 1.5);
	auto action_out = FadeOut::create(time / 5 * 2.5);
	auto action_callback = CallFuncN::create(CC_CALLBACK_0(PacToast::removeToast, layer, label));
	auto seq = Sequence::create(action_in, action_delay, action_out, action_callback, nullptr);
	layer->addChild(label);
	node->addChild(layer, 100);
	layer->runAction(seq);
	label->runAction(seq->clone());
	return true;
}

void PacToast::removeToast(Node* node)
{
	log("node = %s", node->getDescription().c_str());
	this->removeFromParentAndCleanup(true);
}