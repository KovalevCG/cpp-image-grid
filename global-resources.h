#pragma once

#include <array>
#include <string>

class GlobalResources {

public:

    // Settings
    static const int START_CV_WIDTH = 900;
    static const int START_CV_HEIGHT = 700;
    static const int SAVE_RESOLUTION_WIDTH = 1600;
    static const int SAVE_RESOLUTION_HEIGHT = 1200;
    static const std::string VERSION;

    // Methods for "img_paths" array
    static void setImagePath(size_t col, size_t row, const std::string& img_path);
    static std::string getImagePath(size_t col, size_t row);
    static void initGlobalResources();
    static bool anyMergedCols();
    static bool anyMergedRows();
    // Variables
    static int num_of_cols;
    static int num_of_rows;
    // static bool update_ocv_images;
    static std::string bg_path_opencv;
    static std::string bg_path_qt;
    static std::array<bool, 20> merged_cols;
    static std::array<bool, 20> merged_rows;


private:

    // Array "img_paths"
    static constexpr size_t SIZE = 20;
    static std::array<std::array<std::string, SIZE>, SIZE> img_paths;

};