#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <cassert>


struct rule {
    std::string container;
    std::vector<std::string> contents;
};

struct node {
    std::string name;
    std::vector<size_t> parents;

    node(const std::string &name): name(name) {}
};


std::map<std::string, size_t> g_node_name_to_index;
std::vector<node> g_nodes;


std::vector<rule> parse_input(const std::string &filename);
std::vector<std::string> split(const std::string &string, const std::string &separator);
void construct_graph(const std::vector<rule> &rules);
size_t index_for_node(const std::string &node_name);
std::vector<size_t> find_ancestors_of_node(const std::string &node_name);
void find_ancestors_recursive(size_t curr_node_index, std::set<size_t> &accumulated_ancestors);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error: input filename required\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<rule> rules = parse_input(filename);
    construct_graph(rules);
    std::vector<size_t> ancestors = find_ancestors_of_node("shiny gold");

    std::cout << "Number of 'shiny gold' ancestors: " << ancestors.size() << "\n";
}

std::vector<rule> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<rule> rules;

    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<std::string> split_line = split(line, " bags contain ");
        assert(split_line.size() > 1);

        std::vector<std::string> split_contents = split(split_line[1], ", ");
        if (split_contents.size() == 1 && split_contents[0] == "no other bags.") {
            split_contents.clear();
        }
        for (std::string &content : split_contents) {
            size_t i_first_space = content.find(' ');
            size_t i_last_space = content.rfind(' ');
            assert(i_first_space != std::string::npos && i_last_space != std::string::npos && i_first_space != i_last_space);

            content = content.substr(i_first_space + 1, i_last_space - i_first_space - 1);
        }

        rule curr_rule;
        curr_rule.container = split_line[0];
        curr_rule.contents = split_contents;

        rules.push_back(curr_rule);
    }

    return rules;
}

std::vector<std::string> split(const std::string &string, const std::string &separator) {
    std::vector<std::string> result;

    size_t i_begin = 0;
    size_t i_sep;
    do
    {
        i_sep = string.find(separator, i_begin);
        result.push_back(string.substr(i_begin, i_sep - i_begin));
        i_begin = i_sep + separator.size();
    }
    while (i_sep != std::string::npos);

    return result;
}

void construct_graph(const std::vector<rule> &rules) {
    for (const rule &rule : rules) {
        size_t container_index = index_for_node(rule.container);

        for (const std::string &content : rule.contents) {
            size_t content_index = index_for_node(content);

            g_nodes[content_index].parents.push_back(container_index);
        }
    }
}

size_t index_for_node(const std::string &node_name) {
    auto node_mapping = g_node_name_to_index.find(node_name);
    if (node_mapping != g_node_name_to_index.end()) {
        return node_mapping->second;
    }
    else {
        size_t new_index = g_nodes.size();
        g_node_name_to_index[node_name] = new_index;
        g_nodes.push_back(node(node_name));

        return new_index;
    }
}

std::vector<size_t> find_ancestors_of_node(const std::string &node_name) {
    size_t node_index = index_for_node(node_name);
    std::set<size_t> ancestors;

    for (size_t curr_parent_index : g_nodes[node_index].parents) {
        find_ancestors_recursive(curr_parent_index, ancestors);
    }

    return std::vector<size_t>(ancestors.begin(), ancestors.end());
}

void find_ancestors_recursive(size_t curr_node_index, std::set<size_t> &accumulated_ancestors) {
    accumulated_ancestors.insert(curr_node_index);

    for (size_t curr_parent_index : g_nodes[curr_node_index].parents) {
        if (accumulated_ancestors.find(curr_parent_index) != accumulated_ancestors.end()) continue;

        find_ancestors_recursive(curr_parent_index, accumulated_ancestors);
    }
}
