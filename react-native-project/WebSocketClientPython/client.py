import websocket

ws = websocket.WebSocket()
ws.connect("ws://192.168.1.59/ws")

ws.send_binary([100, 220, 130])

ws.close()
