
function OnUserConnected(conn_id)
    print('user ' .. conn_id .. ' connected')
end

function OnUserDisconnected(conn_id)
    print('user ' .. conn_id .. ' disconnected')
end

function OnUserData(conn_id, packet)
    print('user ' .. conn_id .. ' send packet')

    hex_dump(packet)
end
