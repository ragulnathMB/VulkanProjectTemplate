//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <string>
//#define NOMINMAX
//
//#ifdef _WIN32
//#include <windows.h>
//#include <commdlg.h>
//#else
//#include <cstdlib>
//#endif
//
//class BucketFillTool {
//private:
//    cv::Mat image;
//    cv::Mat originalImage;
//    std::string windowName;
//    cv::Scalar fillColor;
//    int tolerance;
//
//public:
//    BucketFillTool() :
//        windowName("Engineering Drawing - Bucket Fill Tool"),
//        fillColor(128, 0, 128), // Purple in BGR
//        tolerance(10) {
//    }
//
//    // Convert wide string (wchar_t*) to UTF-8 std::string
//    std::string WideToUTF8(const std::wstring& wstr) {
//        if (wstr.empty()) return {};
//        int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
//        std::string result(size, 0);
//        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
//        return result;
//    }
//
//    std::string openFileDialog() {
//        std::string filename = "";
//
//#ifdef _WIN32
//        OPENFILENAMEW ofn;
//        wchar_t szFile[260] = { 0 };
//
//        ZeroMemory(&ofn, sizeof(ofn));
//        ofn.lStructSize = sizeof(ofn);
//        ofn.lpstrFile = szFile;
//        ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
//        ofn.lpstrFilter = L"Image Files\0*.png;*.jpg;*.jpeg;*.bmp;*.tiff;*.tif\0All Files\0*.*\0";
//        ofn.nFilterIndex = 1;
//        ofn.lpstrFileTitle = NULL;
//        ofn.nMaxFileTitle = 0;
//        ofn.lpstrInitialDir = NULL;
//        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//        ofn.lpstrTitle = L"Select Engineering Drawing";
//
//        if (GetOpenFileNameW(&ofn)) {
//            filename = WideToUTF8(szFile);
//        }
//#else
//        std::cout << "Please enter the full path to your image file: ";
//        std::getline(std::cin, filename);
//#endif
//        return filename;
//    }
//
//    bool loadImage() {
//        std::string filepath = openFileDialog();
//
//        if (filepath.empty()) {
//            std::cout << "No file selected." << std::endl;
//            return false;
//        }
//
//        image = cv::imread(filepath);
//        if (image.empty()) {
//            std::cout << "Error: Could not load image from " << filepath << std::endl;
//            return false;
//        }
//
//        originalImage = image.clone();
//        std::cout << "Image loaded successfully: " << filepath << std::endl;
//        std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;
//        return true;
//    }
//
//    void floodFill(int x, int y) {
//        if (image.empty()) return;
//
//        cv::Mat mask = cv::Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
//
//        cv::floodFill(image, mask, cv::Point(x, y), fillColor,
//            nullptr, cv::Scalar(tolerance, tolerance, tolerance),
//            cv::Scalar(tolerance, tolerance, tolerance),
//            cv::FLOODFILL_FIXED_RANGE);
//
//
//        std::cout << "Filled region at position (" << x << ", " << y << ")" << std::endl;
//    }
//
//    static void onMouse(int event, int x, int y, int flags, void* userdata) {
//        BucketFillTool* tool = static_cast<BucketFillTool*>(userdata);
//
//        if (event == cv::EVENT_LBUTTONDOWN) {
//            tool->floodFill(x, y);
//            cv::imshow(tool->windowName, tool->image);
//        }
//        else if (event == cv::EVENT_RBUTTONDOWN) {
//            if (!tool->originalImage.empty()) {
//                tool->image = tool->originalImage.clone();
//                cv::imshow(tool->windowName, tool->image);
//                std::cout << "Image reset to original" << std::endl;
//            }
//        }
//    }
//
//    static void onTrackbar(int value, void* userdata) {
//        BucketFillTool* tool = static_cast<BucketFillTool*>(userdata);
//        tool->tolerance = value;
//        std::cout << "Tolerance set to: " << tool->tolerance << std::endl;
//    }
//
//    void saveImage() {
//        if (image.empty()) return;
//
//        std::string filename;
//        std::cout << "Enter filename to save (with extension, e.g., output.png): ";
//        std::getline(std::cin, filename);
//
//        if (!filename.empty()) {
//            if (cv::imwrite(filename, image)) {
//                std::cout << "Image saved successfully: " << filename << std::endl;
//            }
//            else {
//                std::cout << "Error saving image!" << std::endl;
//            }
//        }
//    }
//
//    void run() {
//        std::cout << "=== Engineering Drawing Bucket Fill Tool ===\n";
//        std::cout << "Instructions:\n";
//        std::cout << "1. Select an engineering drawing image file\n";
//        std::cout << "2. Left click on any part to fill with purple\n";
//        std::cout << "3. Right click to reset to original image\n";
//        std::cout << "4. Use tolerance trackbar to adjust sensitivity\n";
//        std::cout << "5. Press 's' to save the modified image\n";
//        std::cout << "6. Press 'r' to reset image\n";
//        std::cout << "7. Press 'q' or ESC to quit\n";
//        std::cout << "=============================================\n";
//
//        if (!loadImage()) {
//            std::cout << "Exiting..." << std::endl;
//            return;
//        }
//
//        cv::namedWindow(windowName, cv::WINDOW_NORMAL);
//        cv::resizeWindow(windowName, 1200, 800);
//
//        cv::setMouseCallback(windowName, onMouse, this);
//
//        cv::createTrackbar("Tolerance", windowName, &tolerance, 50, onTrackbar, this);
//
//        cv::imshow(windowName, image);
//
//        while (true) {
//            int key = cv::waitKey(30) & 0xFF;
//            if (key == 'q' || key == 27) break;
//            else if (key == 's') saveImage();
//            else if (key == 'r') {
//                if (!originalImage.empty()) {
//                    image = originalImage.clone();
//                    cv::imshow(windowName, image);
//                    std::cout << "Image reset to original" << std::endl;
//                }
//            }
//            else if (key == '+' || key == '=') {
//                tolerance = std::min(50, tolerance + 1);
//                cv::setTrackbarPos("Tolerance", windowName, tolerance);
//            }
//            else if (key == '-') {
//                tolerance = std::max(0, tolerance - 1);
//                cv::setTrackbarPos("Tolerance", windowName, tolerance);
//            }
//        }
//
//        cv::destroyAllWindows();
//        std::cout << "Application closed." << std::endl;
//    }
//};
//
//int main() {
//    try {
//        BucketFillTool tool;
//        tool.run();
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//        return -1;
//    }
//
//    return 0;
//}
