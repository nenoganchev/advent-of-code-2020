#include "string_utils.h"

//
// skip implementation
//

skip::skip(const char *s): skipped_characters(s) {}

std::istream & operator >> (std::istream &is, const skip &manip) {
    while (is && strchr(manip.skipped_characters, is.peek())) is.ignore();
    return is;
}

//
// expect implementation
//

expect::expect(const std::string &str): expected_str(str) {}

std::istream & operator >> (std::istream &is, const expect &manip) {
    for (char expected_char : manip.expected_str) {
        char read_char;
        if (!(is.get(read_char) && read_char == expected_char)) {
            throw "UnexpectedArg";
        }
    }

    return is;
}

//
// functions implementation
//

void expect_line(std::istream &is, const std::string &expected_line) {
    std::string line;
    std::getline(is, line);
    if (line != expected_line) throw "Line expectation failed";
}
