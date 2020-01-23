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
#include "import.h"

static const std::vector<std::vector<std::string>> test_layers = {{"Big honking recipe", "Second Smaller Recipe"}, {"Majestic Ingots", "Annoyingly Expensive Control Circuits"}, {"Super Glue", "Silicone", "Some other expensive liquid"}};
static const std::vector<std::pair<std::string, std::string>> test_links = {{"Big honking recipe", "Majestic Ingots"}, {"Big honking recipe", "Annoyingly Expensive Control Circuits"}, {"Second Smaller Recipe", "Majestic Ingots"}, {"Annoyingly Expensive Control Circuits", "Super Glue"}, {"Annoyingly Expensive Control Circuits", "Silicone"}, {"Annoyingly Expensive Control Circuits", "Some other expensive liquid"}};


int main(int argc, char *argv[])
{
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
    main_graph.populateRecipes(test_layers, test_links);
    return app.exec();
}
