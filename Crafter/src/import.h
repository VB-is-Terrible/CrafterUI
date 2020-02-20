#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "yaml-cpp/yaml.h"

namespace crafter {
	struct StackType {
		enum value {Block, Liquid, NonStackable};
	};
	struct Ingredients {
		Ingredients (YAML::Node);
		Ingredients() = default;
		Ingredients(std::string name_, int count_) : name{name_}, count{count_} {};
		std::string name;
		int count;
	};
	struct Recipe {
		Recipe (std::string, YAML::Node);
		Recipe (std::string name) : name{name} {};
		std::string name;
		int makes = 1;
		std::string method;
		std::vector<Ingredients> ingredients;
	};

	using recipe_store = std::unordered_map<std::string, std::vector<Recipe>>;
	recipe_store read_in(std::ifstream& file, bool pre_populate = false);
	recipe_store read_in(std::string file_name, bool pre_populate = false);
	void read_in(std::ifstream& file, recipe_store& store, bool pre_populate = false);
	void read_in(std::string file_name, recipe_store& store, bool pre_populate = false);

	std::vector<Ingredients> get_requests_from_file(const crafter::recipe_store& recipes, const std::string& input_file);
	std::string hello(void);
}
