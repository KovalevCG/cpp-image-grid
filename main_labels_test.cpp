// TEST :: IMAGES IN LABELS

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>

// Console Output
#include <fstream>
#include <Windows.h>
#include <iostream>



// #include <QPainter>
#include <QResizeEvent>

class ImageLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImageLabel(QWidget* parent = nullptr) : QLabel(parent) {}

    void setOriginalPixmap(const QPixmap& pixmap) {
        originalPixmap = pixmap;
        updatePixmap();
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        QLabel::resizeEvent(event);
        updatePixmap(); // Update the pixmap on resize
    }

private:
    QPixmap originalPixmap;

    void updatePixmap() {
        if (!originalPixmap.isNull()) {
            QPixmap scaledPixmap = originalPixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            // Now crop the pixmap to make sure it fits the label size
            int xOffset = (scaledPixmap.width() - this->width()) / 2;
            int yOffset = (scaledPixmap.height() - this->height()) / 2;
            QPixmap croppedPixmap = scaledPixmap.copy(xOffset, yOffset, this->width(), this->height());
            this->setPixmap(croppedPixmap);
        }
    }
};


int main_(int argc, char* argv[]) {

    // Console Output
    AllocConsole();
    FILE* stream;
    stream = freopen("CONOUT$", "w", stdout);
    stream = freopen("CONOUT$", "w", stderr);
    std::cout << "Console output enabled.\n";


    // Qt Application
    QApplication app(argc, argv);
    QMainWindow window;
    window.setFixedSize(450, 450);

    // Create a container widget
    QWidget* centralWidget = new QWidget(&window);

    // Set the layout for the container
    QVBoxLayout* layout_main = new QVBoxLayout;
    centralWidget->setLayout(layout_main);


    const QSize fixedSize(200, 200);

    // FIRST IMAGES LAYOUT
    // layout
    QHBoxLayout* layout_images_1 = new QHBoxLayout;
    // labes
    ImageLabel* label1 = new ImageLabel;
    QPixmap pixmap("test/01.jpg");
    label1->setOriginalPixmap(pixmap);
    //label1->setScaledContents(true);
    // label1->setFixedSize(100,100); // Set the desired size of the label (cell)
    // label1->setAlignment(Qt::AlignCenter);
    QLabel* label2 = new QLabel;
    label2->setFixedSize(fixedSize);
    label2->setAlignment(Qt::AlignCenter);
    pixmap = QPixmap("test/02.jpg");
    QPixmap scaled_pixmap;
    scaled_pixmap = pixmap.scaled(label2->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    // label2->setScaledContents(false);
    label2->setPixmap(scaled_pixmap);
    // add labels to layout
    layout_images_1->addWidget(label1);
    layout_images_1->addWidget(label2);

    // SECONR IMAGES LAYOUT
    // layout
    QHBoxLayout* layout_images_2 = new QHBoxLayout;
    // labes
    QLabel* label3 = new QLabel;
    pixmap = QPixmap("test/03.jpg");
    label3->setPixmap(pixmap);
    // label1->setScaledContents(true);
    QLabel* label4 = new QLabel;
    // label4->setScaledContents(false);
    pixmap = QPixmap("test/04.jpg");
    label4->setPixmap(pixmap.scaled(QSize(200, 200), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    // add labels to layout
    layout_images_2->addWidget(label3);
    layout_images_2->addWidget(label4);


    layout_main->addLayout(layout_images_1);
    layout_main->addLayout(layout_images_2);

    window.setCentralWidget(centralWidget);

    //window.resize(100, 100);


    window.show();
    return app.exec();
}