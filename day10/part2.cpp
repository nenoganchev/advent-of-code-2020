#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int> parse_input(const std::string &filename);
int64_t count_arrangements(const std::vector<int> &sorted_ratings);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<int> ratings = parse_input(filename);
    ratings.push_back(0);  // include the outlet in the list
    std::sort(ratings.begin(), ratings.end());

    int64_t n_arragements = count_arrangements(ratings);

    std::cout << "Total number of arrangements: " << n_arragements << "\n";
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

int64_t count_arrangements(const std::vector<int> &sorted_ratings) {
    std::vector<int64_t> memoized_counts(sorted_ratings.size(), 0);
    int i_end = sorted_ratings.size();

    memoized_counts.back() = 1;  // only one way to connect device to last adapter

    for (int i_examined_adapter = sorted_ratings.size() - 2; i_examined_adapter >= 0; --i_examined_adapter) {
        int64_t count_for_examined_adapter = 0;

        for (int offset = 1; offset <= 3; ++offset) {
            if (i_examined_adapter + offset < i_end &&
                sorted_ratings[i_examined_adapter + offset] - sorted_ratings[i_examined_adapter] <= 3)
            {
                count_for_examined_adapter += memoized_counts[i_examined_adapter + offset];
            }
        }

        memoized_counts[i_examined_adapter] = count_for_examined_adapter;
    }

    return memoized_counts[0];
}
