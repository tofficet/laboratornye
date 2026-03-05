#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    try {
        MainWindow window;
        window.show();
        return app.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Fatal Error", 
            QString("Application failed to start: %1").arg(e.what()));
        return 1;
    } catch (...) {
        QMessageBox::critical(nullptr, "Fatal Error", 
            "Unknown error occurred during application startup");
        return 1;
    }
}