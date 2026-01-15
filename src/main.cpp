#include "Loader.h"
#include <QApplication>
#include <QIcon>
#include <QFont>

int main(int argc, char *argv[]) {
  // These must be set before creating QGuiApplication
  QApplication app(argc, argv);
  QCoreApplication::setApplicationName("FFChan");
  QCoreApplication::setOrganizationName("FFChan");

  // Set application icon (used by window decorations and some platforms)
  app.setWindowIcon(QIcon("qrc:/icon.png"));

  // Set the default font
  QFont defaultFont({"PingFang SC", "Noto Sans", "Noto Sans SC", "Microsoft YaHei", "Sogoe UI", "Arial", "Helvetica", "sans-serif"});
  app.setFont(defaultFont);

  // Initialize and run engine (all QML/engine logic in Loader)
  return FFChan::initializeAndRunEngine();
}
