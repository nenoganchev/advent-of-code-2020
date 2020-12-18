#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "string_utils.h"


struct bus_descriptor {
    int id;
    int list_offset;
};


std::vector<bus_descriptor> parse_input(const std::string &filename);
int64_t calculate_timestamp(const std::vector<bus_descriptor> &buses);
std::pair<bus_descriptor, bus_descriptor> find_buses_with_2_max_ids(const std::vector<bus_descriptor> &buses);
int64_t find_first_alignment(const std::pair<bus_descriptor, bus_descriptor> &max_2_buses);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<bus_descriptor> buses = parse_input(filename);
    int64_t t = calculate_timestamp(buses);

    std::cout << "Timestamp: " << t << "\n";
}

std::vector<bus_descriptor> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<bus_descriptor> result;

    input_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // discard arrival time on first line

    int offset = -1;
    for (;;) {
        ++offset;

        input_file >> skip(",");
        if (input_file.peek() == 'x') {
            input_file >> skip("x");
            continue;
        }

        int bus_id;
        if (!(input_file >> bus_id)) break;

        result.push_back({bus_id, offset});
    }

    return result;
}

int64_t calculate_timestamp(const std::vector<bus_descriptor> &buses) {
    auto max_id_buses = find_buses_with_2_max_ids(buses);
    int64_t first_alignment = find_first_alignment(max_id_buses);
    int64_t alignment_period = max_id_buses.first.id * max_id_buses.second.id;

    for (int64_t i = 0; ; ++i) {
        int64_t tested_t = (first_alignment + i * alignment_period) - max_id_buses.first.list_offset;

        bool condition_met = true;
        for (const bus_descriptor &bus : buses) {
            if (bus.id == max_id_buses.first.id || bus.id == max_id_buses.second.id) continue;

            if ((tested_t + bus.list_offset) % bus.id) {
                condition_met = false;
                break;
            }
        }

        if (condition_met) return tested_t;

        if ((i & 0xfffffff) == 0) {
            std::cout << "Passed timestamp " << tested_t << std::endl;
        }
    }
}

std::pair<bus_descriptor, bus_descriptor> find_buses_with_2_max_ids(const std::vector<bus_descriptor> &buses) {
    bus_descriptor max_bus_1 = {0, 0};
    bus_descriptor max_bus_2 = {0, 0};

    for (const bus_descriptor &bus : buses) {
        if (bus.id > max_bus_1.id) {
            max_bus_1 = bus;
            continue;
        }
        if (bus.id > max_bus_2.id) {
            max_bus_2 = bus;
            continue;
        }
    }

    return std::make_pair(max_bus_1, max_bus_2);
}

int64_t find_first_alignment(const std::pair<bus_descriptor, bus_descriptor> &max_2_buses) {
    int64_t offsets_diff = max_2_buses.second.list_offset - max_2_buses.first.list_offset;

    for (int64_t i = 1; ; ++i) {
        if ((i * max_2_buses.first.id + offsets_diff) % max_2_buses.second.id == 0) {
            return i * max_2_buses.first.id;
        }
    }
}
