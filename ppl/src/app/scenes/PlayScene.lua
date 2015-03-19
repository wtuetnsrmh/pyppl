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
local GameLayer = import(".GameLayer")

local PlayScene = class("PlayScene", function()
    return display.newPhysicsScene("PlayScene")
    -- return display.newScene("PlayScene")
end)

function PlayScene:ctor(params)
    display.newSprite("bg.jpg"):pos(display.cx,display.cy):addTo(self)

	self.gameLayer = GameLayer.new({playScene = self,level = params.level}):pos(10,0)
    self:addChild(self.gameLayer)

    self:initData()

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
    self.m_wait = nil           -- 正在等待发射的泡泡
    self.m_curReady = nil                       -- 当前正要发射的泡泡
    self.m_state = GameState.GS_START  -- 当前游戏状态

    self:initReadyBubble()
    self:initWaitBubble()

    -- self:initDebugUI()
end

function PlayScene:onEnter()
    self.touchLayer = display.newLayer():addTo(self)
    self:setEnable()
    self.touchLayer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        print("event.name",event.name)
        return self:onTouch(event)
    end)

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
                print(sArr[1] ,sArr[2], self.gameLayer.m_board[sArr[1] .. sArr[2] ])
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
    self:addChild(editBox2)

    PYButton("#PlayButton.png","test",function(event)
        
    end):addTo(self)
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

function PlayScene:touchEnded(event)
    self.m_state = GameState.GS_FLY

    local pos = cc.p(event.x,event.y)
    self.gameLayer.m_real = cc.pNormalize(cc.pSub(pos, cc.p(self.m_curReady:getPositionX(),self.m_curReady:getPositionY())))
    self.gameLayer:setCurReady(self.m_curReady)
    self.gameLayer:update()

    self:setDisableEnable()
    self:changeWaitToReady()
end

function PlayScene:setEnable()
    self.touchLayer:setTouchEnabled(true)
end

function PlayScene:setDisableEnable()
    self.touchLayer:setTouchEnabled(false)
end

-- 将wait状态的球换成ready状态
function PlayScene:changeWaitToReady()
    self.m_curReady:setVisible(false)
    local action = cc.Spawn:create(
        cc.MoveTo:create(0.5, cc.p(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y)),
        cc.ScaleTo:create( 0.5,  1))

    self.m_wait:runAction(transition.sequence({action,cc.CallFunc:create(function()
            self.m_curReady = self.m_wait
            self.m_curReady:setVisible(true)
            self.m_curReady:pos(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y)

            self.m_wait = randomBubble()
            self.m_wait:scale(0.8)
            self.m_wait:pos(WAIT_BUBBLE_POS.x,WAIT_BUBBLE_POS.y):addTo(self)
        end)})
    )
    
end

function PlayScene:initReadyBubble()
    self.m_curReady = randomBubble()
    self.m_curReady:pos(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y):addTo(self)
end

function PlayScene:initWaitBubble()
    self.m_wait = randomBubble()
    self.m_wait:scale(0.8)
    self.m_wait:pos(WAIT_BUBBLE_POS.x,WAIT_BUBBLE_POS.y):addTo(self)
end

function PlayScene:onExit()
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

return PlayScene

