#include "Bubble.h"
#include "GameConst.h"
#include "Global.h"

Bubble::Bubble(void)
{
	this->setEnableShot(true);
	this->setBubbleState(Bubble_S_Normal);
	this->setCollisionCount(0);
	this->setEffectType(B_UNKNOWN);

	this->setSpecialAbility(SPECIAL_ABILITY_UNKNOW); 
	_textLabel = NULL;
	_extraEffectSprite = NULL;

	_attachAttribute = B_UNKNOWN;

	_normalAction = NULL;
}
Bubble::~Bubble(void)
{

}


Bubble * Bubble::initWithType(PAOPAO_TYPE type) //∏˘æ›¥´π˝¿¥µƒ¿‡–Õ¿¥…˙≥…≈›≈›  
{  
    Bubble *pRet = Bubble::create();  
    if (pRet && pRet->initWithFile(getStringByType(type))) //≥ı ºªØ≈–∂œ  
    {  

		if(type == PAOPAO_INK)
			pRet->setInkEffectRange(5);
        
		pRet->setColorType(type); //≥ı ºªØ≥…π¶∫Û£¨Ω´_type–ﬁ∏ƒ≥…µ±«∞µƒtype  
		
		cocos2d::Size  sz = pRet->getContentSize();
        if(type == PAOPAO_TYPE_CENTER)
        {
           pRet->setScale((PAOPAO_SIZE/sz.width)*1.9);
        }else if(type != PAOPAO_HEART)
        {
            pRet->setScale(PAOPAO_SIZE/sz.width);
        }
        
		
		pRet->ignoreAnchorPointForPosition(false);
		
    }  
    else  
    {  
        CC_SAFE_RELEASE(pRet);  
        pRet = NULL;  
    }  

    return pRet;  
}  

Bubble * Bubble::initWithPic(std::string pic) //∏˘æ›¥´π˝¿¥µƒ¿‡–Õ¿¥…˙≥…≈›≈›  
{  
	Bubble *pRet = Bubble::create();  
	if (pRet && pRet->initWithFile(pic)) //≥ı ºªØ≈–∂œ  
	{  
		pRet->setColorType(PAOPAO_TYPE_UNKNOW); //≥ı ºªØ≥…π¶∫Û£¨Ω´_type–ﬁ∏ƒ≥…µ±«∞µƒtype  
	}  
	else  
	{  
		CC_SAFE_RELEASE(pRet);  
		pRet = NULL;  
	}  
	return pRet;  
} 


std::string Bubble::getStringByType(PAOPAO_TYPE type) //≤ªÕ¨¿‡–ÕªÒ»°≤ªÕ¨µƒÕº∆¨◊ ‘¥  
{  
    std::string pRet = "";  
	char s[32];
	
	sprintf(s, "monster/%d-01.png", static_cast<int>(type));
	pRet = s;

    return pRet;  
}

void Bubble::setRowCol(int row, int col)
{
	_row = row; _col=col;	

	this->setAngleForce( pow(row-MAX_ROWS/2, 2) + pow(col-MAX_COLS/2, 2)  );

	return;
	char str[64];
	sprintf(str, "%d,%d", row, col);
	//int rd = g_gameLayer->roundCount(row, col);
	//sprintf(str, "%d|%d,%d", rd, row, col);
	LabelTTF *label = LabelTTF::create(str, "Arial", 10);
	label->setColor(Color3B(255,0,0));
	label->setScale(1/PIC_RADIO);
	label->setPosition(cocos2d::Point(PAOPAO_SIZE/2+5, PAOPAO_SIZE/2+5));
	this->addChild(label);
}

void Bubble::playNormalAction(bool loop)
{
	int total = 10;
	//if(this->getColorType() == PAOPAO_TYPE_CENTER){
	//	total = 13;
	//}
	auto ea = Animation::create();
	for(int i=1; i<=total; i++){
		char s1[64];
		if(i<10)
			sprintf(s1,"monster/%d-0%d.png", this->getColorType(), i);
		else
			sprintf(s1,"monster/%d-%d.png", this->getColorType(), i);

		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(0.1f);
	if(loop == true){
		_normalAction = RepeatForever::create(Animate::create(ea)) ;
		this->runAction( _normalAction );
	}
	else{
		_normalAction = Animate::create(ea);
		this->runAction( _normalAction );
	}
}

void Bubble::stopNormalAction()
{
	if(_normalAction!=NULL)
	{
		this->stopAction(_normalAction);
		_normalAction = NULL;
	}
}

void Bubble::playEffectOnce(BubbleEffect type, float delay)
{
	int picCount = 8;
	if(type >= B_Laster)
		picCount = 20;
	auto ea = Animation::create();
	for(int i=1; i<=picCount; i++){
		char s1[64];
		sprintf(s1,"effect/bubble/%d/%d.png", type, i);

		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	this->runAction( Animate::create(ea) );
}

void Bubble::playEffectForEver(BubbleEffect type, float delay=0.05f)
{
	auto ea = Animation::create();
	for(int i=1; i<=8; i++){
		char s1[64];
		sprintf(s1,"effect/bubble/%d/%d.png", type, i);

		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	this->runAction( RepeatForever::create(Animate::create(ea) ) );
}

void Bubble::playEffectForEver2(BubbleEffect type, float delay=0.05f)
{
	char s1[64];
	sprintf(s1,"effect/bubble/%d/%d.png", type, 1);
	_extraEffectSprite = Sprite::create(s1);
	this->addChild(_extraEffectSprite);
	auto ea = Animation::create();
	for(int i=1; i<=8; i++){
		//char s1[64];
		sprintf(s1,"effect/bubble/%d/%d.png", type, i);

		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	_extraEffectSprite->runAction( RepeatForever::create(Animate::create(ea) ) );
	
	_extraEffectSprite->setPosition(this->getContentSize().width/2,  this->getContentSize().height/2);
}



//ƒ´÷≠∞¸π¸◊°≈›≈›
/*void Bubble::surroundMozhi()
{
	Armature *mArmature = Armature::create("mozhi_1");
	mArmature->setScale(4);
	mArmature->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
	this->addChild(mArmature,1);
	//mArmature->setTag(TAG_MOZHI);

	DelayTime *delayAction = DelayTime::create(0.2f);
	DelayTime *removeDelayAction = DelayTime::create(0.6f);
	CallFuncN *callSelectorAction = CallFuncN::create(mArmature, callfuncN_selector(Bubble::playMozhiSurroundEffect));
	CallFuncN *callSelectorAction2 = CallFuncN::create(this, callfuncN_selector(Bubble::removeMozhiSurroundEffectAndChangeType));
	mArmature->runAction(Sequence::create(delayAction,
		callSelectorAction,
		removeDelayAction,
		callSelectorAction2,
		NULL));
}

void Bubble::playMozhiSurroundEffect(Node *node)
{
	dynamic_cast<Armature *>(node)->getAnimation()->playWithIndex(0);

}
void Bubble::removeMozhiSurroundEffectAndChangeType(Node *node)
{
	this->removeChild(node,true);
	changeType(PAOPAO_STONE);
}
*/

void Bubble::changeType(PAOPAO_TYPE type)
{
	char sn[32];
	if(type != PAOPAO_PASS){
		sprintf(sn, "monster/%d-01.png", type);
	}
	else if(type == PAOPAO_PASS){
		sprintf(sn, "monster/p%d-01.png", this->getColorType());
	}

	Texture2D *textture;  

	Sprite * temp_obj = Sprite::create(sn);  

	textture = temp_obj->getTexture();  


	//∏ƒ±‰my_heroµƒÕº∆¨  
	this->stopAllActions();  

	this->setTexture(textture);  


	if(this->getColorType() >= PAOPAO_TYPE1 && this->getColorType() <= PAOPAO_TYPE6)
		this->setOldColorType(this->getColorType());

	//this->setColorType(type);
	this->_colorType = type;

	if(type == PAOPAO_PLAGUE){
		this->playEffectForEver2(B_Plague, 0.2f);
	}
	else if(type == PAOPAO_INK){
		if(_inkEffectRange!=0){
			//this->playNormalAction(true);
			this->playEffectForEver2(B_Ink, 0.2f);
			this->setLocalZOrder(2);
		}
		//this->setInkEffectRange(5);
		
	}
	else {
		if(_extraEffectSprite!=NULL){
			this->removeChild(_extraEffectSprite, true);
			_extraEffectSprite = NULL;
		}
	}

	
}

void Bubble::changeImage(const char *image)
{
	Texture2D *textture;  
	Sprite * temp_obj = Sprite::create(image);  
	textture = temp_obj->getTexture();  
	this->stopAllActions();  

	this->setTexture(textture); 
}

void Bubble::removeInkEffect()
{
	this->removeChild(_extraEffectSprite, true);
	this->setInkEffectRange(0);
	this->removeChild(_textLabel, true);
}

void Bubble::removeEffect2()
{
	this->removeChild(_extraEffectSprite, true);
	_extraEffectSprite = NULL;
}

int Bubble::getInkEffectRange()
{

	return _inkEffectRange;
}

void Bubble::setInkEffectRange(int range)
{
	_inkEffectRange = range;
	char s[32];
	sprintf(s, "%d", range);
	if(_textLabel==NULL){
		
		_textLabel = Label::create("","",30);
		this->addChild(_textLabel, 2);
		_textLabel->setColor(Color3B(255,255,0));
		//_textLabel->setFontSize(30);
		_textLabel->setPosition(cocos2d::Point( this->getContentSize().width/2,  this->getContentSize().height/2));
	}
	//_textLabel->setString(std::string(s));
}

void Bubble::setAttachAttribute(BubbleEffect attribute)
{
	if(attribute >= B_Attribute_AssistLine){
		this->_attachAttribute = attribute;
		this->playEffectForEver2(B_Attribute_AssistLine, 0.2f);
	}
}

BubbleEffect Bubble::getAttachAttribute()
{
	return this->_attachAttribute;
}


void Bubble::startTingle()
{
	this->schedule(schedule_selector(Bubble::tingle), 5, kRepeatForever, 2);  
}
//∂∂∂Ø
void Bubble::tingle(float dt)
{
	cocos2d::Point  pt = this->getPosition();
	auto move = MoveTo::create(0.3f, cocos2d::Point(pt.x, pt.y+10));
	auto move_back = MoveTo::create(0.8f, pt);

	auto move_bounce = EaseBounceOut::create(move);
	auto move_bounce_back = EaseBounceOut::create(move_back);

    auto bossEffect = CallFuncN::create(this,callfuncN_selector(Bubble::bossEffectOnce));
	auto seq = Sequence::create(bossEffect,move, move_bounce_back, NULL);

	this->runAction(seq);
}
void Bubble::bossEffectOnce(Node *node)
{
    int type = rand()%2+1;
    this->playBossEffectOnce(type);
}
void Bubble::playBossEffectOnce(int type,float delay)
{
    int picCount = 3;
	if(type==2)
		picCount = 5;
	auto ea = Animation::create();
	for(int i=1; i<=picCount; i++){
		char s1[64];
		sprintf(s1,"effect/boss/%d/%d.png", type, i);
        
		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	this->runAction(Animate::create(ea) );
}
void Bubble::setColorType(PAOPAO_TYPE type)
{
	this->_colorType = type;
	if(type == PAOPAO_PLAGUE){
		this->playEffectForEver2(B_Plague, 0.2f);
	}
	if(type == PAOPAO_INK){
		if(_inkEffectRange!=0){
			//this->playNormalAction(true);
			this->playEffectForEver2(B_Ink, 0.2f);
			this->setLocalZOrder(2);
		}
		//this->setInkEffectRange(5);

	}
}

PAOPAO_TYPE Bubble::getColorType()
{
	return this->_colorType;
}