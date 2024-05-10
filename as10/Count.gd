extends Label

# Called when the node enters the scene tree for the first time.
func _ready():
	text = str(0)
	
func _enter_tree():
	set_multiplayer_authority(name.to_int())

var Counter = 0

@rpc("call_local")
func Increment():
	Counter += 1
	text = str(Counter)
@rpc("call_local")
func Decrement():
	Counter -= 1
	text = str(Counter)
