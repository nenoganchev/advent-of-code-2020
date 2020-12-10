#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct coordinate {
    int x;
    int y;
};


std::vector<std::string> parse_input();


int main(int argc, char *argv[]) {
    std::vector<std::string> map = parse_input();

    coordinate curr_pos = {0, 0};
    int map_width = map[0].size();
    int map_height = map.size();
    int trees_encountered = 0;

    while (curr_pos.y < map_height) {
        if (map[curr_pos.y][curr_pos.x % map_width] == '#') {
            trees_encountered += 1;
        }

        curr_pos.x += 3;
        curr_pos.y += 1;
    }

    std::cout << "Trees encountered: " << trees_encountered << std::endl;
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
