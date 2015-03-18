#include "NextUI.h"

#include "GameConst.h"
#include "Global.h"

#define NUI 1111

NextUI::NextUI(void)
{
}


NextUI::~NextUI(void)
{
}

bool NextUI::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}  
	//this->setScale(0.3);
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));
    
	this->addChild(lc, 0);
	//添加NextUI
	auto nextUI= cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/next/nextUI_1.json");
cocos2d::Size  sz = nextUI->getContentSize();
cocos2d::Point  center = cocos2d::Point(origin.x+visibleSize.width/2-sz.width/2, origin.y+visibleSize.height/2-sz.height/2);
	nextUI->setPosition(center);
	//nextUI->setTag(NUI);
	this->addChild(nextUI);
	
	g_miscLayer->playWav("win.mp3" );
	//下一关按钮
	Button* nextBtn=(Button*)nextUI->getChildByName("continueBtn");
	
	nextBtn->setTag(BTN_NEXT_BARRIER);
	nextBtn->addTouchEventListener(this, toucheventselector(NextUI::touchEvent));
	nextBtn->runAction(RepeatForever::create(Sequence::create(
		ScaleTo::create(0.5f,0.8f),
		ScaleTo::create(0.5f,1),
		NULL)));

	
    
    if(visibleSize.width<IPADWIDTH)
    {
        this->setScale( visibleSize.width / IPADWIDTH ,visibleSize.height / IPADHEIGHT);
        
    }
	//if(visibleSize.width<MIN_UIWIDTH+20)
	//{
		//float radioX = visibleSize.width / sz.width;
        //nextUI->setScaleX(radioX);
		//cocos2d::Size  newSz = cocos2d::Size(sz.width*radioX, sz.height);
		//cocos2d::Point  diff = cocos2d::Point(origin.x+visibleSize.width/2-newSz.width/2, origin.y+visibleSize.height/2-newSz.height/2);
        //nextUI->setPosition(diff);
	//}
	return true;  
}

void NextUI::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case  TOUCH_EVENT_ENDED:
		switch(tag){
		case BTN_NEXT_BARRIER:
			//g_nextUI->setVisible(false);
			//this->setVisible(false);
			g_miscLayer->removeChild(this, true);
			g_gameLayer->nextBarrier();
			if(g_miscLayer->getChildByTag(LOSEUI)!=NULL)
            {
                g_miscLayer->removeChild(g_miscLayer->getChildByTag(LOSEUI));
            }
			
			break;
		}
		break;
	}

}
void NextUI::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(NextUI::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(NextUI::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(NextUI::onTouchEnded,this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}

void NextUI::onExit()
{
	Layer::onExit();
	
}

bool NextUI::onTouchBegan (Touch *touch, Event *unused_event)
{
    return true;
}
void NextUI::onTouchMoved (Touch *touch, Event *unused_event)
{
    
}
void NextUI::onTouchEnded (Touch *touch, Event *unused_event)
{
    
}