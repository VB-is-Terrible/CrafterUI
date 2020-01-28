#include "qmlcommunication.h"
#include "graphuimanager.h"
#include <iostream>
#include <QObject>

namespace crafter {

QMLCommunication::QMLCommunication(GraphUIManager& graphui) : graphui{graphui} {}

void QMLCommunication::recipeClicked(const QString &recipe) {
	graphui.recipeClicked(recipe.toStdString());
}

void QMLCommunication::addRecipe(QQuickItem *recipe) {
	QObject::connect(recipe, SIGNAL(clicked(QString)), this, SLOT(recipeClicked(QString)));
}

void QMLCommunication::setRecipeSelector(QQuickItem * selector) {
	QObject::connect(selector, SIGNAL(recipeSelect(int)), this, SLOT(recipeSelected(int)));
}

void QMLCommunication::recipeSelected(int index) {
	if (index != graphui.recipeIndex) {
		std::cout << "Firing on index " << index << "\n";
		graphui.recipeSelected(index);
	}
}


}
