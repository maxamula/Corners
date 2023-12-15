#include <opencv2/opencv.hpp>
#include <functional>
#include <vector>

#define FAST_THERHOLD 20
#define FAST_INITIAL_CHECK_EXCEEDS_REQUIRED 0
#define FAST_STREAK_REQUIRED 9

int main()
{
    cv::Mat image = cv::imread("C:\\Users\\nullptr\\Desktop\\lena.png");

    if (image.empty()) 
    {
        std::cerr << "Error loading image!" << std::endl;
        return -1;
    }

    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    
    static cv::Vec2i offsets[] = 
    {
        {0, -3}, {1, -3}, {2, -2}, {3, -1}, {3, 0}, {3, 1}, {2, 2}, {1, 3},
        {0, 3}, {-1, 3}, {-2, 2}, {-3, 1}, {-3, 0}, {-3, -1}, {-2, -2}, {-1, -3}
    };

    static std::function<bool(uchar, uchar)> exceedChecks[] =
    {
        [](uchar offsetPixel, uchar targetPixel) { return offsetPixel > targetPixel + FAST_THERHOLD; },
        [](uchar offsetPixel, uchar targetPixel) { return offsetPixel < targetPixel - FAST_THERHOLD; },
    };

    std::vector<cv::KeyPoint> corners;
    for (int y = 3; y < grayImage.rows - 3; y += 3)
    {
        for (int x = 3; x < grayImage.cols - 3; x += 3)
        {
            uchar pixel = grayImage.at<uchar>(y, x);
            // Initial check
            uchar compass[] = { 1, 5, 9, 13 };
            for (uchar checkIndex = 0; checkIndex < _countof(exceedChecks); checkIndex++)
            {
                uchar numExceeds = 0u;
                for (uchar compassDir = 0u; compassDir < _countof(compass); compassDir++)
                    if (exceedChecks[checkIndex](grayImage.at<uchar>(y + offsets[compass[compassDir]][1], x + offsets[compass[compassDir]][0]), pixel))
                        numExceeds++;
                if (numExceeds > FAST_INITIAL_CHECK_EXCEEDS_REQUIRED)
                {
                    uchar streak = 0;
                    for (uchar offset = 0; offset < _countof(offsets) + FAST_STREAK_REQUIRED - 1; offset++)
                    {
                        if (streak == FAST_STREAK_REQUIRED)
                        {
                            corners.emplace_back(x, y, 5);
                            break;
                        }
                            
                        if (exceedChecks[checkIndex](grayImage.at<uchar>(y + offsets[offset % _countof(offsets)][1], x + offsets[offset % _countof(offsets)][0]), pixel))
                            streak++;
                        else
                            streak = 0;
                    }
                    break;
                }
            }
        }
    }

    cv::Mat resultImage;
    cv::drawKeypoints(image, corners, resultImage, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    cv::imshow("FAST Corners", resultImage);
    cv::waitKey(0);
    return 0;
}
