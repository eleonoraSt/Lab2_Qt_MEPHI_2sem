#include <QApplication>

#include "..\include\Window.h"
#include "..\tests.cpp"

int main(int argc, char** argv) {
    //runAllTests();

    QApplication app(argc, argv);

    Window window;
    window.show();
    return app.exec();
}
