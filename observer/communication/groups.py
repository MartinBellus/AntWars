from communication.blocks import *
from communication.parser import parser

class GameHeader:
    def __init__(self):
        self.player_names : PlayerNamesBlock = PlayerNamesBlock()
        self.spawn_locations : SpawnPositionsBlock = SpawnPositionsBlock()
        self.map_info : MapBlock = MapBlock()

    @classmethod
    def parse(cls, stream : TextIO):
        this = cls()
        player_names = spawn_locations = map_info = None
        while block := parser.next_block(stream):
            match block:
                case PlayerNamesBlock():
                    player_names = block
                case SpawnPositionsBlock():
                    spawn_locations = block
                case MapBlock():
                    map_info = block
                case _:
                    print(f"Unexpected block {block.__class__.__name__}", file=sys.stderr)

        if player_names is None or spawn_locations is None or map_info is None:
            print("Missing required block in Init", file=sys.stderr)
            exit(1)

        this.player_names = player_names
        this.spawn_locations = spawn_locations
        this.map_info = map_info

        return this

    def __str__(self) -> str:
        return f"""
        GameHeader:
            {self.player_names}
            {self.spawn_locations}
        """

class Turn:
    num : int = 0
    def __init__(self):
        self.alive_ants : AliveAntsBlock = AliveAntsBlock()
        self.food : FoodBlock = FoodBlock()
        self.alive_hills : AliveHillsBlock = AliveHillsBlock()
        self.num = Turn.num
        Turn.num += 1

    @classmethod
    def parse(cls, stream : TextIO):
        this = cls()
        while block := parser.next_block(stream):
            match block:
                case AliveAntsBlock():
                    this.alive_ants = block
                case FoodBlock():
                    this.food = block
                case AliveHillsBlock():
                    this.alive_hills = block
                case _:
                    print(f"Unexpected block {block.__class__.__name__}", file=sys.stderr)

        return this

    def __str__(self) -> str:
        return f"""
        Turn {self.num}:
            {self.alive_ants}
            {self.alive_hills}
            {self.food}
        """
