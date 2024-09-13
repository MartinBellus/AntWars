import sys

from typing import List, TextIO, Tuple, Dict

from communication.parser import parser, Block

@parser.register("PLAYER_NAMES")
class PlayerNamesBlock(Block):
    """ Mapping of player IDs to player names

    Data constains map of player IDs to player names.
    """
    def __init__(self):
        super().__init__()
        self.data : Dict[int,str] = {}

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            id, name = line.split()
            this.data[id] = name
        return this

    def __iter__(self):
        return iter(self.data.items())

@parser.register("MAP")
class MapBlock(Block):
    """ Info about the map

    Data contains list of water tiles. Also contains width and height of the map.
    """
    def __init__(self):
        super().__init__()
        self.width : int = 0
        self.height : int = 0
        self.data : List[Tuple[int,int]] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        assert len(this._data) >= 1, "MAP block must contain at least 1 line"
        this.width, this.height = map(int, this._data[0].split())
        for line in this._data[1:]:
            x, y = map(int, line.split())
            this.data.append((x,y))
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("SPAWN_POSITIONS")
class SpawnPositionsBlock(Block):
    """ Starting positions of ant hills

    Data contains tuples of (id, team, x, y)
    """
    def __init__(self):
        super().__init__()
        self.data : List[Tuple[int,int,int,int]] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            id, team, x, y = map(int, line.split())
            this.data.append((id, team, x, y))
        return this

    def __iter__(self):
        return iter(self.data)

@parser.register("ALIVE_ANTS")
class AliveAntsBlock(Block):
    """ List of alive ants

    Data contains map of ant IDs to tuples (team_id, x, y)
    """
    def __init__(self):
        super().__init__()
        self.data : Dict[int, Tuple[int,int,int]] = dict()

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines,stream)
        for line in this._data:
            team_id, id, x, y = line.split()
            this.data[int(id)] = ((int(team_id), int(x), int(y)))
        return this

    def __iter__(self):
        return iter(self.data.items())

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

@parser.register("ALIVE_HILLS")
class AliveHillsBlock(Block):
    """ List of alive hills

    Data contains list of alive hills IDs
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
@parser.register("PLAYER_SCORES")
class PlayerScoresBlock(Block):
    """ Dict of player scores

    Data contains dict of player IDs to scores
    """
    def __init__(self):
        super().__init__()
        self.data: Dict[int, int] = dict()

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        for line in this._data:
            id, score = line.split()
            this.data[id] = score
        return this

    def __iter__(self):
        return iter(self.data.items())
