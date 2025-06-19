// src/main.cpp
#include "Core/Application.h"
#include <iostream>

int main() {
    try {
        Core::Application app("TitanCAD v0.1.0");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "An unhandled exception occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "An unknown unhandled exception occurred." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}