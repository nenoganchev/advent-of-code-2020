#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "string_utils.h"


std::vector<int> parse_input(const std::string &filename);
void play_for_n_turns(std::vector<int> &numbers, int n_turns);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<int> numbers = parse_input(filename);
    play_for_n_turns(numbers, 30000000);

    std::cout << "Last number: " << numbers.back() << "\n";
}

std::vector<int> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<int> result;

    int n;
    while (input_file >> skip(",") >> n) {
        result.push_back(n);
    }

    return result;
}

void play_for_n_turns(std::vector<int> &numbers, int n_turns) {
    std::map<int, int> number_to_turn_map;

    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        number_to_turn_map[numbers[i]] = i;
    }

    for (int turn = numbers.size(); turn < n_turns; ++turn) {
        int last_number = numbers.back();
        int next_number;

        auto last_occurrence = number_to_turn_map.find(last_number);
        if (last_occurrence == number_to_turn_map.end()) {
            next_number = 0;
        }
        else {
            next_number = turn - 1 - last_occurrence->second;
        }

        number_to_turn_map[last_number] = turn - 1;
        numbers.push_back(next_number);
    }
}
