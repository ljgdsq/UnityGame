
local fw=require("framework.c")

local framework={}
print("lua framework init!")


function framework.start(app)
    print("framework.start")
    fw.game_init=app.game_init
    fw.game_update=app.game_update
    fw.game_touch=app.game_touch
    fw.game_message=app.game_message
    fw.game_pause=app.game_pause

    fw.inject()
end


return framework