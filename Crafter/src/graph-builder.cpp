#include "graph-builder.h"

#if __GNUC__ > 7
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#define old_fs
#endif

void add_depend_recipe(crafter::depend_graph& graph, const crafter::Recipe& recipe);
namespace crafter {

craft_count::craft_count(const std::vector<Recipe>& recipes) {
	for (const auto& recipe : recipes) {
		distribution.push_back(0);
		makes.push_back(recipe.makes);
	}
}

CraftingGraph::CraftingGraph (
	const ::crafter::requests& requests,
	const recipe_store& recipes,
	const depend_graph& dependencies)
	: recipes{recipes},  dependencies{dependencies} {
	this->requests = requests;
	build_graph(requests);
	tally_count(requests);
	get_order();
}

void CraftingGraph::build_graph(const std::vector<Ingredients>& requests) {
	std::deque<std::string> queue;
	std::unordered_set<std::string> seen;
	for (const auto& request : requests) {
		queue.push_back(request.name);
		seen.insert(request.name);
	}

	while (!queue.empty()) {
		auto request = queue[0];
		queue.pop_front();
		recipe_graph.InsertNode(request);
		auto recipe_it = recipes.find(request);
		if (recipe_it != recipes.end()) {
			// Add links for all recipes
			for (const auto& recipe : recipe_it->second) {
				for (const auto& ingredient : recipe.ingredients) {
					recipe_graph.InsertNode(ingredient.name);
					if (!recipe_graph.IsConnected(request, ingredient.name)) {
						recipe_graph.InsertEdge(request, ingredient.name, 0);
					}
					if (!seen.count(ingredient.name)) {
						seen.insert(ingredient.name);
						queue.push_back(ingredient.name);
					}
				}
			}
		}
	}
}

void CraftingGraph::tally_count(const std::vector<Ingredients>& requests) {
	std::deque<std::string> queue;
	const auto head_vec = heads(recipe_graph);
	std::unordered_set<std::string> head_set{head_vec.begin(), head_vec.end()};
	for (const auto& node : requests) {
		auto needed = static_cast<size_t>(node.count);
		craft_count craft(recipes.at(node.name));
		craft.base_needed = needed;
		recipe_count[node.name] = craft;
		queue.push_back(node.name);
	}
	while (!queue.empty()) {
		auto request = queue[0];
		queue.pop_front();
		for (const auto& ingredient : recipe_graph.GetConnected(request)) {
			auto ready = check_ingredient(ingredient);
			if (ready) {
				queue.push_back(ingredient);
			}
		}
	}
}

void CraftingGraph::get_order (void) {
	order = crafter::order();
	for (const auto& it : recipe_count) {
		auto& name = it.first;
		auto& craft = it.second;
		if (craft.distance >= order.size()) {
			order.resize(craft.distance + 1);
		}
		order[craft.distance].push_back(name);
	}
	for (auto& level : order) {
		std::sort(level.begin(), level.end());
	}
}

bool CraftingGraph::check_ingredient(const std::string& ingredient) {
	craft_count count;
	if (recipe_count.count(ingredient) != 0) {
		count = recipe_count[ingredient];
		if (count.ready) {
			return true;
		}
	} else {
		if (recipes.count(ingredient)) {
			count = craft_count(recipes.at(ingredient));
		}
	}

	decltype(count.distance) parent_distance = 0;
	count.needed = count.base_needed;

	for (const auto& parent : dependencies.at(ingredient)) {
		if (!recipe_count[parent].ready) {
			return false;
		}
		count.needed += recipe_graph.GetWeight(parent, ingredient);
		parent_distance = std::max(parent_distance, recipe_count[parent].distance);
	}
	count.distance = parent_distance + 1;
	make_distribution(count);
	count.ready = true;
	recipe_count[ingredient] = count;
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
		has = std::min(has, count.needed);
		auto needed = count.needed - has;
		count.distribution[default_recipe] = ceil(needed / (double) count.makes[default_recipe]);
	}
}

void CraftingGraph::link_ingredient(const std::string& ingredient) {
	auto map = calc_ingredients(ingredient);
	for (const auto& pair : map) {
		recipe_graph.SetWeight(ingredient, pair.first, pair.second);
	}
}

ingredient_map CraftingGraph::calc_ingredients(const std::string& item) const {
	ingredient_map result;
	const craft_count& count = recipe_count.at(item);
	auto recipe_it = recipes.find(item);
	// Index 0 should not have a valid recipe
	for (size_t i = 1; i < count.distribution.size(); i++) {
		const auto& recipe = recipe_it->second[i - 1];
		for (const auto& ingredient : recipe.ingredients) {
			if (!result.count(ingredient.name)) {
				result[ingredient.name] = 0;
			}
			result[ingredient.name] += ingredient.count * count.distribution[i];
		}
	}
	return result;
}

std::ostream& operator<< (std::ostream& os, const CraftingGraph& graph) {
	const std::string line = "---------------";
	size_t level_count = 0;
	for (auto level = graph.order.crbegin(); level != graph.order.crend(); level++) {
		level_count++;
		os << line << " " << "Level " << level_count << " " << line << "\n\n";
		for (const auto& name : *level) {
			output_recipe(os, graph, name);
		}
	}
	return os;
}

std::ostream& output_recipe (std::ostream& os, const CraftingGraph& graph, const std::string& name) {
	const auto& recipes = graph.recipes.at(name);
	const auto count = graph.recipe_count.at(name);
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
	if (graph.recipe_graph.GetConnected(name).size() != 0) {
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
			std::cerr << "Recipe not found\n";
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
		for (const auto& recipe : recipes) {
			add_depend_recipe(result, recipe);
		}
	}
}

}

void add_depend_recipe(crafter::depend_graph& graph, const crafter::Recipe& recipe) {
	for (const auto& ingredient : recipe.ingredients) {
		graph[ingredient.name].push_back(recipe.name);
	}
}
