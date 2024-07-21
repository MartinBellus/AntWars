import tkinter
import sys

from enum import StrEnum
from abc import ABC
from typing import Iterable, Tuple, Dict

PADDING = 5
OFFSET = 1

class Color(StrEnum):
    ANT = "red"
    HILL = "brown"
    HILL_RAZED = "grey"
    FOOD = "green"
    WATER = "blue"

class GameCanvas(tkinter.Canvas):
    def __init__(self, master : tkinter.Tk, width : int, height : int):
        super().__init__(master, width=width+OFFSET, height=height+OFFSET)
        self.pack()
        self.ants : Dict[int, Ant] = dict()
        self.hills : Dict[int, Hill] = dict()
        self.dead_ants_info : Dict[int, Tuple[int,int,int]] = dict()
        self.width = width
        self.height = height

    def draw_map(self, width : int, height : int, water_tiles : Iterable[Tuple[int,int]]):
        self.tile_size = min(self.width // width, self.height // height)

        for i in range(height+1):
            self.create_line(OFFSET, i*self.tile_size + OFFSET, self.width + OFFSET, i*self.tile_size+OFFSET)
        for i in range(width+1):
            self.create_line(i*self.tile_size + OFFSET, OFFSET, i*self.tile_size+OFFSET, self.height+OFFSET)

        for x, y in water_tiles:
            self.create_rectangle(x*self.tile_size + OFFSET, y*self.tile_size + OFFSET, (x+1)*self.tile_size + OFFSET, (y+1)*self.tile_size + OFFSET, fill=Color.WATER)

    def clear_food(self):
        self.delete("food")

    def move_ant(self, id : int, x : int, y : int):
        if id in self.ants:
            ant = self.ants[id]
            ant.move(x - ant.x, y - ant.y)
        else:
            print(f"Ant {id} not found", file=sys.stderr)

    def spawn_ant(self, id : int, x : int, y : int, team : int):
        if id in self.ants:
            print(f"Ant {id} already exists", file=sys.stderr)
        else:
            self.ants[id] = Ant(self, x, y, team)

    def kill_ant(self, id : int):
        if id in self.ants:
            self.ants[id].die()
            self.dead_ants_info[id] = (self.ants[id].x, self.ants[id].y, self.ants[id].team)
            del self.ants[id]
        else:
            print(f"Ant {id} not found", file=sys.stderr)

    def revive_ant(self, id : int):
        if id in self.dead_ants_info:
            x, y, team = self.dead_ants_info[id]
            self.ants[id] = Ant(self, x, y, team)
            del self.dead_ants_info[id]
        else:
            print(f"Dead ant {id} not found", file=sys.stderr)

    def spawn_hill(self, id : int, x : int, y : int, team : int):
        if id in self.hills:
            print(f"Hill {id} already exists", file=sys.stderr)
        else:
            self.hills[id] = Hill(self, x, y, team)
    def revive_hill(self, id : int):
        if id in self.hills:
            self.hills[id].revive()
        else:
            print(f"Hill {id} not found", file=sys.stderr)

    def raze_hill(self, id : int):
        if id in self.hills:
            self.hills[id].die()
        else:
            print(f"Hill {id} not found", file=sys.stderr)

    def spawn_food(self, x : int, y : int):
        Food(self, x, y)

    def clear(self):
        self.delete("all")
        self.ants.clear()
        self.hills.clear()

class GameObject(ABC):
    def __init__(self, master : GameCanvas, x : int, y : int, padding : int, color : str, **kwargs):
        self.master = master
        self.color = color
        self.x = x
        self.y = y
        self.id = master.create_rectangle(x*master.tile_size + padding + OFFSET, y*master.tile_size + padding + OFFSET,
                                         (x+1)*master.tile_size - padding + OFFSET, (y+1)*master.tile_size - padding + OFFSET,
                                         fill=color, **kwargs)

    def move(self, dx : int, dy : int):
        self.x += dx
        self.y += dy
        self.master.move(self.id, dx*self.master.tile_size, dy*self.master.tile_size)

    def die(self):
        self.master.delete(self.id)

class Food(GameObject):
    def __init__(self, master : GameCanvas, x : int, y : int):
        super().__init__(master, x, y, PADDING, Color.FOOD, tags="food")

class Ant(GameObject):
    def __init__(self, master : GameCanvas, x : int, y : int, team : int):
        super().__init__(master, x, y, PADDING, Color.ANT)
        self.tag = master.create_text((x + 0.5)*master.tile_size + OFFSET, (y + 0.5)*master.tile_size + OFFSET, text=str(team), fill="black")
        self.team = team

    def move(self, dx : int, dy : int):
        super().move(dx, dy)
        self.master.move(self.tag, dx*self.master.tile_size, dy*self.master.tile_size)

    def die(self):
        super().die()
        self.master.delete(self.tag)

class Hill(GameObject):
    def __init__(self, master : GameCanvas, x : int, y : int, team : int):
        super().__init__(master, x, y, -master.tile_size + PADDING, Color.HILL)
        self.tag = master.create_text((x + 0.5)*master.tile_size + OFFSET, (y + 0.5)*master.tile_size + OFFSET, text=str(team), fill="black")

    def die(self):
        self.master.itemconfig(self.id, fill=Color.HILL_RAZED)

    def revive(self):
        self.master.itemconfig(self.id, fill=Color.HILL)

if __name__ == "__main__":
    root = tkinter.Tk()
    canvas = GameCanvas(root, 800, 800)
    canvas.draw_map(10, 10, [])
    food = Food(canvas, 1, 1)
    ant = Ant(canvas, 2, 2, 1)
    hill = Hill(canvas, 4, 4, 1)
    canvas.bind("<Button-1>", lambda e: ant.move(1, 0))
    root.mainloop()
