
require("config")
require("cocos.init")
require("app.scenes.Constants")
require("framework.init")
require("utils.init")

local MyApp = class("MyApp", cc.mvc.AppBase)

function MyApp:ctor()
    MyApp.super.ctor(self)
end

function MyApp:run()
    cc.FileUtils:getInstance():addSearchPath("res/")
    self:enterScene("LoadingScene")
end

return MyApp
