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
    passport::const_iterator entry;
    int as_i;
    size_t pos;

    entry = passport.find("byr");
    if (entry == passport.end()) return false;
    const std::string &byr = entry->second;
    if (byr.size() != 4) return false;
    as_i = std::stoi(byr, &pos);
    if (pos != 4) return false;
    if (!(1920 <= as_i && as_i <= 2002)) return false;

    entry = passport.find("iyr");
    if (entry == passport.end()) return false;
    const std::string &iyr = entry->second;
    if (iyr.size() != 4) return false;
    as_i = std::stoi(iyr, &pos);
    if (pos != 4) return false;
    if (!(2010 <= as_i && as_i <= 2020)) return false;

    entry = passport.find("eyr");
    if (entry == passport.end()) return false;
    const std::string &eyr = entry->second;
    if (eyr.size() != 4) return false;
    as_i = std::stoi(eyr, &pos);
    if (pos != 4) return false;
    if (!(2020 <= as_i && as_i <= 2030)) return false;

    entry = passport.find("hgt");
    if (entry == passport.end()) return false;
    const std::string &hgt = entry->second;
    as_i = std::stoi(hgt, &pos);
    std::string unit = hgt.substr(pos);
    if (unit == "cm") {
        if (!(150 <= as_i && as_i <= 193)) return false;
    }
    else if (unit == "in") {
        if (!(59 <= as_i && as_i <= 76)) return false;
    }
    else {
        return false;
    }

    entry = passport.find("hcl");
    if (entry == passport.end()) return false;
    const std::string &hcl = entry->second;
    if (hcl.size() != 7) return false;
    if (hcl[0] != '#') return false;
    if (hcl.find_first_not_of("0123456789abcdef", 1) != hcl.npos) return false;

    entry = passport.find("ecl");
    if (entry == passport.end()) return false;
    const std::string &ecl = entry->second;
    std::vector<std::string> valid_colours = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    if (std::find(valid_colours.begin(), valid_colours.end(), ecl) == valid_colours.end()) return false;

    entry = passport.find("pid");
    if (entry == passport.end()) return false;
    const std::string &pid = entry->second;
    if (pid.size() != 9) return false;
    if (pid.find_first_not_of("0123456789") != pid.npos) return false;

    return true;
}
