#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <cassert>


struct rule {
    struct content {
        std::string name;
        int count;
    };

    std::string container;
    std::vector<content> contents;
};

struct node {
    struct child {
        size_t index;
        int count;
    };

    std::string name;
    std::vector<child> children;

    node(const std::string &name): name(name) {}
};


std::map<std::string, size_t> g_node_name_to_index;
std::vector<node> g_nodes;


std::vector<rule> parse_input(const std::string &filename);
std::vector<std::string> split(const std::string &string, const std::string &separator);
void construct_graph(const std::vector<rule> &rules);
size_t index_for_node(const std::string &node_name);
int count_descendants_of_node(const std::string &node_name);
int count_descendants_recursive(size_t node_index, std::map<size_t, int> &memoized_counts);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error: input filename required\n";
        return 1;
    }
    const std::string filename = argv[1];

    std::vector<rule> rules = parse_input(filename);
    construct_graph(rules);
    int descendants_count = count_descendants_of_node("shiny gold");

    std::cout << "Number of 'shiny gold' descendants: " << descendants_count << "\n";
}

std::vector<rule> parse_input(const std::string &filename) {
    std::ifstream input_file(filename);
    std::vector<rule> rules;

    std::string line;
    while (std::getline(input_file, line)) {
        rule curr_rule;

        std::vector<std::string> split_line = split(line, " bags contain ");
        assert(split_line.size() > 1);
        curr_rule.container = split_line[0];

        std::vector<std::string> split_contents = split(split_line[1], ", ");
        if (split_contents.size() == 1 && split_contents[0] == "no other bags.") {
            split_contents.clear();
        }
        for (const std::string &content : split_contents) {
            size_t i_first_space = content.find(' ');
            size_t i_last_space = content.rfind(' ');
            assert(i_first_space != std::string::npos && i_last_space != std::string::npos && i_first_space != i_last_space);

            rule::content curr_content;
            curr_content.name = content.substr(i_first_space + 1, i_last_space - i_first_space - 1);
            curr_content.count = std::stoi(content);

            curr_rule.contents.push_back(curr_content);
        }

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

        for (const rule::content &content : rule.contents) {
            size_t content_index = index_for_node(content.name);

            node::child child = {content_index, content.count};
            g_nodes[container_index].children.push_back(child);
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

int count_descendants_of_node(const std::string &node_name) {
    size_t node_index = index_for_node(node_name);

    int descendants_count = 0;
    std::map<size_t, int> memoized_counts;

    for (const node::child &curr_child : g_nodes[node_index].children) {
        descendants_count += curr_child.count * count_descendants_recursive(curr_child.index, memoized_counts);
    }

    return descendants_count;
}

int count_descendants_recursive(size_t node_index, std::map<size_t, int> &memoized_counts) {
    auto memoized_mapping = memoized_counts.find(node_index);
    if (memoized_mapping != memoized_counts.end()) {
        return memoized_mapping->second;
    }

    int descendants_count = 1;

    for (const node::child &curr_child : g_nodes[node_index].children) {
        descendants_count += curr_child.count * count_descendants_recursive(curr_child.index, memoized_counts);
    }

    memoized_counts[node_index] = descendants_count;
    return descendants_count;
}
