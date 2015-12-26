#include "Block.h"
#include "Define.h"
#include "Tools.h"

USING_NS_CC;

Block::Block(int value)
{
	_value = value;
	_fake_value = value;
	_ignoreAnchorPointForPosition = false;
}

Block::~Block()
{
}

Block* Block::create(int value, int width, int height)
{
	auto block = new Block(value);
	if (block && block->initWithValue(value, width, height))
	{
		block->autorelease();
		return block;
	}
	CC_SAFE_DELETE(block);
	return nullptr;
}

Color4B Block::getBkColorByValue(int value)
{
	static std::map<int, Color4B> bkColorMap = {
		{ 2, Color4B(0xee, 0xe4, 0xda, 0xFF) },
		{ 4, Color4B(0xed, 0xe0, 0xc8, 0xFF) },
		{ 8, Color4B(0xf2, 0xb1, 0x79, 0xFF) },
		{ 16, Color4B(0xf5, 0x95, 0x63, 0xFF) },
		{ 32, Color4B(0xf6, 0x7c, 0x5f, 0xFF) },
		{ 64, Color4B(0xf6, 0x5e, 0x3b, 0xFF) },
		{ 128, Color4B(0xed, 0xcf, 0x72, 0xFF) },
		{ 256, Color4B(0xed, 0xcc, 0x61, 0xFF) },
		{ 512, Color4B(0xed, 0xc8, 0x50, 0xFF) },
		{ 1024, Color4B(0xed, 0xc5, 0x3f, 0xFF) },
		{ 2048, Color4B(0xed, 0xc2, 0x2e, 0xFF) }
	};

	if (FIND_KEY(bkColorMap, value))
	{
		return bkColorMap[value];
	}
	else
	{
		return Color4B(0xee, 0xe4, 0xda, 100);
	}
}

Color4B Block::getTextColorByValue(int value)
{
	static std::map<int, Color4B> textColorMap = {
		{ 2, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 4, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 8, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 16, Color4B(0x77, 0x6e,0x65, 0xFF) },
		{ 32, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 64, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 128, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 256, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 512, Color4B(0x77, 0x6e,0x65, 0xFF) },
		{ 1024, Color4B(0x77, 0x6e, 0x65, 0xFF) },
		{ 2048, Color4B(0x77, 0x6e, 0x65, 0xFF) }
	};

	if (FIND_KEY(textColorMap, value))
	{
		return textColorMap[value];
	}
	else
	{
		return Color4B(0x77, 0x6e, 0x65, 100);
	}
}

void Block::setValue(int value)
{
	if (value != _value)
	{
		_value = value;
		_fake_value = value;
		auto bkColor = getBkColorByValue(value);
		setColor(Color3B(bkColor));
		setOpacity(bkColor.a);
		auto child = getChildByName<Label*>("label");
		if (value > 0)
		{
			child->setString(integer2str(value));
			auto textColor = getTextColorByValue(value);
			child->setTextColor(textColor);
		}
		else
		{
			child->setString("");
		}
	}
}

void Block::setFakeValue(int value)
{
	_fake_value = value;
}

int Block::getValue()
{
	return _fake_value;
}

bool Block::initWithValue(int value, int width, int height)
{
	auto bkColor = getBkColorByValue(value);
	if (initWithColor(bkColor, width, height))
	{
		string s;
		if (value > 0)
		{
			s = integer2str(value);
		}
		auto size = getFontSizeByValue(value);
		auto label = Label::createWithSystemFont(s, "", size, Size(0, 0), TextHAlignment::CENTER, TextVAlignment::CENTER);
		if (IsNotNull(label))
		{
			auto textColor = getTextColorByValue(value);
			label->setTextColor(textColor);
			label->setPosition(width / 2, height / 2);
			addChild(label, 1, "label");
			return true;
		}
	}

	return false;
}

int Block::getFontSizeByValue(int value)
{
	return 50;
}

Block* Block::clone()
{
	auto b = create(_value, _contentSize.width, _contentSize.height);
	if (b != nullptr)
	{
		auto pos = getPosition();
		b->setPosition(pos);
	}
	return b;
}
