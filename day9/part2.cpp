#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int64_t> parse_input(const std::string &filename);
void find_sequence(const std::vector<int64_t> &code, int64_t attacked_number, int &i_seq_first, int &i_seq_last);
void find_min_max(const std::vector<int64_t> &code, int i_first, int i_last, int64_t &min, int64_t &max);
int64_t sequence_sum(const std::vector<int64_t> &code, int i_first, int i_last);


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: <exe> <filename> <attacked number>\n";
        return 1;
    }
    const std::string filename = argv[1];
    int64_t attacked_number = atoi(argv[2]);

    std::vector<int64_t> code = parse_input(filename);

    int i_seq_first, i_seq_last;
    find_sequence(code, attacked_number, i_seq_first, i_seq_last);
    if (i_seq_first == i_seq_last) {
        std::cout << "Error: cannot find weakness\n";
        return 1;
    }

    int64_t min, max;
    find_min_max(code, i_seq_first, i_seq_last, min, max);

    std::cout << "Weakness found: " << min + max << "\n";
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

void find_sequence(const std::vector<int64_t> &code, int64_t attacked_number, int &i_seq_first, int &i_seq_last) {
    i_seq_first = 0;
    i_seq_last = 1;
    int64_t sum = code[0] + code[1];

    for (;;) {
        while (sum < attacked_number) {
            sum += code[++i_seq_last];
        }
        if (sum == attacked_number) return;

        while (sum > attacked_number) {
            sum -= code[i_seq_first++];
        }
        if (sum == attacked_number) return;
    }
}

void find_min_max(const std::vector<int64_t> &code, int i_first, int i_last, int64_t &min, int64_t &max) {
    min = max = code[i_first];
    for (int i = i_first; i <= i_last; ++i) {
        if (min > code[i]) min = code[i];
        if (max < code[i]) max = code[i];
    }
}
