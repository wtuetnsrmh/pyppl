--
-- Author: bbjxl
-- Date: 2015-03-18 21:25:53
--
local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")

local GameLayer = class("GameLayer", function()
    return display.newLayer("GameLayer")
end)

function GameLayer:ctor(params)
	self.playScene = params.playScene
	self.level = params.level

	self:initData()
    self:initUI()

    self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self.onEnterFrame))
end

-- 找到当前最大行数
function GameLayer:findMaxRow()
	local maxRow = 0
	for _,bubble in pairs(self.m_listBubble) do
		if bubble:getRow() > maxRow then
			maxRow = bubble:getRow()
		end
	end
	return maxRow
end

-- 调整位置，保证最下一行在屏幕中间
function GameLayer:adjustLayerPos()
	local maxRow = self:findMaxRow()
	print("maxRow",maxRow)
	local maxY = maxRow * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS
	print("maxY",maxY)
	local moveTargetY = (maxY - display.cy )
	if moveTargetY > 0 then
		transition.moveTo(self,{time = 0.5,y = moveTargetY })
	elseif self:getPositionY() > 0 then
		transition.moveTo(self,{time = 0.5,y = 0 })
	end
end

function GameLayer:onEnterFrame(dt)
    if isCollisionWithBorder(self.m_curReady) then
        self.m_real.x = -self.m_real.x
    end
    
    local pos = cc.p(self.m_curReady:getPositionX(),self.m_curReady:getPositionY())
    self.m_curReady:pos(pos.x + self.m_real.x * BUBBLE_SPEED, pos.y + self.m_real.y * BUBBLE_SPEED)

    -- 如果球出了屏幕顶部太多则不做检测
    if (self.m_curReady:getPositionY() + self:getPositionY()) > (display.height + 40) then
    	self.m_real = cc.p(0,0)
    	self:unscheduleUpdate()
        -- 激活点击发射
        self.playScene:setEnable()
    end

    local collFlag,rc = isCollision(self.m_curReady, self.m_listBubble)
    if collFlag then
        self.m_real = cc.p(0,0)
        
        self:adjustBubblePosition(rc)

        self:execClearBubble(self.m_curReady)

        local fallList = self:checkFallBubble()
        self:FallBubble(fallList)

        self:adjustLayerPos()

        self:unscheduleUpdate()
        -- 激活点击发射
        self.playScene:setEnable()
    end
end

function GameLayer:update()
	self:scheduleUpdate()
end

function GameLayer:setCurReady(curReady)
	local gPos = cc.p(curReady:getPositionX(),curReady:getPositionY())
	dump(gPos)
	local lPos = self:convertToNodeSpace(gPos)
	dump(lPos)
	self.m_curReady = BublleSprite.new(curReady:getModel())
	self.m_curReady:pos(lPos.x,lPos.y):addTo(self)
end

function GameLayer:initData()
    self.m_board = {} --整个面板所有能放的泡泡 -- pairs

    self.m_curReady = nil                       -- 当前正要发射的泡泡

    self.m_listBubble = {}       -- 当前图上所有的球 -- pairs

    self.m_dRadian  = 0                   -- 范围为30度到150;默认为90度， 竖直方向
    self.m_nRotation  = 0                     -- 发射器角度改变方向，也是玩的反弹角度

    self.m_nScore  = 0         -- 游戏所得到的分数

    self.m_nGoldenBubbleCount = 0 -- 特殊球金球
    self.m_nSliveryBubbleCount = 0 -- 特殊球银球

    self.m_real = nil  -- 真实坐标

    local jsonStr = cc.HelperFunc:getFileData(string.format("level-data/%03d.json",self.level))
    local jsonObj = json.decode(jsonStr)
    -- dump(jsonObj.level.graph)
    -- print("jsonObj.level.graph",#jsonObj.level.graph)
    self.graph = jsonObj.level.graph
    -- 重载当前关卡的最大行列数(暂时未用)
    MAX_ROWS = tonumber(jsonObj.level.maxRow)
    MAX_COLS = tonumber(jsonObj.level.maxCol)
    MAX_RANDOM = tonumber(jsonObj.level.maxRandomColors)
    CUR_RANDOM = randomInt(1,MAX_COLOR)
end

-- 测试用的格子
function GameLayer:initTestBar()
    for i = 0,MAX_COLS-1 do
        for j = 0,20 do
            if not (j%2 == 1 and i == MAX_COLS-1) then
                local bar = display.newSprite("#test_bar.png")
                local rcLable =cc.ui.UILabel.new({text = j.."_"..i,size = 20 }):align(display.CENTER, 32, 32):addTo(bar)
                local tempPoint = getPosByRowAndCol(j,i)
                bar:pos(tempPoint.x,tempPoint.y):addTo(self)
            end
        end
    end
end

function GameLayer:initUI()
    self:initBoard()
    self:initTestBar()
end

function GameLayer:hasBall(row, col)
    if self.m_board[ row .. col ] then return true end
    return false
end

--掉落泡泡
function GameLayer:downBubbleAction(pBubble)
    local pos = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
    local offPos = self:convertToNodeSpace(cc.p(pos.x,-100))
    local offY = offPos.y
    pBubble:runAction(
            cc.Sequence:create(
                cc.MoveTo:create((pos.y - offY) / 600.0, cc.p(pos.x, offY)),
                cc.CallFunc:create(function()
                    if pBubble then
                        pBubble:removeSelf()
                    end
                end)
            )
        )
end

-- TODO (这里有BUG，当一次消除太多时，场景重新定位时会出现，有的球没掉落动画，这里最后要放到单独的物理场景中)
--执行可以掉落的泡泡 fallBubbleList为iparis的RowCol结构表
function GameLayer:FallBubble(fallBubbleList)
    for _,rc in pairs(fallBubbleList) do
        local pBubble = self.m_board[ rc.m_nRow .. rc.m_nCol ]
        if pBubble then
            self:downBubbleAction(pBubble)
            self.m_board[ rc.m_nRow .. rc.m_nCol ] = nil
            self.m_listBubble[rc.m_nRow .. rc.m_nCol] = nil
        end
    end

end

function GameLayer:callbackRemoveBubble(obj)
    if obj then
        obj:removeSelf()
    end
end

-- 检查掉落球 --return ipairs
function GameLayer:checkFallBubble()
    local LinkBubbleList = {}
    for i = 0,MAX_COLS - 1 do
        if self.m_board[0 .. i] then
            table.insert(LinkBubbleList, { m_nRow = 0, m_nCol = i })
        end
    end
    if table.nums(LinkBubbleList) == 0 then
        return LinkBubbleList
    end

    local index = 1
    repeat
        local itCur = LinkBubbleList[index]
        local vecRowCol = {}
        GetAround(itCur.m_nRow,itCur.m_nCol,vecRowCol)

        for _,rc in ipairs(vecRowCol) do
            local pBubble = self.m_board[rc.m_nRow .. rc.m_nCol]
            if pBubble then
                local pos = { m_nRow = rc.m_nRow, m_nCol = rc.m_nCol }
                local k = table.RCkeyof(LinkBubbleList,pos)
                if not k then
                    table.insert(LinkBubbleList,pos)
                end
            end
        end

        index = index + 1

    until (index > table.nums(LinkBubbleList))

    local NoLinkBubblelist = {} -- ipairs
    for _,bublle in pairs(self.m_listBubble) do
        local findRowCol = { m_nRow = bublle:getRow(), m_nCol = bublle:getCol() }
        local k = table.RCkeyof(LinkBubbleList, findRowCol)
        if not k then
            table.insert(NoLinkBubblelist, findRowCol)
        end
    end

    return NoLinkBubblelist;
end

--清除一个球的表现动作
function GameLayer:removeBubbleAction(pBubble)
    pBubble:runAction(
        cc.Sequence:create(
            cc.DelayTime:create(0.2),
            cc.FadeOut:create(0.5),
            cc.CallFunc:create(function()
                if pBubble then
                    pBubble:removeSelf()
                end
            end)
        )
    );
end


--清除球的集合
function GameLayer:clearBubble(bubbleList)
    print("#clearBubble num = ",table.nums(bubbleList))
    local nRow, nCol
    for _,rowCol in pairs(bubbleList) do
        nRow = rowCol.m_nRow
        nCol = rowCol.m_nCol
        local obj = self.m_board[nRow .. nCol]
        if obj then
            self:removeBubbleAction(obj)
            self.m_board[nRow .. nCol] = nil
        end

        if self.m_listBubble[ nRow .. nCol ] then
            self.m_listBubble[ nRow .. nCol ] = nil
        end
    end

end

-- 根据调整过后的球的位置和颜色类型， 作出相应的处理，如：金银色特殊泡泡的爆炸，球的下落等
function GameLayer:execClearBubble(pReadyBubble)
    local clearBubbleList = {}
    clearBubbleList = self:findClearBubble(pReadyBubble)
    if clearBubbleList then
        self:clearBubble(clearBubbleList)
    end
end

-- 查找需要清除的球的集合 -- pairs
function GameLayer:findClearBubble(pReadyBubble)
    local clearRowCollist = {}
    if not pReadyBubble then
        return clearRowCollist
    end

    if (pReadyBubble:getType() > 0) then
    -- 特殊球，特殊处理
        clearRowCollist = self:findGoldenBubble(pReadyBubble)
    else
        clearRowCollist = self:findSameBubble(pReadyBubble)
        if table.nums(clearRowCollist) < REMOVE_COUNT then
            print("没有超过3个同色")
            clearRowCollist = nil
        end
    end
    
    return clearRowCollist
end

-- 找到同样的球，并返回找到的同样的球的集合 -- pairs
function GameLayer:findSameBubble(pReadyBubble)
    local samelist = {}
    local nColor= pReadyBubble:getColor()
    local nRow = pReadyBubble:getRow()
    local nCol = pReadyBubble:getCol()
    -- samelist[nRow .. nCol] = {m_nRow = nRow, m_nCol = nCol}
    table.insert(samelist,{m_nRow = nRow, m_nCol = nCol})

    local index = 1
    repeat
        local itCur = samelist[index]
        local vecRowCol = {}
        GetAround(itCur.m_nRow, itCur.m_nCol, vecRowCol)
        print("#vecRowCol",table.nums(vecRowCol))
        -- dump(vecRowCol)
        for i,rowCol in pairs(vecRowCol) do
           local pCurBubble = self.m_board[ rowCol.m_nRow .. rowCol.m_nCol ]

           if pCurBubble and pCurBubble:getColor() == tonumber(nColor) then
                print("nColor , rowCol.m_nRow , rowCol.m_nCol",nColor,rowCol.m_nRow , rowCol.m_nCol)
                local rc = {m_nRow = rowCol.m_nRow, m_nCol = rowCol.m_nCol}
                -- samelist[ rowCol.m_nRow .. rowCol.m_nCol ] = rc
                if not table.RCkeyof(samelist,rc) then
                    table.insert(samelist,rc)
                    print("#samelist",table.nums(samelist))
                end
                
           end
        end

        index = index + 1
    until (index > table.nums(samelist))
    
    return samelist
end

--找出所给数组中与目标点最近的空位 row,col
function GameLayer:findNearRowCol(vecRowCol,curPos)
    local minDis = 10000
    local row,col
    for _,rc in ipairs(vecRowCol) do
        if not self.m_board[rc.m_nRow .. rc.m_nCol] then
            if not row then
                -- init
                row, col = rc.m_nRow, rc.m_nCol
            end
            local dis = cc.pGetDistance( curPos, getPosByRowAndCol(rc.m_nRow,rc.m_nCol) )
            print("dis",dis,rc.m_nRow,rc.m_nCol)
            if dis < minDis then
                minDis = dis
                row, col = rc.m_nRow, rc.m_nCol
            end
        end
    end
    return row,col
end

-- 被撞球及周边球抖动 TODO
function GameLayer:ainmateNearBubble(vecRowCol,row)
	for _,rc in pairs(vecRowCol) do
		if rc.m_nRow <= row then
			local bubble = self.m_board[rc.m_nRow .. rc.m_nCol]
			if bubble then
				local actionM = cc.MoveBy(0.5,cc.p(10,10))
				local actionR = actionM:reverse()
				bubble:runAction(transition.sequence({
					actionM,actionR
					}))
			end
		end
	end
end

function GameLayer:adjustBubblePosition(rowCol)
    local curPos = cc.p(self.m_curReady:getPositionX(),self.m_curReady:getPositionY())
    -- print(curPos.x, curPos.y)
    local row,col = GetRowColByPos(curPos.x, curPos.y)
     -- print(" old row, col",row, col)
    local vecRowCol = {}
    GetAround(rowCol.m_nRow,rowCol.m_nCol,vecRowCol)
    if self.m_board[row .. col] or (not IsValidPos(row,col)) then
        -- print("被撞的球rc",rowCol.m_nRow,rowCol.m_nCol)
        -- print("已经存在泡泡或无效坐标",row,col)
        dump(vecRowCol)
         -- 已经存在泡泡，则找出被撞泡泡四周六个泡中最近的坐标
        row,col = self:findNearRowCol(vecRowCol,curPos)
    end
    -- print(" new row, col",row, col)
    local adjustPos = getPosByRowAndCol(row, col)
    self.m_curReady:pos(adjustPos.x,adjustPos.y)
    
    self.m_curReady:setRowColIndex(row, col)

    self.m_board[row .. col] = self.m_curReady
    self.m_listBubble[row .. col] = self.m_curReady

    -- TODO
    -- self:ainmateNearBubble(vecRowCol,row)
end

function GameLayer:initBoard()
    for i,bublleData in ipairs(self.graph) do 
    	-- 随机颜色球不计算
    	bublleData.color = bublleData.color == MAX_COLOR and MAX_COLOR or (bublleData.color + CUR_RANDOM)% MAX_COLOR
        local bublle = BublleSprite.new(BublleBasic.new(bublleData))
        local tempPoint = getPosByRowAndCol(bublleData.row,bublleData.col)
        bublle:pos(tempPoint.x,tempPoint.y):addTo(self)
        -- self.m_board[bublleData.row .. bublleData.col] = self.m_board[bublleData.row .. bublleData.col] or {}
        self.m_board[bublleData.row .. bublleData.col] = bublle

        self.m_listBubble[bublleData.row .. bublleData.col] = bublle
        -- table.insert(self.m_listBubble,bublle)
    end

end

function GameLayer:clear()
    self.m_board = nil
    self.m_listBubble = nil
end


return GameLayer