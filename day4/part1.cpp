#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>


typedef std::map<std::string, std::string> passport;


std::vector<passport> parse_input();
passport parse_passport_lines(const std::vector<std::string> &lines);
bool is_valid(const passport &passport);


int main(int argc, char *argv[]) {
    std::vector<passport> passports = parse_input();

    int valid_passports = 0;
    for (const passport & passport : passports) {
        if (is_valid(passport)) ++valid_passports;
    }

    std::cout << "Valid passports: " << valid_passports << std::endl;
}

std::vector<passport> parse_input() {
    std::ifstream input_file("input.txt");
    std::vector<passport> passports;

    while (input_file) {
        std::vector<std::string> passport_lines;
        std::string line;
        while (std::getline(input_file, line)) {
            if (line.empty()) break;
            passport_lines.push_back(line);
        }

        passports.push_back(parse_passport_lines(passport_lines));
    }

    return passports;
}

passport parse_passport_lines(const std::vector<std::string> &lines) {
    passport passport;

    for (const std::string & line : lines) {
        std::istringstream iss(line);

        std::string key_value;
        while (iss >> key_value) {
            size_t sep_pos = key_value.find(':');
            if (sep_pos == key_value.npos) throw "InvalidData: missing ':'";

            std::string key = key_value.substr(0, sep_pos);
            std::string value = key_value.substr(sep_pos + 1);
            passport[key] = value;
        }
    }

    return passport;
}

bool is_valid(const passport &passport) {
    std::vector<std::string> required_keys = {
        "byr",
        "iyr",
        "eyr",
        "hgt",
        "hcl",
        "ecl",
        "pid",
    };

    for (const std::string & required_key : required_keys) {
        if (passport.find(required_key) == passport.end()) return false;
    }

    return true;
}
