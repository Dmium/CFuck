./compile $1 "$2asdfghjkl.asm"
nasm -f elf32 "$2asdfghjkl.asm"
gcc -m32 "$2asdfghjkl.o" -o $2
chmod +x $2
rm "$2asdfghjkl.o"
rm "$2asdfghjkl.asm"
