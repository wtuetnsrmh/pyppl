#ifndef _GAMECENTERCPP_H_
#define _GAMECENTERCPP_H_
#include "cocos2d.h"
USING_NS_CC;

class GameCenterCpp :public cocos2d::Sprite

{
public:
	static GameCenterCpp * gSpritewithFile(const char *spName);
	virtual ~GameCenterCpp();
	void openGameCenter();
	void submitScore(float score);
	void share(int uiKind,bool isRecordBroken,const char *sharePicPath);
	void gameoverrecord(int gametime,int score,int step,int mergeTimes,int mergeMaxScore,int state);
private:

};


#endif // !_gamecentercpp_h_
