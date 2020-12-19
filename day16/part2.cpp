#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "string_utils.h"


struct rule {
    std::string field_name;
    std::pair<int, int> range1;
    std::pair<int, int> range2;
};

typedef std::vector<int> ticket;


void parse_input(const std::string &filename,
                 std::vector<rule> &rules,
                 ticket &my_ticket,
                 std::vector<ticket> &nearby_tickets);
rule parse_rule_line(const std::string &line);
ticket parse_ticket_line(const std::string &line);
bool ticket_is_valid(const ticket &ticket, const std::vector<rule> &rules);
bool conforms_to_rule(int n, const rule &rule);
std::map<std::string, int> deduce_field_positions(const std::vector<ticket> &tickets, const std::vector<rule> &rules);
void push_possible_matches(int pos,
                           const std::vector<ticket> &tickets,
                           const std::vector<rule> &rules,
                           const std::set<int> &already_matched_rules,
                           std::vector<int> &matches);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<rule> rules;
    ticket my_ticket;
    std::vector<ticket> nearby_tickets;
    parse_input(filename, rules, my_ticket, nearby_tickets);

    nearby_tickets.erase(std::remove_if(nearby_tickets.begin(),
                                        nearby_tickets.end(),
                                        [&rules] (const ticket &ticket) { return !ticket_is_valid(ticket, rules); }),
                         nearby_tickets.end());

    std::map<std::string, int> field_positions = deduce_field_positions(nearby_tickets, rules);

    uint64_t product = 1;
    for (const auto &[field_name, pos] : field_positions) {
        if (field_name.starts_with("departure")) product *= my_ticket[pos];
    }

    std::cout << "Product: " << product << "\n";
}

void parse_input(const std::string &filename,
                 std::vector<rule> &rules,
                 ticket &my_ticket,
                 std::vector<ticket> &nearby_tickets) {
    std::ifstream input_file(filename);

    std::string line;
    for (;;) {
        std::getline(input_file, line);
        if (line.empty()) break;

        rules.push_back(parse_rule_line(line));
    }

    expect_line(input_file, "your ticket:");
    std::getline(input_file, line);
    my_ticket = parse_ticket_line(line);

    expect_line(input_file, "");
    expect_line(input_file, "nearby tickets:");
    while (std::getline(input_file, line)) {
        nearby_tickets.push_back(parse_ticket_line(line));
    }
}

rule parse_rule_line(const std::string &line) {
    rule rule;

    size_t sep_pos = line.find(':');
    rule.field_name = line.substr(0, sep_pos);

    std::istringstream iss(line.substr(sep_pos + 1));
    iss >> rule.range1.first >> skip("-") >> rule.range1.second >> expect(" or ")
        >> rule.range2.first >> skip("-") >> rule.range2.second;

    return rule;
}

ticket parse_ticket_line(const std::string &line) {
    ticket ticket;

    std::istringstream iss(line);
    int n;
    while (iss >> skip(",") >> n) {
        ticket.push_back(n);
    }

    return ticket;
}

bool ticket_is_valid(const ticket &ticket, const std::vector<rule> &rules) {
    for (int n : ticket) {
        bool could_be_valid = false;
        for (const rule &rule : rules) {
            if (conforms_to_rule(n, rule)) {
                could_be_valid = true;
                break;
            }
        }

        if (!could_be_valid) return false;
    }

    return true;
}

bool conforms_to_rule(int n, const rule &rule) {
    return (rule.range1.first <= n && n <= rule.range1.second) ||
           (rule.range2.first <= n && n <= rule.range2.second);
}

std::map<std::string, int> deduce_field_positions(const std::vector<ticket> &tickets, const std::vector<rule> &rules) {
    std::vector<std::vector<int>> possible_matches;
    std::map<std::string, int> deduced_field_positions;
    std::set<int> matched_rules;
    std::set<int> matched_positions;
    int n_fields = tickets.front().size();

    std::cout << "Deducing field positions\n";

    while (matched_rules.size() < rules.size()) {
        possible_matches.assign(n_fields, std::vector<int>());

        for (int pos = 0; pos < n_fields; ++pos) {
            if (matched_positions.contains(pos)) continue;

            push_possible_matches(pos, tickets, rules, matched_rules, possible_matches[pos]);
        }

        for (size_t pos = 0; pos < possible_matches.size(); ++pos) {
            if (matched_positions.contains(pos)) continue;

            if (possible_matches[pos].empty()) throw "No matches for position";

            if (possible_matches[pos].size() == 1) {
                int i_matched_rule = possible_matches[pos][0];

                matched_rules.insert(i_matched_rule);
                matched_positions.insert(pos);

                deduced_field_positions[rules[i_matched_rule].field_name] = pos;
            }
        }

        std::cout << "  Matched: " << matched_rules.size() << " / " << rules.size() << "\n";
    }

    return deduced_field_positions;
}

void push_possible_matches(int pos,
                           const std::vector<ticket> &tickets,
                           const std::vector<rule> &rules,
                           const std::set<int> &already_matched_rules,
                           std::vector<int> &matches) {
    for (size_t i_rule = 0; i_rule < rules.size(); ++i_rule) {
        if (already_matched_rules.contains(i_rule)) continue;

        bool all_values_conform_to_rule = true;

        for (const ticket &ticket : tickets) {
            if (!conforms_to_rule(ticket[pos], rules[i_rule])) {
                all_values_conform_to_rule = false;
                break;
            }
        }

        if (all_values_conform_to_rule) matches.push_back(i_rule);
    }
}
