
package.path = './?.lua;./scripts/?.lua;./scripts/libs/?.lua'

math.randomseed(os.time())

require('Common')
require('UserEvent')

function SendPkt()
    local pkt = 'test\0test'
    local driver = GetDriver()
    driver:Send(1, pkt)
end

function game_main()
end

print('load game_main.lua')
