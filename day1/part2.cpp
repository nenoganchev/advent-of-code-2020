#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<uint64_t> parse_input();
bool find_numbers(const std::vector<uint64_t> &list, uint64_t &a, uint64_t &b, uint64_t &c);


int main(int argc, char *argv[]) {
    std::vector<uint64_t> input = parse_input();

    uint64_t a, b, c;
    bool found = find_numbers(input, a, b, c);
    if (!found) {
        printf("Error: numbers not found!\n");
        return 1;
    }

    printf("Result: %llu\n", a * b * c);
}

std::vector<uint64_t> parse_input() {
    std::ifstream input_file("input.txt");

    std::vector<uint64_t> parsed_input;
    uint64_t i;
    while (input_file >> i) {
        parsed_input.push_back(i);
    }

    return parsed_input;
}

bool find_numbers(const std::vector<uint64_t> &list, uint64_t &a, uint64_t &b, uint64_t &c) {
    for (size_t i1 = 0; i1 < list.size(); ++i1) {
        for (size_t i2 = i1 + 1; i2 < list.size(); ++i2) {
            for (size_t i3 = i2 + 1; i3 < list.size(); ++i3) {
                if (list[i1] + list[i2] > 2020) {
                    break;
                }

                if (list[i1] + list[i2] + list[i3] == 2020) {
                    a = list[i1];
                    b = list[i2];
                    c = list[i3];
                    return true;
                }
            }
        }
    }

    return false;
}
