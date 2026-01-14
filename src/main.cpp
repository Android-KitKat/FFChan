#include "Backend.h"
#include "Loader.h"
#include "Translator.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QQuickStyle::setStyle("FluentWinUI3");
  QQmlApplicationEngine engine;
  FFChan::Backend backend;
  FFChan::Translator translator;
  engine.rootContext()->setContextProperty("backend", &backend);
  engine.rootContext()->setContextProperty("i18n", &translator);
  translator.setLocale("en");

  bool ok = FFChan::prepareEngine(engine);

  if (engine.rootObjects().isEmpty())
    return -1;
  return app.exec();
}