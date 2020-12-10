#include <iostream>
#include <fstream>
#include <string>
#include <vector>


enum operation {
    acc,
    jmp,
    nop,
};

struct instruction {
    operation op;
    int arg;
};


std::vector<instruction> parse_input(const std::string &filename);
int execute(const std::vector<instruction> &instructions);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error: input filename required\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<instruction> instructions = parse_input(filename);

    int accumulator = execute(instructions);
    std::cout << "Accumulator: " << accumulator << "\n";
}

std::vector<instruction> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<instruction> result;

    std::string op_str;
    instruction instruction;
    while (input_file >> op_str >> instruction.arg) {
        if (op_str == "acc") {
            instruction.op = acc;
        }
        else if (op_str == "jmp") {
            instruction.op = jmp;
        }
        else if (op_str == "nop") {
            instruction.op = nop;
        }
        else {
            throw "UnexpectedInput";
        }

        result.push_back(instruction);
    }

    return result;
}

int execute(const std::vector<instruction> &instructions) {
    int accumulator = 0;
    std::vector<bool> executed(instructions.size(), false);

    size_t pc = 0;
    for (;;) {
        if (pc >= instructions.size()) throw "PC out of bounds";
        if (executed[pc]) {
            return accumulator;
        }
        executed[pc] = true;

        instruction instruction = instructions[pc];

        switch (instruction.op) {
            case jmp:
                pc += instruction.arg;
                break;
            case acc:
                accumulator += instruction.arg;
            case nop:
                ++pc;
                break;
        }
    }
}
