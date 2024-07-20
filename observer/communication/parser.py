from typing import TextIO, Dict, List, Any
from abc import ABC
import sys

TERM_CHAR = '.'

class Block(ABC):
    def __init__(self):
        self._data : List[Any] = []

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = cls()
        for l in range(lines):
            if line := stream.readline():
                if line.strip() == TERM_CHAR:
                    print(f"Too few lines in block {cls.__name__}.", file=sys.stderr)
                    break
                this._data.append(line.strip())
            else:
                print(f"Not enough lines in block {cls.__name__}, line {l}", file=sys.stderr)

        return this

    def __str__(self) -> str:
        return f"""
            {self.__class__.__name__}:
                {("\n" + " "*16).join(self._data)}
        """

class UnknownBlock(Block):
    def __init__(self):
        super().__init__()

    @classmethod
    def parse(cls, lines : int, stream : TextIO):
        this = super().parse(lines, stream)
        print(f"Unknown block with {lines} lines", file=sys.stderr)
        return this

class Parser:
    """Parser for blocks in recording file

    Registers blocks by name and parses them from stream.
    """
    def __init__(self):
        self.blocks : Dict[str,Block]= dict()

    def register(self, name):
        """Register block class with name

        Two blocks can not share same name.
        """
        def wrapper(cls):
            if name in self.blocks:
                print(f"Block {name} already registered", file=sys.stderr)
                return cls
            self.blocks[name] = cls
            return cls
        return wrapper

    def __getitem__(self, name):
        if name not in self.blocks:
            print(f"Block {name} not registered", file=sys.stderr)
            return UnknownBlock
        return self.blocks[name]

    def next_block(self, stream : TextIO):
        """Parse next block from stream

        Selects block based on header line and parses it. If header is not registered, returns UnknowBlock.
        Returns None if there are no more blocks in group.

        Args:
            stream (TextIO): Stream to read from
        """
        if header_line := stream.readline():
            if header_line.strip() == TERM_CHAR:
                return None
            try:
                name, lines = header_line.split()
                return self[name].parse(int(lines), stream)
            except Exception as e:
                print(f"Error parsing block {header_line.strip()}: {e}", file=sys.stderr)
                return UnknownBlock.parse(0, stream)
        else:
            print(f"Recording does not end with a term character", file=sys.stderr)
            return None

parser = Parser()
