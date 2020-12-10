#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


void parse_line(const std::string &line, int &min, int &max, char &required, std::string &password);
bool is_valid(const std::string &password, char required, int pos1, int pos2);


int main(int argc, char *argv[]) {
    std::ifstream input_file("input.txt");
    std::string line;
    int valid_passwords = 0;
    while (std::getline(input_file, line)) {
        int pos1, pos2;
        char required;
        std::string password;
        parse_line(line, pos1, pos2, required, password);
        if (is_valid(password, required, pos1, pos2)) {
            valid_passwords += 1;
        }
    }

    std::cout << "Valid passwords: " << valid_passwords << std::endl;
}

void parse_line(const std::string &line, int &pos1, int &pos2, char &required, std::string &password) {
    std::istringstream iss(line);

    iss >> pos1;
    if (iss.get() != '-') throw "ParseError after min";
    iss >> pos2;
    iss >> required;
    if (iss.get() != ':') throw "ParseError after required";
    iss >> password;
}

bool is_valid(const std::string &password, char required, int pos1, int pos2) {
    int pos1_occur = password[pos1 - 1] == required ? 1 : 0;
    int pos2_occur = password[pos2 - 1] == required ? 1 : 0;

    return pos1_occur + pos2_occur == 1;
}
