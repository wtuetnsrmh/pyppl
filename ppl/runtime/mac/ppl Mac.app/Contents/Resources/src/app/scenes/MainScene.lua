
local MainScene = class("MainScene", function()
    return display.newPhysicsScene("MainScene")
    -- return display.newScene("MainScene")
end)

local GRAVITY         = -1000
local COIN_MASS       = 0
local COIN_RADIUS     = 46
local COIN_FRICTION   = 0.8
local COIN_ELASTICITY = 0.8
local WALL_THICKNESS  = 64
local WALL_FRICTION   = 1.0
local WALL_ELASTICITY = 100

function MainScene:ctor()
    -- create touch layer
    self.layer = display.newLayer()
    self.layer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        return self:onTouch(event.name, event.x, event.y)
    end)
    self:addChild(self.layer)

    --[[display.newSprite("#bublle01.png"):align(display.LEFT_TOP,150,960):addTo(self.layer)
    self.layer:pos(0,-10)


    local box = display.newSprite("#bar1.png"):pos(100,300):addTo(self.layer)
    -- dump(self.layer:getContentSize())
    local temp = {cc.p(54.95,119.85),cc.p(67.2,118.05),cc.p(78.2,90.1),cc.p(32.1,90.15),cc.p(42.6,118)}
    local temp3 = {}
    for i,v in ipairs(temp) do
        temp3[i] = cc.p(v.x + 50,v.y)
    end
    local boxPhysicsBody = cc.PhysicsBody:createEdgePolygon(temp3, cc.PhysicsMaterial(100, 1, 0))
    -- cc.PhysicsBody:createBox(cc.size(100,100), cc.PhysicsMaterial(100, 1, 0))
    boxPhysicsBody:setDynamic(false)
    box:setPhysicsBody(boxPhysicsBody)

    local box = display.newSprite("#bar2.png"):pos(100,300):addTo(self.layer)
    -- dump(self.layer:getContentSize())
    local temp2 = {}
    for i,v in ipairs(temp) do
        temp2[i] = cc.p(v.x + 223,v.y)
    end
    local boxPhysicsBody = cc.PhysicsBody:createEdgePolygon(temp2, cc.PhysicsMaterial(100, 1, 0))
    -- cc.PhysicsBody:createBox(cc.size(100,100), cc.PhysicsMaterial(100, 1, 0))
    boxPhysicsBody:setDynamic(false)
    box:setPhysicsBody(boxPhysicsBody)

    -- create label
    cc.ui.UILabel.new({text = "TAP SCREEN", size = 32, color = display.COLOR_WHITE})
        :align(display.CENTER, display.cx, display.cy)
        :addTo(self)
        ]]

    -- create physics world
    self.world = self:getPhysicsWorld()
    self.world:setGravity(cc.p(0, GRAVITY))

    local wallBox = display.newNode()
    wallBox:setAnchorPoint(cc.p(0.5, 0.5))
    wallBox:setPhysicsBody(
        cc.PhysicsBody:createEdgeBox(cc.size(display.width, display.height)))
    wallBox:setPosition(display.cx, display.height/2)
    self:addChild(wallBox)

    -- add debug node
    self:getPhysicsWorld():setDebugDrawMask(
        true and cc.PhysicsWorld.DEBUGDRAW_ALL or cc.PhysicsWorld.DEBUGDRAW_NONE)
end

function MainScene:createBublle(x, y)
    -- add sprite to scene
    local coinSprite = display.newSprite("#bublle02.png")
    self:addChild(coinSprite)
    local coinBody = cc.PhysicsBody:createCircle(COIN_RADIUS,
        cc.PhysicsMaterial(COIN_RADIUS, COIN_FRICTION, COIN_ELASTICITY))
    coinBody:setMass(1000)
    coinBody:setRotationEnable(true)
    coinSprite:setPhysicsBody(coinBody)
    coinSprite:setPosition(x, y)
end

function MainScene:onTouch(event, x, y)
    if event == "began" then
        self:createBublle(x, y)
    end
end

function MainScene:onEnter()
    self.layer:setTouchEnabled(true)
    self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self.onEnterFrame))
    self:scheduleUpdate()
end

function MainScene:onEnterFrame(dt)
    -- body
end

return MainScene
