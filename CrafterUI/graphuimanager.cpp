#include "graphuimanager.h"


namespace crafter {

GraphUIManager::GraphUIManager(QQmlApplicationEngine* engine)
        : communicator{*this} {
	this->engine = engine;

	findScene();
        communicator.setRecipeSelector(recipeSelector);
}

void GraphUIManager::removeChildren(QQuickItem* parent) {
	for (auto child : parent->childItems()) {
		child->setParentItem(nullptr);
		delete child;
	}
}


void GraphUIManager::findScene(void) {
	auto root = engine->rootObjects()[0];
	auto rootLayout = root->findChild<QQuickItem *>("Centre of the Universe");
	auto graph = rootLayout->property("mainGraph").value<QQuickItem*>();
	flickable = graph->property("scene").value<QQuickItem*>();
	scene = flickable->findChild<QQuickItem *>("recipeStore");
	lineShape = flickable->findChild<crafter::LineShape *>("betterShape");
	auto sideColumn = rootLayout->property("sideColumn").value<QQuickItem*>();
	sideStack = sideColumn->findChild<QQuickItem *>("mainView");
	rawDisplay = sideColumn->findChild<QQuickItem *>("rawPage");
	recipeDisplay = sideColumn->findChild<QQuickItem *>("recipeDetailed");
	recipeMaterials = recipeDisplay->findChild<QQuickItem *>("recipeMaterials");
        recipeSelector = recipeDisplay->findChild<QQuickItem *>("recipeSelector");
        recipeColumns = recipeDisplay->findChild<QQuickItem *>("recipeColumns");
}

QQuickItem* GraphUIManager::createRecipeDisplay(std::string title) {
	QQmlComponent recipeComponent(engine, QUrl((RECIPE_LOCATION)));
	QQuickItem* recipe = qobject_cast<QQuickItem *>(recipeComponent.create());
	recipe->setProperty("heading", QString::fromStdString(title));
	recipe->setProperty("contents", QString::fromStdString("Test contents"));
        recipe->setProperty("width", recipe_width);
        recipe->setProperty("height", recipe_height);
	return recipe;
}

void GraphUIManager::appendRecipeDisplay (QQuickItem* recipe) {
	recipe->setParentItem(scene);
	engine->setObjectOwnership(recipe, QQmlEngine::JavaScriptOwnership);
	communicator.addRecipe(recipe);
}

template <typename T>
struct size_comp {
    	bool operator() (const T a, const T b) {
        	return a.size() < b.size();
    	}
};

location_map GraphUIManager::populateRecipes(void) {
	removeChildren(scene);
	location_map result;
	const auto height = graph->order.size() * (recipe_height + recipe_margin_bottom) - recipe_margin_bottom;
	const auto max_items = (*std::max_element(
    	                            graph->order.begin(), graph->order.end(),
    	                            size_comp<decltype(graph->order.at(0))>())
	                        ).size();
	const auto width = max_items * (recipe_width + recipe_margin_right) - recipe_margin_right;
	flickable->setProperty("contentHeight", QVariant(static_cast<unsigned long long>(height)));
	flickable->setProperty("contentWidth", QVariant(static_cast<unsigned long long>(width)));
	auto y_level = 0;
	for (const auto& level : graph->order) {
        	const auto gap = calc_gap(level.size(), width);
        	auto x_level = gap;
	        for (const auto& recipe_name : level) {
			auto recipe = createRecipeDisplay(recipe_name);
			recipe->setX(x_level);
			recipe->setY(y_level);
        		recipe->setProperty("contents",
                                QVariant(QString::fromStdString(
                                             output_ingredients(graph->calc_ingredients(recipe_name)))));
			result[recipe_name] = std::pair(x_level, y_level);
			appendRecipeDisplay(recipe);
			x_level += recipe_width + recipe_margin_right + gap;
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
		connections.push_back(connection);
	}
	lineShape->setLines(std::move(connections));

}

void GraphUIManager::populateRecipes(std::shared_ptr<crafter::CraftingGraph> graph) {
	this->graph = graph;
	const auto locations = populateRecipes();
	populateRecipeLinks(locations, graph->make_pairings());
	populateRawMaterials();
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

void GraphUIManager::addRawMaterial(const std::string& name, const size_t count) {
        auto row = createRow(name, count);
	row->setParentItem(rawDisplay);
}

void GraphUIManager::populateRawMaterials() {
	removeChildren(rawDisplay);
	for (const auto& ingredient : graph->raw_ingredients) {
		const auto needed = graph->recipe_count.at(ingredient).distribution[0];
		addRawMaterial(ingredient, needed);
	}
}

void GraphUIManager::recipeClicked(const std::string& name) {
	std::cerr << "Recieved click from " << name << "\n";
        selected = name;
	fillOutRecipe(name);
}

void GraphUIManager::fillOutRecipe(const std::string& name) {
	recipeDisplay->setProperty("recipeName", QString::fromStdString(name));
	recipeDisplay->setProperty("recipeValues", nameRecipeOptions(name));
        recipeIndex = 1;
	recipeSelector->setProperty("currentIndex", 1);
	const auto& recipes = graph->recipes.at(name);
	appendDetailedRecipe(recipes[1]);
        makeRecipeColumns(name);
}

QQuickItem* GraphUIManager::createRow(const std::string& name, const size_t count) {
        QQmlComponent rowComponent(engine, QUrl(ROW_LOCATION));
        QQuickItem* row = qobject_cast<QQuickItem *>(rowComponent.create());
        row->setProperty("itemName", QString::fromStdString(name));
        row->setProperty("itemCount", QString::fromStdString(std::to_string(count)));
        engine->setObjectOwnership(row, QQmlEngine::JavaScriptOwnership);
        return row;
}

void GraphUIManager::makeRecipeColumns(const std::string& name) {
        const auto& count = graph->recipe_count.at(name);
        const auto& recipes = graph->recipes.at(name);
        auto ingredient_lists = graph->calc_ingredients(name, count);
        removeChildren(recipeColumns);

        for (size_t i = 0; i < ingredient_lists.size(); i++) {
                const auto& list = ingredient_lists[i];
                const auto& recipe = recipes[i];
                if (list.empty()) {
                        continue;
                }
                auto column = makeSingleRecipe(list, recipe, count.distribution[i]);
                column->setParentItem(recipeColumns);
        }
}

QList<QVariant> GraphUIManager::nameRecipeOptions(const std::string& name) {
	const auto& recipes = graph->recipes.at(name);
	QList<QVariant> result{QString("Pre-crafted"), QString("Default")};
	for (size_t i = 2; i < recipes.size(); i++) {
		result.push_back(QString::fromStdString("Recipe " + std::to_string(i - 1)));
	}
	return result;
}

void GraphUIManager::appendDetailedRecipe(const Recipe& recipe) {
        removeChildren(recipeMaterials);
	for (const auto& ingredient : recipe.ingredients) {
                auto row = createRow(ingredient.name, ingredient.count);
		row->setParentItem(recipeMaterials);
	}

}

QQuickItem* GraphUIManager::makeSingleRecipe(const ingredient_map& ingredients, const Recipe& recipe, const size_t needed) {
        QQmlComponent singleComponent(engine, QUrl(COLUMN_LOCATION));
        auto single = qobject_cast<QQuickItem *>(singleComponent.create());
        auto insert_point = single->property("column").value<QQuickItem*>();
        for (const auto& [name, count] : recipe.ingredients) {
                auto row = createRow(name, ingredients.at(name));
                row->setParentItem(insert_point);
        }
        single->setProperty("methodName", QString::fromStdString(recipe.method));
        single->setProperty("hasMethod", !recipe.method.empty());
        single->setProperty("makes", static_cast<unsigned int>(needed));
        return single;
}

void GraphUIManager::recipeSelected(int index) {
        recipeIndex = index;
        const auto& recipes = graph->recipes.at(selected);
        appendDetailedRecipe(recipes[index]);
}

}
