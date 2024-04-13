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

void OpenCV::startOpencvMainLoop() {

    bool close_ocv = false;
    bool update_ocv_images = true;

    cv::namedWindow("Edit", cv::WINDOW_AUTOSIZE);
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

    // this->merged_cols = GlobalResources::merged_cols;
    // this->merged_rows = GlobalResources::merged_rows;

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
            // cv::Mat border_w_highlighted = border_w.clone();
            // border_w_highlighted.setTo(highlight_color);
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
                        cv::Mat img_edit = createImage(c, r); // As above
                        cv::Mat border_h = cv::Mat::zeros(img_edit.rows, 1, CV_8UC3);
                        //cv::Mat border_h_highlighted = border_h.clone();
                        //border_h_highlighted.setTo(highlight_color);

                        //if (mouseOnType == "grid_v" && mouseOnNum == col) {
                        //    cv::hconcat(stack_r, img_edit, stack_r); // Append horizontally
                        //    cv::hconcat(stack_r, border_h_highlighted, stack_r);
                        //}
                        /*else {*/
                        // cv::hconcat(stack_r, border_h, stack_r);
                        cv::hconcat(stack_r, img_edit, stack_r);
                        cv::hconcat(stack_r, border_h, stack_r);
                        /*}*/
                    }
                }
                
                //if (mouseOnType == "grid_h" && mouseOnNum == row) {
                //    cv::vconcat(stack, stack_r, stack); // Append vertically
                //    cv::vconcat(stack, border_w_highlighted, stack);
                //}
                //else {

                cv::vconcat(stack, stack_r, stack);
                cv::vconcat(stack, border_w, stack);
                
                //}
            }

        } else {    // "else" - we have at least one combined column
            cv::Mat border_h = cv::Mat::zeros(height_total, 1, CV_8UC3);
            // cv::Mat border_h_highlighted = border_h.clone();
            // border_h_highlighted.setTo(highlight_color);
            stack = border_h.clone();
            for (int c = 0; c < num_of_cols; ++c) {
                cv::Mat stack_c;
                if (GlobalResources::merged_cols[c]) {
                    cv::Mat img_edit = createImage(c, 0, "col");
                    cv::Mat border_w = cv::Mat::zeros(1, cell_widths[c], CV_8UC3);
                    stack_c = cv::Mat::zeros(1, cell_widths[c], CV_8UC3);
                    // std::cout << "stack_c x: " << stack_c.cols << "; stack_c y: " << stack_c.rows << std::endl;
                    // std::cout << "img_edit x: " << img_edit.cols << "; img_edit y: " << img_edit.rows << std::endl;
                    
                    cv::vconcat(stack_c, img_edit, stack_c);
                    cv::vconcat(stack_c, border_w, stack_c);
                }
                else {
                    cv::Mat border_w = cv::Mat::zeros(1, cell_widths[c], CV_8UC3);
                    //cv::Mat border_w_highlighted = border_w.clone();
                    //border_w_highlighted.setTo(highlight_color);
                    stack_c = border_w.clone();
                    for (int r = 0; r < num_of_rows; ++r) {
                        cv::Mat img_edit = createImage(c, r, "none");
                        //if (mouseOnType == "grid_h" && mouseOnNum == r) {
                        //    cv::vconcat(stack_c, img_edit, stack_c);
                        //    cv::vconcat(stack_c, border_w_highlighted, stack_c);
                        //}
                        //else {
                        cv::vconcat(stack_c, img_edit, stack_c);
                        cv::vconcat(stack_c, border_w, stack_c);
                        //}
                    }
                }
                /*if (mouseOnType == "grid_v" && mouseOnNum == c) {
                    cv::hconcat(stack, stack_c, stack);
                    cv::hconcat(stack, border_h_highlighted, stack);
                }
                else {*/
                cv::hconcat(stack, stack_c, stack);
                cv::hconcat(stack, border_h, stack);
                /*}*/
            }
        }

        // std::cout << "stack x: " << stack.cols << "; stack y: " << stack.rows << std::endl;
        cv::imshow("Edit", stack);


        //if (cv::waitKey(30) >= 0) {
        //    close_ocv = true;
        //    std::cout << "waitKey(30) >= 0";
        //}
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
        // std::cout << "LMB Down" << std::endl;
        // self->mousePosition(x, y);
        // cout << "Type: " << self->mouse_on_type << "; Num: " << self->mouse_on_num << endl;
        // cout << "mouse_on_cell COL: " << self->mouse_on_cell[0] << "; mouse_on_cell ROW : " << self->mouse_on_cell[1] << endl;
        if (self->mouse_on_type == "cell") {
            self->move = true;

            self->start_x = x - self->tr_x[c][r];
            self->start_y = y - self->tr_y[c][r];
        }
        else if (self->mouse_on_type == "save_button") {
            // Trigger save file dialog
            // self->saveFileDialog();
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
            int c = self->mouse_on_cell[0];
            int r = self->mouse_on_cell[1];
            self->tr_x[c][r] = x - self->start_x;
            //if (self->tr_x[c][r] >= self->images[c][r].cols) {
            //    self->tr_x[c][r] = self->images[c][r].cols - 5;
            //}
            self->tr_y[c][r] = y - self->start_y;
        }
        //if (self->resize) {
        //    if (self->mouse_on_type == "border_v") {
        //        auto delta = int((x - self->width_total) / self->num_of_cols);
        //        self->adjustWidths(delta, flags & cv::EVENT_FLAG_SHIFTKEY);
        //    }
        //    else if (self->mouse_on_type == "border_h") {
        //        auto delta = int((y - self->height_total) / self->num_of_rows);
        //        self->adjustHeights(delta);
        //    }
        //}
        break;
    case cv::EVENT_MOUSEWHEEL:
        cout << "scroll flags: " << flags << "  zoom: " << self->zoom[0][0] <<endl;
        if (flags == 7864336) {  // Zoom up all
            cout << "UP ALL" << endl;
            for (auto& row : self->zoom) {
                for (auto& z : row) {
                    z *= 1.1;
                }
            }
        }
        else if (flags == -7864304) {  // Zoom down all
            cout << "DOWN ALL" << endl;
            for (auto& row : self->zoom) {
                for (auto& z : row) {
                    z /= 1.1;
                }
            }
        }
        if (flags == 7864320) {  // Zoom up current
            cout << "UP CURRENT" << endl;
            self->zoom[c][r] *= 1.1;
        }
        else if (flags == -7864320) {  // Zoom down current
            cout << "DOWN CURRENT" << endl;
            self->zoom[c][r] /= 1.1;
        }
        break;
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

    resize = true;
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
        int delta = 1 - crop_x_end;
        // cout << "delta" << delta << "crop_x_start" << crop_x_start << "crop_x_end" << crop_x_end << endl;
        crop_x_start += delta;
        crop_x_end += delta;
        // cout << "delta" << delta << "crop_x_start" << crop_x_start << "crop_x_end" << crop_x_end << endl;
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
        if (combined == "rows") {
            target_size = cv::Size(width_total, cell_heights[row]);
        }
        else if (combined == "columns") {
            target_size = cv::Size(cell_widths[col], height_total);
        }
        else {
            target_size = cv::Size(cell_widths[col], cell_heights[row]);
        }
        cv::resize(img_edit, img_edit, target_size, 0, 0, cv::INTER_AREA);
        // cout << "rows resize: " << img_edit.rows << "; cols resize: " << img_edit.cols << endl;
    }

    return img_edit;
}