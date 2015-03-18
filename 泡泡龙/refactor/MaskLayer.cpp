//
//  MaskLayer.cpp
//  bubble2
//
//  Created by MacMini on 14-7-16.
//
//
#include "MaskLayer.h"

MaskLayer::MaskLayer(void)
{
    
}
MaskLayer::~MaskLayer(void)
{
    
}
bool MaskLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));
    
	this->addChild(lc, 0);
    return true;
}
void MaskLayer::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MaskLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(MaskLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(MaskLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//≤ªœÚœ¬¥´µ›¥•√˛
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}

void MaskLayer::onExit()
{
	Layer::onExit();
	
}

bool MaskLayer::onTouchBegan (Touch *touch, Event *unused_event)
{
    return true;
}
void MaskLayer::onTouchMoved (Touch *touch, Event *unused_event)
{
    
}
void MaskLayer::onTouchEnded (Touch *touch, Event *unused_event)
{
    
}