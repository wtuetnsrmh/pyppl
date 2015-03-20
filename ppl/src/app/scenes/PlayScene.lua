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
        cc.PhysicsBody:createEdgeBox(cc.size(display.width, display.height+40)))
    wallBox:setPosition(display.cx, display.cy+20)
    self:addChild(wallBox)

    -- 底部用于检测是否碰到清除球 TODO
    -- local removeBarSp = display.newSprite("#test_bar.png")
    -- local removeBarBody = cc.PhysicsBody:createBox(cc.size(display.width,100),cc.PhysicsMaterial(33, 100, 10 ))
    -- removeBarBody:setDynamic(false)
    -- removeBarBody:setTag(0)
    -- removeBarSp:setPhysicsBody(removeBarBody)
    -- removeBarSp:setPosition(display.cx,50)
    -- self:addChild(removeBarSp)
    -- removeBarBody:setCategoryBitmask(1);    -- 0110
    -- removeBarBody:setContactTestBitmask(1); -- 0010
    -- removeBarBody:setCollisionBitmask(1);   -- 1000

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

    -- 调整球层位置
    self.gameLayer:adjustLayerPos()
    -- self:initDebugUI()
end

function PlayScene:onEnter()
    self.touchLayer = display.newLayer():addTo(self)
    self:setEnable()
    self.touchLayer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        return self:onTouch(event)
    end)

    local function onContactBegin(contact)
        print("onContactBegin")
        local aBody = contact:getShapeA():getBody()
        if aBody:getTag() ~= 0 then
            aBody:getNode():removeSelf()
        end
        local bBody = contact:getShapeB():getBody()
        if bBody:getTag() ~= 0 then
            bBody:getNode():removeSelf()
        end
        -- return contact:getContactData().normal.y < 0;
        return true
      end

    local function onContactLevel(contact)
        print("onContactLevel")
        return true
      end

    local contactListener = cc.EventListenerPhysicsContact:create()
    contactListener:registerScriptHandler(onContactBegin, cc.Handler.EVENT_PHYSICS_CONTACT_BEGIN)
    -- contactListener:registerScriptHandler(onContactBegin, cc.Handler.EVENT_PHYSICS_CONTACT_PRESOLVE)
    -- contactListener:registerScriptHandler(onContactBegin, cc.Handler.EVENT_PHYSICS_CONTACT_POSTSOLVE)
    contactListener:registerScriptHandler(onContactLevel, cc.Handler.EVENT_PHYSICS_CONTACT_SEPERATE)
    local eventDispatcher = self:getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(contactListener, self)
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
    if self.m_state ~= GameState.GS_START then
        return
    end

    self.m_state = GameState.GS_FLY

    local pos = cc.p(event.x,event.y)
    self.gameLayer.m_real = cc.pNormalize(cc.pSub(pos, cc.p(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y)))
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
    self.m_curReady:removeSelf()
    self.m_curReady = self.m_wait
    local action = cc.Spawn:create(
        cc.MoveTo:create(0.5, cc.p(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y)),
        cc.ScaleTo:create( 0.5,  1))

    self.m_wait:runAction(transition.sequence({action,cc.CallFunc:create(function()
            self.m_state = GameState.GS_START
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

--掉落泡泡
function PlayScene:downBubbleAction(pos,color)
    local offPos = self:convertToNodeSpace(cc.p(pos.x,-100))
    local offY = offPos.y
    local bubble = display.newSprite(string.format("#bublle%02d.png",color))
    bubble:pos(pos.x,pos.y):addTo(self)
    bubble:runAction(
            cc.Sequence:create(
                cc.MoveTo:create((pos.y - offY) / 600.0, cc.p(pos.x, offY)),
                cc.CallFunc:create(function()
                    bubble:removeSelf()
                end)
            )
        )
end

function PlayScene:createBublle(color,x, y,rc)
    -- add sprite to scene
    local bubble = display.newSprite(string.format("#bublle%02d.png",color))
    self:addChild(bubble)
    local bubbleBody = cc.PhysicsBody:createCircle(33,
        cc.PhysicsMaterial(33, BUBBLE_FRICTION, BUBBLE_ELASTICITY))
    bubbleBody:setMass(1000)
    -- bubbleBody:setGroup(-1)
    -- TODO
    -- bubbleBody:setCategoryBitmask(1);    -- 0011
    -- bubbleBody:setContactTestBitmask(1); -- 1000
    -- bubbleBody:setCollisionBitmask(2);   -- 0100
    bubbleBody:setRotationEnable(true)
    bubbleBody:setTag(color)
    bubble:setPhysicsBody(bubbleBody)
    bubble:setPosition(x, y)
    -- self:performWithDelay(function()
    --     bubble:removeFromParent()
    --     end, 1)
end

-- 碰撞多边形
-- local box = display.newSprite("#bar1.png"):pos(100,300):addTo(self.layer)
--     -- dump(self.layer:getContentSize())
--     local temp = {cc.p(54.95,119.85),cc.p(67.2,118.05),cc.p(78.2,90.1),cc.p(32.1,90.15),cc.p(42.6,118)}
--     local temp3 = {}
--     for i,v in ipairs(temp) do
--         temp3[i] = cc.p(v.x + 50,v.y)
--     end
--     local boxPhysicsBody = cc.PhysicsBody:createEdgePolygon(temp3, cc.PhysicsMaterial(100, 1, 0))
--     -- cc.PhysicsBody:createBox(cc.size(100,100), cc.PhysicsMaterial(100, 1, 0))
--     boxPhysicsBody:setDynamic(false)
--     box:setPhysicsBody(boxPhysicsBody)

--     local box = display.newSprite("#bar2.png"):pos(100,300):addTo(self.layer)
--     -- dump(self.layer:getContentSize())
--     local temp2 = {}
--     for i,v in ipairs(temp) do
--         temp2[i] = cc.p(v.x + 223,v.y)
--     end
--     local boxPhysicsBody = cc.PhysicsBody:createEdgePolygon(temp2, cc.PhysicsMaterial(100, 1, 0))
--     -- cc.PhysicsBody:createBox(cc.size(100,100), cc.PhysicsMaterial(100, 1, 0))
--     boxPhysicsBody:setDynamic(false)
--     box:setPhysicsBody(boxPhysicsBody)

return PlayScene

