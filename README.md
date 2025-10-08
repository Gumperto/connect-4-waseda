# connect-4-waseda build and run instructions
This README is for the CSCE Lab A DSA Project, Fall Semester AY 2025 at 早稲田大学. We include build and run instructions for contributors and users here.

### NOTE: This project is still highly under construction! This README might change at any moment, so please keep an eye out!

1. Open your terminal application (Bash for Windows, iTerm2 for MacOS, your terminal of choice for Linux, etc). 

2. In your desired directory, clone the repository and navigate to the connect-4-waseda directory:
```
git clone https://github.com/Gumperto/connect-4-waseda.git
cd connect-4-waseda
```

3. Build the program using the following command in your directory:
```
make
```

4. Test running the program
```
./build/connect-4-waseda
```

5. Fix any features you desire (source files in /src) and make a pull request/direct push to the repo! In case of PR I'll review it within 1 - 2 days.

6. If you would like to clean up the directory a bit, type:
```
make clean
```

# To-do
- [x] Project spine (create, print, fill board done)
- [x] Build and run instructions
- [x] How to use git and GitHub
- [x] Implemented actual logic for playing connect 4
- [ ] Error checking: check when the board is completely filled
- [ ] Reset/quit game feature (should be exposed enough so the previous error checking thing can also just access it to end the game when it detects there's no possible move left)
- [ ] Misc features (game modes, scoreboard, fancier text effects like highlighting winning segment, potentially implement fancier graphical libraries)
- - [x] Bot mode
- - [ ] Score board
- - [ ] Highlight segment(s) that won the game (shouldn't be hard, just shoot rays from the winning shot and color those tiles. Problem is sending that signal to print_board)
- - [ ] Very optional: refactor completely so we can get a GUI (gtk etc.) or TUI (through ncurses)
- [ ] Presentation
