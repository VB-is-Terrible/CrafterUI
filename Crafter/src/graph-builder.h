#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <math.h>
#include <algorithm>
#include "yaml-cpp/yaml.h"

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
using pairings = std::vector<std::pair<std::string, std::string>>;
using distribution_map = std::unordered_map<std::string, std::vector<size_t>>;

struct CraftingGraphState {
	crafter::requests requests;
	distribution_map distributions;
};

class CraftingGraph {
public:
	CraftingGraph (
		const requests& requests,
		const recipe_store& recipes,
		const depend_graph& dependencies
	);

	CraftingGraph(
		const CraftingGraphState& state,
		const recipe_store& recipes,
		const depend_graph& dependencies
	);

	recipe_graph graph;
	craft_store recipe_count;
	recipe_order order;
	std::vector<std::string> raw_ingredients;
	const recipe_store recipes;
	const depend_graph dependencies;

	pairings make_pairings(void) const;
	ingredient_map calc_ingredients(const std::string& ingredient) const;
	std::vector<ingredient_map> calc_ingredients(const std::string& name, const craft_count& count) const;
	void update(const std::vector<std::string>&);
	std::unordered_set<std::string> get_children(const std::string &);
	std::unordered_set<std::string> get_parents(const std::string &);
	CraftingGraphState getState();
private:
	void build_graph(const std::vector<Ingredients>&);
	void build_graph(const std::vector<std::string>&);
	void build_graph(std::deque<std::string>&, std::unordered_set<std::string>&);
	void build_graph_node(
		std::deque<std::string>&, std::unordered_set<std::string>&,
		const std::string&, const Recipe&);
	void pre_link(const std::vector<std::string>&);
	void tally_count(const std::vector<Ingredients>&);
	void tally_count(const std::vector<std::string>&);
	void tally_count(std::deque<std::string>&);
	void get_order (void);
	void handle_dangling_nodes(void);

	bool check_ingredient(const std::string& ingredient);
	static void make_distribution(craft_count& count);
	void link_ingredient(const std::string& ingredient);
	/**
	 * Tag all items needed as not ready
	 * @param requests The items to begin marking from
	 */
	void mark(const std::vector<Ingredients>&);
	void mark(const std::vector<std::string>&);
	void mark(std::deque<std::string>&);
	void setDistribution(const distribution_map&);
	friend std::ostream& operator<< (std::ostream& os, const CraftingGraph&);
	friend std::ostream& output_recipe (std::ostream& os, const CraftingGraph&, const std::string& name);
};

requests get_requests (const recipe_store& recipes, const std::string& input_file);
requests get_requests_from_input (const recipe_store& recipes);
recipe_store read_templates(std::string template_location = data_location);
bool valid_extension(std::string);
depend_graph build_depend_graph (const recipe_store& recipes);

}

namespace YAML {
template <>
struct convert<crafter::Ingredients> {
	static Node encode(const crafter::Ingredients& ingredient) noexcept {
		Node result;
		result["name"] = ingredient.name;
		result["count"] = ingredient.count;
		return result;
	}
	static bool decode(const Node& node, crafter::Ingredients& ingredient) {
		if(!node.IsMap() || node.size() != 2) {
			return false;
		}
		ingredient.count = node["count"].as<int>();
		ingredient.name = node["name"].as<std::string>();
		return true;
	}
};

template <>
struct convert<crafter::distribution_map> {
	static Node encode(const crafter::distribution_map& distribution) {
		Node node;
		for (const auto& [name, value] : distribution) {
			node[name] = value;
		}
		return node;
	}
	static bool decode(const Node& node, crafter::distribution_map& distribution) {
		if (!node.IsMap()) {
			return false;
		}
		for (const auto& it : node) {
			const auto name = it.first.as<std::string>();
			const auto one_distribution = it.second.as<std::vector<size_t>>();
			distribution[name] = one_distribution;
		}
		return true;
	}
};

template <>
struct convert<crafter::CraftingGraphState> {
	static Node encode(const crafter::CraftingGraphState& state) {
		Node result;
		result["requests"] = state.requests;
		result["distributions"] = state.distributions;
		return result;
	}
	static bool decode (const Node& node, crafter::CraftingGraphState& state) {
		if(!node.IsMap() || node.size() != 2) {
			return false;
		}
		state.requests = node["requests"].as<crafter::requests>();
		state.distributions = node["distributions"].as<crafter::distribution_map>();
		return true;
	}
};

}
