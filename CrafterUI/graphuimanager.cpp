#include "graphuimanager.h"


namespace crafter {

GraphUIManager::GraphUIManager(QQmlApplicationEngine* engine) {
	this->engine = engine;
	findScene();
}

void GraphUIManager::findScene(void) {
	auto root = engine->rootObjects()[0];
	QQuickItem* rootLayout = root->findChild<QQuickItem *>("Centre of the Universe");
	QQuickItem* graph = (rootLayout->property("mainGraph").value<QQuickItem*>());
	flickable = (graph->property("scene").value<QQuickItem*>());
	scene = flickable->findChild<QQuickItem *>("recipeStore");
	lineShape = flickable->findChild<crafter::LineShape *>("betterShape");
}

QQuickItem* GraphUIManager::createRecipeDisplay(std::string title) {
	QQmlComponent recipeComponent(engine, QUrl((RECIPE_LOCATION)));
	QQuickItem* recipe = qobject_cast<QQuickItem *>(recipeComponent.create());
	recipe->setProperty("heading", QString::fromStdString(title));
	recipe->setProperty("contents", QString::fromStdString("Test contents"));
	return recipe;
}

void GraphUIManager::appendRecipeDisplay (QQuickItem* recipe) {
	recipe->setParentItem(scene);
	engine->setObjectOwnership(recipe, QQmlEngine::JavaScriptOwnership);
}

void GraphUIManager::removeAllRecipeDisplays(void) {
	for (auto child : scene->childItems()) {
		child->setParentItem(nullptr);
		delete child;
	}
}

template <typename T>
struct size_comp {
    	bool operator() (const T a, const T b) {
        	return a.size() < b.size();
    	}
};

location_map GraphUIManager::populateRecipes(const crafter::CraftingGraph& graph, bool) {
	location_map result;
	const auto height = graph.order.size() * (recipe_height + recipe_margin_bottom) - recipe_margin_bottom;
	const auto max_items = (*std::max_element(
    	                            graph.order.begin(), graph.order.end(),
    	                            size_comp<decltype(graph.order.at(0))>())
	                        ).size();
	const auto width = max_items * recipe_width + (max_items - 1) * recipe_margin_right;
	flickable->setProperty("contentHeight", QVariant(static_cast<unsigned long long>(height)));
	flickable->setProperty("contentWidth", QVariant(static_cast<unsigned long long>(width)));
	auto y_level = 0;
	for (const auto& level : graph.order) {
        	const auto gap = calc_gap(level.size(), width);
        	auto x_level = gap;
	        for (const auto& recipe_name : level) {
			auto recipe = createRecipeDisplay(recipe_name);
			recipe->setX(x_level);
			recipe->setY(y_level);
        		recipe->setProperty("contents",
                                QVariant(QString::fromStdString(
                                             output_ingredients(graph.calc_ingredients(recipe_name)))));
			result[recipe_name] = std::pair(x_level, y_level);
			appendRecipeDisplay(recipe);
			x_level += recipe_width + recipe_margin_right;
	        }
	        y_level += recipe_height + recipe_margin_bottom;
	}
	return result;
}

constexpr unsigned long GraphUIManager::calc_gap(size_t items, size_t width) {
	const auto needed = items * (recipe_width + recipe_margin_right) - recipe_margin_right;
	return (width - needed) / (items + 1);
}

void GraphUIManager::populateRecipeLinks(location_map locations, recipe_links links) {
	std::vector<crafter::LineConnection> connections;
	for (const auto& [source, destination] : links) {
		crafter::LineConnection connection;
		const auto source_location = locations.at(source);
		const auto destination_location = locations.at(destination);
		connection.start = std::pair<double, double>(source_location.first + recipe_width / 2, source_location.second + recipe_height);
		connection.end = std::pair<double, double>(destination_location.first + recipe_width / 2, destination_location.second);
		// std::cout << connection;
		connections.push_back(connection);
	}
	lineShape->setLines(std::move(connections));

}

void GraphUIManager::populateRecipes(const crafter::CraftingGraph& graph) {
	removeAllRecipeDisplays();
	const auto locations = populateRecipes(graph, false);
	populateRecipeLinks(locations, graph.make_pairings());
}

std::string GraphUIManager::output_ingredients(const crafter::ingredient_map& map) {
	std::vector<std::string> names;
	for (const auto& [name, count] : map) {
		names.push_back(name);
	}
	std::sort(names.begin(), names.end());
	std::string result;
	for (const auto& name : names) {
		result += std::to_string(map.at(name)) + "\t" + name + "\n";
	}
	return result;
}
}
