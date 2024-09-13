from canvas import GameCanvas
from playback import Playback
import tkinter
import sys

if __name__ == "__main__":
    if(len(sys.argv) != 2):
        print("Usage: python observer.py <game_directory>")
        sys.exit(1)
    root = tkinter.Tk()
    game = GameCanvas(root, 500, 500)
    game.pack()
    playback = Playback(root, game, sys.argv[1] + "/recording.txt")
    root.mainloop()
