#include "MiscLayer.h"
#include "LQFlyNumber.h"
#include "SimpleAudioEngine.h"
#include "EffectDef.h"
#include "Global.h"
#include "Bubble.h"
#include "BossSprite.h"
#include "AssistLine.h"
#include "AlertDialog.h"
#include "FireLayer.h"
#include "EffectSprite.h"
#include "MaskLayer.h"

using namespace std;
using namespace CocosDenshion; 

using namespace cocostudio;

#define POWEREFFECT 2222
#define LOADING 1111
MiscLayer::MiscLayer(void)
{
	_lastBossShowtime = time(NULL);
	LINE_LENGTH[0] = 770;
	LINE_LENGTH[1] = 649;
	LINE_LENGTH[2] = 524;
	LINE_LENGTH[3] = 432;
	LINE_LENGTH[4] = 378;
	LINE_LENGTH[5] = 285;
	LINE_LENGTH[6] = 184;
}


MiscLayer::~MiscLayer(void)
{
}

bool MiscLayer::init()  
{  
	//////////////////////////////  
	// 1. super init first  
	if ( !Layer::init() )  
	{  
		return false;  
	} 

	return true;  
}  

void MiscLayer::flyNumber(std::list<Point> &ptList)
{
	for (list<Point>::iterator it = ptList.begin(); it != ptList.end(); ++it)
	{
		cocos2d::Point pt = (*it);
		auto sprite = LQFlyNumber::create(1, true);
		this->addChild(sprite);
		sprite->setPosition(pt);

		sprite->startNumberAnimation();
	}

}


void MiscLayer::dropBubbleAction(Bubble* pBubble)
{

	cocos2d::Point bPo = pBubble->getPosition();
	MyRect gf = g_gameLayer->getGameFrame();
	float x = pBubble->getPosition().x;
	float y = pBubble->getPosition().y;
	cocos2d::Point center = g_gameLayer->getCenterPoint();
	float tox1 = x;
	float toy1 = PAOPAO_SIZE/2 + y;
	
	/*if(x > center.x){
		tox1 = x + rand()% PAOPAO_SIZE;
	}
	else{
		tox1 = x - rand()%PAOPAO_SIZE;
	}*/
	float tox2 = 0.0f;
    float deltadist = rand()% PAOPAO_SIZE * 2+2*PAOPAO_SIZE;
	if(rand()%2==0){
		tox1 = x + deltadist;
		tox2 = std::min( x + rand() % 3 * PAOPAO_SIZE, (float)gf.x+gf.width );
	}
	else{
		tox1 = x - deltadist;
		tox2 = std::max(0.0f, x - rand() % 3 * PAOPAO_SIZE);
	}

	cocos2d::Point startPoint = cocos2d::Point( tox1, toy1 );
	cocos2d::Point endPoint = cocos2d::Point( tox2,  gf.y + PAOPAO_SIZE/2);
    float moveuptime =0.05f*(rand()%1000/1000.0);
	auto move_up = MoveTo::create(moveuptime, cocos2d::Point(x, y));
    float movedowntime =1.0f + deltadist/2.0f/PAOPAO_SIZE*0.6f;
	auto move_down =  EaseBounceOut::create(MoveBy::create(movedowntime, cocos2d::Point(0, gf.y + PAOPAO_SIZE/2 - y) ) );
	auto movehorizontal = EaseOut::create(MoveBy::create(movedowntime,cocos2d::Point(tox1 - x,0)), 1.3f);
    
	ccBezierConfig bc;
	bc.controlPoint_1 = startPoint;
	bc.controlPoint_2 = cocos2d::Point( startPoint.x+(endPoint.x-startPoint.x)*0.5, startPoint.y + (endPoint.y-startPoint.y)*0.5 );
	bc.endPosition = endPoint;
	auto bezier = EaseBounceOut::create( BezierTo::create(1.0f + (float)(rand()%100)/100.0f, bc) );
	
	pBubble->runAction(
		Sequence::create(
		//move_up,
        //DelayTime::create(moveuptime),
		CCCallFuncN::create(this, callfuncN_selector(MiscLayer::playDownBubbleEffect)),
        Spawn::create(move_down, movehorizontal,NULL),
		CCCallFuncND::create(this, callfuncND_selector(MiscLayer::flyOneNumber), (void*)pBubble),
		CCCallFuncND::create(this, callfuncND_selector(MiscLayer::playDropBubbleEffect), (void*)pBubble),
		DelayTime::create(movedowntime),
		FadeOut::create(0.5f),
		CallFuncN::create(this, callfuncN_selector(MiscLayer::removeNodeAfterEffect)),
		NULL
		)
		);

	//‘ˆº”power “‘º∞∂Øª≠

	g_gameLayer->addPower(pBubble->getColorType(),1,bPo);
}

void MiscLayer::flyOneNumber(Node *obj, void *data)
{
	Bubble *pBubble = ((Bubble*)data);
	cocos2d::Point pt = pBubble->getPosition();
	auto sprite = LQFlyNumber::create(1, true);
	this->addChild(sprite);
	sprite->setPosition(pt);

	sprite->startNumberAnimation();
}

void MiscLayer::flyOneNumber2(Point pt, int nubmer)
{
	auto sprite = LQFlyNumber::create(nubmer, true);
	this->addChild(sprite);
	sprite->setPosition(pt);

	sprite->startNumberAnimation();
}

void MiscLayer::clearBubbleAction(Bubble* pBubble,bool isAddPower)
{
	cocos2d::Point bPo=pBubble->getPosition();
	if(isAddPower==false)
	{
		pBubble->runAction(
                           Sequence::create(
                                            CCCallFuncND::create(this, callfuncND_selector(MiscLayer::playClearBubbleEffect), (void*)pBubble),
                                            DelayTime::create(0.8f),
                                            CallFuncN::create(this, callfuncN_selector(MiscLayer::removeNodeAfterEffect)),
                                            NULL
                                            )
                           );
	}
	else
	{
		pBubble->runAction(
                           Sequence::create(
                                            CCCallFuncND::create(this, callfuncND_selector(MiscLayer::playClearBubbleEffect), (void*)pBubble),
                                            //CCCallFuncND::create(this, callfuncND_selector(MiscLayer::playPowerEffect1),(void*)pBubble),
                                            DelayTime::create(0.2f),
                                            //CallFuncN::create(this, callfuncN_selector(MiscLayer::removePlayPowerEffect)),
                                            CCCallFuncND::create(this, callfuncND_selector(MiscLayer::playPowerEffect2),(void*)pBubble),
                                           DelayTime::create(0.5f),
                                           // CallFuncN::create(this, callfuncN_selector(MiscLayer::playPowerEffect3)),
                                            //DelayTime::create(1.2f),
                                            CallFuncN::create(this, callfuncN_selector(MiscLayer::removePlayPowerEffect)),
                                            CallFuncN::create(this, callfuncN_selector(MiscLayer::removeNodeAfterEffect)),
                                            NULL
                                            )
                           );
        
	}
	//‘ˆº”power
    if(isAddPower==true)
		g_gameLayer->addPower(pBubble->getColorType(),0,bPo);
}
void MiscLayer::playPowerEffect1(Node *node, void *data)
{
	cocos2d::Point bPo=((Bubble*)data)->getPosition();
    
	if(this->getChildByTag(POWEREFFECT)!=NULL)
	{
		this->removeChild(powerEffect);
	}
	powerEffect = EffectSprite::initWithType(1);
	this->addChild(powerEffect);
	powerEffect->playEffectOnce();
	powerEffect->setPosition(bPo);
	powerEffect->setTag(POWEREFFECT);
	//powerEffect->setScale(PAOPAO_SIZE);
}
void MiscLayer::playPowerEffect2(Node *node, void *data)
{
	cocos2d::Point bPo=((Bubble*)data)->getPosition();
    
	if(this->getChildByTag(POWEREFFECT)!=NULL)
	{
		this->removeChild(powerEffect);
	}
	powerEffect = EffectSprite::initWithType(2);
	this->addChild(powerEffect);
	powerEffect->playEffectOnce();
	powerEffect->setPosition(bPo);
	powerEffect->setTag(POWEREFFECT);
	//powerEffect->setScale(PAOPAO_SIZE/);
    
	MyRect gameFrame = g_gameLayer->getGameFrame();
    cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	//cocos2d::Point uiPoint =cocos2d::Point(visibleSize.width-230,30);
    cocos2d::Point uiPoint =cocos2d::Point(visibleSize.width-230,90);
	powerEffect->runAction(MoveTo::create(0.5f,uiPoint));
    
}
void MiscLayer::playPowerEffect3(Node *node)
{
	MyRect gameFrame = g_gameLayer->getGameFrame();
    cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	if(this->getChildByTag(POWEREFFECT)!=NULL)
	{
		this->removeChild(powerEffect);
	}
	powerEffect = EffectSprite::initWithType(3);
	this->addChild(powerEffect);
	powerEffect->playEffectOnce();
	powerEffect->setPosition(visibleSize.width-powerEffect->getContentSize().width/2-66,powerEffect->getContentSize().height/2);
	powerEffect->setTag(POWEREFFECT);
    
    
    cocos2d::Size oldSz = powerEffect->getContentSize();
    
    if(visibleSize.width> UIWIDTH)
    {
        float radioX = visibleSize.width / UIWIDTH;
        float radioY = visibleSize.height / UIHEIGHT ;
        powerEffect->setScale(radioX , radioY);
		cocos2d::Size  newSz = cocos2d::Size(oldSz.width*radioX, oldSz.height*radioY);
		cocos2d::Point  diff = cocos2d::Point(visibleSize.width-newSz.width-30,30);
        powerEffect->setAnchorPoint(cocos2d::Point(0,0));
        powerEffect->setPosition(diff);
    }
}
void MiscLayer::removePlayPowerEffect(Node *node)
{
	if(this->getChildByTag(POWEREFFECT)!=NULL)
	{
		this->removeChild(powerEffect);
	}
}
void MiscLayer::playDownBubbleEffect(Node *node)
{
	//g_miscLayer->playWav();
}
void MiscLayer::playDropBubbleEffect(Node *obj, void *data)
{
	((Bubble*)data)->playEffectOnce(B_Drop_Broken);
	g_miscLayer->playMp3("clear1.mp3");
}

void MiscLayer::removeNodeAfterEffect(Node *obj)
{
	if (obj != NULL)
	{
		g_miscLayer->removeChild(obj, true);
	}
}

void MiscLayer::playClearBubbleEffect(Node *obj, void *data)
{
	if (((Bubble*)data)->getColorType() == PAOPAO_BOMB)
	{
		//((Bubble*)data)->playEffectOnce(B_Bumb_Red);
		((Bubble*)data)->removeEffect2();
		((Bubble*)data)->playEffectOnce(B_Bumb_Broken);
	}
	else{
		((Bubble*)data)->playEffectOnce(B_Bumb_Broken);
	}
}

void MiscLayer::showLoseUI()
{

}

void MiscLayer::removeLoseUI()
{
	this->removeAllChildren();
}

void MiscLayer::playWav(const char * file)
{
    if(isSound==false)return;
	char str[64];
	sprintf(str, "wav/%s", file);
	SimpleAudioEngine::getInstance()->playEffect( str );
}


void MiscLayer::playMp3(const char * file)
{
    if(isSound==false)return;
	char str[64];
	sprintf(str, "wav/%s", file);
	SimpleAudioEngine::getInstance()->playEffect( str );
}
void MiscLayer::playBackgroundMusic(const char * file)
{
    //if(isSound==false)return;
	char str[64];
	sprintf(str, "wav/%s", file);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(str, true);
}


void MiscLayer::showAlert(int type)
{
	AlertDialog * dlg = AlertDialog::create();
	this->addChild(dlg);
	dlg->setText(type);
}
void MiscLayer::flyToPower(Point pobubble)
{
	Sprite *sp = Sprite::create("effect/bubble/pass.png");
	sp->setPosition(pobubble);
	this->addChild(sp);

	//sp->runAction
}
void MiscLayer::addFireShot(bool isAdd)
{
	
	if(isAdd==true)
	{
		g_fireLayer = FireLayer::create();
		g_miscLayer->addChild(g_fireLayer);
		g_fireLayer->setTag(FIRELAYER);
	}else
	{
		g_miscLayer->removeChild(g_fireLayer);
	}
}
void MiscLayer::addShiZhong(bool isAdd)
{
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();

	if(isAdd==true)
	{
		armature = Armature::create("shizhong");
		armature->setPosition(cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2)  );
		armature->getAnimation()->playWithIndex(0);
		this->addChild(armature);
	}else
	{
		this->removeChild(armature);
	}
}
void MiscLayer::fireLaster(cocos2d::Point endPoint)
{
	cocos2d::Point startPoint = g_shotLayer->getGunPosition();
		//float angle = ::atan2(endPoint.y-startPoint.y, endPoint.x-startPoint.x);
		
		float offRealY = endPoint.y - startPoint.y ;
		if(offRealY>0)
			offRealY = -0.0001f;
		float offRealX = endPoint.x - startPoint.x;
		float angleRadians = atanf((float)offRealX / (float)offRealY);

		float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
		
		float length =   sqrt((endPoint.x-startPoint.x)*(endPoint.x-startPoint.x) + (endPoint.y-startPoint.y)*(endPoint.y-startPoint.y));

		char s[32];
		int laster = this->getShortestLine(length);
		sprintf(s, "effect/extra/laster/%d.png", laster);

		float cosV = cos(abs(angleRadians));
		float needLength = length;
		/*if(cosV < 1e-4)
			needLength = length;
		else
			needLength = length /cosV;
		*/

		Sprite *sp = Sprite::create(s);
		sp->setAnchorPoint(cocos2d::Point(0.5, 0.5));
		this->addChild(sp);
		sp->setPosition(startPoint);

		sp->setScale(needLength/LINE_LENGTH[laster-1]);


		sp->setRotation( angleDegrees );

		sp->runAction(
			Sequence::create(
			FadeOut::create(1.0f),
			DelayTime::create(0.5f),
			CallFuncN::create(this, callfuncN_selector(MiscLayer::removeNodeAfterEffect)),
			NULL
			)
			);
	
}

int MiscLayer::getShortestLine(float length)
{
	int min = 100000;
	int whichLine = 1;
	for(int i=1; i<=7; i++)
	{
		int lineLength = LINE_LENGTH[i-1];
		if(min > abs(lineLength - length))
		{
			whichLine = i;
			min = abs(lineLength - length);
		}
	}
	return whichLine;
}
void MiscLayer::showLoading(bool isLoading)
{
    if (this->getChildByTag(LOADING)!=NULL&&isLoading==false) {
        this->removeChildByTag(LOADING);
        this->removeChildByTag(MASKLAYER);
    }else if(this->getChildByTag(LOADING)==NULL&&isLoading==true)
    {
        MaskLayer *mask = MaskLayer::create();
        this->addChild(mask);
        mask->setTag(MASKLAYER);
        
        Sprite *load = Sprite::create("effect/bubble/400/1.png");
        this->addChild(load);
        load->setTag(LOADING);
        load->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
        
        Animation *ea = Animation::create();
        for(int i=1; i<=9; i++){
            char s1[64];
            sprintf(s1,"effect/bubble/400/%d.png",i);
            
            ea->addSpriteFrameWithFile(s1);
        }
        ea->setDelayPerUnit(0.3f);
        
        load->runAction( RepeatForever::create( Animate::create(ea) ));
    }
    
}
