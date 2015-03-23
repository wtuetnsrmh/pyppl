--
-- Author: yzm
-- Date: 2015-03-09 16:57:52
--[[
 	旋转关卡场景
]]
--

local json = require("framework.json")
local BublleSprite = import(".BublleSprite")
local BublleBasic = import("..modle.BublleBasic")
local WheelGameLayer = import(".WheelGameLayer")

local WheelScene = class("WheelScene", function()
    return display.newPhysicsScene("WheelScene")
end)

function WheelScene:ctor(params)
    display.newSprite("bg.jpg"):pos(display.cx,display.cy):addTo(self)

	self.gameLayer = WheelGameLayer.new({playScene = self,level = params.level}):pos(10,0)
    self:addChild(self.gameLayer)

    self:initData()

    self.world = self:getPhysicsWorld()
    self.world:setGravity(cc.p(0, GRAVITY))

    local wallBox = display.newNode()
    wallBox:setAnchorPoint(cc.p(0.5, 0.5))
    wallBox:setPhysicsBody(
        cc.PhysicsBody:createEdgeBox(cc.size(display.width, display.height+40), cc.PhysicsMaterial(0.1, WALL_ELASTICITY, WALL_FRICTION)))
    wallBox:setPosition(display.cx, display.cy+20)
    self:addChild(wallBox)

    -- 蜘蛛
    local box = display.newSprite("#test_bar.png"):pos(100,300):addTo(self)
    -- dump(self.layer:getContentSize())
    local temp = {cc.p(54.95,119.85),cc.p(67.2,118.05),cc.p(78.2,90.1),cc.p(32.1,90.15),cc.p(42.6,118)}
    local temp3 = {}
    for i,v in ipairs(temp) do
        temp3[i] = cc.p(v.x + 50,v.y)
    end
    local boxPhysicsBody = cc.PhysicsBody:createEdgePolygon(temp3, cc.PhysicsMaterial(0.1, 0.99, 0))
    boxPhysicsBody:setDynamic(false)
    boxPhysicsBody:setContactTestBitmask(16); -- 1111
    boxPhysicsBody:setGroup(2)
    box:setPhysicsBody(boxPhysicsBody)

    local box = display.newSprite("#test_bar.png"):pos(100,300):addTo(self)
    -- dump(self.layer:getContentSize())
    local temp2 = {}
    for i,v in ipairs(temp) do
        temp2[i] = cc.p(v.x + 223,v.y)
    end
    local boxPhysicsBody = cc.PhysicsBody:createEdgePolygon(temp2, cc.PhysicsMaterial(0.1, 0.99, 0))
    boxPhysicsBody:setGroup(2)
    boxPhysicsBody:setContactTestBitmask(16); -- 1111
    boxPhysicsBody:setDynamic(false)
    box:setPhysicsBody(boxPhysicsBody)

    -- 底部用于检测是否碰到清除球 TODO
    local removeBarSp = display.newSprite("#test_bar.png")
    local removeBarBody = cc.PhysicsBody:createBox(cc.size(display.width,100),cc.PhysicsMaterial(0.1, 1, 0 ))
    removeBarBody:setDynamic(false)
    removeBarSp:setPhysicsBody(removeBarBody)
    removeBarSp:setPosition(display.cx,50)
    self:addChild(removeBarSp)
    -- removeBarBody:setCategoryBitmask(1);    -- 0110
    removeBarBody:setContactTestBitmask(16); -- 1111
    removeBarBody:setCollisionBitmask(16);   -- 1111

    -- add debug node
    -- self:getPhysicsWorld():setDebugDrawMask(
    --     true and cc.PhysicsWorld.DEBUGDRAW_ALL or cc.PhysicsWorld.DEBUGDRAW_NONE)

    -- test
end

function WheelScene:initData()
    self.m_wait = nil           -- 正在等待发射的泡泡
    self.m_curReady = nil                       -- 当前正要发射的泡泡
    self.m_state = GameState.GS_START  -- 当前游戏状态
    self.m_real = nil  -- 球移动的单位向量

    self:initReadyBubble()
    self:initWaitBubble()

    -- self:initDebugUI()
end

function WheelScene:onEnter()
    self.touchLayer = display.newLayer():addTo(self)
    self:setEnable()
    self.touchLayer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        return self:onTouch(event)
    end)
    local flag = true
    local function onContactBegin(contact)
        
        local aBody = contact:getShapeA():getBody()
        -- print("aBody.group",aBody:getGroup())
        local bBody = contact:getShapeB():getBody()
        -- print("bBody.group",bBody:getGroup())
        if flag and (aBody:getGroup() == 2 or bBody:getGroup() == 2) then
            print("onContactBegin")
            flag = false
            return true
        end

        if aBody:getGroup() == 0 then
            bBody:getNode():removeSelf()
        end
        if bBody:getGroup() == 0 then
            aBody:getNode():removeSelf()
        end
        -- if aBody:getTag() ~= 0 then
        --     aBody:getNode():removeSelf()
        -- end
        
        -- if bBody:getTag() ~= 0 then
        --     bBody:getNode():removeSelf()
        -- end
        -- return contact:getContactData().normal.y < 0;
        return true
      end

    local function onContactLevel(contact)
        if not flag then
            print("onContactLevel")
            flag = true
        end
        
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

function WheelScene:updateLoop()
	if not self.curMoveBubble then
		return
	elseif isCollisionWithBorder(self.curMoveBubble) then
        self.m_real.x = -self.m_real.x
    elseif isCollisionWithTopBorder(self.curMoveBubble) then
    	self.m_real.y = -self.m_real.y
    elseif isCollisionWithBottom(self.curMoveBubble) then
    	self.curMoveBubble:removeSelf()
    	self.curMoveBubble = nil
    	return
    end
    
    local pos = cc.p(self.curMoveBubble:getPositionX(),self.curMoveBubble:getPositionY())
    self.curMoveBubble:pos(pos.x + self.m_real.x * BUBBLE_SPEED, pos.y + self.m_real.y * BUBBLE_SPEED)

    local findPp = self.gameLayer:stickAGrid(self.curMoveBubble, self.touchLayer)
end


function WheelScene:initDebugUI()
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

function WheelScene:onTouch(event)
    local name, x, y, prevX, prevY = event.name, event.x, event.y, event.prevX, event.prevY
    if name == "began" then
        
        return true
    elseif name == "moved" then

    elseif name == "ended" then
        self:touchEnded(event)
    end
    return true
end

function WheelScene:touchEnded(event)
    if self.m_state ~= GameState.GS_START then
        return
    end

    self.m_state = GameState.GS_FLY

    local pos = cc.p(event.x,event.y)
    self.m_real = cc.pNormalize(cc.pSub(pos, cc.p(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y)))
    self.gameLayer.m_real = self.m_real
    self:setCurMoveBubble(self.m_curReady)

    self.gameLayer:update()

    self:setDisableEnable()
    self:changeWaitToReady()
end


function WheelScene:setCurMoveBubble(curReady)
	self.curMoveBubble = BublleSprite.new(curReady:getModel())
	self.curMoveBubble:pos(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y):addTo(self.touchLayer)
end

function WheelScene:setEnable()
    self.touchLayer:setTouchEnabled(true)
end

function WheelScene:setDisableEnable()
    self.touchLayer:setTouchEnabled(false)
end

-- 将wait状态的球换成ready状态
function WheelScene:changeWaitToReady()
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

function WheelScene:initReadyBubble()
    self.m_curReady = randomBubble()
    self.m_curReady:pos(READY_BUBBLE_POS.x,READY_BUBBLE_POS.y):addTo(self)
end

function WheelScene:initWaitBubble()
    self.m_wait = randomBubble()
    self.m_wait:scale(0.8)
    self.m_wait:pos(WAIT_BUBBLE_POS.x,WAIT_BUBBLE_POS.y):addTo(self)
end

function WheelScene:onExit()
end

--掉落泡泡
function WheelScene:downBubbleAction(pos,color)
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

--[[
type：描述了一系列的形状，例如圆形，矩形，多边形等。
area：用于计算刚体的质量。密度和体积决定了刚体的质量。
mass：刚体所含的物质的量，可以用两种方式进行测量：物体在给定的力下获得的加速度大小，或者在一个引力场中物体受到力的大小。
moment：决定了获得特定角加速度所需要的转矩。
offset：在刚体的当前坐标中，相对于刚体的重心所偏移的量。
tag：用以使开发者较容易地确定形状。你大概还能记得把？你可以为所有的节点都分配一个标签，以进行辨识和实现更容易的访问
]]
function WheelScene:createBublle(color,x, y,rc)
    -- add sprite to scene
    local bubble = display.newSprite(string.format("#bublle%02d.png",color))
    self:addChild(bubble)
    local bubbleBody = cc.PhysicsBody:createCircle(33,
        cc.PhysicsMaterial(BUBBLE_DENSITY, BUBBLE_ELASTICITY, BUBBLE_FRICTION))
    -- bubbleBody:setMass(1000)
    bubbleBody:setGroup(-1)
    -- TODO
    -- bubbleBody:setCategoryBitmask(16);    -- 0011
    bubbleBody:setContactTestBitmask(1); -- 1000
    -- bubbleBody:setCollisionBitmask(2);   -- 0100
    bubbleBody:setRotationEnable(true)
    bubbleBody:setTag(color)
    bubble:setPhysicsBody(bubbleBody)
    bubble:setPosition(x, y)
    -- self:performWithDelay(function()
    --     bubble:removeFromParent()
    --     end, 1)
end

return WheelScene

