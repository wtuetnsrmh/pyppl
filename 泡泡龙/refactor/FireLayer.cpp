#include "FireLayer.h"
#include "Global.h"
#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocostudio;
FireLayer::FireLayer(void)
{
}


FireLayer::~FireLayer(void)
{
}

bool FireLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();

    //LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 200));
    
	//this->addChild(lc, 0);
    
	 Armature *armature = Armature::create("fire");
	 armature->setPosition(cocos2d::Point(origin.x+visibleSize.width/2-2, origin.y+visibleSize.height/2+8)  );
	 armature->getAnimation()->playWithIndex(0);
	 this->addChild(armature);

    if(visibleSize.width> UIWIDTH)
	{
		this->setScale( visibleSize.width / UIWIDTH,visibleSize.height / UIHEIGHT);
        
	}
	if(visibleSize.width<UIWIDTH)
	{
		this->setScale( visibleSize.width / UIWIDTH,visibleSize.height / UIHEIGHT);
	}
    
    
	return true;
    
}