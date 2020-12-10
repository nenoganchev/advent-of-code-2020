#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int64_t> parse_input(const std::string &filename);
bool number_is_valid(const std::vector<int64_t> &code, int i_checked_number, int preable_size);


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: <exe> <filename> <preamble length>\n";
        return 1;
    }
    const std::string filename = argv[1];
    int preamble_size = atoi(argv[2]);

    std::vector<int64_t> code = parse_input(filename);

    for (size_t i = preamble_size; i < code.size(); ++i) {
        if (!number_is_valid(code, i, preamble_size)) {
            std::cout << "Found invalid number: " << code[i] << "\n";
            return 0;
        }
    }

    std::cout << "All numbers are valid\n";
}

std::vector<int64_t> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<int64_t> result;

    int64_t n;
    while (input_file >> n) {
        result.push_back(n);
    }

    return result;
}

bool number_is_valid(const std::vector<int64_t> &code, int i_checked_number, int preable_size) {
    for (int i = i_checked_number - preable_size; i < i_checked_number; ++i) {
        for (int j = i + 1; j < i_checked_number; ++j) {
            if (code[i] == code[j]) continue;
            if (code[i] + code[j] == code[i_checked_number]) return true;
        }
    }

    return false;
}
