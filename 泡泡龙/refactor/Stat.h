#ifndef __Stat_H__
#define __Stat_H__
struct StatData
{
	int bubbleEliminate;
	int bubbleDrop;
	int score;
	int coin;
	int stage;
	int baseScore;

	void reset()
	{
		bubbleEliminate = 0;
		bubbleDrop = 0;
		score = 0;
		baseScore = 0;
		coin = 0;
		stage = 0;
	}
};
#endif