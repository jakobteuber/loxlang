Loxlang
=======

This is a C++ implementation of an interpreter for the Lox programming language, following the book [*Crafting Interpreters*](https://craftinginterpreters.com/) by Robert Nystrom.

- For Nystrom’s implementation, see <https://github.com/munificent/craftinginterpreters>
- For other Lox implementations, see <https://github.com/munificent/craftinginterpreters/wiki/Lox-Implementations>

Building & Running
------------------

To build the project, run the following:

```bash
mkdir build && cd build
cmake ..
make
```

This will generate two executable in `build`, `loxlang` and `tester`. With `loxlang` you can either interpret
Lox files with 
```bash
./loxlang my-script.lox
```
or enter a Read-Evaluate-Print-Loop with
```bash
./loxlang
```

The second executable, `tester`, allows you to run the projects test suite.


License
-------

Loxlang is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Loxlang is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Loxlang (the `License` file in this repository).  
If not, see <http://www.gnu.org/licenses/>.