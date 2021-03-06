#include <istream>
#include <string>

struct skip {
    skip(const char *skipped_characters);
    friend std::istream & operator >> (std::istream &is, const skip &manip);

    const char *skipped_characters;
};

struct expect {
    expect(const std::string &expected_string);
    friend std::istream & operator >> (std::istream &is, const expect &manip);

    std::string expected_str;
};

void expect_line(std::istream &is, const std::string &expected_line);

bool is_digit(char c);
bool is_space(char c);
