import tkinter

from enum import Enum, auto

from recording import Recording
from canvas import GameCanvas

class State(Enum):
    PAUSED = auto()
    PLAYING = auto()

class Playback:
    class PlaybackController(tkinter.Frame):
        def __init__(self, master : tkinter.Tk, playback : "Playback"):
            super().__init__(master)
            self.pack()
            self.playback = playback
            self.step_button = tkinter.Button(self, text="<", command=self.step_back)
            self.step_button.pack(side="left")
            self.play_button = tkinter.Button(self, text="\u25b6", command=self.play)
            self.play_button.pack(side="left")
            self.step_button = tkinter.Button(self, text=">", command=self.step)
            self.step_button.pack(side="left")
            self.turn_count = tkinter.Label(self, text="0")
            self.turn_count.pack(side="left")

        def update_turn_count(self):
            self.turn_count.configure(text=str(self.playback.turn))

        def play(self):
            self.playback.state = State.PLAYING
            self.play_button.configure(text="\u25a0", command=self.pause)

        def pause(self):
            self.playback.state = State.PAUSED
            self.play_button.configure(text="\u25b6", command=self.play)

        def step(self):
            if self.playback.state == State.PLAYING:
                self.pause()
            self.playback.next_turn()

        def step_back(self):
            if self.playback.state == State.PLAYING:
                self.pause()
            self.playback.prev_turn()

    def __init__(self, master : tkinter.Tk, game : GameCanvas, file_name : str):
        self.recording : Recording = Recording.open(file_name)
        self.state : State = State.PAUSED
        self.turn : int = 0
        self.delay : int = 500

        self.controller = self.PlaybackController(master, self)

        self.game = game

        # Process INIT
        # Set game map
        map = self.recording.init.map_info
        self.game.draw_map(map.width, map.height, map)

        # Spawn starting hills
        for id, team, x, y in self.recording.init.spawn_locations:
            self.game.spawn_hill(id, x, y, team)

        self.loop()

    def loop(self):
        if self.state == State.PLAYING:
            self.next_turn()

        self.game.after(self.delay, self.loop)

    def next_turn(self):
        if self.turn == len(self.recording.turns) - 1:
            return

        self.turn += 1

        self.handle_food(self.turn)

        # Kill dead ants
        for ant in self.recording.turns[self.turn].ant_kills:
            self.game.kill_ant(ant)

        # Move ants
        for id, x, y in self.recording.turns[self.turn].moves:
            self.game.move_ant(id, x, y)

        # Spawn new ants
        for id, team, x, y in self.recording.turns[self.turn].ant_spawns:
            self.game.spawn_ant(id, x, y, team)

        # Raze hills
        for id in self.recording.turns[self.turn].hill_razes:
            self.game.raze_hill(id)

        self.controller.update_turn_count()


    def prev_turn(self):
        if self.turn == 0:
            return

        self.turn -= 1

        self.handle_food(self.turn)

        # Revive dead ants
        for ant in self.recording.turns[self.turn+1].ant_kills:
            self.game.revive_ant(ant)

        # Move ants
        for id, x, y in self.recording.turns[self.turn].moves:
            self.game.move_ant(id, x, y)

        # Move ants spawned this turn
        for id, team, x, y in self.recording.turns[self.turn].ant_spawns:
            self.game.move_ant(id, x, y)

        # Kill spawned ants
        for id, team, x, y in self.recording.turns[self.turn+1].ant_spawns:
            self.game.kill_ant(id)

        # Revive razed hills
        for id in self.recording.turns[self.turn+1].hill_razes:
            self.game.revive_hill(id)

        self.controller.update_turn_count()

    def handle_food(self, turn : int):
        self.game.clear_food()
        for x, y in self.recording.turns[turn].food:
            self.game.spawn_food(x, y)

if __name__ == "__main__":
    root = tkinter.Tk()
    game = GameCanvas(root, 500, 500)
    game.pack()
    playback = Playback(root, game, "test/recording.txt")
    root.mainloop()
