#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <math.h>
#include <algorithm>

#include "import.h"
#include "graph.h"
#include "graph-builder.h"

std::string read_args(int argc, char const *argv[]);


template <typename N, typename E>
std::vector<N> heads(graph::Graph<N, E>);

template <typename N, typename E>
std::vector<N> tails(graph::Graph<N, E>);


int main(int argc, char const *argv[]) {
	auto input = read_args(argc, argv);

	const auto recipes = crafter::read_templates(crafter::data_location);
	const auto dependencies = crafter::build_depend_graph(recipes);

	if (input == "") {
		std::cout << "Loaded " << recipes.size() << " recipes\n";
	}

	auto requests = crafter::get_requests(recipes, input);

	if (requests.size() == 0) {
		std::cout << "No input given\n";
		return 0;
	}

	auto graph = crafter::CraftingGraph(requests, recipes, dependencies);
	
	std::cout << graph;
	return 0;
}

std::string read_args (int argc, char const *argv[]) {
	if (argc == 1) {
		return "";
	} else if (argc == 2) {
		return argv[1];
	} else {
		std::cerr << "Got " << argc << " arguements, expected 0 or 1\n";
		throw std::invalid_argument(argv[2]);
	}
}
