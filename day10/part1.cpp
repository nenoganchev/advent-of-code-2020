#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int> parse_input(const std::string &filename);
void count_differences(const std::vector<int> &sorted_ratings, int &n_1jolt_diffs, int &n_3jolt_diffs);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<int> ratings = parse_input(filename);
    std::sort(ratings.begin(), ratings.end());

    int n_1jolt_diffs, n_3jolt_diffs;
    count_differences(ratings, n_1jolt_diffs, n_3jolt_diffs);

    std::cout << "Diffs (1/3): " << n_1jolt_diffs << "/" << n_3jolt_diffs << "\n"
              << "Product: " << n_1jolt_diffs * n_3jolt_diffs << "\n";
}

std::vector<int> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<int> result;

    int n;
    while (input_file >> n) {
        result.push_back(n);
    }

    return result;
}

void count_differences(const std::vector<int> &sorted_ratings, int &n_1jolt_diffs, int &n_3jolt_diffs) {
    int diff_counts[4] = {0, 0, 0, 1};

    ++diff_counts[sorted_ratings[0]];
    for (size_t i = 1; i < sorted_ratings.size(); ++i) {
        ++diff_counts[sorted_ratings[i] - sorted_ratings[i - 1]];
    }

    n_1jolt_diffs = diff_counts[1];
    n_3jolt_diffs = diff_counts[3];
}
