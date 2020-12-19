#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


class pocket_dimension {
public:
    void activate(int x, int y, int z);
    pocket_dimension simulate_cycle() const;
    int active_count() const;

private:
    int active_neighbours_count(int x, int y, int z) const;

    std::set<std::tuple<int, int, int>> active;
};


pocket_dimension parse_input(const std::string &filename);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    pocket_dimension dimension = parse_input(filename);

    for (int i = 0; i < 6; ++i) {
        dimension = dimension.simulate_cycle();
    }

    std::cout << "Active cubes: " << dimension.active_count() << "\n";
}

pocket_dimension parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    pocket_dimension result;

    int y = -1;
    std::string line;
    while (std::getline(input_file, line)) {
        ++y;

        for (size_t x = 0; x < line.size(); ++x) {
            if (line[x] == '#') result.activate(x, y, 0);
        }
    }

    return result;
}


void pocket_dimension::activate(int x, int y, int z) {
    this->active.insert({x, y, z});
}

pocket_dimension pocket_dimension::simulate_cycle() const {
    int min_x, max_x, min_y, max_y, min_z, max_z;
    bool min_max_inited = false;
    for (auto [x, y, z] : this->active) {
        if (!min_max_inited) {
            min_x = max_x = x;
            min_y = max_y = y;
            min_z = max_z = z;

            min_max_inited = true;
        }
        else {
            if (min_x > x) min_x = x;
            if (max_x < x) max_x = x;
            if (min_y > y) min_y = y;
            if (max_y < y) max_y = y;
            if (min_z > z) min_z = z;
            if (max_z < z) max_z = z;
        }
    }

    pocket_dimension result;
    for (int x = min_x - 1; x <= max_x + 1; ++x) {
        for (int y = min_y - 1; y <= max_y + 1; ++y) {
            for (int z = min_z - 1; z <= max_z + 1; ++z) {
                int active_neighbours = this->active_neighbours_count(x, y, z);

                if (this->active.contains({x, y, z})) {
                    if (active_neighbours == 2 || active_neighbours == 3) result.activate(x, y, z);
                }
                else {
                    if (active_neighbours == 3) result.activate(x, y, z);
                }
            }
        }
    }

    return result;
}

int pocket_dimension::active_count() const {
    return this->active.size();
}

int pocket_dimension::active_neighbours_count(int x, int y, int z) const {
    int active_neighbours = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
                if (dx == 0 && dy == 0 && dz == 0) continue;

                if (this->active.contains({x + dx, y + dy, z + dz})) ++active_neighbours;
            }
        }
    }

    return active_neighbours;
}
