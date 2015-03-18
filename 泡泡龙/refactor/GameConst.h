#ifndef __GameConst_H__  
#define __GameConst_H__  
  
#define MAX_COLS 15
#define MAX_ROWS 15  
  
#define MAX_WAIT_PAOPAO 4  
  
#define READY_PAOPAO_POS cocos2d::Point(320, 75)  
  
#define PAOPAO_SPEED 1200.0  

//#define PAOPAO_SIZE 66
#define  PAOPAO_ORIGINAL_SIZE 108
#define PAOPAO_NORMAL_TYPE_COUNT 6

#define PAOPAO_TOTAL_TYPE_COUNT 8

#define PAOPAO_TYPE_CENTER 100

#define PI	(3.14)  

#define REMOVE_COUNT 3

#define  SHOT_BUBBLE_DIFF_GUN_Y 18

#define SCORE_NUMBER 5 //分数最高位数
#define BARRIER_NUMBER 2 //关卡位数
#define COIN_NUMBER 7 

#define SHOW_EFFECT_X_DIFF 5 //出现效果的x位移

#define PROGRESS_PAOPAO_NUMBER 6
#define SKILL_NUMBER 4 //技能数

#define BTN_SHARE 6000
#define BTN_RESTART 6001
#define BTN_MENU 6002
#define BTN_UPGRADE 6003
#define BTN_SHOP 6004


#define BTN_CLOSE 3
#define BTN_NEXT_BARRIER 4

#define NEXTUI 2000
#define LOSEUI 2001
#define SHOPUI 2002
#define UPGRADE 2003
#define HELPUI 2004
#define COINHELPUI 2005
#define GOODSHOP 2010
#define MASKLAYER 2011

#define SHOPBTN 10

#define BUYCOINS 3000 //shop界面按钮
#define LEFTBTN 3001
#define RIGHTBTN 3002
#define BUYGOODSBTN 3003
#define GETCOINSBTN 3004
#define BACKBTN 3005

#define UP_BUYCOINS 4000 //升级天赋界面按钮
#define UP_GETCOINSBTN 4001
#define UP_BACKBTN 4002
#define UP_UPGRADE 4003
#define UP_Clear 4004

#define  TAG_ASSIST_LINE 1000

#define SKILL_BWIDTH 115
#define SKILL_SWIDTH 20

#define TALENTLEVEL 4//天赋最高等级

#define UIWIDTH 768
#define UIHEIGHT 1136

#define IPADWIDTH 1536
#define IPADHEIGHT 2048

#define MIN_UIWIDTH 640
#define MIN_UIHEIGHT 960


#define BOTTOMMENU 5000
#define MENU_SOUNDBTN 5001
#define MENU_SHOPBTN 5002
#define MENU_TALENT 5003
#define MENU_HELP 5004
#define MENUUI 5005
#define OPENMENU 5006

#define P_CHOOSE 7000
#define P_OPEN 7001

#define FIRELAYER 8000

#define NO_GOLD 0
#define BUY_SUCCESS 1

#define MOREGEM 1111
#define YES 1112
#define NO 1113
#define BACK 1114


struct RowCol
{
	bool operator==( const RowCol & rPos ) const 
	{
		return m_nRow == rPos.m_nRow && m_nCol == rPos.m_nCol;
	}

	RowCol( int nRow, int nCol )
	{
		m_nRow = nRow;
		m_nCol = nCol;
	}
	int m_nRow;
	int m_nCol;
};
  
#endif  