local framework = require "framework"
local Sprite= require("game.sprite")
local game={}


local sp1=Sprite.new("face")
local x=100
local y=200
function game.game_init()
    print("game_init")

end

function game.game_update()
    --print("game_update")
    x=x+1
    y=y+2
    sp1:pos(x,y)
    sp1:draw()
end

framework.start(game)
