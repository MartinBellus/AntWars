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
        self.moves : MoveBlock = MoveBlock()
        self.ant_spawns : AntSpawnBlock = AntSpawnBlock()
        self.food : FoodBlock = FoodBlock()
        self.ant_kills : AntKillBlock = AntKillBlock()
        self.hill_razes : RazedHillsBlock = RazedHillsBlock()
        self.num = Turn.num
        Turn.num += 1

    @classmethod
    def parse(cls, stream : TextIO):
        this = cls()
        while block := parser.next_block(stream):
            match block:
                case MoveBlock():
                    this.moves = block
                case AntSpawnBlock():
                    this.ant_spawns = block
                case FoodBlock():
                    this.food = block
                case AntKillBlock():
                    this.ant_kills = block
                case RazedHillsBlock():
                    this.hill_razes = block
                case _:
                    print(f"Unexpected block {block.__class__.__name__}", file=sys.stderr)

        return this

    def __str__(self) -> str:
        return f"""
        Turn {self.num}:
            {self.moves}
            {self.ant_spawns}
            {self.food}
            {self.ant_kills}
            {self.hill_razes}
        """
