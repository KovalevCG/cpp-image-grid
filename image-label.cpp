#include "image-label.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QImage>
#include <QPixmap>

#include <iostream>
using namespace std;
//#include <Windows.h>



ImageLabel::ImageLabel(int col, int row, QWidget* parent) : QLabel(parent), label_col(col), label_row(row) {

    setAcceptDrops(true);
    // setStyleSheet("QLabel { border: 2px dashed gray; background-color: rgba(255, 255, 255, 0.5); }");
    setStyleSheet("QLabel { border: 4px dashed #aaa; background-color: rgba(255, 255, 255, 0.5); }");
    // setMinimumSize(100, 100); // Ensure the label is visible
    // setFixedSize(200, 150);
    // setScaledContents(true);
    // setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    // setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setAlignment(Qt::AlignCenter);
    // setText("Drop Image Here");
    setImage();
}

//void ImageLabel::setPosition(int col, int row) {
//    label_col = col;
//    label_row = row;
//}

void ImageLabel::setImage() {
    QString img_qstr_path = QString::fromStdString(GlobalResources::getImagePath(label_col, label_row));
    QPixmap icon(img_qstr_path);
    // setScaledContents(true);
    // Scale pixmap to fill the fixed size of the label, maintaining aspect ratio without expanding.
    QPixmap scaledPixmap = icon.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    setPixmap(scaledPixmap);
    // setPixmap(icon);
}
    // setPixmap(icon.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    // setAlignment(Qt::AlignCenter);



//void ImageLabel::resizeEvent(QResizeEvent* event) {
//
//    QLabel::resizeEvent(event);
//    setImage(); // Update the pixmap on resize
//
//    std::cout << getCol() << " " << getRow() << " Updated;" << std::endl;
//}

int ImageLabel::getCol() {
    return label_col;
}

int ImageLabel::getRow() {
    return label_row;
}

void ImageLabel::dragEnterEvent(QDragEnterEvent* event) {
    // Accept the drag event if it contains image data
    if (event->mimeData()->hasImage()) {
        // I should remove this later
        event->acceptProposedAction();
        std::cout << "hasImage() Drag" << std::endl;

    }
    else if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        std::cout << "hasUrls() Drag" << std::endl;
    }
}

void ImageLabel::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasImage()) {
        // I should remove this later
        //std::cout << "hasImage() Drop" << std::endl;
        // Direct image data (e.g., from another application)
        //auto image = qvariant_cast<QImage>(mimeData->imageData());
        //setPixmap(QPixmap::fromImage(image).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        //event->acceptProposedAction();
    }
    else if (mimeData->hasUrls()) {
        //std::cout << "hasUrls() Drop" << std::endl;
        // Image file dropped
        // 
        //event.setDropAction(QtCore.Qt.CopyAction)
        //file_path = event.mimeData().urls()[0].toLocalFile()
        //self.set_image(file_path)
        //self.assignImagePath(self.objectName(), file_path)
        //event.accept()
        //update_ocv_images = True


        auto urls = mimeData->urls();
        if (!urls.isEmpty()) {
            // Assuming only the first URL is relevant
            // cout << "urls: " << typeid(urls.first()).name() << endl;
            cout << "urls: " << mimeData->urls().first().toLocalFile().toStdString() << endl;
            cout << "Col: " << getCol() << endl;
            cout << "Row: " << getRow() << endl;
            auto url = urls.first();
            QImage image(url.toLocalFile());
            if (!image.isNull()) {
                // setPixmap(QPixmap::fromImage(image).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                GlobalResources::setImagePath(getCol(), getRow(), mimeData->urls().first().toLocalFile().toStdString());
                ImageLabel::setImage();
                //cout << "img_paths[0][0]: " << globalResources.getImagePath(0, 0) << endl;
                //cout << "img_paths[1][0]: " << globalResources.getImagePath(1, 0) << endl;
            }
        }
        event->acceptProposedAction();
    }
}