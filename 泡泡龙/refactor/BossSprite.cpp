#include "BossSprite.h"

#include "EffectDef.h"
#include "Global.h"
#include "Bubble.h"

using namespace std;

using namespace cocostudio;

BossSprite::BossSprite(void)
{
	_currentBoss = -1;
	this->setInWhere(NULL);
}


BossSprite::~BossSprite(void)
{
}

BossSprite * BossSprite::initWithType(BOSS_TYPE type)  
{  
	BossSprite *pRet = BossSprite::create();  
	pRet->setCurrentBoss(type);
	
	return pRet;  
}  


void BossSprite::bossShow(Layer *inWhere)
{
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
cocos2d::Size   visibleSize = Director::getInstance()->getWinSize();  
	this->setInWhere(inWhere);
	inWhere->addChild(this);
	this->setPosition(cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2)  );
	this->setVisible(false);
	this->playWarning();
}

void BossSprite::playAnimation()
{
	if(_currentBoss == BOSS_TYPE0){
		Armature *pArmature  = Armature::create("boss_0");
		MyRect gf = g_gameLayer->getGameFrame();
	
		float delay = 3.0f;

		this->addChild(pArmature);
		pArmature->getAnimation()->play("Animation1");

		pArmature->runAction(
			Sequence::create(
			DelayTime::create(0.2f),
			FadeOut::create(delay),
			CallFuncN::create(this, callfuncN_selector(BossSprite::stopAnimation)),
			NULL
			)
			);


		tuMozhi();
	}

}


void BossSprite::playWarning()
{
	int delay = 1.8f;

	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Size   visibleSize = Director::getInstance()->getWinSize();  
		auto pArmature = Armature::create("boss_coming");

	this->setVisible(true);
	this->addChild(pArmature);

	if(pArmature!=NULL){
		pArmature->getAnimation()->play("Animation1");

		pArmature->runAction(
			Sequence::create(
			DelayTime::create(0.2f),
			FadeOut::create(delay),
			CallFuncN::create(this, callfuncN_selector(BossSprite::stopWarningAndShowBoss)),
			NULL
			)
			);
	}
}

void BossSprite::stopWarningAndShowBoss(Node *node)
{
	if(node != NULL){
		node->stopAllActions();
		this->removeChild(node, true);
		this->setVisible(true);

		this->playAnimation();
	}
}

void BossSprite::preloadAnimation()
{

	//ArmatureDataManager::getInstance()->addArmatureFileInfo("monster/boss/boss_0/boss_0.png" , "monster/boss/boss_0/boss_0.plist" , "monster/boss/boss_0/boss_0.ExportJson");
	//warning
	//ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/extra/boss/boss_coming/boss_coming.png" , "effect/extra/boss/boss_coming/boss_coming.plist" , "effect/extra/boss/boss_coming/boss_coming.ExportJson");

	//墨汁
	//ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/bubble/mozhi/mozhi_0/mozhi_0.png" , "effect/bubble/mozhi/mozhi_0/mozhi_0.plist" , "effect/bubble/mozhi/mozhi_0/mozhi_0.ExportJson");
	//墨汁散开
	//ArmatureDataManager::getInstance()->addArmatureFileInfo("effect/bubble/mozhi/mozhi_1/mozhi_1.png" , "effect//bubble/mozhi/mozhi_1/mozhi_1.plist" , "effect//bubble/mozhi/mozhi_1/mozhi_1.ExportJson");

	//登陆界面动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("ui/menuUI/paopaodenglu/paopaodenglu0.png" , "ui/menuUI/paopaodenglu/paopaodenglu0.plist" , "ui/menuUI/paopaodenglu/paopaodenglu.ExportJson");
	//fire
	ArmatureDataManager::getInstance()->addArmatureFileInfo("fire/fire00.png" , "fire/fire00.plist" , "fire/fire.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("fire/fire01.png" , "fire/fire01.plist" , "fire/fire.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("fire/fire02.png" , "fire/fire02.plist" , "fire/fire.ExportJson");

	ArmatureDataManager::getInstance()->addArmatureFileInfo("shizhong/shizhong0.png" , "shizhong/shizhong0.plist" , "shizhong/shizhong.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo("shizhong/shizhong1.png" , "shizhong/shizhong1.plist" , "shizhong/shizhong.ExportJson");

}



void BossSprite::stopAnimation(Node *node)
{
	if(node != NULL){
		this->removeChild(node, true);
	}

	if(this->getInWhere() != NULL){
		this->getInWhere()->removeChild(this);
	}

}

//吐墨汁
void BossSprite::tuMozhi()
{
	DelayTime *delayAction = DelayTime::create(1.0f);
	// perform the selector call
	CallFuncN *callSelectorAction = CallFuncN::create(this, callfuncN_selector(BossSprite::doTuMozhi));
	// run the action
	this->runAction(Sequence::create(delayAction,
		callSelectorAction,
		NULL));


}

void BossSprite::doTuMozhi(Node *node)
{
	//for(int i=0; i<2; i++){
	Bubble *blackPp = Bubble::initWithType(PAOPAO_BLACK);
	g_miscLayer->addChild(blackPp);
	MyRect gf = g_gameLayer->getGameFrame();
	blackPp->setPosition(cocos2d::Point(gf.x + gf.width/2 - 60, gf.y+gf.height/2+90));

	blackPp->runAction(Sequence::create(
		MoveTo::create(3.0f, cocos2d::Point(gf.x + 100 + rand() % (gf.width/2 - 60), gf.y+gf.height-PAOPAO_SIZE/2)),
			CallFuncN::create(this, callfuncN_selector(BossSprite::callMozhiEffect)),
			NULL));
	//}
}

void BossSprite::callMozhiEffect(Node *node)
{
	
}