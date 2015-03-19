--
-- Author: yzm
-- Date: 2015-03-10 11:11:56
--
BUBBLE_RADIUS = 32 -- 球的半径  65

-- INIT_LINE			(4)


BUBBLE_SPEED		= 30 -- 球速
READY_BUBBLE_POS	= cc.p(display.cx, 128)  -- 发射球的初始位置
WAIT_BUBBLE_POS     = cc.p(display.cx + 100, 100)  -- 等待球的初始位置
COLLISON_OFFSET     = 5 --碰撞检测比真实半径小的数量
---------------------------------------------------------------------
MAX_WAIT_BUBBLE		= 1  -- 最多有几个等待发射球（暂时只能为1）
PI                  = math.pi
MAX_RANDOM          = 1 --最大随机数，这个值由外部配置文件重载（MAX_RANDOM最大为5 即：MAX_COLOR-1）
CUR_RANDOM          = 0 -- 当前随机数，用于加到所有球的颜色值上
MAX_ROWS			= 11 --这个值由外部配置文件重载
MAX_COLS			= 11 --这个值由外部配置文件重载
REMOVE_COUNT		= 3 --多少个可以消除
MAX_COLOR           = 6-- 总共6种颜色球 从1 -6 ：红蓝绿黄紫 随机
GameState =
	{
		GS_START  = 1,
		GS_FLY    = 2,
		GS_FIXED  = 3,
		GS_FALL   = 4,
		GS_END    = 5,
	}

-- 关卡模式
LEVEL_MODEL = {
	class = "class",
	save  = "save",
	wheel = "wheel",
	clear  = "clear",
	time = "time",
}



--[[
未知泡泡：触碰一次或者周围有一个泡泡消除时显示泡泡真正颜色。

黑洞泡泡：发射的泡泡触碰之后会把泡泡吸入黑洞并消除，3次之后黑洞泡泡消。

黑心泡泡：无法被消除只能掉落的特殊泡泡。

虚实泡泡（自创）：一回合实体两回合虚无的泡泡。

炸弹泡泡：能把自己和触碰到的泡泡周围的泡泡炸没

闪电泡泡：触碰一次之后放出闪电，消除所在行的所有泡泡。

三色泡泡(自创)：可以同时被认为为红蓝绿三种颜色，但优先级为红>蓝>绿。
]]
BUBLLE_TYPE = {
	normal = 0,
	fog = 1,
	black = 2,
	solid = 3,
	void = 4,
	bomb = 5,
	flash = 6,
	colour = 7,
}



-- 物理相关
GRAVITY         = -1000
COIN_MASS       = 0
COIN_RADIUS     = 46
COIN_FRICTION   = 0.8
COIN_ELASTICITY = 0.8
WALL_THICKNESS  = 64
WALL_FRICTION   = 1.0
WALL_ELASTICITY = 100

AUDIO_ASSETS = {
	bg      = "sound/bg.wav",
    btn     = "sound/btn.wav",
    clear1     = "sound/clear1.mp3",
    down     = "sound/down.wav",
    shot     = "sound/shot.wav",
    suiji     = "sound/suiji.wav",
}

TEXTUER_ASSETS = {
	"texture/common_ui"
}