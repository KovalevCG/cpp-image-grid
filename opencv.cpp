#include "opencv.h"
#include "global-resources.h"
#include "qt-window.h"

#include <QWidget>
#include <QApplication>

#include <chrono>
#include <algorithm>

using std::cout;
using std::endl;

OpenCV::OpenCV() {
    for (auto& row : zoom) { // Initialize all elements to 1.0
        row.fill(1.0);
    }
}

// Main Loop
void OpenCV::startOpencvMainLoop() {

    bool close_ocv = false;
    bool update_ocv_images = true;

    //cv::namedWindow("Edit", cv::WINDOW_AUTOSIZE | cv::WINDOW_GUI_NORMAL);
    cv::namedWindow("Edit");
    cv::moveWindow("Edit", 50, 50);
    cv::setMouseCallback("Edit", onMouse, this);

    // First start or changed number of rows/cols
    if (num_of_cols != GlobalResources::num_of_cols || num_of_rows != GlobalResources::num_of_rows){
        num_of_cols = GlobalResources::num_of_cols;
        num_of_rows = GlobalResources::num_of_rows;

        // Set initial cell sizes;
        initial_cell_width = start_width / num_of_cols;
        initial_cell_height = start_height / num_of_rows;
        for (int c = 0; c < num_of_cols; ++c) {
            cell_widths[c] = initial_cell_width;
        }
        for (int r = 0; r < num_of_rows; ++r) {
            cell_heights[r] = initial_cell_height;
        }
    }

    // Calculate width_total and height_total
    setTotalSizes();

    // Save As Button
    // Load masks
    cv::Mat mask_thin = cv::imread("images/mask_thin.png", cv::IMREAD_GRAYSCALE);
    cv::Mat mask_thick = cv::imread("images/mask_thick.png", cv::IMREAD_GRAYSCALE);
    // Create foreground
    cv::Mat foreground_highlighted = cv::Mat::zeros(40, 40, CV_8UC3);
    foreground_highlighted.setTo(HIGHLIGHT_COLOR);
    cv::Mat foreground_white = cv::Mat::zeros(40, 40, CV_8UC3);
    foreground_white.setTo(WHITE_COLOR);
    cv::Mat foreground_black = cv::Mat::zeros(40, 40, CV_8UC3);
    foreground_black.setTo(BLACK_COLOR);


    while (!close_ocv) {

        // update opencv images if needed
        if (update_ocv_images) {
            opencvReadImages();
            update_ocv_images = false;
        }

        // Stack Images
        // If we don't have combined columns
        cv::Mat stack;
        if (!GlobalResources::anyMergedCols()) {
            cv::Mat border_w = cv::Mat::zeros(1, width_total, CV_8UC3);
            cv::Mat border_w_highlighted = border_w.clone();
            border_w_highlighted.setTo(HIGHLIGHT_COLOR);
            stack = border_w.clone();
            for (int r = 0; r < num_of_rows; ++r) {
                cv::Mat stack_r;
                // If current row merged
                if (GlobalResources::merged_rows[r]) {
                     cv::Mat img_edit = createImage(0, r, "row");
                     cv::Mat border_h = cv::Mat::zeros(cell_heights[r], 1, CV_8UC3);
                     stack_r = cv::Mat::zeros(cell_heights[r], 1, CV_8UC3);
                     cv::hconcat(stack_r, img_edit, stack_r);
                     cv::hconcat(stack_r, border_h, stack_r);
                }
                else {
                    stack_r = cv::Mat::zeros(cell_heights[r], 1, CV_8UC3);
                    for (int c = 0; c < num_of_cols; ++c) {
                        cv::Mat img_edit = createImage(c, r);
                        cv::Mat border_h = cv::Mat::zeros(img_edit.rows, 1, CV_8UC3);
                        cv::Mat border_h_highlighted = border_h.clone();
                        border_h_highlighted.setTo(HIGHLIGHT_COLOR);

                        if (mouse_on_type == "grid_v" && mouse_on_num == c) {
                            cv::hconcat(stack_r, img_edit, stack_r);
                            cv::hconcat(stack_r, border_h_highlighted, stack_r);
                        }
                        else {
                            cv::hconcat(stack_r, img_edit, stack_r);
                            cv::hconcat(stack_r, border_h, stack_r);
                        }
                    }
                }
                
                if ((mouse_on_type == "grid_h") && (mouse_on_num == r)) {
                    cv::vconcat(stack, stack_r, stack);
                    cv::vconcat(stack, border_w_highlighted, stack);
                }
                else {
                    cv::vconcat(stack, stack_r, stack);
                    cv::vconcat(stack, border_w, stack);
                }
            }

        } else {    // "else" - we have at least one combined column
            cv::Mat border_h = cv::Mat::zeros(height_total, 1, CV_8UC3);
            cv::Mat border_h_highlighted = border_h.clone();
            border_h_highlighted.setTo(HIGHLIGHT_COLOR);
            stack = border_h.clone();
            for (int c = 0; c < num_of_cols; ++c) {
                cv::Mat stack_c;
                if (GlobalResources::merged_cols[c]) {
                    cv::Mat img_edit = createImage(c, 0, "col");
                    cv::Mat border_w = cv::Mat::zeros(1, cell_widths[c], CV_8UC3);
                    stack_c = cv::Mat::zeros(1, cell_widths[c], CV_8UC3);

                    cv::vconcat(stack_c, img_edit, stack_c);
                    cv::vconcat(stack_c, border_w, stack_c);
                }
                else {
                    cv::Mat border_w = cv::Mat::zeros(1, cell_widths[c], CV_8UC3);
                    cv::Mat border_w_highlighted = border_w.clone();
                    border_w_highlighted.setTo(HIGHLIGHT_COLOR);
                    stack_c = border_w.clone();
                    for (int r = 0; r < num_of_rows; ++r) {
                        cv::Mat img_edit = createImage(c, r, "none");
                        if (mouse_on_type == "grid_h" && mouse_on_num == r) {
                            cv::vconcat(stack_c, img_edit, stack_c);
                            cv::vconcat(stack_c, border_w_highlighted, stack_c);
                        }
                        else {
                            cv::vconcat(stack_c, img_edit, stack_c);
                            cv::vconcat(stack_c, border_w, stack_c);
                        }
                    }
                }
                if ((mouse_on_type == "grid_v") && (mouse_on_num == c)) {
                    cv::hconcat(stack, stack_c, stack);
                    cv::hconcat(stack, border_h_highlighted, stack);
                }
                else {
                    cv::hconcat(stack, stack_c, stack);
                    cv::hconcat(stack, border_h, stack);
                }
            }
        }

        // Set the last two rows to highlight_color
        if (mouse_on_type == "border_h") {
            stack.row(stack.rows - 2) = HIGHLIGHT_COLOR;
            stack.row(stack.rows - 1) = HIGHLIGHT_COLOR;
        }
        // Set the last two cols to highlight_color
        if (mouse_on_type == "border_v") {
            stack.col(stack.cols - 2) = HIGHLIGHT_COLOR;
            stack.col(stack.cols - 1) = HIGHLIGHT_COLOR;
        }

        // Save Button
        // Creating ROI
        cv::Rect roi_save(stack.cols - mask_thick.cols, 0, mask_thick.cols, mask_thick.rows);
        cv::Mat roi = stack(roi_save);
        // Apply masks and combine
        if (mouse_on_type == "save_button") {
            foreground_highlighted.copyTo(roi, mask_thick);
        }
        else {
            foreground_white.copyTo(roi, mask_thick);
            foreground_black.copyTo(roi, mask_thin);
        }

        // Text Free Version
        int stack_height = stack.rows;
        int stack_width = stack.cols;
        cv::Point textOrigin(25, stack_height - 8);
        std::string text = "ImageGrid v. 2.0.0 FREE VERSION. Personal and non-commercial use only.";
        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.3;
        int thickness = 1;
        cv::Scalar textColor(128, 128, 128);
        // Put text on the image
        cv::putText(stack, text, textOrigin, fontFace, fontScale, textColor, thickness, cv::LINE_AA);



        // Show Final Image 
        cv::imshow("Edit", stack);

        if ((cv::waitKey(1) & 0xFF) == 'q') {
            close_ocv = true;
        }

        // Check if the window is no longer visible
        if (cv::getWindowProperty("Edit", cv::WND_PROP_VISIBLE) < 1) {
            close_ocv = true;
        } 

    }
    cv::destroyAllWindows();
}

void OpenCV::opencvReadImages() {
    // Load images based on paths in GlobalResources
    for (int c = 0; c < GlobalResources::num_of_cols; ++c) {
        for (int r = 0; r < GlobalResources::num_of_rows; ++r) {
            images[c][r] = cv::imread(GlobalResources::getImagePath(c, r), cv::IMREAD_COLOR);
        }
    }
}

void OpenCV::setTotalSizes() {
    width_total = 0;
    height_total = 0;

    for (int c = 0; c < num_of_cols; ++c) {
        width_total += cell_widths[c];
    }
    for (int r = 0; r < num_of_rows; ++r) {
        height_total += cell_heights[r];
    }
    width_total += num_of_cols + 1;
    height_total += num_of_rows + 1;

}

void OpenCV::onMouse(int event, int x, int y, int flags, void* userdata) {
    OpenCV* self = static_cast<OpenCV*>(userdata);

    int c;
    int r;

    cout << "x: " << x << ";   y: " << y << endl;

    if (!self->resize) {
        self->mousePosition(x, y);
        c = self->mouse_on_cell[0];
        r = self->mouse_on_cell[1];
    }

    switch (event) {

        case cv::EVENT_LBUTTONDOWN:
            if (self->mouse_on_type == "cell") {
                self->move = true;
                self->start_x = x - self->tr_x[c][r];
                self->start_y = y - self->tr_y[c][r];
            }
            else if (self->mouse_on_type == "save_button") {
                // Trigger save file dialog
                self->runSaveFileDialogFromQt();
            }
            else {
                self->resize = true;
            }
            break;

        case cv::EVENT_LBUTTONUP:
            self->move = false;
            self->resize = false;
            break;

        case cv::EVENT_MOUSEMOVE:
            if (self->move) {
                self->tr_x[c][r] = x - self->start_x;
                self->tr_y[c][r] = y - self->start_y;
            }
            if (self->resize) {
                // bool block = false;
                // Resize Vertical Border
                if (self->mouse_on_type == "border_v") {
                    int delta = int((x - self->width_total) / self->num_of_cols);
                    for (int i = 0; i < self->cell_widths.size(); ++i) {
                        int new_width = self->cell_widths[i] + delta;
                        if (new_width > 40) { // Ensure the new width is above the minimum
                            self->cell_widths[i] = new_width;
                        }
                    }
                    self->setTotalSizes();
                }
                else if (self->mouse_on_type == "grid_v") {

                    if (flags == 17) {  // SHIFT pressed

                        bool block = false;
                        int width = 1;
                        std::array<int, SIZE> tmp_cell_widths = self->cell_widths;
                        int delta = 0;
                        int i;
                        int quit_loop = 0;
                        int mod = 0;

                        for (int i = 0; i <= self->mouse_on_num; ++i) {
                            width += self->cell_widths[i] + 1;
                        }

                        delta = x - width;

                        // Moving Mouse to Right
                        if (delta > 0) {
                            // Adjustments for Right Side
                            i = 0;
                            while (i < delta) {
                                self->resize_inc_right += 1;
                                mod = self->resize_inc_right % (self->num_of_cols - (self->mouse_on_num + 1));
                                int col = mod + self->mouse_on_num + 1;
                                int tmp = tmp_cell_widths[col] - 1;
                                if (tmp >= 40) {
                                    tmp_cell_widths[col] = tmp;
                                    i += 1;
                                    quit_loop = 0;

                                } else {
                                    quit_loop += 1;
                                }
                                if (quit_loop > 20) {
                                    block = true;
                                    break;
                                }
                            }
                            // Adjustments for Left Side
                            i = 0;
                            while (i < delta) {
                                self->resize_inc_left += 1;
                                int col = self->resize_inc_left % (self->mouse_on_num + 1);
                                tmp_cell_widths[col] += 1;
                                i += 1;
                            }

                        }

                        // Moving Mouse to Left
                        quit_loop = false;
                        if (delta < 0) {
                            // Adjustments for Left Side
                            i = 0;
                            while (i < abs(delta)) {
                                self->resize_inc_left += 1;
                                int col = self->resize_inc_left % (self->mouse_on_num + 1);
                                int tmp = tmp_cell_widths[col] - 1;
                                if (tmp >= 40) {
                                    tmp_cell_widths[col] = tmp;
                                    i += 1;
                                    quit_loop = 0;
                                } else {
                                    quit_loop += 1;
                                }
                                if (quit_loop > 20) {
                                    block = true;
                                    break;
                                }
                            }
                            // Adjustments for Right Side
                            i = 0;
                            while (i < abs(delta)) {
                                self->resize_inc_right += 1;
                                mod = self->resize_inc_right % (self->num_of_cols - (self->mouse_on_num + 1));
                                int col = mod + self -> mouse_on_num + 1;
                                tmp_cell_widths[col] += 1;
                                i += 1;
                            }

                        }
                       
                        if (!block) {
                            self->cell_widths = tmp_cell_widths;
                            self->setTotalSizes();
                        }
                    }
                    else {
                        int width = 1;
                        for (int i = 0; i <= self->mouse_on_num; ++i) {
                            width += self->cell_widths[i] + 1;
                        }
                        int delta = x - width;

                        int width_left = self->cell_widths[self->mouse_on_num] + delta;
                        int width_right = self->cell_widths[self->mouse_on_num + 1] - delta;
                        if ((width_left >= 40) && (width_right >= 40)) {
                            self->cell_widths[self->mouse_on_num] = width_left;
                            self->cell_widths[self->mouse_on_num + 1] = width_right;
                        }
                    }
                }
                else if (self->mouse_on_type == "border_h") {
                    // self->border_time = std::chrono::system_clock::now();
                    int delta = int((y - self->height_total) / self->num_of_rows);
                    for (int i = 0; i < self->cell_heights.size(); ++i) {
                        int new_height = self->cell_heights[i] + delta;
                        if (new_height > 40) { // Ensure the new width is above the minimum
                            self->cell_heights[i] = new_height;
                        }
                    }
                    self->setTotalSizes();
                }
                else if (self->mouse_on_type == "grid_h") {
                    int height = 1;
                    for (int i = 0; i <= self->mouse_on_num; ++i) {
                        height += self->cell_heights[i] + 1;
                    }
                    int delta = y - height;

                    int height_top = self->cell_heights[self->mouse_on_num] + delta;
                    int height_bottom = self->cell_heights[self->mouse_on_num + 1] - delta;
                    if ((height_top >= 40) && (height_bottom >= 40)) {
                        self->cell_heights[self->mouse_on_num] = height_top;
                        self->cell_heights[self->mouse_on_num + 1] = height_bottom;
                    }
                }
            }
            break;

        case cv::EVENT_MOUSEWHEEL:
            switch (flags) {
            case 7864336:
            case 15728656:
            case 23592976:
            case 31457296:
                for (auto& row : self->zoom) {
                    for (auto& z : row) {
                        if (z < 50) {
                            z *= 1.1;
                        }
                    }
                }
                break;
            case -7864304:
            case -15728624:
            case -23592944:
            case -31457264:
                for (auto& row : self->zoom) {
                    for (auto& z : row) {
                        if (z > 0.03) {
                            z /= 1.1;
                        }
                    }
                }
                break;
            case 7864320:
            case 15728640:
            case 23592960:
            case 31457280:
            case 39321600:
                if (self->zoom[c][r] < 50) {
                    self->zoom[c][r] *= 1.1;
                }
                break;
            case -7864320:
            case -15728640:
            case -23592960:
            case -31457280:
            case -39321600:
                if (self->zoom[c][r] > 0.03) {
                    self->zoom[c][r] /= 1.1;
                }
                break;
            default:
                break;
            }

    }
}

void OpenCV::mousePosition(int x, int y) {
    // Save Button
    if ((x > (width_total - 40)) && (y < 40)) {
        mouse_on_type = "save_button";
        return;
    }
    // Right Border
    if (x > width_total - 9) {
        // border_time = std::chrono::system_clock::now();
        mouse_on_type = "border_v";
        mouse_on_num = num_of_cols - 1;
        return;
    }
    // Bottom Border
    if (y > height_total - 9) {
        // border_time = std::chrono::system_clock::now();
        mouse_on_type = "border_h";
        mouse_on_num = num_of_rows - 1;
        return;
    }
    // Vertical Grid
    int width = 1;
    int height = 1;
    for (int i = 0; i < num_of_cols - 1; ++i) {
        width += cell_widths[i] + 1;
        if (width - 7 < x && x < width + 7) {
            if (GlobalResources::anyMergedRows()) {
                for (int a = 0; a < num_of_rows; ++a) {
                    height += cell_heights[a] + 1;
                    if (y < height) {
                        if (!GlobalResources::merged_rows[a]) {
                            mouse_on_type = "grid_v";
                            mouse_on_num = i;
                        }
                        return;
                    }
                }
            }
            else {
                mouse_on_type = "grid_v";
                mouse_on_num = i;
                return;
            }
        }
    }
    // Horizontal Grid
    width = 1;
    height = 1;
    for (int i = 0; i < num_of_rows - 1; ++i) {
        height += cell_heights[i] + 1;
        if (height - 7 < y && y < height + 7) {
            if (GlobalResources::anyMergedCols()) {
                for (int a = 0; a < num_of_cols; ++a) {
                    width += cell_widths[a] + 1;
                    if (x < width) {
                        if (!GlobalResources::merged_cols[a]) {
                            mouse_on_type = "grid_h";
                            mouse_on_num = i;
                        }
                        return;
                    }
                }
            }
            else {
                mouse_on_type = "grid_h";
                mouse_on_num = i;
                return;
            }
        }
    }
    // Cursor on Cell
    width = 1;
    height = 1;
    for (int r = 0; r < num_of_rows; ++r) {
        height += cell_heights[r] + 1;
        if (y < height) {
            for (int c = 0; c < num_of_cols; ++c) {
                width += cell_widths[c] + 1;
                if (x < width) {
                    mouse_on_type = "cell";
                    if (!move) {
                        if (GlobalResources::merged_rows[r]) {
                            mouse_on_cell = { 0, r };
                            return;
                        }
                        if (GlobalResources::merged_cols[c]) {
                            mouse_on_cell = { c, 0 };
                            return;
                        }
                        mouse_on_cell = { c, r };
                        return;
                    }
                }
            }
        }
    }
}

cv::Mat OpenCV::createImage(int col, int row, std::string combined, bool resize) {

    // resize = true;
    int delta = 0;
    // Local Variables for borders
    int border_top = 0, border_bottom = 0;
    int border_left = 0, border_right = 0;
    // Get the image dimensions
    int y = images[col][row].rows;
    int x = images[col][row].cols;

    int crop_x_start, crop_x_end, crop_y_start, crop_y_end;

    int width_total_no_borders = width_total - 2;
    int height_total_no_borders = height_total - 2;

    if (combined == "row") {
        crop_x_start = x / 2 - width_total_no_borders / 2 / zoom[col][row] - tr_x[col][row] / zoom[col][row];
        crop_x_end = crop_x_start + width_total_no_borders / zoom[col][row];
        crop_y_start = y / 2 - cell_heights[row] / 2 / zoom[col][row] - tr_y[col][row] / zoom[col][row];
        crop_y_end = crop_y_start + cell_heights[row] / zoom[col][row];
    }
    else if (combined == "col") {
        crop_x_start = x / 2 - cell_widths[col] / 2 / zoom[col][row] - tr_x[col][row] / zoom[col][row];
        crop_x_end = crop_x_start + cell_widths[col] / zoom[col][row];
        crop_y_start = y / 2 - height_total_no_borders / 2 / zoom[col][row] - tr_y[col][row] / zoom[col][row];
        crop_y_end = crop_y_start + height_total_no_borders / zoom[col][row];
    }
    else { // no combined rows or columns
        crop_x_start = x / 2 - cell_widths[col] / 2 / zoom[col][row] - tr_x[col][row] / zoom[col][row];
        crop_x_end = crop_x_start + cell_widths[col] / zoom[col][row];
        crop_y_start = y / 2 - cell_heights[row] / 2 / zoom[col][row] - tr_y[col][row] / zoom[col][row];
        crop_y_end = crop_y_start + cell_heights[row] / zoom[col][row];
    }

    // If image out of opencv cell -> move image back, at least 1 pixel should be visible
    if (crop_x_end < 1) {
        delta = 1 - crop_x_end;
        crop_x_start += delta;
        crop_x_end += delta;
    }
    if (crop_y_end < 1) {
        delta = 1 - crop_y_end;
        crop_y_start += delta;
        crop_y_end += delta;
    }
    if ( crop_x_start > ( x - 1 )) {
        delta = crop_x_start - (x - 1);
        crop_x_start -= delta;
        crop_x_end -= delta;
    }
    if (crop_y_start > (y - 1)) {
        delta = crop_y_start - (y - 1);
        crop_y_start -= delta;
        crop_y_end -= delta;
    }

    // Region to crop on image (without negative values)
    int crop_x_region_start = std::max(0, crop_x_start);
    int crop_x_region_end = std::min(x, crop_x_end);
    int crop_y_region_start = std::max(0, crop_y_start);
    int crop_y_region_end = std::min(y, crop_y_end);

    // Crop the image
    cv::Rect crop_region(crop_x_region_start, crop_y_region_start, crop_x_region_end - crop_x_region_start, crop_y_region_end - crop_y_region_start);
    cv::Mat img_edit = images[col][row](crop_region);

    // Add borders if needed
    if (crop_x_start < 0) border_left = std::abs(crop_x_start);
    if (crop_y_start < 0) border_top = std::abs(crop_y_start);
    if (crop_x_end > x) border_right = crop_x_end - x;
    if (crop_y_end > y) border_bottom = crop_y_end - y;

    cv::copyMakeBorder(img_edit, img_edit, border_top, border_bottom, border_left, border_right, cv::BORDER_REPLICATE);

    // Resize the image if needed
    if (resize) {
        cv::Size target_size;
        if (combined == "row") {
            target_size = cv::Size(width_total_no_borders, cell_heights[row]);
        }
        else if (combined == "col") {
            target_size = cv::Size(cell_widths[col], height_total_no_borders);
        }
        else {
            target_size = cv::Size(cell_widths[col], cell_heights[row]);
        }
        cv::resize(img_edit, img_edit, target_size, 0, 0, cv::INTER_AREA);
    }
    else {
        // out << "- No Resize -" << endl;
    }

    return img_edit;
}

cv::Mat OpenCV::createSaveImage(int col, int row, std::string combined, bool resize) {
    // Create image without resize
    cv::Mat img_edit = createImage(col, row, combined, false);

    if (resize) {
        cv::Size target_size;
        if (combined == "row") {
            /*target_size = cv::Size(width_save_total + num_of_cols - 1, cell_save_heights[row]);*/
            target_size = cv::Size(width_save_total-2, cell_save_heights[row]);
        }
        else if (combined == "col") {
            // target_size = cv::Size(cell_save_widths[col], height_save_total + num_of_rows - 1);
            target_size = cv::Size(cell_save_widths[col], height_save_total-2);
        }
        else {
            target_size = cv::Size(cell_save_widths[col], cell_save_heights[row]);
        }
        cv::resize(img_edit, img_edit, target_size, 0, 0, cv::INTER_AREA);

    }

    return  img_edit;

}

void OpenCV::saveImage(std::string path) {

    std::vector <double> cell_scale_coefs;
    cv::Mat img_edit;

    // Calculation of a scale coefficients of all visible cells
    // If we don't have combined columns (Nothing combined or rows combined)
    if (!GlobalResources::anyMergedCols()) {
        // Loop all rows
        for (int row = 0; row < num_of_rows; row++) {
            // If current row merged
            if (GlobalResources::merged_rows[row]) {
                img_edit = createImage(0, row, "row", false);
                cell_scale_coefs.push_back(double(img_edit.rows) / double(cell_heights[row]));
            }
            else {
                for (int col = 0; col < num_of_cols; col++) {
                    img_edit = createImage(col, row, "none", false);
                    cell_scale_coefs.push_back(double(img_edit.rows) / double(cell_heights[row]));
                }
            }

        }
    }
    else {  // if we have combined columns
        for (int col = 0; col < num_of_cols; col++) {
            // If current collumn combined
            if (GlobalResources::merged_cols[col]) {
                img_edit = createImage(col, 0, "col", false);
                cell_scale_coefs.push_back(double(img_edit.cols) / double(cell_widths[col]));
            }
            else {
                for (int row = 0; row < num_of_rows; row++) {
                    img_edit = createImage(col, row, "none", false);
                    cell_scale_coefs.push_back(double(img_edit.cols) / double(cell_widths[col]));
                }
            }
        }
    }

    // Calculation of global list vars "cell_save_widths[]" and "cell_save_heights[]"
    auto max_scale_coef = std::max_element(cell_scale_coefs.begin(), cell_scale_coefs.end());
    if ((width_total * *max_scale_coef) < GlobalResources::SAVE_RESOLUTION_WIDTH) {
        for (int i = 0; i < cell_widths.size(); i++) {
            cell_save_widths[i] = int(cell_widths[i] * *max_scale_coef);
        }
        for (int i = 0; i < cell_heights.size(); i++) {
            cell_save_heights[i] = int(cell_heights[i] * *max_scale_coef);
        }
    }
    else {
        double resolution_coef = double(GlobalResources::SAVE_RESOLUTION_WIDTH) / double(width_total);
        for (int i = 0; i < cell_widths.size(); i++) {
            cell_save_widths[i] = int(cell_widths[i] * resolution_coef);
        }
        for (int i = 0; i < cell_heights.size(); i++) {
            cell_save_heights[i] = int(cell_heights[i] * resolution_coef);
        }
    }

    // Calculate global variables "width_save_total" and "height_save_total"
    width_save_total = 0;
    for (int i = 0; i < num_of_cols; i++) {
        width_save_total += cell_save_widths[i];   //ERROR ARRAY
    }
    height_save_total = 0;
    for (int i = 0; i < num_of_rows; i++) {
        height_save_total += cell_save_heights[i];
    }
    width_save_total += num_of_cols + 1;
    height_save_total += num_of_rows + 1;


    // Stack Images
    // If we don't have combined columns
    cv::Mat stack;
    if (!GlobalResources::anyMergedCols()) {
        cv::Mat border_w = cv::Mat::zeros(1, width_save_total, CV_8UC3);
        stack = border_w.clone();
        for (int r = 0; r < num_of_rows; ++r) {
            cv::Mat stack_r;
            // If current row merged
            if (GlobalResources::merged_rows[r]) {
                cv::Mat img_edit = createSaveImage(0, r, "row", true);
                cv::Mat border_h = cv::Mat::zeros(cell_save_heights[r], 1, CV_8UC3);
                stack_r = cv::Mat::zeros(cell_save_heights[r], 1, CV_8UC3);
                cv::hconcat(stack_r, img_edit, stack_r);
                cv::hconcat(stack_r, border_h, stack_r);
            }
            else {  // If current row not merged
                stack_r = cv::Mat::zeros(cell_save_heights[r], 1, CV_8UC3);
                for (int c = 0; c < num_of_cols; ++c) {
                    cv::Mat img_edit = createSaveImage(c, r, "none", true);
                    cv::Mat border_h = cv::Mat::zeros(img_edit.rows, 1, CV_8UC3);
                    cv::hconcat(stack_r, img_edit, stack_r);
                    cv::hconcat(stack_r, border_h, stack_r);
                }
            }
            cv::vconcat(stack, stack_r, stack);
            cv::vconcat(stack, border_w, stack);
        }
    }
    else {    // "else" - we have at least one combined column
        cv::Mat border_h = cv::Mat::zeros(height_save_total, 1, CV_8UC3);
        stack = border_h.clone();
        for (int c = 0; c < num_of_cols; ++c) {
            cv::Mat stack_c;
            if (GlobalResources::merged_cols[c]) {
                cv::Mat img_edit = createSaveImage(c, 0, "col", true);
                cv::Mat border_w = cv::Mat::zeros(1, cell_save_widths[c], CV_8UC3);
                stack_c = cv::Mat::zeros(1, cell_save_widths[c], CV_8UC3);
                cv::vconcat(stack_c, img_edit, stack_c);
                cv::vconcat(stack_c, border_w, stack_c);
            }
            else {
                cv::Mat border_w = cv::Mat::zeros(1, cell_save_widths[c], CV_8UC3);
                stack_c = border_w.clone();
                for (int r = 0; r < num_of_rows; ++r) {
                    cv::Mat img_edit = createSaveImage(c, r, "none", true);
                    cv::vconcat(stack_c, img_edit, stack_c);
                    cv::vconcat(stack_c, border_w, stack_c);
                }
            }
            cv::hconcat(stack, stack_c, stack);
            cv::hconcat(stack, border_h, stack);
        }
    }

    // Text Free Version
    int stack_height = stack.rows;
    int stack_width = stack.cols;
    cv::Point textOrigin(25, stack_height - 8);
    std::string text = "ImageGrid v. 2.0.0 FREE VERSION. Personal and non-commercial use only.";
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.3;
    int thickness = 1;
    cv::Scalar textColor(128, 128, 128);
    // Put text on the image
    cv::putText(stack, text, textOrigin, fontFace, fontScale, textColor, thickness, cv::LINE_AA);

    cv::imwrite(path, stack);

}

void OpenCV::runSaveFileDialogFromQt() {
    QWidget* mainWindow = qApp->property("mainWindow").value<QWidget*>();
    QtWindow* window = qobject_cast<QtWindow*>(mainWindow);
    window->saveFileDialog();
}