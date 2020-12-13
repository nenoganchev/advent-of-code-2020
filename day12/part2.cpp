#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <charconv>


enum action {
    move_north,
    move_south,
    move_east,
    move_west,
    turn_left,
    turn_right,
    move_forward,
};

struct instruction {
    action action;
    int arg;
};


std::vector<instruction> parse_input(const std::string &filename);
std::pair<int, int> execute_instructions(const std::vector<instruction> &instructions);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<instruction> instructions = parse_input(filename);

    auto [x, y] = execute_instructions(instructions);

    std::cout << "Ship bound for [" << x << ", " << y << "], distance: " << std::abs(x) + std::abs(y) << "\n";
}

std::vector<instruction> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<instruction> result;

    std::string line;
    while (std::getline(input_file, line)) {
        instruction instruction;

        switch (line[0]) {
        case 'N':
            instruction.action = move_north;
            break;
        case 'S':
            instruction.action = move_south;
            break;
        case 'E':
            instruction.action = move_east;
            break;
        case 'W':
            instruction.action = move_west;
            break;
        case 'L':
            instruction.action = turn_left;
            break;
        case 'R':
            instruction.action = turn_right;
            break;
        case 'F':
            instruction.action = move_forward;
            break;
        default:
            throw "InvalidAction";
        }

        std::from_chars(line.data() + 1, line.data() + line.size(), instruction.arg);

        if ((instruction.action == turn_left || instruction.action == turn_right) &&
            instruction.arg % 90 != 0)
        {
            throw "InvalidTurnArgument";
        }

        result.push_back(instruction);
    }

    return result;
}

std::pair<int, int> execute_instructions(const std::vector<instruction> &instructions) {
    int ship_x = 0, ship_y = 0;
    int waypoint_rel_x = 10, waypoint_rel_y = 1;

    for (const instruction &instruction : instructions) {
        switch (instruction.action) {
        case move_north:
            waypoint_rel_y += instruction.arg;
            break;

        case move_south:
            waypoint_rel_y -= instruction.arg;
            break;

        case move_east:
            waypoint_rel_x += instruction.arg;
            break;

        case move_west:
            waypoint_rel_x -= instruction.arg;
            break;

        case turn_left:
            {
                int n_turns = instruction.arg / 90;
                for (int i = 0; i < n_turns; ++i) {
                    int new_waypoint_rel_x = -waypoint_rel_y;
                    int new_waypoint_rel_y = waypoint_rel_x;

                    waypoint_rel_x = new_waypoint_rel_x, waypoint_rel_y = new_waypoint_rel_y;
                }
            }
            break;

        case turn_right:
            {
                int n_turns = instruction.arg / 90;
                for (int i = 0; i < n_turns; ++i) {
                    int new_waypoint_rel_x = waypoint_rel_y;
                    int new_waypoint_rel_y = -waypoint_rel_x;

                    waypoint_rel_x = new_waypoint_rel_x, waypoint_rel_y = new_waypoint_rel_y;
                }
            }
            break;

        case move_forward:
            ship_x += instruction.arg * waypoint_rel_x;
            ship_y += instruction.arg * waypoint_rel_y;
            break;
        }
    }

    return std::make_pair(ship_x, ship_y);
}
