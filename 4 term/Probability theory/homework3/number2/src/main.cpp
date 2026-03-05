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
        QMessageBox::critical(nullptr, "Ошибка", 
            QString("Не удалось запустить приложение: %1").arg(e.what()));
        return 1;
    }
}