#pragma once

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

namespace crafter {

const auto data_location = "data/recipes/";
const auto default_recipe = 1;

struct craft_count {
	craft_count(const std::vector<Recipe>& recipes);
	craft_count() = default;
	size_t needed = 0;
	bool ready = false;
	size_t distance = 0;
	size_t base_needed = 0;
	std::vector<size_t> distribution = {0,};
	std::vector<size_t> makes{1,};
};

using recipe_graph = graph::Graph<std::string, int>;
using craft_store = std::unordered_map<std::string, craft_count>;
using requests = std::vector<Ingredients>;
using depend_graph = std::unordered_map<std::string, std::vector<std::string>>;
using ingredient_map = std::unordered_map<std::string, int>;
using recipe_order = std::vector<std::vector<std::string>>;

class CraftingGraph {
public:
	CraftingGraph (
		const requests& requests,
		const recipe_store& recipes,
		const depend_graph& dependencies
	);

	recipe_graph graph;
	craft_store recipe_count;
	recipe_order order;
	std::vector<std::string> raw_ingredients;
	const recipe_store recipes;
	const depend_graph dependencies;
private:
	void build_graph(const std::vector<Ingredients>&);
	void tally_count(const std::vector<Ingredients>&);
	void get_order (void);

	bool check_ingredient(const std::string& ingredient);
	static void make_distribution(craft_count& count);
	void link_ingredient(const std::string& ingredient);
	ingredient_map calc_ingredients(const std::string& ingredient) const;

	friend std::ostream& operator<< (std::ostream& os, const CraftingGraph&);
	friend std::ostream& output_recipe (std::ostream& os, const CraftingGraph&, const std::string& name);
};

requests get_requests (const recipe_store& recipes, const std::string& input_file);
requests get_requests_from_input (const recipe_store& recipes);
recipe_store read_templates(std::string template_location = data_location);
bool valid_extension(std::string);
depend_graph build_depend_graph (const recipe_store& recipes);

template <typename N, typename E>
std::vector<N> heads(graph::Graph<N, E> g);

template <typename N, typename E>
std::vector<N> tails(graph::Graph<N, E> g);

}
