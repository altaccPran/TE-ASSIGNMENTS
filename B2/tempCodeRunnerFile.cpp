#include <iostream>
#include "pass1.cpp" // include your AssemblerPass1 header/source

int main() {
    // Create assembler object with file paths
    AssemblerPass1 assembler(
        "input.asm",     // Assembly source file
        "IC.txt",        // Intermediate code output
        "SYMTAB.txt",    // Symbol table output
        "LITTAB.txt"     // Literal table output
    );

    // Run Pass 1
    assembler.performPass();

    std::cout << "\nPass 1 completed. Check IC.txt, SYMTAB.txt, and LITTAB.txt\n";
    return 0;
}
