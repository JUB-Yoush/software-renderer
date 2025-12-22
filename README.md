# Software Based .obj renderer
- Followed along with [Marian Pekar's Odin Software renderer series](https://www.marianpekar.com/) but implemented it in c++.
- Requires raylib to run, although we only use the window/input management and DrawPixel(), everything else is re-implemented.
- The cmake file should be evertything you need to run it, code in main.cpp should be quite self explainatory.
- UV mapping seems to be off, probably due to differences between odin mulitpointers and the Color* raylib returns.
