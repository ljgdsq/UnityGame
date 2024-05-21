local framework = require "framework"
local game={}

function game.game_init()
    print("game_init")
end

function game.game_update()
    print("game_update")
end

framework.start(game)
