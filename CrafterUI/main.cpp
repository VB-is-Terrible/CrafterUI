#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <vector>
#include <string>
#include <QQmlComponent>
#include <QQuickItem>
#include <iostream>
#include <QPainterPath>
#include <unordered_map>
#include <filesystem>

#include "lineshape.h"
#include "graphuimanager.h"
#include "graph-builder.h"

std::string read_args (int argc, char *argv[]);


int main(int argc, char *argv[])
{
        auto input_file = read_args(argc, argv);
        std::cout << "Testing import, hello import, hello " << crafter::hello() << "\n";

        std::cout << "Current path is " << std::filesystem::current_path() << '\n';

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;

        qmlRegisterType<crafter::LineShape>("com.chow", 1, 0, "LineShape");

        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        engine.load(url);

        crafter::GraphUIManager main_graph(&engine);

        const auto recipes = crafter::read_templates();
        const auto dependencies = crafter::build_depend_graph(recipes);
        const auto requests = crafter::get_requests(recipes, input_file);

        crafter::CraftingGraph graph(requests, recipes, dependencies);

        main_graph.populateRecipes(graph.order, graph.make_pairings());

        return app.exec();
}

std::string read_args (int argc, char *argv[]) {
	if (argc == 1) {
		return "";
	} else if (argc == 2) {
		return argv[1];
	} else {
		std::cerr << "Got " << argc << " arguements, expected 0 or 1\n";
		throw std::invalid_argument(argv[2]);
	}
}
