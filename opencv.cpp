#include "opencv.h"
#include "global-resources.h"

#include <chrono>

using std::cout;
using std::endl;

// Constructor
OpenCV::OpenCV() {

    for (auto& row : zoom) { // Initialize all elements to 1.0
        row.fill(1.0);
    }

}

// Main Loop
void OpenCV::startOpencvMainLoop() {

    bool close_ocv = false;
    bool update_ocv_images = true;

    // cv::namedWindow("Edit", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Edit", cv::WINDOW_AUTOSIZE | cv::WINDOW_GUI_NORMAL);
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
    // ---------------

    while (!close_ocv) {

        // update opencv images if needed
        if (update_ocv_images) {
            std::cout << "--- Update Images Start ---" << std::endl;
            opencvReadImages();
            update_ocv_images = false;
        }

        // Stack Images
        // If we don't have combined columns

        cv::Mat stack;

        if (!GlobalResources::anyMergedCols()) {
            cv::Mat border_w = cv::Mat::zeros(1, width_total, CV_8UC3);
            cv::Mat border_w_highlighted = border_w.clone();
            border_w_highlighted.setTo(highlight_color);
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
                        border_h_highlighted.setTo(highlight_color);

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
            border_h_highlighted.setTo(highlight_color);
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
                    border_w_highlighted.setTo(highlight_color);
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
            stack.row(stack.rows - 2) = highlight_color;
            stack.row(stack.rows - 1) = highlight_color;
        }
        // Set the last two cols to highlight_color
        if (mouse_on_type == "border_v") {
            stack.col(stack.cols - 2) = highlight_color;
            stack.col(stack.cols - 1) = highlight_color;
        }

        cv::imshow("Edit", stack);

        if ((cv::waitKey(1) & 0xFF) == 'q') {
            close_ocv = true;  // Assuming close_ocv is the equivalent boolean controlling the loop.
        }

        // Check if the window is no longer visible
        if (cv::getWindowProperty("Edit", cv::WND_PROP_VISIBLE) < 1) {
            close_ocv = true;
        } 

    }
    cv::destroyAllWindows();
}

void OpenCV::onMouse(int event, int x, int y, int flags, void* userdata) {
    OpenCV* self = static_cast<OpenCV*>(userdata);

    int c;
    int r;

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
                cout << "SAVE" << endl;
                // Trigger save file dialog
                // self->saveFileDialog();
            }
            else {
                self->resize = true;
                cout << "RESIZE START" << endl;
            }
            break;

        case cv::EVENT_LBUTTONUP:
            self->move = false;
            self->resize = false;
            cout << "STOP" << endl;
            break;

        case cv::EVENT_MOUSEMOVE:
            if (self->move) {
                // cout << "mousemove && move" << endl;
                self->tr_x[c][r] = x - self->start_x;
                self->tr_y[c][r] = y - self->start_y;
            }
            if (self->resize) {
                // bool block = false;
                // Resize Vertical Border
                if (self->mouse_on_type == "border_v") {
                    // self->border_time = std::chrono::system_clock::now();
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
                        std::array<int, 20> tmp_cell_widths = self->cell_widths;
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
                                // mod = self->resize_inc_right % (self->num_of_cols - (self->mouse_on_num + 1));
                                // cout << self->resize_inc_right << "mod: " << mod << endl;
                                // int col = mod + self->mouse_on_num + 1;
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



                        //int left_index = self->mouse_on_num;
                        //int right_index = left_index + 1 < self->cell_widths.size() ? left_index + 1 : left_index;

                        //if (self->cell_widths[left_index] + delta > 40 && self->cell_widths[right_index] - delta > 40) {
                        //    self->cell_widths[left_index] += delta;
                        //    self->cell_widths[right_index] -= delta;
                        //}
                        // self->setTotalSizes();
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
        border_time = std::chrono::system_clock::now();
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

void OpenCV::opencvReadImages() {
    // Load images based on paths in GlobalResources
    for (int c = 0; c < GlobalResources::num_of_cols; ++c) {
        for (int r = 0; r < GlobalResources::num_of_rows; ++r) {
            std::cout << GlobalResources::getImagePath(c, r) << std::endl;
            images[c][r] = cv::imread(GlobalResources::getImagePath(c, r), cv::IMREAD_COLOR);
        }
    }
}

void OpenCV::setTotalSizes() {
    width_total = 0;
    height_total = 0;

    for (int c = 0; c < GlobalResources::num_of_cols; ++c) {
        width_total += cell_widths[c];
    }
    for (int r = 0; r < GlobalResources::num_of_rows; ++r) {
        height_total += cell_heights[r];
    }
    width_total += GlobalResources::num_of_cols + 1;
    height_total += GlobalResources::num_of_rows + 1;

    // std::cout << "width_total : " << width_total << ";   height_total : " << height_total << std::endl;
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
        // cout << "rows resize: " << img_edit.rows << "; cols resize: " << img_edit.cols << endl;
    }

    return img_edit;
}