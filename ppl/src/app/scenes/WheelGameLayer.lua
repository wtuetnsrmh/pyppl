--
-- Author: bbjxl
-- Date: 2015-03-18 21:25:53
--
local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")

local WheelGameLayer = class("WheelGameLayer", function()
    return display.newLayer("wheelGameLayer")
end)

function WheelGameLayer:ctor(params)
	self.playScene = params.playScene
	self.level = params.level

	self:initData()
    self:initUI()

    self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self.onEnterFrame))
end

function WheelGameLayer:initData()
    self._board = {} -- 棋盘
    self._centerPoint = cc.p(0,0)
    self._centerPointOffset = cc.p(0,0)
    self._paopaoOffset = cc.p(0,0)
    self._gameFrame = cc.p(0,0)

    self._rorateDegeree = 0

    self._bumbingTime = 0

    self._enemyBossHp = 0
    self._oldEnemyBossHp = 0

    self._currrentBarrierProgress = 0
    self._currentBarrierData = {}

    self._bubbleOperationList = {} -- 记录最后三次的操作
    

    self._statData = nil

    self.centerPp = nil -- 旋转点所在的球
    

    self._lasterClearList = {}
    -----------------
    self.m_real = {x = 0, y = 0}

    -- self.m_board = {} --整个面板所有能放的泡泡 -- pairs

    -- self.m_curReady = nil                       -- 当前正要发射的泡泡

    self.m_listBubble = {}       -- 当前图上所有的球 -- pairs

    -- self.m_dRadian  = 0                   -- 范围为30度到150;默认为90度， 竖直方向
    -- self.m_nRotation  = 0                     -- 发射器角度改变方向，也是玩的反弹角度

    -- self.m_nScore  = 0         -- 游戏所得到的分数


    local path = string.format("mapData/%s.json",self.level)--%03d.json
    if device.platform == "windows" then
        -- win下可写入路径跟MAC不同
        path = cc.FileUtils:getInstance():getWritablePath() .."res/" .. path
    end
    print(path)
    local jsonStr = cc.HelperFunc:getFileData(path)
    local jsonObj = json.decode(jsonStr)
    -- dump(jsonObj.level.graph)
    -- print("jsonObj.level.graph",#jsonObj.level.graph)
    self.level = jsonObj.level
    self.graph = jsonObj.level.graph
    -- 重载当前关卡的最大行列数(暂时未用)
    MAX_ROWS = tonumber(jsonObj.level.maxRow)
    MAX_COLS = tonumber(jsonObj.level.maxCol)
    MAX_RANDOM = tonumber(jsonObj.level.maxRandomColors)
    CUR_RANDOM = randomInt(1,MAX_COLOR)
end

function WheelGameLayer:onEnterFrame(dt)

    local easing = 0.03
    local dx = math.abs(self._rorateDegeree)
    local addX = easing * dx  

    if (self._rorateDegeree > 0) then
        self:setRotation( self:getRotation() + addX)
        self._rorateDegeree = self._rorateDegeree - addX
        self:checkGameOver()
        self:processBubbleAfterRatation()
    end
    
    if (self._rorateDegeree < 0) then
        self:setRotation( self:getRotation() - addX)
        self._rorateDegeree = self._rorateDegeree + addX
        self:checkGameOver()
        self:processBubbleAfterRatation()
    end

    if (math.abs(self._rorateDegeree) < 2 and self._rorateDegeree ~= 0) then
        self._rorateDegeree = 0
        self:checkBubbleWarning()
        self:processBubbleAfterRatation()
    end
    
    if self.playScene then
        self.playScene:updateLoop(fDelta)
    end

    self:checkBumbBubble()

    self:animationNormalBubble(fDelta)
end

function WheelGameLayer:update()
	self:scheduleUpdate()
end

function WheelGameLayer:initTheBoard()  
    for i,bublleData in ipairs(self.graph) do 
        -- 随机颜色球不计算
        local t = (bublleData.color + CUR_RANDOM)
        bublleData.color = bublleData.color == MAX_COLOR and MAX_COLOR or (t >= MAX_COLOR and (t % MAX_COLOR)+1 or t) -- +1为了保证不为0
        local bublle = BublleSprite.new(BublleBasic.new(bublleData))
        local tempPoint = getPosByRowAndCol(bublleData.row,bublleData.col)
        bublle:pos(tempPoint.x,tempPoint.y):addTo(self)
        self._board[bublleData.row .. bublleData.col] = bublle

        self.m_listBubble[bublleData.row .. bublleData.col] = bublle
    end

    --记录中心旋转点
    local centerRow,centerCol = self.level.graphPivot.row,self.level.graphPivot.col
    self.centerPp = self._board[ centerRow .. centerCol ]

end

-- 有向面积
--     如果area>0，则说明ABC三点呈现逆时针排列；  
--     如果area=0，则ABC三点共线；
--     如果area<0，则说明ABC三点呈现顺时针排列。
function WheelGameLayer:area(x0,y0,x1,y1,x2,y2)
	return x0*y1+y0*x2+x1*y2-x2*y1-x0*y2-x1*y0
end

function WheelGameLayer:calOffsetXY()
    -- local origin = cc.Director:getInstance():getVisibleOrigin()
    -- local visibleSize = cc.Director:getInstance():getWinSize()
    
    -- local width = visibleSize.width - origin.x
    -- local height = visibleSize.height - origin.y

    -- self._centerPoint = cc.p(origin.x + visibleSize.width/2 , origin.y+visibleSize.height/2);
    
    -- _paopaoOffset.x = 0;
    -- _paopaoOffset.y = 0;
    
    -- PAOPAO_SIZE = std::min(width, height) / MAX_ROWS;
    -- if(PAOPAO_SIZE%2==1)
    --     PAOPAO_SIZE -= 1;
    -- PIC_RADIO =  (float) PAOPAO_SIZE / (float)PAOPAO_ORIGINAL_SIZE;

    -- int logicWidth = PAOPAO_SIZE * MAX_COLS;
    -- int logicHeight = PAOPAO_SIZE * MAX_ROWS;

    -- int cRow = floor(MAX_ROWS/2.0);
    -- int cCol = floor(MAX_COLS/2.0);
    -- int BUBBLE_RADIUS = PAOPAO_SIZE/2;
    
    -- self._gameFrame.x =  self._centerPoint.x - logicWidth/2;
    -- self._gameFrame.y =  self._centerPoint.y - logicHeight/2;
    -- self._gameFrame.width = logicWidth;//this->_totalCol * PAOPAO_SIZE;
    -- self._gameFrame.height = logicHeight;//this->_totalRow * PAOPAO_SIZE;

    -- _paopaoOffset = cocos2d::Point( self._centerPoint.x - logicWidth/2 ,  self._centerPoint.y - logicHeight/2 );

    -- //÷––ƒ≈›≈›µƒ µº ◊¯±Í
    -- //int cposX = cCol * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (cRow % 2) * BUBBLE_RADIUS + self._gameFrame.x;
    -- //int cposY = ( cRow * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS ) + self._gameFrame.y;
    -- cocos2d::Point  centerPpPos = getPosByRowAndCol(cRow, cCol);

    -- self._centerPointOffset.x = self._centerPoint.x - centerPpPos.x;
    -- self._centerPointOffset.y = self._centerPoint.y - centerPpPos.y;
end

-- delete
function WheelGameLayer:checkBubbleWarning()
end

function WheelGameLayer:findNonLinkedBubble()
    local LinkBubbleList = {}
    local NoLinkBubblelist = {} -- ipairs
    local center = { m_nRow = self.centerPp:getRow(), m_nCol = self.centerPp:getCol() }
    table.insert(LinkBubbleList,center)

    local index = 1
    repeat
        local itCur = LinkBubbleList[index]
        local vecRowCol = {}
        GetAround(itCur.m_nRow,itCur.m_nCol,vecRowCol)

        for _,rc in ipairs(vecRowCol) do
            local pBubble = self._board[rc.m_nRow .. rc.m_nCol]
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

    
    for _,bublle in pairs(self.m_listBubble) do
        local findRowCol = { m_nRow = bublle:getRow(), m_nCol = bublle:getCol() }
        -- TODO 最后要验证一下
        local k = (findRowCol.m_nRow ~= center.m_nRow and findRowCol.m_nCol ~= center.m_nCol) or table.RCkeyof(LinkBubbleList, findRowCol)
        if not k then
            table.insert(NoLinkBubblelist, findRowCol)
        end
    end

    return NoLinkBubblelist

end

-- 掉落单独一个球
function WheelGameLayer:dropOneBubble(pBubble)
    local pos = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
    local worldP = self:convertToWorldSpace(pos)
    self.playScene:createBublle(pBubble:getColor(),worldP.x,worldP.y)
    pBubble:removeSelf()
end

function WheelGameLayer:dropBubble(fallBubbleList)
    for _,bubble in pairs(fallBubbleList) do
        local pBubble = self._board[ bubble:getRow() .. bubble:getCol() ]
        if pBubble then
            local pos = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
            local worldP = self:convertToWorldSpace(pos)
            self.playScene:createBublle(pBubble:getColor(),worldP.x,worldP.y)
            self._board[ bubble:getRow() .. bubble:getCol() ] = nil
            self.m_listBubble[bubble:getRow() .. bubble:getCol()] = nil
            pBubble:removeSelf()
        end
    end
end

-- 找到粘住位置
function WheelGameLayer:stickAGrid(paopao, sl)
    local angle = self:getRotation()
    local paopaoPos = cc.p(paopao:getPositionX(),paopao:getPositionY())
    local truePoint = convertA1ToA2(self._centerPoint, paopaoPos, angle)

    local finalSpeedX = self.m_real.x
    local finalSpeedY = self.m_real.y
    local finalX = 0
    local finalY = 0
    local bAF = paopao:getAngleForce()

    -- if(paopao->getColorType() == PAOPAO_PASS)
    --     return false;

    local rc = self:checkCollision(paopao, angle)
    if (rc.m_nRow == -1 and rc.m_nCol == -1) then
        -- print(" error: stickAGrid 1")
        -- self:unscheduleUpdate()
        return false
    end

    if (IsValidPos(rc.m_nRow, rc.m_nCol) == false) then
        -- 定位在一个无效的位置，如超出屏幕，则直接掉落
        local drop_bubble = BublleSprite.new(paopao:getModel())
        local adjustPos = getPosByRowAndCol(rc.m_nRow, rc.m_nCol)
        drop_bubble:setRowColIndex(rc.m_nRow, rc.m_nCol)
        drop_bubble:pos(adjustPos.x,adjustPos.y):addTo(self)

        sl:removeChild(paopao, true)
        dropOneBubble(paopao)
        -- sl:stopShot(false, false)
        -- self:gameOver()
        
        return false
    end
    
    finalX = paopao:getPositionX()
    finalY = paopao:getPositionY()

    local bubble = BublleSprite.new(paopao:getModel())
    local adjustPos = getPosByRowAndCol(rc.m_nRow, rc.m_nCol)
    bubble:setRowColIndex(rc.m_nRow, rc.m_nCol)
    bubble:pos(adjustPos.x,adjustPos.y):addTo(self)
    sl:removeChild(paopao, true)
    self.playScene.curMoveBubble = nil
    
    self._board[ rc.m_nRow .. rc.m_nCol ] = bubble
    self.m_listBubble[ rc.m_nRow .. rc.m_nCol ] = bubble

    if (finalSpeedX~=0 and finalSpeedY~=0) then

        local temp = self:area( self._centerPoint.x, self._centerPoint.y, finalX, finalY, finalX+finalSpeedX, finalY+finalSpeedY )
        
        local angelSpeed = self:getAngleSpeed(3000, bAF )
        if (temp > 0) then
            self._rorateDegeree = self._rorateDegeree - angelSpeed
        else
            self._rorateDegeree = angelSpeed
        end
    end

    -- if (math.abs(self._rorateDegeree) < 2) then
    --     self:checkBubbleWarning()
    -- end

    local clearBubbleList = self:findClearBubble(bubble)
    if nil == clearBubbleList or table.nums(clearBubbleList) < REMOVE_COUNT then
        local tempList = {}
        table.insert(tempList,rc)
        GetAround(rc.m_nRow, rc.m_nCol, tempList)
        self:removeCenterInList(tempList)
        self:crowdEffect(tempList, self.m_real.x,self.m_real.y, 6)

        local tempList2 = {}
        self:findAGroundInList(tempList, tempList2)
        self:removeCenterInList(tempList2)
        self:crowdEffect(tempList2, self.m_real.x,self.m_real.y, 4)

    else
        self:clearBubble(clearBubbleList)

        -- 有消除操作时才检测是否有掉落
        local fallList = self:findNonLinkedBubble()
        self:FallBubble(fallList)
    end
    
    self:checkGameOver()

    self:specialBubbleProcess()

    return true;
end

function WheelGameLayer:checkCollision(pp, angle)
    local r = BUBBLE_RADIUS - COLLISON_OFFSET
    local speedx, speedy
    speedx = self.m_real.x
    speedy = self.m_real.y
    local totalspeed = math.sqrt(speedx*speedx+speedy*speedy)
    local newx = pp:getPositionX()
    local newy =  pp:getPositionY()
    local oldx = newx - speedx
    local oldy = newy - speedy
    local mindist = 10000000
    local k = -1
    local truex = -1
    local truey = -1
    local ddd = 0
    local stickPp
    
    for _,bubble in pairs(self._board) do
        local txi = bubble:getPositionX()
        local tyi = bubble:getPositionY()
        local tempPt = bubble:convertToWorldSpaceAR(cc.p(0,0))
        local xi = tempPt.x
        local yi = tempPt.y
        local h = distance(xi,yi,oldx,oldy,newx,newy)
        if h < 2*r - 1e-4 then
            local temp = chuizu(xi,yi,oldx,oldy,newx,newy)
            local xx = temp.x
            local yy = temp.y
            local dd = math.sqrt(4*r*r - h*h)
            local ax = xx - speedx * dd/totalspeed
            local ay = yy - speedy * dd/totalspeed
            if ((ax-oldx)*(ax - newx) <= 0) and ((ay-oldy)*(ay -newy) <= 0) then
                ddd = dist(oldx, oldy, ax, ay)
                if (ddd < mindist) then
                    mindist = ddd
                    k = 1
                    truex = ax
                    truey = ay
                    stickPp = bubble
                end
            end
            
        end
        
    end

    if (k ~= -1) then
        print("k",k)
        print("truex,truey,angle",truex, truey,angle)
        local truePt = convertA1ToA2(self._centerPoint, cc.p(truex, truey), angle)
        print("truePt.x, truePt.y",truePt.x, truePt.y)
        local r,c = GetRowColByPos(truePt.x, truePt.y)
        local rc = {m_nRow = r, m_nCol = c }
        pp:setStickPp(stickPp)
        dump(rc)
        -- self:unscheduleUpdate()
        rc = self:findTrueBlankPos(rc, cc.p(newx, newy), speedx, speedy, angle)
       
        return rc
    end
    
    return rc or {m_nRow = -1, m_nCol = -1 }
end

function WheelGameLayer:findTrueBlankPos(rc, currentXY, speedx, speedy, angle)
    local ret = {m_nRow = MAX_ROWS, m_nCol = MAX_COLS}
    if (IsValidPos(rc.m_nRow, rc.m_nCol) == false) then
        return rc
    end
    if not self._board[rc.m_nRow .. rc.m_nCol] then
        return rc
    else
        for i=0,BUBBLE_RADIUS*2 do
            local r = BUBBLE_RADIUS
            local skew = math.sqrt(math.pow(speedx, 2) + math.pow(speedy, 2))
            local oldx = currentXY.x - r * speedx /skew * i
            local oldy = currentXY.y - r * speedy /skew * i
            local skewPoint = convertA1ToA2(self._centerPoint, cc.p(oldx, oldy), angle)

            local r,c = GetRowColByPos(skewPoint.x, skewPoint.y)
            local temp =  {m_nRow = r, m_nCol = c }
            if (IsValidPos(temp.m_nRow, temp.m_nCol) == false) then
                print("findTrueBlankPos 1")
                return temp
            end
            if not self._board[temp.m_nRow .. temp.m_nCol] then
                return temp
            end
        end
        print("error:findTrueBlankPos")
        return ret
    end
end

-- 特殊球处理
function WheelGameLayer:specialBubbleProcess()
end

-- 检测是否游戏结束
function WheelGameLayer:checkGameOver()
end

-- 被打到的球周围抖动
function WheelGameLayer:crowdEffect(rcList, speedx, speedy, deltaDistance)
    for _,rc in pairs(rcList) do
        local bubble = self._board[rc.m_nRow .. rc.m_nCol]
        if bubble then
            local oldx = bubble:getPositionX()
            local oldy = bubble:getPositionY()
            local deltax,deltay=0,0
            if (speedx<0) then
                deltax = deltax-deltaDistance
            else
                deltax = deltaDistance
            end
            if (speedy<0) then
                deltay = deltay-deltaDistance
            else
                deltay = deltaDistance
            end
            bubble:runAction(
                cc.Sequence:create(
                cc.MoveTo:create(0.03, cc.p(oldx+deltax, oldy+deltay)),
                cc.MoveTo:create(0.07, cc.p(oldx, oldy))
                )
            )
        end
    end
    
end

function WheelGameLayer:findAGroundInList(list, ret)
    local keymap = {}
    for _,rc in pairs(list) do
        local nRow,nCol = rc.m_nRow,rc.m_nCol
        if not keymap[nRow .. nCol] then
            keymap[nRow .. nCol] = true
        end
    end

    for _,rc in pairs(list) do
        local nRow,nCol = rc.m_nRow,rc.m_nCol
        local vec = {}
        GetAround(nRow, nCol, vec)
        for _,rc2 in pairs(vec) do
            if not keymap[nRow .. nCol] then
                keymap[nRow .. nCol] = true
                table.insert(ret,rc2)
            end
        end
        
    end

end

function WheelGameLayer:checkBumbBubble() -- 判断是否有炸弹要爆炸
end

function WheelGameLayer:findSameBubbleByColor(nColor, row, col)
end

function WheelGameLayer:killFinalEnemy(pp)
end

function WheelGameLayer:removeCenterInList(list)
    for _,rc in pairs(list) do
        if rc.m_nRow == self.centerPp:getRow() and rc.m_nCik == self.centerPp:getCol() then
            table.remove(list,rc)
        end
    end
    
end

function WheelGameLayer:isKillFinalEnemy(row, col)
end

function WheelGameLayer:hurtEnemyBossByBubbleClear(obj, param)
end

-- 用不到，为了像墨汁球时保技角度向下
function WheelGameLayer:processBubbleAfterRatation()
    -- for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
    -- {  
    --     for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
    --     {  
    --         if(_board[i][j]!=NULL && _board[i][j]->getColorType() == PAOPAO_INK){
    --             _board[i][j]->setRotation(-this->getRotation());
    --         }
    --     }  
    -- } 
end

function WheelGameLayer:isInRowColList(vec, row, col)
    table.RCkeyof(vec,{ m_nRow = row,m_nCol = col })
end

--  设置附加效果
function WheelGameLayer:setAttachAttribute(pBubble, effect)
end

-- TODO 球被消除时，如果是一些特殊球，则会有一些特殊效果在这里处理
function WheelGameLayer:checkAttachEffect(obj)
end

function WheelGameLayer:changeMinTypeInBoard() -- 获得颜色最少的球
end

function WheelGameLayer:getAngleSpeed(v, r)
    local sum = 1.0
    for _,bubble in pairs(self._board) do
        sum = sum+bubble:getAngleForce()
    end
    return v * r / sum
end


function WheelGameLayer:resetValue()
end

function WheelGameLayer:restorBubbleTypeByPlague(row, col) -- 中瘟疫的需要恢复
end

function WheelGameLayer:checkSpecialReward() -- 检查特殊奖励
end

function WheelGameLayer:changeSmallest2OtherColor()
end

function WheelGameLayer:animationNormalBubble(fDelta) -- 随机动画
end

function WheelGameLayer:dropAllLeftBubbleWhenWin(node)
    self:dropBubble(self.m_listBubble)
end

function WheelGameLayer:addBubbleOperation(type)
end

function WheelGameLayer:removeLasterClearList(obj)
end

function WheelGameLayer:lasterKillBoss(obj)
end

function WheelGameLayer:getCurrentBarrierGold(size)
end



-- 测试用的格子
function WheelGameLayer:initTestBar()
    for i = 0,MAX_COLS-1 do
        for j = 0,MAX_ROWS+5 do
            if not (j%2 == 1 and i == MAX_COLS-1) then
                local bar = display.newSprite("#test_bar.png")
                local rcLable =cc.ui.UILabel.new({text = j.."_"..i,size = 20 }):align(display.CENTER, 32, 32):addTo(bar)
                local tempPoint = getPosByRowAndCol(j,i)
                bar:pos(tempPoint.x,tempPoint.y):addTo(self)
            end
        end
    end
end

function WheelGameLayer:initUI()
    self:initTheBoard()
    self:initTestBar()
end

function WheelGameLayer:hasBall(row, col)
    if self._board[ row .. col ] then return true end
    return false
end


--执行可以掉落的泡泡 fallBubbleList为iparis的RowCol结构表
function WheelGameLayer:FallBubble(fallBubbleList)
    for _,rc in pairs(fallBubbleList) do
        local pBubble = self._board[ rc.m_nRow .. rc.m_nCol ]
        if pBubble then
            local pos = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
            local worldP = self:convertToWorldSpace(pos)
            self.playScene:createBublle(pBubble:getColor(),worldP.x,worldP.y)
            self._board[ rc.m_nRow .. rc.m_nCol ] = nil
            self.m_listBubble[rc.m_nRow .. rc.m_nCol] = nil
            pBubble:removeSelf()
        end
    end

end

function WheelGameLayer:callbackRemoveBubble(obj)
    if obj then
        obj:removeSelf()
    end
end



--清除一个球的表现动作
function WheelGameLayer:removeBubbleAction(pBubble)
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
function WheelGameLayer:clearBubble(bubbleList)
    print("#clearBubble num = ",table.nums(bubbleList))
    local nRow, nCol
    for _,rowCol in pairs(bubbleList) do
        nRow = rowCol.m_nRow
        nCol = rowCol.m_nCol
        local obj = self._board[nRow .. nCol]
        if obj then
            self:checkAttachEffect(obj)
            self:removeBubbleAction(obj)
            self._board[nRow .. nCol] = nil
        end

        if self.m_listBubble[ nRow .. nCol ] then
            self.m_listBubble[ nRow .. nCol ] = nil
        end
    end

end

-- 根据调整过后的球的位置和颜色类型， 作出相应的处理，如：金银色特殊泡泡的爆炸，球的下落等
function WheelGameLayer:execClearBubble(pReadyBubble)
    local clearBubbleList = {}
    clearBubbleList = self:findClearBubble(pReadyBubble)
    if clearBubbleList then
        self:clearBubble(clearBubbleList)
    end
end

-- 查找需要清除的球的集合 -- pairs
function WheelGameLayer:findClearBubble(pReadyBubble)
    local clearRowCollist = {}
    if not pReadyBubble then
        return clearRowCollist
    end
    print("pReadyBubble:getType()",pReadyBubble:getType())
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
function WheelGameLayer:findSameBubble(pReadyBubble)
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
           local pCurBubble = self._board[ rowCol.m_nRow .. rowCol.m_nCol ]

           if pCurBubble and pCurBubble:getType() ~= BUBBLE_TYPE.center and pCurBubble:getColor() == tonumber(nColor) then
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


-- 被撞球及周边球抖动 TODO
function WheelGameLayer:ainmateNearBubble(vecRowCol,row)
	for _,rc in pairs(vecRowCol) do
		if rc.m_nRow <= row then
			local bubble = self._board[rc.m_nRow .. rc.m_nCol]
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


function WheelGameLayer:clear()
    self._board = nil
    self.m_listBubble = nil
end


return WheelGameLayer