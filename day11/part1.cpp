#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


enum tile_type: char {
    free_seat = 'L',
    taken_seat = '#',
    floor = '.',
};

class waiting_area_map {
public:
    void add_row(const std::vector<tile_type> &row_tiles);

    tile_type at(int x, int y) const;
    int cols() const;
    int rows() const;

    int count_of(tile_type type) const;
    int count_of_adjacent(tile_type type, int x, int y) const;

    void begin_update() const;
    void update(int x, int y, tile_type new_type) const;
    bool apply_update();

private:
    std::vector<tile_type> tiles;
    mutable std::vector<tile_type> updated_tiles;
    int x_end = 0;
    int y_end = 0;
};


waiting_area_map parse_input(const std::string &filename);
bool apply_transformation(waiting_area_map &map);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    waiting_area_map map = parse_input(filename);

    bool map_changed;
    do {
        map_changed = apply_transformation(map);
    }
    while (map_changed);

    std::cout << "Occupied seats at equilibrium: " << map.count_of(taken_seat) << "\n";
}

waiting_area_map parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    waiting_area_map map;

    auto char_to_tile = [](char c) {
        switch (c) {
        case 'L': return free_seat;
        case '#': return taken_seat;
        case '.': return floor;
        default: throw "InvalidInput";
        }
    };

    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<tile_type> row;
        for (char c : line) {
            row.push_back(char_to_tile(c));
        }

        map.add_row(row);
    }

    return map;
}

bool apply_transformation(waiting_area_map &map) {
    map.begin_update();

    for (int y = 0; y < map.rows(); ++y) {
        for (int x = 0; x < map.cols(); ++x) {
            switch (map.at(x, y)) {
            case free_seat:
                if (map.count_of_adjacent(taken_seat, x, y) == 0) {
                    map.update(x, y, taken_seat);
                }
                break;
            case taken_seat:
                if (map.count_of_adjacent(taken_seat, x, y) >= 4) {
                    map.update(x, y, free_seat);
                }
                break;
            case floor:
                break;
            }
        }
    }

    return map.apply_update();
}


void waiting_area_map::add_row(const std::vector<tile_type> &row_tiles) {
    this->tiles.insert(this->tiles.end(), row_tiles.begin(), row_tiles.end());

    this->x_end = row_tiles.size();
    ++this->y_end;
}

tile_type waiting_area_map::at(int x, int y) const {
    return this->tiles[x + y * this->x_end];
}

int waiting_area_map::cols() const {
    return this->x_end;
}

int waiting_area_map::rows() const {
    return this->y_end;
}

int waiting_area_map::count_of(tile_type type) const {
    int count = 0;
    for (tile_type tile : this->tiles) {
        if (tile == type) ++count;
    }

    return count;
}

int waiting_area_map::count_of_adjacent(tile_type type, int x, int y) const {
    int count = 0;

    std::pair<int, int> neighbour_offsets[] = {{-1, -1}, {0, -1}, {1, -1},
                                               {-1,  0},          {1,  0},
                                               {-1,  1}, {0,  1}, {1,  1}};
    for (auto [dx, dy] : neighbour_offsets) {
        int neighbour_x = x + dx;
        int neighbour_y = y + dy;

        if (neighbour_x < 0 || neighbour_x >= this->x_end) continue;
        if (neighbour_y < 0 || neighbour_y >= this->y_end) continue;
        if (this->at(neighbour_x, neighbour_y) == type) ++count;
    }

    return count;
}

void waiting_area_map::begin_update() const {
    this->updated_tiles = this->tiles;
}

void waiting_area_map::update(int x, int y, tile_type new_type) const {
    this->updated_tiles[x + y * this->x_end] = new_type;
}

bool waiting_area_map::apply_update() {
    bool map_changed = this->tiles != this->updated_tiles;

    this->tiles = this->updated_tiles;

    return map_changed;
}
