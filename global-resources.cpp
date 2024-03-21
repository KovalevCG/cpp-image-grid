#include "global-resources.h"

// Init
void GlobalResources::initGlobalResources() {
    for (auto& row : img_paths) {
        for (auto& element : row) {
            element = "images//bg_image_opencv.png";
        }
    }
}

// Variables
std::array<std::array<std::string, 20>, 20> GlobalResources::img_paths;
std::array<bool, 20> GlobalResources::merged_cols{ false };
std::array<bool, 20> GlobalResources::merged_rows{ false };
int GlobalResources::num_of_cols = 2;
int GlobalResources::num_of_rows = 2;


void GlobalResources::setImagePath(size_t col, size_t row, const std::string& path) {

    if (row < SIZE && col < SIZE) {
        img_paths[col][row] = path;
    }
}

std::string GlobalResources::getImagePath(size_t col, size_t row) {

    if (row < SIZE && col < SIZE) {
        return img_paths[col][row];
    }
    return {};
}