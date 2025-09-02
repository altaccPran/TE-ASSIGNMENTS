#include <iostream>
#include "pass1.h"
#include "pass2.h"

int main() {
    const std::string source = "input.asm";
    const std::string ic = "IC.txt";
    const std::string sym = "SYMTAB.txt";
    const std::string lit = "LITTAB.txt";
    const std::string machine = "MACHINE_CODE.txt";

    std::cout << "Starting Two-Pass Assembler...\n\n";

    try {
        AssemblerPass1 pass1(source, ic, sym, lit);
        pass1.performPass();

        std::cout << "\nPass 1 completed. Files generated:\n"
                  << "  - " << ic << "\n"
                  << "  - " << sym << "\n"
                  << "  - " << lit << "\n\n";

        AssemblerPass2 pass2(ic, sym, lit, machine);
        pass2.performPass();

        std::cout << "\nPass 2 completed. File generated:\n"
                  << "  - " << machine << "\n";
                  
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
