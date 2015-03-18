#include "HelpLayer.h"
#include "HelpSprite.h"
#include "Global.h"
#include "FileUtil.h"

#define HELPNUM 5
HelpLayer::HelpLayer(void)
{
	_type = 1;
}
HelpLayer::~HelpLayer(void)
{
}
bool HelpLayer::init()
{

	if(!Layer::init())
	{
		return false;
	}
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	lc = LayerColor::create(Color4B(0, 0, 0, 110));

	g_miscLayer->addChild(lc, 0);
	
	_helpSp = HelpSprite::create();
	_helpSp->initWithType(_type);
    _helpSp->cocos2d::Node::setAnchorPoint(cocos2d::Point(0,0));
	this->addChild(_helpSp);
	_helpSp->setPosition(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2);
	


    cocos2d::Size sz = _helpSp->getContentSize();
    cocos2d::Point po =_helpSp->getPosition();
    
	Button *closeBtn=Button::create();
	closeBtn->setTouchEnabled(true);
	closeBtn->loadTextures("help/close1.png","help/close2.png","");
	this->addChild(closeBtn);
	closeBtn->setTag(1);
	closeBtn->setPosition(Point(po.x+sz.width/2-closeBtn->getContentSize().width/2,po.y+sz.height/2-closeBtn->getContentSize().height/2));
	closeBtn->addTouchEventListener(this, toucheventselector(HelpLayer::touchEvent));
    Sprite *pageControlSprite;
	for (int i=1; i<=HELPNUM; i++) {  

		pageControlSprite=Sprite::create("help/y_1.png");

		pageControlSprite->setPosition(Point( origin.x + (visibleSize.width - 5 * pageControlSprite->getContentSize().width)/2 + pageControlSprite->getContentSize().width * (i-1), po.y-sz.height/2-50));

		pageControlSprite->setTag(100+i);  

		this->addChild(pageControlSprite, 1);  

	}

	Texture2D *textture;  
	Sprite *selectedSrite = (Sprite *)this->getChildByTag(101);  
	Sprite * temp_obj = Sprite::create("help/y_2.png"); 
	textture = temp_obj->getTexture(); 
	selectedSrite->setTexture(textture);
	
 



	_curPage=1;  

//	if(visibleSize.width> UIWIDTH + 20)
//	{
//		this->setScale( visibleSize.width / UIWIDTH,visibleSize.height / UIHEIGHT);
//
//	}
	if(visibleSize.width<IPADWIDTH)
	{
		this->setScale( visibleSize.width / IPADWIDTH,visibleSize.height / IPADHEIGHT);
	}


	return true;
}
void HelpLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case  TOUCH_EVENT_ENDED:
		switch(tag){
		case 1:
			this->setVisible(false);
			if(g_miscLayer->getChildByTag(HELPUI)!=NULL)
			{
				g_miscLayer->removeChild(this);
                g_miscLayer->removeChild(lc);
                FileUtil::writeGuide();
			}
			if(g_menuLayer->getChildByTag(HELPUI)!=NULL)
			{
				g_menuLayer->removeChild(this);
                FileUtil::writeGuide();
                g_miscLayer->removeChild(lc);
			}

			break;
		}
		break;
	}

}
void HelpLayer::changePageControlSprite()
{
	for (int i=1; i<=HELPNUM; i++) {  

		if (_curPage==i) { 
			Texture2D *textture;  
			Sprite *selectedSrite = (Sprite *)this->getChildByTag(100+_curPage);
			Sprite * temp_obj = Sprite::create("help/y_2.png"); 
			textture = temp_obj->getTexture(); 
			selectedSrite->setTexture(textture);
		}  
		else  
		{  
			Texture2D *textture;  
			Sprite *selectedSrite = (Sprite *)this->getChildByTag(100+i);  
			Sprite * temp_obj = Sprite::create("help/y_1.png"); 
			textture = temp_obj->getTexture(); 
			selectedSrite->setTexture(textture);
		} 
	}

}
void HelpLayer::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelpLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(HelpLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(HelpLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//≤ªœÚœ¬¥´µ›¥•√˛
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);

}
void HelpLayer::onExit()
{
	Layer::onExit();

}

bool HelpLayer::onTouchBegan (Touch *touch, Event *unused_event)
{
	_touchBeganPoint = touch->getLocation();
	return true;
}
void HelpLayer::onTouchMoved (Touch *touch, Event *unused_event)
{
	_touchMovePoint = touch->getLocation();

}
void HelpLayer::onTouchEnded (Touch *touch, Event *unused_event)
{
	_touchEndPoint = touch->getLocation();
	if(_touchEndPoint.x>_touchBeganPoint.x+50)
	{
		if(_type>1&&_curPage>1)
		{
			_helpSp->changeType(--_type);
			--_curPage;
			this->changePageControlSprite();
		}
		
	}else if(_touchEndPoint.x<_touchBeganPoint.x+50)
	{
		if(_type<HELPNUM&&_curPage<HELPNUM)
		{
			_helpSp->changeType(++_type);
			++_curPage;
			this->changePageControlSprite();
		}
	}
    if (_curPage>=HELPNUM) {
        this->setVisible(false);
        if(g_miscLayer->getChildByTag(HELPUI)!=NULL)
        {
            g_miscLayer->removeChild(this);
            FileUtil::writeGuide();
             g_miscLayer->removeChild(lc);
        }
        if(g_menuLayer->getChildByTag(HELPUI)!=NULL)
        {
            g_menuLayer->removeChild(this);
            FileUtil::writeGuide();
             g_miscLayer->removeChild(lc);
        }
    }
    
}