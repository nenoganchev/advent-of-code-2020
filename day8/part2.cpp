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

struct result {
    bool finished;
    int accumulator;
};


std::vector<instruction> parse_input(const std::string &filename);
void switch_operation(operation &op);
result execute(const std::vector<instruction> &instructions);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error: input filename required\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<instruction> instructions = parse_input(filename);

    for (instruction &instruction : instructions) {
        if (instruction.op == acc) continue;

        switch_operation(instruction.op);

        result res = execute(instructions);
        if (res.finished) {
            std::cout << "[FIXED] Accumulator: " << res.accumulator << "\n";
            return 0;
        }

        switch_operation(instruction.op);
    }

    std::cout << "[CANNOT FIX]\n";
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

void switch_operation(operation &op) {
    op = (op == jmp ? nop : jmp);
}

result execute(const std::vector<instruction> &instructions) {
    int accumulator = 0;
    std::vector<bool> executed(instructions.size(), false);

    size_t pc = 0;
    for (;;) {
        if (pc == instructions.size()) {
            return {true, accumulator};
        }
        if (pc > instructions.size() || executed[pc]) {
            return {false, accumulator};
        }

        executed[pc] = true;

        switch (instructions[pc].op) {
            case jmp:
                pc += instructions[pc].arg;
                break;
            case acc:
                accumulator += instructions[pc].arg;
            case nop:
                ++pc;
                break;
        }
    }
}
