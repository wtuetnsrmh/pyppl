#include "BottomMenuLayer.h"
#include "Global.h"
#include "ShopLayer.h"
#include "SimpleAudioEngine.h"
#include "UpGradeLayer.h"
#include "HelpLayer.h"
using namespace CocosDenshion;

BottomMenuLayer::BottomMenuLayer(void)
{
   _isSound=true;

}
BottomMenuLayer::~BottomMenuLayer(void)
{


}
bool BottomMenuLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
    //this->setContentSize(cocos2d::Size(200,200));
    this->setAnchorPoint(cocos2d::Point(0,0));
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	_menuBtn = Button::create();
	//_menuBtn->loadTextures("ui/menu/menu1.png","ui/menu/menu2.png","");
    _menuBtn->loadTextures("ui/menu/menu1.png","ui/menu/menu1.png","");
	_menuBtn->setTag(BOTTOMMENU);
	_menuBtn->setAnchorPoint(cocos2d::Point(0,0));
	_menuBtn->setTouchEnabled(true);
	this->addChild(_menuBtn);

	_menuBtn->setPosition(cocos2d::Point(0,0));
    _menuBtn->addTouchEventListener(this, toucheventselector(BottomMenuLayer::touchEvent));

//    if(visibleSize.width> UIWIDTH + 20)
//    {
//        this->setScale(visibleSize.width / UIWIDTH , visibleSize.height / UIHEIGHT);
//    }
    if(visibleSize.width<IPADWIDTH)
    {
        float radioX = visibleSize.width / IPADWIDTH;
        float radioY = visibleSize.height / IPADHEIGHT ;
        this->setScale(radioX , radioY);
    }
	return true;
    
    
}
void BottomMenuLayer::openMenu()
{
   auto menuUI= cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/menu/bottommenu.ExportJson");
	menuUI->setPosition(cocos2d::Point(0,0) );
    this->addChild(menuUI);
    menuUI->setTag(MENUUI);

    Button *menu = (Button*)menuUI->getChildByName("menu");
    menu->setTag(OPENMENU);
    menu->addTouchEventListener(this, toucheventselector(BottomMenuLayer::touchEvent));
    
    soundBtn = (Button*)menuUI->getChildByName("soundBtn");
    if(g_miscLayer->isSound==false)
    {
        soundBtn->loadTextures("ui/menu/close.png", "","");
    }
    soundBtn->setTag(MENU_SOUNDBTN);
    soundBtn->setScale(0.3f);
    soundBtn->runAction(ScaleTo::create(0.3f, 1));
    soundBtn->addTouchEventListener(this, toucheventselector(BottomMenuLayer::touchEvent));
    
    Button *shopBtn = (Button*)menuUI->getChildByName("shopBtn");
    shopBtn->setTag(MENU_SHOPBTN);
    shopBtn->setScale(0.3f);
    shopBtn->runAction(ScaleTo::create(0.6f, 1));
    shopBtn->addTouchEventListener(this, toucheventselector(BottomMenuLayer::touchEvent));
    
    Button *talentBtn = (Button*)menuUI->getChildByName("talentBtn");
    talentBtn->setTag(MENU_TALENT);
    talentBtn->setScale(0.3f);
    talentBtn->runAction(ScaleTo::create(0.9f, 1));
    talentBtn->addTouchEventListener(this, toucheventselector(BottomMenuLayer::touchEvent));
    
    Button *helpBtn = (Button*)menuUI->getChildByName("helpBtn");
    helpBtn->setTag(MENU_HELP);
    helpBtn->setScale(0.3f);
    helpBtn->runAction(ScaleTo::create(1.2f, 1));
    helpBtn->addTouchEventListener(this, toucheventselector(BottomMenuLayer::touchEvent));
    
    
    
}
void BottomMenuLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
        case TOUCH_EVENT_BEGAN:
            break;
       case TOUCH_EVENT_MOVED:
            break;
        case  TOUCH_EVENT_ENDED:
            g_miscLayer->playWav("btn.wav" );
            switch (tag)
		{
			case BOTTOMMENU:
                openMenu();
                _menuBtn->setVisible(false);
               break;
            case OPENMENU:
                _menuBtn->setVisible(true);
                this->removeChild(this->getChildByTag(MENUUI));
                break;
            case MENU_SOUNDBTN:
                if(_isSound==true)
                {
                    soundBtn->loadTextures("ui/menu/close.png", "","");
                    _isSound=false;
                    g_miscLayer->isSound=false;
                    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                    
                }else
                {
                    soundBtn->loadTextures("ui/menu/open.png", "","");
                    _isSound=true;
                    g_miscLayer->isSound=true;
                    SimpleAudioEngine::getInstance()->playBackgroundMusic("wav/bg.wav",true);
                }
                break;
            case MENU_SHOPBTN:
               // if (g_miscLayer->getChildByTag(SHOPUI)==NULL)
               // {
                  //  auto ShopUI = ShopLayer::create();
                   // g_miscLayer->addChild(ShopUI);
                   // ShopUI->setTag(SHOPUI);
                //}
				if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
				{
					g_coinShopUI = CoinShopUI::create();
					g_miscLayer->addChild(g_coinShopUI);
					g_coinShopUI->setTag(COINHELPUI);
				}
                break;
            case MENU_TALENT:
                if (g_miscLayer->getChildByTag(UPGRADE)==NULL)
                {
                    g_upGradeLayer = UpGradeLayer::create();
                    g_miscLayer->addChild(g_upGradeLayer);
                    g_upGradeLayer->setTag(UPGRADE);
                }
                break;
            case MENU_HELP:
				if (g_miscLayer->getChildByTag(HELPUI)==NULL)
				{
					auto helpUI = HelpLayer::create();
					g_miscLayer->addChild(helpUI);
					helpUI->setTag(HELPUI);
				}
                break;
		}
            
	}
}

