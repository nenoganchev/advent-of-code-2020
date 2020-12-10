#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int> parse_input();
bool find_numbers(const std::vector<int> &list, int &a, int &b);


int main(int argc, char *argv[]) {
    std::vector<int> input = parse_input();

    int a, b;
    bool found = find_numbers(input, a, b);
    if (!found) {
        printf("Error: numbers not found!\n");
        return 1;
    }

    printf("Result: %d\n", a * b);
}

std::vector<int> parse_input() {
    std::ifstream input_file("day1_input.txt");

    std::vector<int> parsed_input;
    int i;
    while (input_file >> i) {
        parsed_input.push_back(i);
    }

    return parsed_input;
}

bool find_numbers(const std::vector<int> &list, int &a, int &b) {
    for (size_t i = 0; i < list.size(); ++i) {
        for (size_t j = i + 1; j < list.size(); ++j) {
            if (list[i] + list[j] == 2020) {
                a = list[i];
                b = list[j];
                return true;
            }
        }
    }

    return false;
}
