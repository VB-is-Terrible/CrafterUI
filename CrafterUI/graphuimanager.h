#ifndef GRAPHUIMANAGER_H
#define GRAPHUIMANAGER_H

#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include "lineshape.h"

static const auto RECIPE_LOCATION = "qrc:/qml/RecipeNode.qml";

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
        void removeAllRecipeDisplays (void);
        location_map populateRecipes(recipe_layout order);
        void populateRecipeLinks(location_map locations, recipe_links links);
        void populateRecipes(recipe_layout order, recipe_links links);
private:
        QQmlApplicationEngine* engine;
        void appendRecipeDisplay (QQuickItem* recipe);
        void findScene();
        constexpr unsigned long calc_gap(size_t items, size_t width);
        QQuickItem* scene;
        QQuickItem* flickable;
        LineShape* lineShape;
};

}

#endif // GRAPHUIMANAGER_H
