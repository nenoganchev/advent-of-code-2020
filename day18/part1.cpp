#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include "string_utils.h"


struct expression {
    enum type { leaf, compound };
    enum operation { add, multiply };

    expression(type type);
    ~expression();

    int64_t evaluate() const;

    type type;
    int64_t value;
    operation op;
    expression * p_left_operand;
    expression * p_right_operand;
};

struct token {
    enum type { number, op_add, op_mult, paren_left, paren_right };

    type type;
    int value;
};


std::vector<expression *> g_expressions;


void parse_input(const std::string &filename);
std::vector<token> tokenize(const std::string &expr_str);
expression * build_expression(std::vector<token> &tokens);
expression * consume_maximal_expression(std::vector<token> &tokens);
expression * consume_minimal_expression(std::vector<token> &tokens);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: <exe> <filename>\n";
        return 1;
    }
    const std::string filename = argv[1];

    parse_input(filename);

    int64_t sum = 0;
    for (const expression *p_expr : g_expressions) {
        int64_t value = p_expr->evaluate();
        sum += value;
    }

    std::cout << "Expressions sum: " << sum << "\n";

    for (const expression *p_expr : g_expressions) delete p_expr;
    g_expressions.clear();
}

void parse_input(const std::string &filename) {
    std::ifstream input_file(filename);

    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<token> tokens = tokenize(line);
        g_expressions.push_back(build_expression(tokens));
    }
}

std::vector<token> tokenize(const std::string &expr_str) {
    std::vector<token> tokens;
    std::map<char, enum token::type> char_to_token_map = {
        {'+', token::op_add},
        {'*', token::op_mult},
        {'(', token::paren_left},
        {')', token::paren_right},
    };

    std::istringstream iss(expr_str);
    char next_char;
    while ((next_char = iss.get()) != EOF) {
        if (is_space(next_char)) continue;

        token token;

        if (is_digit(next_char)) {
            token.type = token::number;
            iss.unget() >> token.value;
        }
        else if (char_to_token_map.contains(next_char)) {
            token.type = char_to_token_map[next_char];
        }
        else {
            throw std::runtime_error("tokenize error");
        }

        tokens.push_back(token);
    }

    return tokens;
}

expression * build_expression(std::vector<token> &tokens) {
    expression * p_expr = consume_maximal_expression(tokens);

    if (!tokens.empty()) throw std::runtime_error("build_expression error");

    return p_expr;
}

expression * consume_maximal_expression(std::vector<token> &tokens) {
    if (tokens.empty()) throw std::runtime_error("consume_maximal_expression error");

    expression *p_expr = consume_minimal_expression(tokens);
    if (tokens.empty()) return p_expr;

    token next_token = tokens.back();

    if (next_token.type == token::paren_left) {
        return p_expr;
    }
    else if (next_token.type == token::op_add || next_token.type == token::op_mult) {
        tokens.pop_back();

        expression *p_compound_expr = new expression(expression::compound);

        p_compound_expr->op = next_token.type == token::op_add ? expression::add : expression::multiply;
        p_compound_expr->p_right_operand = p_expr;
        p_compound_expr->p_left_operand = consume_maximal_expression(tokens);

        return p_compound_expr;
    }
    else {
        throw std::runtime_error("consume_maximal_expression error");
    }
}

expression * consume_minimal_expression(std::vector<token> &tokens) {
    if (tokens.empty()) throw std::runtime_error("consume_minimal_expression error");

    token next_token = tokens.back();
    tokens.pop_back();

    if (next_token.type == token::number) {
        expression *p_leaf_expr = new expression(expression::leaf);

        p_leaf_expr->value = next_token.value;

        return p_leaf_expr;
    }
    else if (next_token.type == token::paren_right) {
        expression *p_sub_expr = consume_maximal_expression(tokens);

        if (tokens.empty() || tokens.back().type != token::paren_left) {
            throw std::runtime_error("consume_minimal_expression error");
        }
        tokens.pop_back();

        return p_sub_expr;
    }
    else {
        throw std::runtime_error("consume_minimal_expression error");
    }
}


expression::expression(enum type type): type(type) {}

expression::~expression() {
    if (this->type == compound) {
        delete this->p_left_operand;
        delete this->p_right_operand;
    }
}

int64_t expression::evaluate() const {
    if (this->type == leaf) {
        return this->value;
    }
    else if (this->op == add) {
        return this->p_left_operand->evaluate() + this->p_right_operand->evaluate();
    }
    else {
        return this->p_left_operand->evaluate() * this->p_right_operand->evaluate();
    }
}
