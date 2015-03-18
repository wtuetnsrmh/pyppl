#ifndef __ScoreLayer_H__
#define __ScoreLayer_H__
#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;
class Bubble;

class ScoreLayer :
	public Layer
{
public:
	ScoreLayer(void);
	virtual ~ScoreLayer(void);

	virtual bool init();  
	CREATE_FUNC(ScoreLayer);  

	void addScore(int n);
	void clearScore();
	void showLog(const char * pStr);

	void changeNumber(Sprite *sp, int num);
	
	void changeHpNumber(Sprite *sp, int num);

	void setProgressLeftPaoPao(int progress, int addDifficultyRound);
	void reset();

	void setBossHp(int hp,int bossHp);
	

	void updateGold(int gold);

	void touchEvent(Sprite* pSender,TouchEventType type);
    void setStage(int stage);

	int getScore();
	void resetBossHpShow();
	Sprite *iceSprite;
    int _runGold;

private:
	void resetProgress(Node *node);
private:
	Label *_bossHp;
	ProgressTimer *_blood;
	int _score;

	Sprite **_scoreNumbers;
	

	Bubble ** _progressBubble; //这个进度条没有的时候，表示会增加泡泡了

	int _resetHeartRound;
	int _dieBubbleCount;

	Label *_goldLabel;
    Label *_scoreLabel;
    Label *_stageLabel;

};

#endif

