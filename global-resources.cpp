#include "global-resources.h"

#include <algorithm>

// Init
void GlobalResources::initGlobalResources() {
    for (auto& row : img_paths) {
        for (auto& element : row) {
            element = bg_path_opencv;
        }
    }
}

// Variables
std::array<std::array<std::string, GlobalResources::SIZE>, GlobalResources::SIZE> GlobalResources::img_paths;
std::array<bool, GlobalResources::SIZE> GlobalResources::merged_cols{ false };
std::array<bool, GlobalResources::SIZE> GlobalResources::merged_rows{ false };
int GlobalResources::num_of_cols = 2;
int GlobalResources::num_of_rows = 2;
std::string GlobalResources::bg_path_opencv = "images//bg_image_opencv.png";
std::string GlobalResources::bg_path_qt = "images//bg_image_qt_white.png";
std::string GlobalResources::save_path = "";
// Settings
const std::string GlobalResources::VERSION = "2.0.0";
long GlobalResources::WIN_VERSION = 0;

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

bool GlobalResources::anyMergedCols() {
    bool hasTrue = std::any_of(merged_cols.begin(), merged_cols.end(), [](bool value) {
        return value;
        });
    return hasTrue;
}

bool GlobalResources::anyMergedRows() {
    bool hasTrue = std::any_of(merged_rows.begin(), merged_rows.end(), [](bool value) {
        return value;
        });
    return hasTrue;
}