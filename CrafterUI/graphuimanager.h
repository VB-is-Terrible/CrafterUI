#ifndef GRAPHUIMANAGER_H
#define GRAPHUIMANAGER_H

#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>

#include "graph-builder.h"
#include "lineshape.h"
#include "qmlcommunication.h"

namespace crafter {
const auto RECIPE_LOCATION = "qrc:/qml/RecipeNode.qml";
const auto ROW_LOCATION = "qrc:/qml/TableRow.ui.qml";
const auto COLUMN_LOCATION = "qrc:/qml/SingleRecipe.ui.qml";

using location_map = std::unordered_map<std::string, std::pair<size_t, size_t>>;
using recipe_layout = const std::vector<std::vector<std::string>>&;
using recipe_links = const std::vector<std::pair<std::string, std::string>>&;
static constexpr const auto recipe_height = 150;
static constexpr const auto recipe_width = 300;
static constexpr const auto recipe_margin_bottom = 50;
static constexpr const auto recipe_margin_right = 30;

class GraphUIManager {
public:
        GraphUIManager(QQmlApplicationEngine* engine);
        QQuickItem* createRecipeDisplay(std::string title);
        void populateRecipeLinks(location_map locations, recipe_links links);
        void populateRecipes(std::shared_ptr<crafter::CraftingGraph>);
private:
        QQmlApplicationEngine* engine;
        LineShape* lineShape;
        QQuickItem *scene, *flickable, *sideStack, *rawDisplay,
                   *recipeDisplay, *recipeMaterials, *recipeColumns,
                   *recipeSelector;
        QMLCommunication communicator;
        std::shared_ptr<crafter::CraftingGraph> graph;
        std::string selected;
        int recipeIndex;





        void appendRecipeDisplay (QQuickItem* recipe);
        void findScene();
        static std::string output_ingredients (const crafter::ingredient_map&);
        constexpr unsigned long calc_gap(size_t items, size_t width);
        location_map populateRecipes(void);
        void populateRawMaterials(void);
        void addRawMaterial(const std::string& name, const size_t count);
        void recipeClicked(const std::string& name);
        void fillOutRecipe(const std::string& name);
        void makeRecipeColumns(const std::string& name);
        QQuickItem* makeSingleRecipe(const ingredient_map& ingredients, const Recipe& recipe);
        void removeChildren(QQuickItem* parent);
        QQuickItem* createRow(const std::string& name, const size_t count);
        QList<QVariant> nameRecipeOptions(const std::string& name);
        void appendDetailedRecipe(const Recipe&);
        void recipeSelected(int index);
        friend QMLCommunication;
};

}

#endif // GRAPHUIMANAGER_H
