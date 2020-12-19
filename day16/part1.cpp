#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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
int calculate_scanning_error_rate(const std::vector<rule> &rules, const std::vector<ticket> &tickets);
bool contains(const std::pair<int, int> &range, int n);


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

    std::cout << "Error rate: " << calculate_scanning_error_rate(rules, nearby_tickets) << "\n";
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

int calculate_scanning_error_rate(const std::vector<rule> &rules, const std::vector<ticket> &tickets) {
    int error_rate = 0;

    for (const ticket &ticket : tickets) {
        for (int n : ticket) {
            bool is_valid = false;
            for (const rule &rule : rules) {
                if (contains(rule.range1, n) || contains(rule.range2, n)) {
                    is_valid = true;
                    break;
                }
            }

            if (!is_valid) error_rate += n;
        }
    }

    return error_rate;
}

bool contains(const std::pair<int, int> &range, int n) {
    return range.first <= n && n <= range.second;
}
