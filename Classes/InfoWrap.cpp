#include "InfoWrap.h"

InfoWrap::InfoWrap(int width, int height)
{
	this->width = width;
	this->height = height;
}


InfoWrap::~InfoWrap()
{
}

bool InfoWrap::init()
{
	return Node::init();
}

InfoWrap* InfoWrap::create(int width, int height)
{
	auto info = new InfoWrap(width, height);
	if (info && info->init())
	{
		info->autorelease();
		return info;
	}
	CC_SAFE_DELETE(info);
	return nullptr;
}