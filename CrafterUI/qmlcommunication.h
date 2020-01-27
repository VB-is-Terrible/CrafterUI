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
	void recipeClicked(const QString& recipe);
	void addRecipe(QQuickItem* recipe);
private:
	GraphUIManager& graphui;
};

}

#endif /* end of include guard: QMLCommunication */
