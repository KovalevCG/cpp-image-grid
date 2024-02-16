#pragma once

#include <array>
#include <string>

class GlobalResources {

public:

    GlobalResources();

    // Methods for "img_paths" array
    void setImagePath(size_t col, size_t row, const std::string& img_path);
    std::string getImagePath(size_t col, size_t row) const;

    // "num_of_col" and "num_of_rows" - Accessors and Mutators
    int getNumOfCols() const;
    int getNumOfRows() const;
    void setNumOfCols(int cols);
    void setNumOfRows(int rows);

    void defaultFillImgPaths();

private:

    // Array "img_paths"
    static constexpr size_t SIZE = 20;
    static std::array<std::array<std::string, SIZE>, SIZE> img_paths;

    // Cols and Rows
    static int num_of_cols;
    static int num_of_rows;
};