#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include "string_utils.h"


struct command {
    enum type {
        set_bitmask,
        store_value,
    };

    type type;
    std::string mask;
    uint64_t addr;
    uint64_t value;
};


std::vector<command> parse_input(const std::string &filename);
std::map<uint64_t, uint64_t> execute(const std::vector<command> &commands);
void set_bit(uint64_t &n, size_t bit_pos);
void clear_bit(uint64_t &n, size_t bit_pos);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<command> commands = parse_input(filename);
    std::map<uint64_t, uint64_t> addr_space = execute(commands);

    uint64_t sum = 0;
    for (auto [addr, value] : addr_space) {
        sum += value;
    }

    std::cout << "Sum: " << sum << "\n";
}

std::vector<command> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<command> commands;

    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        command command;

        if (line.starts_with("mask")) {
            command.type = command::set_bitmask;

            iss >> expect("mask = ") >> command.mask;

        }
        else if (line.starts_with("mem")) {
            command.type = command::store_value;

            iss >> expect("mem[") >> command.addr >> expect("] = ") >> command.value;
        }
        else throw "InvalidArg";

        commands.push_back(command);
    }

    return commands;
}

std::map<uint64_t, uint64_t> execute(const std::vector<command> &commands) {
    std::map<uint64_t, uint64_t> addr_space;
    uint64_t and_mask, or_mask;

    for (const command &command : commands) {
        if (command.type == command::set_bitmask) {
            and_mask = 0xffff'ffff'ffff'ffff;
            or_mask = 0;

            for (size_t i = 0; i < command.mask.size(); ++i) {
                switch (command.mask[command.mask.size() - i -1]) {
                case '0':
                    clear_bit(and_mask, i);
                    break;

                case '1':
                    set_bit(or_mask, i);
                    break;
                }
            }
        }
        else if (command.type == command::store_value) {
            uint64_t stored_value = command.value;
            stored_value &= and_mask;
            stored_value |= or_mask;
            addr_space[command.addr] = stored_value;
        }
    }

    return addr_space;
}

void set_bit(uint64_t &n, size_t bit_pos) {
    n |= (1ull << bit_pos);
}

void clear_bit(uint64_t &n, size_t bit_pos) {
    n &= ~(1ull << bit_pos);
}
