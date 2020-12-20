#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "string_utils.h"


struct rule {
    enum type { character, compound };

    type type;
    char matched_character;
    std::vector<std::vector<int>> sub_rules;
};


std::vector<rule> g_rules;
std::vector<std::string> g_rule_42_matches;
std::vector<std::string> g_rule_31_matches;


void parse_input(const std::string &filename, std::vector<std::string> &messages);
rule parse_rule(std::istream &is);
void calculate_top_non_recursive_rules_matches();
bool build_matches_for_rule(int rule_number,
                            const std::map<int, std::vector<std::string>> &already_built_matches,
                            std::vector<std::string> &matches);
bool matches_first_rule(const std::string &message);
std::vector<std::string>::const_iterator find_match_at_start(const std::string_view &message,
                                                             const std::vector<std::string> &matches);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<std::string> messages;
    parse_input(filename, messages);
    calculate_top_non_recursive_rules_matches();

    int n_matching_messages = 0;
    for (const std::string &message : messages) {
        if (matches_first_rule(message)) {
            ++n_matching_messages;
            std::cout << "Matched message: " << message << "\n";
        }
    }

    std::cout << "Matching messages: " << n_matching_messages << "\n";
}

void parse_input(const std::string &filename, std::vector<std::string> &messages) {
    std::ifstream input_file(filename);
    std::string line;

    std::map<int, rule> rules_cache;
    while (std::getline(input_file, line) && !line.empty()) {
        std::istringstream iss(line);

        int rule_number;
        iss >> rule_number >> expect(":");

        rules_cache[rule_number] = parse_rule(iss);
    }
    g_rules.resize(rules_cache.rbegin()->first + 1);
    for (const auto &[rule_number, rule] : rules_cache) {
        g_rules[rule_number] = rule;
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

void calculate_top_non_recursive_rules_matches() {
    std::map<int, std::vector<std::string>> rule_matches;

    for (size_t i = 0; i < g_rules.size(); ++i) {
        if (g_rules[i].type == rule::character) {
            rule_matches[i] = std::vector<std::string>(1, std::string(1, g_rules[i].matched_character));
        }
    }

    while (!(rule_matches.contains(42) && rule_matches.contains(31))) {
        for (size_t i = 0; i < g_rules.size(); ++i) {
            if (g_rules[i].type != rule::compound) continue;

            std::vector<std::string> matches;
            if (build_matches_for_rule(i, rule_matches, matches)) {
                rule_matches[i] = matches;
            }
        }
    }

    g_rule_42_matches = rule_matches[42];
    g_rule_31_matches = rule_matches[31];
}

bool build_matches_for_rule(int rule_number,
                            const std::map<int, std::vector<std::string>> &already_built_matches,
                            std::vector<std::string> &matches) {
    matches.clear();

    for (const std::vector<int> &partial_sub_rules : g_rules[rule_number].sub_rules) {
        std::vector<std::string> partial_matches(1, "");

        for (int sub_rule_number : partial_sub_rules) {
            if (!already_built_matches.contains(sub_rule_number)) {
                return false;
            }

            const std::vector<std::string> &sub_rule_matches = already_built_matches.find(sub_rule_number)->second;

            std::vector<std::string> combined_matches;
            for (const std::string &partial_match : partial_matches) {
                for (const std::string &sub_rule_match : sub_rule_matches) {
                    combined_matches.push_back(partial_match + sub_rule_match);
                }
            }

            partial_matches = combined_matches;
        }

        matches.insert(matches.end(), partial_matches.begin(), partial_matches.end());
    }

    return true;
}

bool matches_first_rule(const std::string &message) {
    // Rule 0 when expanded is 42+ (42 31)+
    std::string_view message_v = message;
    std::vector<std::string>::const_iterator match_it;

    int n_42_matches = 0;
    while ((match_it = find_match_at_start(message_v, g_rule_42_matches)) != g_rule_42_matches.end()) {
        ++n_42_matches;
        message_v.remove_prefix(match_it->size());
    }

    int n_31_matches = 0;
    while ((match_it = find_match_at_start(message_v, g_rule_31_matches)) != g_rule_31_matches.end()) {
        ++n_31_matches;
        message_v.remove_prefix(match_it->size());
    }

    return message_v.empty() && n_42_matches > 1 && n_31_matches > 0 && n_42_matches > n_31_matches;
}

std::vector<std::string>::const_iterator find_match_at_start(const std::string_view &message,
                                                             const std::vector<std::string> &matches) {
    return std::find_if(matches.begin(),
                        matches.end(),
                        [&message](const std::string &match) { return message.starts_with(match); });
}
