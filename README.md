# bf-interpreter
BF interpreter with basic optimizations. Made with the [esolang.org](https://esolangs.org/wiki/Brainfuck) BF specification.
## Building
```
git clone https://github.com/linuxnoodle/bf-interpreter
cd bf-interpreter
make
```
## TODO
- [X] Basic Optimizations (grouping together addition + subtraction)
- [ ] More complex optimizations, removing redundancies. 
- [X] Refactoring code to be more modular.
- [ ] Add in some way to compile to either another language, or directly to a binary.
