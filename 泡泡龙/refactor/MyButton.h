#ifndef __MyBtton_H__
#define __MyBtton_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

class MyBtton: public Button
{
public:
	MyBtton();
	~MyBtton();
	static MyBtton* create();
private:

};

MyBtton::MyBtton()
{
	//_touchPassedEnabled = false;
}

MyBtton::~MyBtton()
{
}

MyBtton* MyBtton::create()
{
	MyBtton* widget = new MyBtton();
	if (widget && widget->init())
	{
		widget->autorelease();
		//widget->_touchPassedEnabled = true;
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

#endif