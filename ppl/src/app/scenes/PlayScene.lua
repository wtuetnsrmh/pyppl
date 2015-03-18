--
-- Author: yzm
-- Date: 2015-03-09 16:57:52
--[[
 游戏的主逻辑，全部放在此模块中， 主要包括：
               1、游戏泡泡和等待发射的泡泡初始化
               2、游戏触摸的开关和处理
               3、泡泡的碰撞检测和处理（调整泡泡的位置）
               4、碰撞后查找相同泡泡的算法
               5、泡泡的消除
               6、消除后是否有悬挂泡泡的检测
]]
--

local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")

local PlayScene = class("PlayScene", function()
    return display.newPhysicsScene("PlayScene")
    -- return display.newScene("PlayScene")
end)

function PlayScene:ctor()
	self.layer = display.newLayer()
    self.layer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        -- print("event.name",event.name)
        return self:onTouch(event)
    end)
    self:addChild(self.layer)

    self:initData()
    self:initUI()

    self.world = self:getPhysicsWorld()
    self.world:setGravity(cc.p(0, GRAVITY))

    local wallBox = display.newNode()
    wallBox:setAnchorPoint(cc.p(0.5, 0.5))
    wallBox:setPhysicsBody(
        cc.PhysicsBody:createEdgeBox(cc.size(display.width, display.height)))
    wallBox:setPosition(display.cx, display.cy)
    self:addChild(wallBox)

    -- add debug node
    self:getPhysicsWorld():setDebugDrawMask(
        true and cc.PhysicsWorld.DEBUGDRAW_ALL or cc.PhysicsWorld.DEBUGDRAW_NONE)
end

function PlayScene:initData()
    self.m_board = {} --整个面板所有能放的泡泡 -- pairs

    self.m_wait = nil           -- 正在等待发射的泡泡

    self.m_curReady = nil                       -- 当前正要发射的泡泡

    self.m_listBubble = {}       -- 当前图上所有的球 -- pairs

    self.m_dRadian  = 0                   -- 范围为30度到150;默认为90度， 竖直方向
    self.m_nRotation  = 0                     -- 发射器角度改变方向，也是玩的反弹角度

    self.m_nWidth = 0        -- 游戏界面的宽度和高度
    self.m_nHeight = 0

    self.m_nScore  = 0         -- 游戏所得到的分数

    self.m_nGoldenBubbleCount = 0 -- 特殊球金球
    self.m_nSliveryBubbleCount = 0 -- 特殊球银球

    self.m_state = GameState.GS_START  -- 当前游戏状态

    self.m_real = nil  -- 真实坐标

    local jsonStr = cc.HelperFunc:getFileData(string.format("level-data/%03d.json",1))
    local jsonObj = json.decode(jsonStr)
    -- dump(jsonObj.level.graph)
    -- print("jsonObj.level.graph",#jsonObj.level.graph)
    self.graph = jsonObj.level.graph
    -- 重载当前关卡的最大行列数(暂时未用)
    MAX_ROWS = tonumber(jsonObj.level.maxRow)
    MAX_COLS = tonumber(jsonObj.level.maxCol)
    MAX_RANDOM = tonumber(jsonObj.level.maxRandomColors)
end

-- 测试用的格子
function PlayScene:initTestBar()
    for i = 0,MAX_COLS-1 do
        for j = 0,MAX_ROWS do
            if not (j%2 == 1 and i == MAX_COLS-1) then
                local bar = display.newSprite("#test_bar.png")
                local rcLable =cc.ui.UILabel.new({text = j.."_"..i,size = 30 }):align(display.CENTER, 32, 32):addTo(bar)
                local tempPoint = getPosByRowAndCol(j,i)
                bar:pos(tempPoint.x,tempPoint.y):addTo(self.layer)
            end
        end
    end
end

function PlayScene:initDebugUI()
    local editBox2 = cc.ui.UIInput.new({
        image = "#test_bar.png",
        size = cc.size(100, 50),
        x = 50,
        y = 400,
        listener = function(event, editbox)
            if event == "began" then
                
            elseif event == "ended" then
                local s = editbox:getText()
                local sArr = string.split(s,",")
                print(sArr[1] ,sArr[2], self.m_board[sArr[1] .. sArr[2] ])
            elseif event == "return" then
                -- self:onEditBoxReturn(editbox)
            elseif event == "changed" then
                -- self:onEditBoxChanged(editbox)
            else
                print("EditBox event %s", tostring(event))
            end
        end
    })
    editBox2:setReturnType(cc.KEYBOARD_RETURNTYPE_SEND)
    self.layer:addChild(editBox2)

    PYButton("#PlayButton.png","test",function(event)
        
    end):addTo(self.layer)
end

function PlayScene:initUI()
    display.newSprite("bg.jpg"):pos(display.cx,display.cy):addTo(self.layer)
    
    self:initBoard()
    self:initTestBar()
    self:initDebugUI()
    self:initReadyBubble()
    self:initWaitBubble()
end

function PlayScene:hasBall(row, col)
    if self.m_board[ row .. col ] then return true end
    return false
end

function PlayScene:setEnable()
    self.layer:setTouchEnabled(true)
end

function PlayScene:setDisableEnable()
    self.layer:setTouchEnabled(false)
end

-- 将wait状态的球换成ready状态
function PlayScene:changeWaitToReady()
    self.m_curReady = self.m_wait
    self.m_curReady:pos(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y)

    self.m_wait = randomBubble()
    self.m_wait:pos(WAIT_BUBBLE_POS.x,WAIT_BUBBLE_POS.y):addTo(self.layer)
end

--掉落泡泡
function PlayScene:downBubbleAction(pBubble)
    local offY = -100

    local pos = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
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

--执行可以掉落的泡泡 fallBubbleList为iparis的RowCol结构表
function PlayScene:FallBubble(fallBubbleList)
    for _,rc in pairs(fallBubbleList) do
        local pBubble = self.m_board[ rc.m_nRow .. rc.m_nCol ]
        if pBubble then
            self:downBubbleAction(pBubble)
            self.m_board[ rc.m_nRow .. rc.m_nCol ] = nil
            self.m_listBubble[rc.m_nRow .. rc.m_nCol] = nil
        end
    end

end

function PlayScene:callbackRemoveBubble(obj)
    if obj then
        obj:removeSelf()
    end
end

-- 检查掉落球 --return ipairs
function PlayScene:checkFallBubble()
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
function PlayScene:removeBubbleAction(pBubble)
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
function PlayScene:clearBubble(bubbleList)
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
function PlayScene:execClearBubble(pReadyBubble)
    local clearBubbleList = {}
    clearBubbleList = self:findClearBubble(pReadyBubble)
    if clearBubbleList then
        self:clearBubble(clearBubbleList)
    end
end

-- 查找需要清除的球的集合 -- pairs
function PlayScene:findClearBubble(pReadyBubble)
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
function PlayScene:findSameBubble(pReadyBubble)
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

function PlayScene:adjustBubblePosition(rowCol)
    local curPos = cc.p(self.m_curReady:getPositionX(),self.m_curReady:getPositionY())
    print(curPos.x, curPos.y)
    local row,col = GetRowColByPos(curPos.x, curPos.y)
    local row1,col1 = GetRowColByPos(410.47814941406, 585.22210693359)
    print("test",row1,col1)
    if self.m_board[row .. col] then
        print("被撞的球rc",rowCol.m_nRow,rowCol.m_nCol)
        print("已经存在泡泡",row,col)
         -- 已经存在泡泡，则找出被撞泡泡四周六个泡中离
        local vecRowCol = {}
        GetAround(row,col,vecRowCol)
        for _,rc in ipairs(vecRowCol) do
            
        end
    end

    local adjustPos = getPosByRowAndCol(row, col)
    self.m_curReady:pos(adjustPos.x,adjustPos.y)
    print("row, col",row, col)
    self.m_curReady:setRowColIndex(row, col)

    self.m_board[row .. col] = self.m_curReady
    self.m_listBubble[row .. col] = self.m_curReady
    -- table.insert(self.m_listBubble,self.m_curReady)
end

function PlayScene:initBoard()
    for i,bublleData in ipairs(self.graph) do 
        local bublle = BublleSprite.new(BublleBasic.new(bublleData))
        local tempPoint = getPosByRowAndCol(bublleData.row,bublleData.col)
        bublle:pos(tempPoint.x,tempPoint.y):addTo(self.layer)
        -- self.m_board[bublleData.row .. bublleData.col] = self.m_board[bublleData.row .. bublleData.col] or {}
        self.m_board[bublleData.row .. bublleData.col] = bublle

        self.m_listBubble[bublleData.row .. bublleData.col] = bublle
        -- table.insert(self.m_listBubble,bublle)
    end

end

function PlayScene:initReadyBubble()
    self.m_curReady = randomBubble()
    self.m_curReady:pos(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y):addTo(self.layer)
end

function PlayScene:initWaitBubble()
    self.m_wait = randomBubble()
    self.m_wait:pos(WAIT_BUBBLE_POS.x,WAIT_BUBBLE_POS.y):addTo(self.layer)
end

function PlayScene:clear()
    self.m_board = nil
    self.m_listBubble = nil
end

-- function PlayScene:createBublle(x, y)
--     -- add sprite to scene
--     local coinSprite = display.newSprite("#bublle02.png")
--     self:addChild(coinSprite)
--     local coinBody = cc.PhysicsBody:createCircle(COIN_RADIUS,
--         cc.PhysicsMaterial(COIN_RADIUS, COIN_FRICTION, COIN_ELASTICITY))
--     coinBody:setMass(1000)
--     coinBody:setRotationEnable(true)
--     coinSprite:setPhysicsBody(coinBody)
--     coinSprite:setPosition(x, y)
-- end

function PlayScene:touchEnded(event)
    self.m_state = GameState.GS_FLY

    local pos = cc.p(event.x,event.y)
    self.m_real = cc.pNormalize(cc.pSub(pos, cc.p(self.m_curReady:getPositionX(),self.m_curReady:getPositionY())))

    self:setDisableEnable()
    self:scheduleUpdate()
    -- self:onEnterFrame()
end

function PlayScene:onTouch(event)
    local name, x, y, prevX, prevY = event.name, event.x, event.y, event.prevX, event.prevY
    if name == "began" then
        
        return true
    elseif name == "moved" then

    elseif name == "ended" then
        self:touchEnded(event)
    end
    return true
end

function PlayScene:onEnter()
    self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self.onEnterFrame))
    self:setEnable()
end

function PlayScene:onEnterFrame(dt)
    if isCollisionWithBorder(self.m_curReady) then
        self.m_real.x = -self.m_real.x
    end
    
    local pos = cc.p(self.m_curReady:getPositionX(),self.m_curReady:getPositionY())
    self.m_curReady:pos(pos.x + self.m_real.x * BUBBLE_SPEED, pos.y + self.m_real.y * BUBBLE_SPEED)
    local collFlag,rc = isCollision(self.m_curReady, self.m_listBubble)
    if collFlag then
        self.m_real = cc.p(0,0)
        self:adjustBubblePosition(rc)

        self:execClearBubble(self.m_curReady)

        local fallList = self:checkFallBubble()
        self:FallBubble(fallList)

        self:unscheduleUpdate()
        self:changeWaitToReady()
        self:setEnable()
    end
end


function PlayScene:onExit()
end

return PlayScene