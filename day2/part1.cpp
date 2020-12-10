#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void parse_line(const std::string &line, int &min, int &max, char &required, std::string &password);
bool is_valid(const std::string &password, char required, int min, int max);


int main(int argc, char *argv[]) {
    std::ifstream input_file("input.txt");
    std::string line;
    int valid_passwords = 0;
    while (std::getline(input_file, line)) {
        int min, max;
        char required;
        std::string password;
        parse_line(line, min, max, required, password);
        if (is_valid(password, required, min, max)) {
            valid_passwords += 1;
        }
    }

    std::cout << "Valid passwords: " << valid_passwords << std::endl;
}

void parse_line(const std::string &line, int &min, int &max, char &required, std::string &password) {
    std::istringstream iss(line);

    iss >> min;
    if (iss.get() != '-') throw "ParseError after min";
    iss >> max;
    iss >> required;
    if (iss.get() != ':') throw "ParseError after required";
    iss >> password;
}

bool is_valid(const std::string &password, char required, int min, int max) {
    int occurrences = 0;
    for (char c : password) {
        if (c == required) {
            occurrences += 1;
        }
    }

    return min <= occurrences && occurrences <= max;
}
