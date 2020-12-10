#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


typedef std::vector<std::set<char>> group_answers;


std::vector<group_answers> parse_input(const char * input_filename);
std::string collate_group_answers(const group_answers &answers);


int main(int argc, char *argv[]) {
    std::vector<group_answers> groups = parse_input(argv[1]);

    int total_yes_answers = 0;
    for (const group_answers &group : groups) {
        std::string collated_answers = collate_group_answers(group);
        total_yes_answers += collated_answers.size();
    }

    std::cout << "Total yes answers: " << total_yes_answers << "\n";
}

std::vector<group_answers> parse_input(const char * input_filename) {
    std::ifstream input_file(input_filename);
    std::vector<group_answers> result;

    while (input_file) {
        group_answers curr_group;
        std::string line;
        while (std::getline(input_file, line)) {
            if (line.empty()) break;

            std::set<char> curr_answer;
            for (char c : line) {
                curr_answer.insert(c);
            }
            curr_group.push_back(curr_answer);
        }

        result.push_back(curr_group);
    }

    return result;
}

std::string collate_group_answers(const group_answers &answers) {
    std::set<char> all_answers;

    const std::set<char> &first_answer = answers[0];
    for (char c : first_answer) {
        bool omnipresent = true;
        for (int i = 1; i < answers.size(); ++i) {
            if (answers[i].count(c) == 0) {
                omnipresent = false;
                break;
            }
        }

        if (omnipresent) all_answers.insert(c);
    }

    std::string collated_answers;
    for (char answer : all_answers) {
        collated_answers.push_back(answer);
    }
    return collated_answers;
}
