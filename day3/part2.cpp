#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>


struct coordinate {
    int x;
    int y;
};


std::vector<std::string> parse_input();
int count_trees_for_slope(const std::vector<std::string> &map, int d_x, int d_y);


int main(int argc, char *argv[]) {
    std::vector<std::string> map = parse_input();

    int offsets[][2] = {
        {1, 1},
        {3, 1},
        {5, 1},
        {7, 1},
        {1, 2},
    };

    uint64_t trees_product = 1;
    for (int i = 0; i < std::size(offsets); ++i) {
        int trees_in_current_slope = count_trees_for_slope(map, offsets[i][0], offsets[i][1]);
        trees_product *= trees_in_current_slope;
    }

    std::cout << "Trees product: " << trees_product << std::endl;
}

std::vector<std::string> parse_input() {
    std::ifstream input_file("input.txt");
    std::vector<std::string> result;

    std::string line;
    while (std::getline(input_file, line)) {
        result.push_back(line);
    }

    return result;
}

int count_trees_for_slope(const std::vector<std::string> &map, int d_x, int d_y) {
    coordinate curr_pos = {0, 0};
    int map_width = map[0].size();
    int map_height = map.size();
    int trees_encountered = 0;

    while (curr_pos.y < map_height) {
        if (map[curr_pos.y][curr_pos.x % map_width] == '#') {
            trees_encountered += 1;
        }

        curr_pos.x += d_x;
        curr_pos.y += d_y;
    }

    return trees_encountered;
}
