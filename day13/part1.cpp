#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct skip {
    const char *skipped_characters;
    skip(const char *s) { skipped_characters = s; }

    friend std::istream &operator >> (std::istream &is, const skip &manip) {
        while (is && strchr(manip.skipped_characters, is.peek())) is.ignore();
        return is;
    }
};


void parse_input(const std::string &filename, int &start_time, std::vector<int> &bus_ids);
void calculate_earliest_bus(int start_time,
                            const std::vector<int> &bus_ids,
                            int &earliest_bus_id,
                            int &minutes_to_wait);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    int start_time;
    std::vector<int> bus_ids;
    parse_input(filename, start_time, bus_ids);

    int earliest_bus_id, minutes_to_wait;
    calculate_earliest_bus(start_time, bus_ids, earliest_bus_id, minutes_to_wait);

    std::cout << "Earliest bus: " << earliest_bus_id << ", minutes to wait " << minutes_to_wait
              << ". Product: " << earliest_bus_id * minutes_to_wait << "\n";
}

void parse_input(const std::string &filename, int &start_time, std::vector<int> &bus_ids) {
    std::ifstream input_file(filename);

    input_file >> start_time;

    int bus_id;
    while (input_file >> skip(",x") >> bus_id) {
        bus_ids.push_back(bus_id);
    }
}

void calculate_earliest_bus(int start_time,
                            const std::vector<int> &bus_ids,
                            int &earliest_bus_id,
                            int &minutes_to_wait) {
    minutes_to_wait = std::numeric_limits<int>::max();

    for (int bus_id : bus_ids) {
        int desync = start_time % bus_id;
        if (desync == 0) {
            earliest_bus_id = bus_id;
            minutes_to_wait = 0;
            return;
        }

        int wait_time = bus_id - desync;
        if (minutes_to_wait > wait_time) {
            earliest_bus_id = bus_id;
            minutes_to_wait = wait_time;
        }
    }
}
