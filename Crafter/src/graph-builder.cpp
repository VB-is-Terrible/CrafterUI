#include "graph-builder.h"

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
	
}


std::ostream& operator<< (std::ostream& os, const CraftingGraph&) {

}

std::ostream& output_recipe (std::ostream& os, const std::string& name) {

}

requests get_requests (const recipe_store& recipes, const std::string& input_file) {

}

requests get_requests_from_input (const recipe_store& recipes) {

}

recipe_store read_templates(std::string template_location) {

}

bool valid_extension(std::string) {

}


}
