# CFuck
Brainfuck Compiler and Interpreter written in c

Note: Written for Linux. May work on other OSes. Will almost certainly not work on Windows.

# Compiler Requirements

- nasm
- gcc

# Compiler Usage

1. Compile compile.c (ie: ```gcc -o compile compile.c```)
2. Run run.sh with format ```./run.sh brainfuckfile.bf outputbinary```
3. Test your code with ```./outputbinary```

# Interpreter Requirements

- Some kind of c compiler (ie: gcc, clang)

# Interpreter Usage

1. Compile interpret.c (ie: ```gcc -o interpret interpret.c``` or ```clang -o interpret interpret.c```)
2. run interpret on your Brainfuck program (ie: ```./interpret brainfuckfile.bf```)

# A note on optimization

- I don't intend to optimize the current compile.c further as I feel it would lose
some of it's charm as it gets further from bf.

- I may make a second compiler that optimizes bf a lot heavier.
