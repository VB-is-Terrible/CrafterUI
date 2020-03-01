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


constexpr auto TEMP_FILE = "/tmp/CrafterUIContinue.yaml";
constexpr auto HELP_STRING = "Usage: CrafterUI [OPTION]... FILE\nA GUI to help with long crafting recipes\n\n  -c, Config file to use\n  -h, Display this message\n";

struct options {
        bool help;
        std::string config;
        std::string input;
};

struct config {
        std::string template_location;
};

struct options read_args(int argc, char *argv[]);
struct config read_config(std::string config_file);
struct config default_config();

int main(int argc, char *argv[])
{
        const auto options = read_args(argc, argv);
        if (options.help) {
                std::cout << HELP_STRING;
                return 0;
        }
        auto input_file = options.input;

        const auto config = read_config(options.config);

        std::cout << "Testing import, hello import, hello " << crafter::hello() << "\n";

        std::cout << "Current path is " << std::filesystem::current_path() << '\n';

        const auto recipes = crafter::read_templates(config.template_location);
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

struct options read_args (int argc, char *argv[]) {
        auto pointer = 1;
        struct options options{};
        while (pointer < argc) {
                if (strcmp(argv[pointer], "-h") == 0) {
                        options.help = true;
                        pointer += 1;
                } else if (strcmp(argv[pointer], "-c") == 0) {
                        if (pointer + 1 < argc) {
                                options.config = argv[pointer + 1];
                                pointer += 2;
                        } else {
                                pointer += 1;
                        }
                } else {
                        if (options.input == "") {
                                options.input = argv[pointer];
                        }
                        pointer += 1;
                }
        }
        return options;
}

struct config read_config(std::string) {
        struct config result = default_config();
        return result;
}

struct config default_config() {
        struct config result;
        const auto exe_location = std::filesystem::read_symlink("/proc/self/exe");
        auto templates = exe_location.parent_path().parent_path();
        templates /= "data";
        templates /= "recipes";
        result.template_location = templates;
        return result;
}
