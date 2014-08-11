
require("luapb")

function encodeSimp()
    pb.import('simple.proto')

    local msg = pb.new('simple.Simple')
    msg.name = 'jack'
    msg.id = 2
    msg.email = 'jack@request.com'
    msg.longid = 111

    local enc_msg = pb.serializeToString(msg)
 
    local resProto = pb.new('simple.Simple')
    pb.parseFromString(resProto, enc_msg)
    print('res: ' .. resProto.name1)
end

function test()
    print('test after exception')
end
