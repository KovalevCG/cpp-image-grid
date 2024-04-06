#include "image-label.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPixmap>


ImageLabel::ImageLabel(int col, int row, QWidget* parent) : QLabel(parent), label_col(col), label_row(row) {

    setAcceptDrops(true);
    setStyleSheet("QLabel { border: 4px dashed #aaa;}");
    setAlignment(Qt::AlignCenter);
}

void ImageLabel::setImage() {
    std::string path = GlobalResources::getImagePath(label_col, label_row);
    if (path == GlobalResources::bg_path_opencv) {
        path = GlobalResources::bg_path_qt;
        QString img_qstr_path = QString::fromStdString(path);
        QPixmap icon(img_qstr_path);
        setPixmap(icon);
    }
    else {
        QString img_qstr_path = QString::fromStdString(path);
        QPixmap icon(img_qstr_path);
        QPixmap scaledPixmap = icon.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }
}

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
    }
    else if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
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
            auto url = urls.first();
            QImage image(url.toLocalFile());
            if (!image.isNull()) {
                // setPixmap(QPixmap::fromImage(image).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                GlobalResources::setImagePath(getCol(), getRow(), mimeData->urls().first().toLocalFile().toStdString());
                ImageLabel::setImage();
            }
        }
        event->acceptProposedAction();
    }
}