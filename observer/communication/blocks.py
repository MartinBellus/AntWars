import sys

from typing import List, TextIO, Tuple, Dict

from communication.parser import parser, Block

@parser.register("PLAYER_NAMES")
class PlayerNamesBlock(Block):
    """ Mapping of player IDs to player names

    Data constains list of player names in order of their ID.
    """
    def __init__(self):
        super().__init__()
        self.data : List[str] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            this.data.append(line)
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("SPAWN_POSITIONS")
class SpawnPositionsBlock(Block):
    """ Starting positions of ant hills

    Data contains tuples of (team, x, y)
    """
    def __init__(self):
        super().__init__()
        self.data : List[Tuple[int,int,int]] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            team, x, y = map(int, line.split())
            this.data.append((team, x, y))
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("MOVES")
class MoveBlock(Block):
    """ Ant moves

    Data contains tuples of (ant_id, x, y)
    """
    def __init__(self):
        super().__init__()
        self.data : List[Tuple[int,int,int]] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines,stream)
        for line in this._data:
            id, x, y = line.split()
            this.data.append((int(id), int(x), int(y)))
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("SPAWNS")
class AntSpawnBlock(Block):
    """ Ant spawns

    Data contains tuples of (team, x, y)
    """
    def __init__(self):
        super().__init__()
        self.data : List[Tuple[int,int]] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            x, y = line.split()
            this.data.append((int(x), int(y)))
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("FOOD")
class FoodBlock(Block):
    """ Food locations

    Data contains tuples of (x, y)
    """
    def __init__(self):
        super().__init__()
        self.data : List[Tuple[int,int]] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            x, y = line.split()
            this.data.append((int(x), int(y)))
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("KILLS")
class KillBlock(Block):
    """ Ant and hill kills

    Data contains list of killed IDs
    """
    def __init__(self):
        super().__init__()
        self.data: List[int] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            this.data.append(int(line))
        return this

    def __iter__(self):
        return iter(self.data)
