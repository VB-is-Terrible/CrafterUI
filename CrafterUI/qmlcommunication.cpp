#include "qmlcommunication.h"
#include "graphuimanager.h"
#include <iostream>

namespace crafter {

QMLCommunication::QMLCommunication(GraphUIManager& graphui) : graphui{graphui} {}

void QMLCommunication::recipeClicked(const QString &recipe) {
	graphui.recipeClicked(recipe.toStdString());
}

void QMLCommunication::addRecipe(QQuickItem *recipe) {
	QObject::connect(recipe, SIGNAL(clicked(QString)), this, SLOT(recipeClicked(QString)));
}
}
