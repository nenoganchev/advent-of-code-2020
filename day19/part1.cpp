#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "string_utils.h"


struct rule {
    enum type { character, compound };

    type type;
    char matched_character;
    std::vector<std::vector<int>> sub_rules;
};


void parse_input(const std::string &filename, std::vector<rule> &rules, std::vector<std::string> &messages);
rule parse_rule(std::istream &is);
bool matches_first_rule(const std::string &message, const std::vector<rule> &rules);
bool matches_rule(const std::string &message,
                  size_t start_pos,
                  const std::vector<rule> &rules,
                  size_t i_rule,
                  size_t &match_end_pos);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<rule> rules;
    std::vector<std::string> messages;
    parse_input(filename, rules, messages);

    int n_matching_messages = 0;
    for (const std::string &message : messages) {
        if (matches_first_rule(message, rules)) {
            ++n_matching_messages;
            std::cout << "Matched message: " << message << "\n";
        }
    }

    std::cout << "Matching messages: " << n_matching_messages << "\n";
}

void parse_input(const std::string &filename, std::vector<rule> &rules, std::vector<std::string> &messages) {
    std::ifstream input_file(filename);
    std::string line;

    std::map<int, rule> rules_cache;
    while (std::getline(input_file, line) && !line.empty()) {
        std::istringstream iss(line);

        int rule_number;
        iss >> rule_number >> expect(":");

        rules_cache[rule_number] = parse_rule(iss);
    }
    rules.resize(rules_cache.rbegin()->first + 1);
    for (const auto &[rule_number, rule] : rules_cache) {
        rules[rule_number] = rule;
    }

    while (std::getline(input_file, line)) {
        messages.push_back(line);
    }
}

rule parse_rule(std::istream &is) {
    rule parsed_rule;

    is >> skip(" ");

    if (is.peek() == '"') {
        parsed_rule.type = rule::character;
        is >> expect("\"") >> parsed_rule.matched_character >> expect("\"");
    }
    else {
        parsed_rule.type = rule::compound;

        for (;;) {
            std::vector<int> matched_rules;

            int rule_number;
            while (is >> rule_number) {
                matched_rules.push_back(rule_number);
            }
            parsed_rule.sub_rules.push_back(matched_rules);

            if (is.eof()) break;
            is.clear();
            if (is.get() != '|') throw std::runtime_error("parse_rule failed (expected `|`)");
        }
    }

    return parsed_rule;
}

bool matches_first_rule(const std::string &message, const std::vector<rule> &rules) {
    size_t match_end_pos;
    return matches_rule(message, 0, rules, 0, match_end_pos) && match_end_pos == message.size();
}

bool matches_rule(const std::string &message,
                  size_t start_pos,
                  const std::vector<rule> &rules,
                  size_t i_rule,
                  size_t &match_end_pos) {
    if (start_pos >= message.size()) return false;

    if (rules[i_rule].type == rule::character) {
        match_end_pos = start_pos + 1;
        return message[start_pos] == rules[i_rule].matched_character;
    }
    else {
        for (const std::vector<int> &matched_rules : rules[i_rule].sub_rules) {
            bool all_rules_matched = true;
            size_t curr_match_pos = start_pos;

            for (int curr_matched_rule : matched_rules) {
                if (!matches_rule(message, curr_match_pos, rules, curr_matched_rule, match_end_pos)) {
                    all_rules_matched = false;
                    break;
                }

                curr_match_pos = match_end_pos;
            }

            if (all_rules_matched) return true;
        }

        return false;
    }
}
