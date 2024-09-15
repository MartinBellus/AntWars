This is developer documentation for server running the game.

The server is split into several modules, each responsible of handling a
different part of the functionality. Every module contains a `include` and a
`src` directory.

# Game

This module is responsible for simulating the game as described in
[rules](./RULES.md).

It contains a single class `Game`, which handles the initialization and game
loop logic and a couple of helper functions used to evaluate turn phases.

A lifetime of `Game` instance starts in a class method `from_config`, where a
instance of `Game` is created from `game_config` and `player_config`. This
method returns a `optional<Game>` that indicates, if the `Game` initialization
was succesful.

The `Game` can be started using `run` method. After that, the private `init`
will be called, which sends initial messages to observer and players. Then
`game_loop` will run until one of the end game conditoins is satisfied. Then
the `Game` will save final scores and issue `PlayerManager` to clean up its
temporary files.

`Game` is also responsible for deciding, which players to kill (for example if
they exceed the given time limit).

# Helper

This module contains some helper classes like `Logger` or `Observer`,
definitions of objects found in game (`Ant`, `Hill`, `Player` and `Food`) and
definitions of basic types to ensure type safety when working with `PlayerID`s
etc.

# Init

This module is responsible only for reading the configuration files.

# Map

This module is responsible for reading the map file and parsing the specific
file formats used for maps. Currently, the only supported format is `.ppm` in
ascii mode.

# Process

This is the most complicated module. It is responsible for comunicating with
player processes and with their isolation.

## PlayerManager

Class `PlayerManager` is responsible for holding information about all
processes currently running and their mapping to `PlayerID`s known by `Game`.

It is also responsible for reaping exited child processes and cleaning up after
getting a external signal.

All processes, that are not currently read from, are maintained in paused state
 (`SIGPAUSE`) by the `PlayerManager`.

### Interface

- `stringstream read_player(PlayerID id, Status& status)`
    - try to read a message from player with a given `id`.
    - reader threads are used to allow main thread to not block when reading player response
    - `status` contains the state of the result of the operation
    - returns string stream with the response or empty string stream if error occured
- `void send_player(PlayerID id ,const string& message)`
    - send `message` to player with given `id`
- `stringstream send_and_read_player(PlayerID id, const string& message, Status& status)`
    - combination of send and read operation from player
- `void kill_player(PlayerID id)`
    - terminate process run by a player with given `id`
- `void cleanup()`
    - terminates running processes and removes temporary directory used for process sandboxes

## Process

Struct `Process` encapsulates process data and basic input and output methods
used to communicate with it.

### Interface

- `int run(const string& log_path))`
    - starts the process with file in `log_path` used as its `stderr`
    - uses `run_isolated` to create a new process running the specified command
- `void send(const string& message)`
    - writes `message` to processes `stdin`
- `std::string read_stdin(std::atomic<bool>& stop)`
    - this function is intended to be run inside a thread, because it can block the execution of main thread if no data is available in child `stdout`
    - continuously uses `poll` to check, if data is available. If `stop` flag is set, it ends its execution to prevent blocking of main thread
- `void send_signal(int sig)`
    - sends signal to process
- `ProcessState check_status()`
    - checks the state of process using the non-blocking `status_pipe` created by `run_isolated`


## Isolation

> [!NOTE]
> This was inspired by isolation techniques used in [https://github.com/ioi/isolate]()

Isolation is done by using a combination of namespaces and a `chroot` system call.

Firstly, a proxy process is created inside a new user and pid namespace using a
`clone` call. The parent process then creates appropriate uid and gid mappings
in the new namespace and signals the new process, that is is ready. The proxy
process can then gain root privilages inside this new namespace needed for
setting up limits later.

Proxy process then forks, creating a new child process that will run the users program. Before executing it using `execvp`:

1. `chroot` into sandbox directory (if enabled) to prevent user process from accesing host filesystem
2. setup file descriptors - redirect `stdin`, `stdout` and `stderr` to specified file descriptors and close all other open file descriptors
3. set resource limits for this process
4. drop all capabilities (since the process is curently running as `root`, we neet to also set `no_root` security bit to ensure, it does not escalate its capabilities)

Meanwhile the main process is notified through another pipe, that the child is
running and that the proxy function waiting for the child to exit. After the
child exits, the proxy function writes its exit code to the same pipe. This
pipe is later used to inspect the state of the player process.

Some utility functions used in `isolate.cpp` are define in
[util](./server/process/include/util.h) header.

# Protocol

This module contains definitions of structs used to store game data for
comunication and parsing of data from users. The `format.h` header contains
formatting functions that format game data into messages, that can be directly
sent to players.
