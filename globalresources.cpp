#include "globalresources.h"

GlobalResources::GlobalResources() {

   

}

// Define and initialize static member variables
int GlobalResources::num_of_cols = 2; // Default value for num_of_cols
int GlobalResources::num_of_rows = 2; // Default value for num_of_rows
std::array<std::array<std::string, 20>, 20> GlobalResources::img_paths;

void GlobalResources::setImagePath(size_t col, size_t row, const std::string& path) {

    if (row < SIZE && col < SIZE) {
        img_paths[col][row] = path;
    }
}

std::string GlobalResources::getImagePath(size_t col, size_t row) const {

    if (row < SIZE && col < SIZE) {
        return img_paths[col][row];
    }
    return {};
}

int GlobalResources::getNumOfCols() const {
    return num_of_cols;
}

int GlobalResources::getNumOfRows() const {
    return num_of_rows;
}

void GlobalResources::setNumOfCols(int cols) {
    num_of_cols = cols;
}

void GlobalResources::setNumOfRows(int rows) {
    num_of_rows = rows;
}

void GlobalResources::defaultFillImgPaths() {
    for (auto& row : img_paths) {
        for (auto& element : row) {
            element = "images/bg_image_opencv.png";
        }
    }
}