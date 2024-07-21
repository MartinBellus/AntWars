import sys

from typing import List

from communication.parser import parser
from communication.groups import GameHeader, Turn

class Recording:
    def __init__(self):
        self.init : GameHeader = GameHeader()
        self.turns : List[Turn] = []

    @classmethod
    def open(cls, filename : str):
        this = cls()
        with open(filename, "r") as file:
            if header := file.readline():
                if header.strip() != "INIT":
                    print(f"Invalid first group {header.strip()}", file=sys.stderr)
                    exit(1)
                this.init = GameHeader.parse(file)
            else:
                print(f"Empty file", file=sys.stderr)

            while header := file.readline():
                name, num = header.strip().split()

                if name != "TURN":
                    print(f"Unexpected group {name}", file=sys.stderr)
                    exit(1)

                if int(num) != len(this.turns):
                    print(f"Unexpected turn number {num}", file=sys.stderr)
                    exit(1)

                this.turns.append(Turn.parse(file))
        return this

    def __str__(self) -> str:
        return f"""
        Recording:
            {self.init}
            {"\n".join(map(str,self.turns))}
        """


if __name__ == "__main__":
    import sys
    r = Recording.open(sys.argv[1])
    print(r)
