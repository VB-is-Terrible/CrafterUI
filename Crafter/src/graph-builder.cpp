#include "graph-builder.h"
#include <numeric>

#if __GNUC__ > 7
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#define old_fs
#endif

static constexpr int STRING_PAD = 30;
void add_depend_recipe(crafter::depend_graph& graph, const crafter::Recipe& recipe);
bool recipe_empty(const crafter::Recipe& recipe) {
	if (recipe.ingredients.size() == 0) {
		return true;
	} else {
		return false;
	}
}

namespace crafter {

template <typename N, typename E>
std::vector<N> heads(graph::Graph<N, E> g);

template <typename N, typename E>
std::vector<N> tails(graph::Graph<N, E> g);

craft_count::craft_count(const std::vector<Recipe>& recipes) {
	distribution.clear();
	makes.clear();
	auto first = true;
	for (const auto& recipe : recipes) {
		distribution.push_back(0);
		if (first) {
			first = false;
			makes.push_back(recipe.makes);
		} else {
			if (recipe_empty(recipe)) {
				makes.push_back(0);
			} else {
				makes.push_back(recipe.makes);
			}
		}
	}
}

CraftingGraph::CraftingGraph (
	const ::crafter::requests& requests,
	const recipe_store& recipes,
	const depend_graph& dependencies)
	: recipes{recipes},  dependencies{dependencies} {
	build_graph(requests);
	tally_count(requests);
	get_order();
}

CraftingGraph::CraftingGraph(
	const CraftingGraphState& state,
	const recipe_store& recipes,
	const depend_graph& dependencies) : recipes{recipes}, dependencies{dependencies} {
	build_graph(state.requests);
	setDistribution(state.distributions);
	tally_count(state.requests);
	get_order();

}

pairings CraftingGraph::make_pairings(void) const {
	pairings result;
	for (const auto& item : graph) {
		for (const auto& connected : graph.GetConnected(item)) {
			if (graph.GetWeight(item, connected) != 0
			    && recipe_count.at(connected).distribution.size() != 1) {
				result.push_back({item, connected});
			}
		}
	}
	return result;
}

void CraftingGraph::build_graph(const std::vector<Ingredients>& requests) {
	std::deque<std::string> queue;
	std::unordered_set<std::string> seen;
	for (const auto& request : requests) {
		queue.push_back(request.name);
		seen.insert(request.name);
	}
	build_graph(queue, seen);
}

void CraftingGraph::build_graph(const std::vector<std::string>& requests) {
	std::deque<std::string> queue;
	std::unordered_set<std::string> seen;
	for (const auto& name : requests) {
		queue.push_back(name);
		seen.insert(name);
	}
	build_graph(queue, seen);
}


void CraftingGraph::build_graph(std::deque<std::string>& queue, std::unordered_set<std::string> &seen) {
	while (!queue.empty()) {
		auto request = queue[0];
		queue.pop_front();
		graph.InsertNode(request);
		auto recipe_it = recipes.find(request);
		if (recipe_it != recipes.end()) {
			// Add links for all recipes
			std::vector<size_t> distribution;
			if (recipe_count.count(request)) {
				distribution = recipe_count.at(request).distribution;
			} else {
				distribution = {0, 1};
			}
			for (size_t i = 0; i < distribution.size(); i++) {
				if (distribution[i] == 0 && i != default_recipe) {
					continue;
				}
				build_graph_node(queue, seen, request, recipe_it->second[i]);
			}
		}
	}
}

void CraftingGraph::build_graph_node(std::deque<std::string>& queue, std::unordered_set<std::string>& seen, const std::string& request, const Recipe& recipe) {
	for (const auto& ingredient : recipe.ingredients) {
		graph.InsertNode(ingredient.name);
		if (!graph.IsConnected(request, ingredient.name)) {
			graph.InsertEdge(request, ingredient.name, 0);
		} else {
			graph.SetWeight(request, ingredient.name, 0);
		}
		if (!seen.count(ingredient.name)) {
			seen.insert(ingredient.name);
			queue.push_back(ingredient.name);
		}
	}
}

void CraftingGraph::tally_count(const std::vector<Ingredients>& requests) {
	std::deque<std::string> queue;

	mark(requests);

	for (const auto& node : requests) {
		auto needed = static_cast<size_t>(node.count);
		craft_count craft;
		if (recipe_count.count(node.name)) {
			craft = recipe_count.at(node.name);
		} else {
			craft = (recipes.at(node.name));
		}
		craft.base_needed = needed;
		recipe_count[node.name] = craft;
		queue.push_back(node.name);
	}

	tally_count(queue);
}

void CraftingGraph::tally_count(const std::vector<std::string>& requests) {
	std::deque<std::string> queue;
	mark(requests);

	for (const auto& name : requests) {
		queue.push_back(name);
	}

	tally_count(queue);
}

void CraftingGraph::tally_count(std::deque<std::string>& queue) {
	std::unordered_set<std::string> seen;
	while (!queue.empty()) {
		auto request = queue[0];
		queue.pop_front();
		if (seen.count(request)) {
			continue;
		}
		auto ready = check_ingredient(request);
		if (ready) {
			seen.insert(request);
			if (recipes.count(request)) {
				for (const auto& recipe : recipes.at(request)) {
					for (const auto& ingredient : recipe.ingredients) {
						if (recipe_count.count(ingredient.name)) {
							queue.push_back(ingredient.name);
						}
					}
				}
			}
		}
	}
}

void CraftingGraph::get_order (void) {
	order = recipe_order();
	std::unordered_set<std::string> raw;
	for (const auto& it : recipe_count) {
		auto& name = it.first;
		auto& craft = it.second;
		// Ignore totally empty items
		if (std::accumulate(craft.distribution.begin(), craft.distribution.end(), 0) == 0) {
			continue;
		}
		if (craft.distribution[0] > 0) {
			raw.insert(name);
		}
		if (!recipes.count(name)) {
			continue;
		}
		if (craft.distance >= order.size()) {
			order.resize(craft.distance + 1);
		}
		order[craft.distance].push_back(name);
	}
	for (auto& level : order) {
		std::sort(level.begin(), level.end());
	}
	raw_ingredients = std::vector(raw.begin(), raw.end());
	std::sort(raw_ingredients.begin(), raw_ingredients.end());
}

bool CraftingGraph::check_ingredient(const std::string& ingredient) {
	craft_count& count = recipe_count[ingredient];
	if (count.ready) {
		return true;
	}

	int parent_distance = -1;
	count.needed = count.base_needed;

	for (const auto& parent : dependencies.at(ingredient)) {
		if (!recipe_count.count(parent)) {
			continue;
		}
		if (!recipe_count[parent].ready) {
			return false;
		}
	}

	std::cout << "Doing " << ingredient << "\n";
	for (const auto& parent : graph.GetIncoming(ingredient)) {
		if (!recipe_count.count(parent)) {
			continue;
		}
		count.needed += graph.GetWeight(parent, ingredient);
		parent_distance = std::max(parent_distance, (int) recipe_count[parent].distance);
	}

	count.distance = parent_distance + 1;
	make_distribution(count);
	count.ready = true;
	link_ingredient(ingredient);

	return true;
}

void CraftingGraph::make_distribution(craft_count &count)
{
	if (count.makes.size() == 1) {
		// Has no recipes
		count.distribution[0] = count.needed;
	} else {
		// Has at least one recipe
		size_t has = 0;
		for (size_t i = 0; i < count.makes.size(); i++) {
			if (i == default_recipe) {
				continue;
			}
			has += count.distribution[i] * count.makes[i];
		}
		if (count.makes[default_recipe] == 0) {
			has -= count.distribution[0];
			has = std::min(has, count.needed);
			count.distribution[0] = count.needed - has;
		} else {
			has = std::min(has, count.needed);
			auto needed = count.needed - has;
			count.distribution[default_recipe] = ceil(needed / (double) count.makes[default_recipe]);
		}
	}
}

void CraftingGraph::link_ingredient(const std::string& ingredient) {
	auto map = calc_ingredients(ingredient);
	for (const auto& dest : graph.GetConnected(ingredient)) {
		graph.SetWeight(ingredient, dest, 0);
	}
	for (const auto& pair : map) {
		graph.SetWeight(ingredient, pair.first, pair.second);
	}
}

ingredient_map CraftingGraph::calc_ingredients(const std::string& item) const {
	ingredient_map result;
	const craft_count& count = recipe_count.at(item);
	if (count.distribution.size() <= default_recipe) {
		return result;
	}
	auto recipe_it = recipes.find(item);
	// Recipe 0 is a special self loop, and meant to represent a precrafted item
	// Hence, ignore it for internal bookkeeping
	for (size_t i = 1; i < count.distribution.size(); i++) {
		if (count.distribution[i] == 0) {
			continue;
		}
		const auto& recipe = recipe_it->second[i];
		for (const auto& ingredient : recipe.ingredients) {
			if (!result.count(ingredient.name)) {
				result[ingredient.name] = 0;
			}
			result[ingredient.name] += ingredient.count * count.distribution[i];
		}
	}
	return result;
}

std::vector<ingredient_map> CraftingGraph::calc_ingredients(const std::string& name, const craft_count& count) const {
	std::vector<ingredient_map> result;
	auto recipe_it = recipes.find(name);
	for (size_t i = 0; i < count.distribution.size(); i++) {
		ingredient_map current;
		if (count.distribution[i] == 0) {
			result.push_back(current);
			continue;
		}
		const auto& recipe = recipe_it->second[i];
		for (const auto& ingredient : recipe.ingredients) {
			if (!current.count(ingredient.name)) {
				current[ingredient.name] = 0;
			}
			current[ingredient.name] += ingredient.count * count.distribution[i];
		}
		result.push_back(current);
	}
	return result;
}


std::ostream& operator<< (std::ostream& os, const CraftingGraph& graph) {
	const std::string line = "---------------";
	os << line << " Raw Materials " << line << "\n";
	for (const auto& ingredient : graph.raw_ingredients) {
		os << ingredient;
		const auto pad_length = std::max(STRING_PAD - (int) ingredient.size(), 1);
		os << std::string(pad_length, ' ');
		os << graph.recipe_count.at(ingredient).distribution[0] << "\n";
	}
	os << "\n";

	size_t level_count = 0;
	for (auto level = graph.order.crbegin(); level != graph.order.crend(); level++) {
		level_count++;
		os << line << " " << "Level " << level_count << " " << line << "\n\n";
		for (const auto& name : *level) {
			output_recipe(os, graph, name);
			os << "\n";
		}
	}
	return os;
}

std::ostream& output_recipe (std::ostream& os, const CraftingGraph& graph, const std::string& name) {
	const auto count = graph.recipe_count.at(name);
	if (!graph.recipes.count(name)) {
		os << name;
		const auto pad_length = std::max(STRING_PAD - (int) name.size(), 1);
		os << std::string(pad_length, ' ');
		return os;
	}
	const auto& recipes = graph.recipes.at(name);
	for (size_t i = 0; i < recipes.size(); i++) {
		if (count.distribution[i] == 0) {
			continue;
		}
		os << name << " (" << count.distribution[i] << ")\n";
		const auto& recipe = recipes[i];
		for (const auto& ingredient : recipe.ingredients) {
			os << ingredient.count * count.distribution[i];
			os << "\t" << ingredient.name << "\n";
		}
	}
	if (!graph.graph.IsNode(name)) {
		os << "\n";
	}
	return os;
}

requests get_requests (const recipe_store& recipes, const std::string& input_file) {
	if (input_file == "") {
		return get_requests_from_input(recipes);
	} else {
		return crafter::get_requests_from_file(recipes, input_file);
	}
}

requests get_requests_from_input (const recipe_store& recipes) {
	std::cout << "Input Recipe: ";
	std::string in;
	getline(std::cin, in);

	std::vector<crafter::Ingredients> requests;
	while (in != "") {
		auto it = recipes.find(in);
		if (it == recipes.end()) {
			std::cerr << "Recipe '" << in << "' not found\n";
		} else {
			requests.push_back(crafter::Ingredients(in, 1));
		}
		std::cout << "Input Recipe: ";
		getline(std::cin, in);
	}
	std::cout << "\n";
	return requests;
}

#ifndef old_fs
crafter::recipe_store read_templates(std::string template_location) {
	crafter::recipe_store result;
	for (const auto& entry : fs::directory_iterator(template_location)) {
		if (entry.is_regular_file() && valid_extension(entry.path().extension())) {
			crafter::read_in(entry.path(), result, true);
		}
	}
	return result;
}
#else
crafter::recipe_store read_templates(std::string template_location) {
	crafter::recipe_store result;
	for (const auto& entry : fs::directory_iterator(template_location)) {
		if (fs::is_regular_file(entry) && valid_extension(entry.path().extension())) {
			crafter::read_in(entry.path(), result, true);
		}
	}
	return result;
}
#endif

bool valid_extension(std::string extension) {
	if (extension == ".yaml" || extension == ".yml") {
		return true;
	}
	return false;
}

depend_graph build_depend_graph (const recipe_store& recipe_store) {
	depend_graph result;
	for (const auto& [_, recipes] : recipe_store) {
		if (recipes.size() > 1) {
			add_depend_recipe(result, recipes.at(default_recipe));
		}
	}
	return result;
}

template <typename N, typename E>
std::vector<N> heads(graph::Graph<N, E> g) {
	std::vector<N> result;
	for (const auto& node : g) {
		if (g.GetIncoming(node).size() == 0) {
			result.push_back(node);
		}
	}
	return result;
}

template <typename N, typename E>
std::vector<N> tails(graph::Graph<N, E> g) {
	std::vector<N> result;
	for (const auto& node : g) {
		if (g.GetConnected(node).size() == 0) {
			result.push_back(node);
		}
	}
	return result;
}

void CraftingGraph::mark(const std::vector<Ingredients>& requests) {
	std::deque<std::string> queue;
	for (const auto& node : requests) {
		queue.push_back(node.name);
	}
	mark(queue);
}

void CraftingGraph::mark(const std::vector<std::string>& requests) {
	std::deque<std::string> queue;
	for (const auto& name : requests) {
		queue.push_back(name);
	}
	mark(queue);
}

void CraftingGraph::mark(std::deque<std::string>& queue) {
	std::unordered_set<std::string> seen;
	while (!queue.empty()) {
		auto item = queue[0];
		queue.pop_front();
		if (seen.count(item)) {
			continue;
		} else {
			seen.insert(item);
		}
		if (recipes.count(item) == 0) {
			recipe_count[item] = craft_count();
			continue;
		} else {
			if (recipe_count.count(item)) {
				recipe_count[item].ready = false;
			}	 else {
				recipe_count[item] = craft_count(recipes.at(item));
			}
		}
		for (size_t i = 0; i < recipe_count[item].distribution.size(); i++) {
			if (i == default_recipe || recipe_count[item].distribution[i] > 0) {
				for (const auto& ingredient : recipes.at(item)[i].ingredients) {
					queue.push_back(ingredient.name);
				}
			}
		}
	}
}

void CraftingGraph::update(const std::vector<std::string>& updated) {
	build_graph(updated);
	pre_link(updated);
	tally_count(updated);
	handle_dangling_nodes();
	get_order();
}

void CraftingGraph::handle_dangling_nodes(void) {
	std::vector<std::string> dangling;
	for (const auto& [name, count] : recipe_count) {
		size_t sum = 0;
		auto summer = [&sum, &graph = graph, &name = name](const std::string& parent) {
			sum += graph.GetWeight(parent, name);
		};
		const auto parents = graph.GetIncoming(name);
		std::for_each(parents.begin(), parents.end(), summer);
		if (sum == 0) {
			dangling.push_back(name);
		}
	}
	tally_count(dangling);
}

void CraftingGraph::pre_link(const std::vector<std::string>& updated) {
	for (const auto& updated_item : updated) {
		link_ingredient(updated_item);
	}
}

std::unordered_set<std::string> CraftingGraph::get_children(const std::string & root) {
	std::unordered_set<std::string> result;
	std::deque<std::string> queue;
	queue.push_back(root);

	while (!queue.empty()) {
		auto item = queue[0];
		queue.pop_front();
		if (result.count(item)) {
			continue;
		}
		result.insert(item);
		if (!graph.IsNode(item)) {
			continue;
		}
		for (const auto& connected : graph.GetConnected(item)) {
			if (graph.GetWeight(item, connected) != 0) {
				queue.push_back(connected);
			}
		}
	}
	return result;
}

std::unordered_set<std::string> CraftingGraph::get_parents(const std::string & root) {
	std::unordered_set<std::string> result;
	std::deque<std::string> queue;
	queue.push_back(root);

	while (!queue.empty()) {
		auto item = queue[0];
		queue.pop_front();
		if (result.count(item)) {
			continue;
		}
		result.insert(item);
		if (!graph.IsNode(item)) {
			continue;
		}
		for (const auto& connected : graph.GetIncoming(item)) {
			if (graph.GetWeight(connected, item) != 0) {
				queue.push_back(connected);
			}
		}
	}
	return result;
}

CraftingGraphState CraftingGraph::getState(void) {
	CraftingGraphState result;
	for (const auto& [recipe, count] : recipe_count) {
		if (count.base_needed) {
			result.requests.emplace_back(recipe, count.base_needed);
		}
		if (count.distribution.size() > (default_recipe)) {
			auto other_recipes = std::accumulate(
				count.distribution.cbegin() + default_recipe + 1,
				count.distribution.cend(),
				0
			);
			if (count.makes[default_recipe] != 0) {
				other_recipes += count.distribution[0];
			}
			if (other_recipes != 0) {
				result.distributions[recipe] = count.distribution;
			}
		}
	}

	return result;
}

void CraftingGraph::setDistribution(const distribution_map& dist) {
	for (const auto& [name, distribution] : dist) {
		if (!recipe_count.count(name)) {
			if (recipes.count(name)) {
				recipe_count[name] = craft_count(recipes.at(name));
			} else {
				recipe_count[name] = craft_count();
			}

		}
		craft_count& count = recipe_count.at(name);
		for (size_t i = 0; i < distribution.size(); i++) {
			count.distribution[i] = distribution[i];
		}
	}
}

}

void add_depend_recipe(crafter::depend_graph& graph, const crafter::Recipe& recipe) {
	for (const auto& ingredient : recipe.ingredients) {
		graph[ingredient.name].push_back(recipe.name);
	}
	graph[recipe.name];
}
