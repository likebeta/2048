#include "Toast.h"

USING_NS_CC;

void PacToast::makeText(cocos2d::Node *node, const std::string &msg, const float &time)
{
	log(msg.c_str());
	/*
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto pLabel = Label::createWithSystemFont(msg.c_str(), "", 30);
	pLabel->setColor(Color3B::WHITE);
	pLabel->ignoreAnchorPointForPosition(false);
	pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	auto ly = LayerColor::create(Color4B(130, 120, 120, 255));
	ly->ignoreAnchorPointForPosition(false);
	ly->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ly->setContentSize(pLabel->getContentSize() + Size(20, 15));

	node->addChild(ly, 100);
	node->addChild(pLabel, 100);
	ly->setPosition(Vec2(visibleSize.width / 2, -pLabel->getContentSize().height));
	pLabel->setPosition(ly->getPosition());
	auto seq1 = Sequence::create(FadeIn::create(time / 5), DelayTime::create(time / 5 * 1.5), FadeOut::create(time / 5 * 2.5), CallFuncN::create(ly, CC_CALLFUNCN_SELECTOR(PacToast::removeToast)), nullptr);
	auto seq2 = Sequence::create(EaseSineIn::create(MoveBy::create(time / 5, Vec2(0, 200))), DelayTime::create(time / 5 * 2), EaseSineOut::create(MoveBy::create(time / 3, Vec2(0, -200))), nullptr);
	auto spawn = Spawn::create(seq1, seq2, nullptr);
	auto action = Repeat::create(spawn, 1);
	ly->setOpacity(0);
	pLabel->setOpacity(0);
	ly->runAction(action);
	pLabel->runAction(action->clone());*/
}

void PacToast::removeToast(Node* node)
{
	log("node = %s", node->getDescription().c_str());
	this->removeFromParentAndCleanup(true);
}