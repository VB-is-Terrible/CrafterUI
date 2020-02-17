#include "qmlcommunication.h"
#include "graphuimanager.h"
#include <iostream>
#include <QObject>

namespace crafter {

QMLCommunication::QMLCommunication(GraphUIManager& graphui) : graphui{graphui} {}

void QMLCommunication::recipeClicked(const QString &recipe) {
	QMetaObject::invokeMethod(graphui.sideColumn, "toRecipes");

	graphui.recipeClicked(recipe.toStdString());
	QMetaObject::invokeMethod(graphui.recipeDisplay, "toOverview");

}

void QMLCommunication::addRecipe(QQuickItem *recipe) {
	QObject::connect(recipe, SIGNAL(clicked(QString)), this, SLOT(recipeClicked(QString)));
}

void QMLCommunication::setRecipeSelector(QQuickItem * selector) {
	QObject::connect(selector, SIGNAL(recipeSelect(int)), this, SLOT(recipeSelected(int)));
}

void QMLCommunication::recipeSelected(int index) {
	if (index != graphui.recipeIndex) {
		graphui.recipeSelected(index);
	}
}

void QMLCommunication::setRecipeAccept(QQuickItem * acceptor) {
	QObject::connect(acceptor, SIGNAL(changeRecipe(int)), this, SLOT(recipeAccept(int)));
}

void QMLCommunication::recipeAccept(int new_amount) {
	if (new_amount < 0) {
		throw std::invalid_argument("new_amount cannot be negative, got " + std::to_string(new_amount) + "\n");
	}
	QMetaObject::invokeMethod(graphui.sideColumn, "toRawMaterials");
	graphui.recipeAmountChanged(new_amount);
}

void QMLCommunication::realConstructor() {
	setRecipeAccept(graphui.recipeDisplay);
	setRecipeSelector(graphui.recipeSelector);
	QObject::connect(graphui.graphView, SIGNAL(backClicked()), this, SLOT(backClicked()));
}

void QMLCommunication::backClicked(void) {
	graphui.resetSelected();
	QMetaObject::invokeMethod(graphui.recipeDisplay, "toOverview");
	QMetaObject::invokeMethod(graphui.sideColumn, "toRawMaterials");
}

}
