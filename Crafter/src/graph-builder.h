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

struct craft_count {
	size_t count = 0;
	size_t needed = 0;
	bool ready = false;
	size_t distance = 0;
	std::vector<size_t> distribution;
};

using recipe_graph = graph::Graph<std::string, int>;
using craft_store = std::unordered_map<std::string, craft_count>;
using requests = std::vector<Ingredients>;

struct CraftingGraph {
	CraftingGraph (const requests& requests, const recipe_store& recipes);

	recipe_graph recipe_graph;
	craft_store recipe_counts;
private:
	void build_graph(void);
	void tally_count(void);
	void get_order (void);

	bool check_ingredient(const std::string& ingredient);
	bool check_parent(const std::string& parent);

	friend std::ostream& operator<< (std::ostream& os, const CraftingGraph&);
	friend std::ostream& output_recipe (std::ostream& os, const std::string& name);
};

requests get_requests (const recipe_store& recipes, const std::string& input_file);
requests get_requests_from_input (const recipe_store& recipes);
recipe_store read_templates(std::string template_location);
bool valid_extension(std::string);

}
