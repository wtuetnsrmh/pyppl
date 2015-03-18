#include "MenuLayer.h"
#include "Global.h"
#include "HelpLayer.h"

#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocostudio;
MenuLayer::MenuLayer(void)
{
}


MenuLayer::~MenuLayer(void)
{
}

bool MenuLayer::init()  
{  
    //////////////////////////////  
    // 1. super init first  
    if ( !CCLayer::init() )  
    {  
        return false;  
    }
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();

	//ÃÌº”πÿø®UI
	auto mapUI= cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/menuUI/menuUI.ExportJson"); 
	mapUI->setAnchorPoint( cocos2d::Point(0.5, 0.5) );
	cocos2d::Size  sz = mapUI->getContentSize();
	mapUI->setPosition(cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2)  );
	
	if(visibleSize.width>sz.width){
		mapUI->setScaleX(visibleSize.width/sz.width);
	}
	if(visibleSize.height>sz.height){
		mapUI->setScaleY(visibleSize.height/sz.height);
	}
	//mapUI->setScale(std::max(visibleSize.width/sz.width,visibleSize.height/sz.height) );
	this->addChild(mapUI);
 
	 //ªÒ»°ÃÙ’Ω∞¥≈•
	 Button* playBtn=(Button*)mapUI->getChildByName("playBtn");
     playBtn->setTag(1);
	 playBtn->addTouchEventListener(this, toucheventselector(MenuLayer::touchEvent));
    playBtn->runAction(RepeatForever::create(Sequence::create(
                                                              ScaleTo::create(0.8f,0.8f),
                                                              ScaleTo::create(0.8f,1.0f),
                                                              NULL)));
    Button* helpBtn=(Button*)mapUI->getChildByName("helpBtn");
    helpBtn->setTag(2);
    helpBtn->addTouchEventListener(this, toucheventselector(MenuLayer::touchEvent));
    helpBtn->runAction(RepeatForever::create(Sequence::create(
                                                              DelayTime::create(2.0f),
                                                              ScaleTo::create(0.8f,0.5f),
                                                              ScaleTo::create(0.8f,0.7f),
                                                              DelayTime::create(2.0f),
                                                              NULL)));
    
	 //Ãÿ–ß∂Øª≠
	 Armature *armature = Armature::create("paopaodenglu");
	 armature->setPosition(cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2+115)  );
	 armature->getAnimation()->playWithIndex(0);
	 this->addChild(armature);
	 armature->runAction(Sequence::create(
		 DelayTime::create(5.0f),
		CallFuncN::create(this, callfuncN_selector(MenuLayer::doStart)),
		 NULL));
	
    cocos2d::Size  asz = armature->getContentSize();
    if(visibleSize.width<asz.width){
		armature->setScale(0.8);
	}
    return true;  
}
void MenuLayer::doStart(Node* node)
{
	dynamic_cast<Armature *>(node)->getAnimation()->playWithIndex(1);
}
void MenuLayer::touchEvent(Sprite* pSender,TouchEventType type)
{//∞¥≈•µ„ª˜ ¬º˛
        int tag=((Sprite*)pSender)->getTag();
		switch(type){
		case TOUCH_EVENT_BEGAN:
			
			break;
		case TOUCH_EVENT_MOVED:

			break;
		case  TOUCH_EVENT_ENDED:
            switch(tag){
                case 1:
                    g_miscLayer->playWav("btn.wav");
                    this->removeAllChildren();
                    break;
                case 2:
				if (g_menuLayer->getChildByTag(HELPUI)==NULL)
				{
					auto helpUI = HelpLayer::create();
					g_menuLayer->addChild(helpUI);
					helpUI->setTag(HELPUI);
				}
                    break;
            }
			
			break;
		}
}
void MenuLayer::initChapter()
{

}
