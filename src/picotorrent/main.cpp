#include <memory>

#include <picotorrent/core/configuration.hpp>
#include <picotorrent/core/database.hpp>
#include <picotorrent/core/environment.hpp>
#include <picotorrent/geoip/geoip.hpp>

#include "application.hpp"
#include "mainwindow.hpp"
#include "translator.hpp"

int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    pt::Application app(argc, argv);

    if (!app.isSingleInstance())
    {
        app.activateOtherInstance();
        return 0;
    }

    auto env = std::make_shared<pt::Environment>();
    auto db = std::make_shared<pt::Database>(env);

    if (!db->migrate())
    {
        return -1;
    }

    auto cfg = std::make_shared<pt::Configuration>(db);

    pt::Translator& translator = pt::Translator::instance();
    translator.loadEmbedded(GetModuleHandle(NULL));
    translator.setLanguage(cfg->getInt("language_id"));

    pt::MainWindow wnd(env, db, cfg);
    wnd.show();

    return app.exec();
}
