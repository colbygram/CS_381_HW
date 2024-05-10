extends Control

var peer = ENetMultiplayerPeer.new()
@export var game_scene: PackedScene

func AddClient(client_id = 1):
	var client = game_scene.instantiate()
	client.name = str(client_id)
	call_deferred("add_child", client)

func Host():
	peer.create_server(135)
	multiplayer.multiplayer_peer = peer
	multiplayer.peer_connected.connect(AddClient)
	AddClient()

func Join():
	peer.create_client("localhost", 135)
	multiplayer.multiplayer_peer = peer


@rpc("call_local")
func Increment():
	pass


@rpc("call_local")
func Decrement():
	pass


