#include "VideoPlayer.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QMainWindow mainWindow;
  mainWindow.setCentralWidget(new VideoPlayer);
  mainWindow.show();

  return app.exec();
}
