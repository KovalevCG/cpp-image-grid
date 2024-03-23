#pragma once

#include <array>
#include <string>

class GlobalResources {

public:

    // Methods for "img_paths" array
    static void setImagePath(size_t col, size_t row, const std::string& img_path);
    static std::string getImagePath(size_t col, size_t row);
    static void initGlobalResources();
    static bool anyMergedCols();
    static bool anyMergedRows();
    // Variables
    static int num_of_cols;
    static int num_of_rows;
    static std::array<bool, 20> merged_cols;
    static std::array<bool, 20> merged_rows;

private:

    // Array "img_paths"
    static constexpr size_t SIZE = 20;
    static std::array<std::array<std::string, SIZE>, SIZE> img_paths;

};