#include "PowerLayer.h"
#include "Global.h"
#include "PowerUpsUI.h"

#define MAXPOWER 150
#define EFFECTSPRITE 1111
PowerLayer::PowerLayer(void)
{
}
PowerLayer::~PowerLayer(void)
{
}
bool PowerLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
    
    cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	MyRect gameFrame = g_gameLayer->getGameFrame();
    //cocos2d::Point uiPoint =cocos2d::Point(gameFrame.x+gameFrame.width, gameFrame.y);
    
    this->setContentSize(cocos2d::Size(677,276));
    this->setPosition(origin.x+visibleSize.width-677,origin.y);
    this->setAnchorPoint(cocos2d::Point(0,0));
    cocos2d::Size  oldSz = this->getContentSize();

    cocos2d::Point oldPo = this->getPosition();

    
    
	Sprite *sp = Sprite::create("power/power.png");
    //Sprite *sp = Sprite::create("ipad/power.png");

    
	_power = ProgressTimer::create(sp);
	_power->setType(ProgressTimer::Type::BAR);
	_power->setMidpoint(cocos2d::Point(0,0));
	_power->setBarChangeRate(cocos2d::Point(1, 0));
	_power->setPosition(cocos2d::Point(0,0));
	_power->setAnchorPoint(cocos2d::Point(0,0));
	_power->setPercentage(0);

	

	Sprite *powerBg = Sprite::create("power/powerBg.png");
    //Sprite *powerBg = Sprite::create("ipad/powerBg.png");
	cocos2d::Size psz = cocos2d::Size(powerBg->getContentSize());
    
	powerBg->addChild(_power,2);
    powerBg->setAnchorPoint(cocos2d::Point(0,0));
	powerBg->setPosition(cocos2d::Point(0,0));
	this->addChild(powerBg,0);
    _powerType=TT_AssistLine;
    
    
    
	powericon = Button::create();
	powericon->setTouchEnabled(true);
	powericon->loadTextures("power/2-01.png","power/2-01.png","");
	this->addChild(powericon,2);
    powericon->setScale(0.7f);
    cocos2d::Size bsz = powericon->getContentSize();
    powericon->setPosition(cocos2d::Point(oldSz.width-bsz.width*0.7f/2-38,oldSz.height/2));
	powericon->addTouchEventListener(this,toucheventselector(PowerLayer::touchEvent));
    

	

	_powerLabel = Label::create("","Copperplate Gothic Bold.ttf",50);
	_powerLabel->setColor(Color3B(255,255,255));
	_powerLabel->setPosition(bsz.width/2+100,oldPo.y+bsz.height/2);
	_powerLabel->setString("0/150");
	this->addChild(_powerLabel);
    
    
    if(visibleSize.width<IPADWIDTH)
    {
        float radioX = visibleSize.width / IPADWIDTH;
        float radioY = visibleSize.height / IPADHEIGHT ;
        this->setScale(radioX , radioY);
		cocos2d::Size  newSz = cocos2d::Size(oldSz.width*radioX, oldSz.height*radioY);
		cocos2d::Point  diff = cocos2d::Point(origin.x+visibleSize.width-newSz.width,origin.y);
        this->setAnchorPoint(cocos2d::Point(0,0));
        this->setPosition(diff);
    }
	return true;
}
void PowerLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		g_miscLayer->playWav("btn.wav");
        if(_isPower==true)
        {
            g_gameLayer->setPowerTrick(_powerType);
            this->reset();
        }
		
	}
}
void PowerLayer::reset()
{
	_isPower=false;
	powericon->setTouchEnabled(false);
    powericon->stopAllActions();
	if(this->getChildByTag(EFFECTSPRITE)!=NULL)
	{
		_effectSprite->stopAllActions();
		this->removeChild(_effectSprite);
	}
	
    g_gameLayer->setPower(0);
    changePowerBar(0);
}
void PowerLayer::changePowerBar(float num)
{
	int progressFrom = _power->getPercentage();

	int progressTo =(static_cast<float>(num)/static_cast<float>(MAXPOWER))*100.0;

	ProgressFromTo *to2 = ProgressFromTo::create(0.2f, progressFrom, progressTo);
	_power->runAction(to2);

	int n = num;
	char s[64];
	sprintf(s,"%d/150",n);
	_powerLabel->setString(s);
}
void PowerLayer::changePowerIcon(Talent_Type type)
{
	_powerType=type;
	int t=type;
	char s[64];
	sprintf(s,"power/%d-01.png",type);
	powericon->loadTextures(s,s,"");
}
void PowerLayer::setPowericonEffect()
{
	_isPower=true;
	this->powerEffect();
	powericon->setTouchEnabled(true);
	powericon->runAction(RepeatForever::create(Sequence::create(
                              ScaleTo::create(0.6f,0.5f),
                              ScaleTo::create(0.6f,0.7f),
                             NULL)));
	
}
void PowerLayer::powerEffect()
{
	if(this->getChildByTag(EFFECTSPRITE)!=NULL)
	{
		return;
	}
	_effectSprite = Sprite::create("power/powerEffect.png");
	_effectSprite->setTag(EFFECTSPRITE);
	this->addChild(_effectSprite,1);
	auto ro1 = RotateTo::create(2, 180, 180);
    auto ro2 = RotateTo::create(2, 0, 0);
	_effectSprite->runAction(RepeatForever::create(Sequence::create(
                            ro1,
                            ro2,
                            NULL)));
	
	_effectSprite->setPosition(this->getContentSize().width-_effectSprite->getContentSize().width/4, this->getContentSize().height/2);
}
void PowerLayer::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PowerLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(PowerLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(PowerLayer::onTouchEnded,this);
	//listener->setSwallowTouches(true);//≤ªœÚœ¬¥´µ›¥•√˛
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}

void PowerLayer::onExit()
{
	Layer::onExit();
	
}
bool PowerLayer::onTouchBegan (Touch *touch, Event *unused_event)
{
	cocos2d::Size  oldSz = this->getContentSize();
    cocos2d::Point oldPo = this->getPosition();

	cocos2d::Rect rect = cocos2d::Rect(oldPo.x,oldPo.y,oldSz.width,oldSz.height); 
	cocos2d::Point touchPo=touch->getLocation();

    if (rect.containsPoint(touchPo))//≈–∂œ¥•√˛µ„ «∑Ò‘⁄ƒø±Íµƒ∑∂Œßƒ⁄
      return true;
	else
      return false;
}
void PowerLayer::onTouchMoved (Touch *touch, Event *unused_event)
{
    
}
void PowerLayer::onTouchEnded (Touch *touch, Event *unused_event)
{
	if(_isPower==true)
	{
		g_miscLayer->playWav("btn.wav");
		g_gameLayer->setPowerTrick(_powerType);
		this->reset();
	}
   
}