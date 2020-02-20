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
#include <memory>
#include <QIcon>
#include "yaml-cpp/yaml.h"

#include "lineshape.h"
#include "graphuimanager.h"
#include "graph-builder.h"

std::string read_args (int argc, char *argv[]);

constexpr auto TEMP_FILE = "/tmp/CrafterUIContinue.yaml";

int main(int argc, char *argv[])
{
        auto input_file = read_args(argc, argv);
        std::cout << "Testing import, hello import, hello " << crafter::hello() << "\n";

        std::cout << "Current path is " << std::filesystem::current_path() << '\n';

        const auto recipes = crafter::read_templates();
        const auto dependencies = crafter::build_depend_graph(recipes);
        std::shared_ptr<crafter::CraftingGraph> graph;
        if (input_file == "" && std::filesystem::exists(TEMP_FILE)) {
                auto yaml_file = YAML::LoadFile(TEMP_FILE);
                crafter::CraftingGraphState state;
                YAML::convert<crafter::CraftingGraphState>::decode(yaml_file, state);
                graph = std::make_shared<crafter::CraftingGraph>(
                        state,
                        recipes, dependencies);
        } else {
                const auto requests = crafter::get_requests(recipes, input_file);
                graph = std::make_shared<crafter::CraftingGraph>(
                        requests, recipes, dependencies);
        }


        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QGuiApplication app(argc, argv);

        QIcon icon(":/icon.png");
        app.setWindowIcon(icon);
        QQmlApplicationEngine engine;

        qmlRegisterType<crafter::LineShape>("com.chow", 1, 0, "LineShape");

        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        engine.load(url);

        auto main_graph = std::make_shared<crafter::GraphUIManager>(&engine, TEMP_FILE);
        main_graph->populateRecipes(graph);

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
