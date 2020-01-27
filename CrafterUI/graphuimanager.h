#ifndef GRAPHUIMANAGER_H
#define GRAPHUIMANAGER_H

#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include "graph-builder.h"
#include "lineshape.h"
#include "qmlcommunication.h"

static const auto RECIPE_LOCATION = "qrc:/qml/RecipeNode.qml";
static const auto ROW_LOCATION = "qrc:/qml/TableRow.ui.qml";

namespace crafter {
using location_map = std::unordered_map<std::string, std::pair<size_t, size_t>>;
using recipe_layout = const std::vector<std::vector<std::string>>&;
using recipe_links = const std::vector<std::pair<std::string, std::string>>&;
static constexpr const auto recipe_height = 200;
static constexpr const auto recipe_width = 400;
static constexpr const auto recipe_margin_bottom = 50;
static constexpr const auto recipe_margin_right = 50;

class GraphUIManager {
public:
        GraphUIManager(QQmlApplicationEngine* engine);
        QQuickItem* createRecipeDisplay(std::string title);
        void removeAllRecipeDisplays(void);
        void populateRecipeLinks(location_map locations, recipe_links links);
        void populateRecipes(const crafter::CraftingGraph&);
private:
        QQmlApplicationEngine* engine;
        QQuickItem* scene;
        QQuickItem* flickable;
        LineShape* lineShape;
        QQuickItem* sideStack;
        QQuickItem* rawDisplay;
        QMLCommunication communicator;

        void appendRecipeDisplay (QQuickItem* recipe);
        void findScene();
        static std::string output_ingredients (const crafter::ingredient_map&);
        constexpr unsigned long calc_gap(size_t items, size_t width);
        location_map populateRecipes(const crafter::CraftingGraph&, bool);
        void populateRawMaterials(const crafter::CraftingGraph&);
        void removeAllRawMaterials(void);
        void addRawMaterial(std::string name, size_t count);
        void recipeClicked(const std::string& name);

        friend QMLCommunication;
};

}

#endif // GRAPHUIMANAGER_H
