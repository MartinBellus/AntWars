import tkinter
from enum import Enum, auto

from recording import Recording

PADDING = 5

class GameCanvas(tkinter.Canvas):
    def __init__(self, master : tkinter.Tk, map, width : int, height : int):
        super().__init__(master, width=width, height=height)
        self.pack()
        self.food = dict()
        self.ants = dict()
        self.hills = dict()
        self.tile_size = min(width // len(map[0]), height // len(map))

        for i in range(len(map)):
            self.create_line(0, i*self.tile_size, width, i*self.tile_size)
        for i in range(len(map[0])):
            self.create_line(i*self.tile_size, 0, i*self.tile_size, height)

class GameObject:
    def __init__(self, master : GameCanvas, x : int, y : int, padding : int, color : str):
        self.master = master
        self.color = color
        self.id = master.create_rectangle(x*master.tile_size + padding, y*master.tile_size + padding,
                                          (x+1)*master.tile_size - padding, (y+1)*master.tile_size - padding, fill=color)

    def move(self, dx : int, dy : int):
        self.master.move(self.id, dx*self.master.tile_size, dy*self.master.tile_size)

    def die(self):
        self.master.delete(self.id)

class Food(GameObject):
    def __init__(self, master : GameCanvas, x : int, y : int):
        super().__init__(master, x, y, PADDING, "green")

class Ant(GameObject):
    def __init__(self, master : GameCanvas, x : int, y : int, team : int):
        super().__init__(master, x, y, PADDING, "red")
        self.tag = master.create_text((x + 0.5)*master.tile_size, (y + 0.5)*master.tile_size, text=str(team), fill="black")

    def move(self, dx : int, dy : int):
        super().move(dx, dy)
        self.master.move(self.tag, dx*self.master.tile_size, dy*self.master.tile_size)

    def die(self):
        super().die()
        self.master.delete(self.tag)

class Hill(GameObject):
    def __init__(self, master : GameCanvas, x : int, y : int, team : int):
        super().__init__(master, x, y, -master.tile_size + PADDING, "blue")
        self.tag = master.create_text((x + 0.5)*master.tile_size, (y + 0.5)*master.tile_size, text=str(team), fill="black")

    def die(self):
        super().die()
        self.master.delete(self.tag)

class State(Enum):
    PAUSED = auto()
    PLAYING = auto()

class Playback:
    class PlaybackController(tkinter.Frame):
        def __init__(self, master : tkinter.Tk, playback : "Playback"):
            super().__init__(master)
            self.pack()
            self.playback = playback
            self.play_button = tkinter.Button(self, text="Play", command=self.play)
            self.play_button.pack()
            self.pause_button = tkinter.Button(self, text="Pause", command=self.pause)
            self.pause_button.pack()
            self.step_button = tkinter.Button(self, text="Step", command=self.step)
            self.step_button.pack()

        def play(self):
            self.playback.state = State.PLAYING

        def pause(self):
            self.playback.state = State.PAUSED

        def step(self):
            self.playback.turn += 1

    def __init__(self, game : GameCanvas, file_name : str):
        self.recording : Recording = Recording.open(file_name)
        self.state : State = State.PAUSED
        self.turn : int = 0

        self.game = canvas

        # Process INIT
        for team, x, y in self.recording.init.spawn_locations:
            self.game.hills[team] = Hill(self.game, x, y, team)




if __name__ == "__main__":
    root = tkinter.Tk()
    canvas = GameCanvas(root, [[0 for i in range(10)] for i in range(10)], 300, 300)
    food = Food(canvas, 1, 1)
    ant = Ant(canvas, 2, 2, 1)
    hill = Hill(canvas, 4, 4, 1)
    canvas.bind("<Button-1>", lambda e: ant.move(1, 0))
    root.mainloop()
