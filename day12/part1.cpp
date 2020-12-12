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
    std::map<action, std::pair<int, int>> direction_offsets = {
        {move_east,  { 1,  0}},
        {move_west,  {-1,  0}},
        {move_north, { 0,  1}},
        {move_south, { 0, -1}},
    };
    const std::vector<action> directions_right_turn_order = {move_north, move_east, move_south, move_west};

    int curr_x = 0, curr_y = 0;
    action curr_direction = move_east;

    auto curr_direction_index = [&directions_right_turn_order, curr_direction] {
        auto curr_direction_iter = std::find(directions_right_turn_order.begin(),
                                             directions_right_turn_order.end(),
                                             curr_direction);
        return curr_direction_iter - directions_right_turn_order.begin();
    };

    for (const instruction &instruction : instructions) {
        switch (instruction.action) {
        case move_north:
            curr_y += instruction.arg;
            break;

        case move_south:
            curr_y -= instruction.arg;
            break;

        case move_east:
            curr_x += instruction.arg;
            break;

        case move_west:
            curr_x -= instruction.arg;
            break;

        case turn_left:
        case turn_right:
            {
                int i_curr_direction = curr_direction_index();
                int n_turns = instruction.arg / 90;
                int shift_modifier = instruction.action == turn_right ? 1 : -1;
                int i_new_direction = (i_curr_direction + shift_modifier * n_turns) % directions_right_turn_order.size();
                if (i_new_direction < 0) i_new_direction += directions_right_turn_order.size();
                curr_direction = directions_right_turn_order[i_new_direction];
            }
            break;

        case move_forward:
            auto [dx, dy] = direction_offsets[curr_direction];
            curr_x += instruction.arg * dx;
            curr_y += instruction.arg * dy;
            break;
        }
    }

    return std::make_pair(curr_x, curr_y);
}
