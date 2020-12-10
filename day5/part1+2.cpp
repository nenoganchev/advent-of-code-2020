#include <iostream>
#include <fstream>
#include <string>
#include <vector>


void decode_seat(const std::string &code, int &row, int &column, int &id);


int main(int argc, char *argv[]) {
    std::ifstream input_file("input.txt");

    std::vector<int> seats;

    std::string line;
    while (std::getline(input_file, line)) {
        int row, column, id;
        decode_seat(line, row, column, id);

        seats.push_back(id);
    }

    std::sort(seats.begin(), seats.end());

    for (int i = 1; i < seats.size(); ++i) {
        if (seats[i - 1] + 1 != seats[i]) {
            std::cout << "Found empty seat between " << seats[i - 1] << " and " << seats[i]
                      << " (must be " << seats[i] - 1 << ")" << std::endl;
        }
    }
}

void decode_seat(const std::string &code, int &row, int &column, int &id) {
    const int row_code_len = 7;
    std::bitset<row_code_len> row_bits;
    for (int i = 0; i < row_code_len; ++i) {
        if (code[i] == 'B') {
            row_bits.set(row_code_len - i - 1);
        }
    }
    row = row_bits.to_ulong();

    const int column_code_len = 3;
    std::bitset<column_code_len> column_bits;
    for (int i = 0; i < column_code_len; ++i) {
        if (code[i + row_code_len] == 'R') {
            column_bits.set(column_code_len - i - 1);
        }
    }
    column = column_bits.to_ulong();

    id = row * 8 + column;
}
