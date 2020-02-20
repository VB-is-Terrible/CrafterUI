#ifndef QMLCOMMUNICATION_H
#define QMLCOMMUNICATION_H

#include <QObject>
#include <QString>
#include <QQuickItem>

namespace crafter {
class GraphUIManager;


class QMLCommunication : public QObject {
	Q_OBJECT
public:
	QMLCommunication(class GraphUIManager&);
public slots:
	void setRecipeSelector(QQuickItem*);
	void setRecipeAccept(QQuickItem*);
	void recipeClicked(const QString& recipe);
	void addRecipe(QQuickItem* recipe);
	void recipeSelected(int index);
	void recipeAccept(int new_amount);
	void backClicked(void);
private:
	GraphUIManager& graphui;
};

}

#endif /* end of include guard: QMLCommunication */
