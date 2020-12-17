#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <bitset>
#include <cmath>


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


struct expect {
    expect(const std::string &str): expected_str(str) {}

    friend std::istream &operator >> (std::istream &is, const expect &manip) {
        for (char expected_char : manip.expected_str) {
            char read_char;
            if (!(is.get(read_char) && read_char == expected_char)) {
                throw "UnexpectedArg";
            }
        }

        return is;
    }

    std::string expected_str;
};


std::vector<command> parse_input(const std::string &filename);
std::map<uint64_t, uint64_t> execute(const std::vector<command> &commands);
std::vector<uint64_t> generate_modified_addresses(uint64_t addr, const std::vector<int> &floating_bit_positions);


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
    std::bitset<36> or_mask;
    std::vector<int> floating_bit_positions;

    for (const command &command : commands) {
        if (command.type == command::set_bitmask) {
            or_mask.reset();
            floating_bit_positions.clear();

            for (size_t i = 0; i < command.mask.size(); ++i) {
                switch (command.mask[command.mask.size() - i - 1]) {
                case '1':
                    or_mask[i] = 1;
                    break;
                case 'X':
                    floating_bit_positions.push_back(i);
                    break;
                }
            }
        }
        else if (command.type == command::store_value) {
            uint64_t addr = command.addr | or_mask.to_ullong();

            for (uint64_t modified_addr : generate_modified_addresses(addr, floating_bit_positions)) {
                addr_space[modified_addr] = command.value;
            }
        }
    }

    return addr_space;
}

std::vector<uint64_t> generate_modified_addresses(uint64_t addr, const std::vector<int> &floating_bit_positions) {
    std::vector<uint64_t> generated_addresses;

    unsigned n_permutations = 1 << floating_bit_positions.size();
    for (unsigned permutation = 0; permutation < n_permutations; ++permutation) {
        std::bitset<64> modified_addr = addr;
        std::bitset<64> permutation_bitset = permutation;

        for (unsigned i = 0; i < floating_bit_positions.size(); ++i) {
            modified_addr[floating_bit_positions[i]] = permutation_bitset[i];
        }

        generated_addresses.push_back(modified_addr.to_ullong());
    }

    return generated_addresses;
}
