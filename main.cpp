#include <iostream>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>

float getLineSlope(cv::Vec4i& line) {
    float yDiff = (line[3]-line[1]);
    float xDiff = (line[2]-line[0]);

    if (xDiff == 0) {
        xDiff = (-1 + 1) + (((float) rand()) / (float) RAND_MAX) * (1 - (-1 + 1));
    }

    float slope = (yDiff/xDiff);
    return slope;
}

float getLineLength(cv::Vec4i& line) {
    // Use Pythagoras theorem to calculate distance
    float xDistance = pow((line[2]-line[0]), 2);
    float yDistance = pow((line[3]-line[1]), 2);
    float length = sqrt(xDistance + yDistance);
    return length;
}

std::vector<cv::Vec4i> getLongestLines(std::vector<cv::Vec4i>& lines, int amount) {
    std::map<int, float> linesLength;

    for (int i = 0; i < lines.size(); i++) {
        float length = getLineLength(lines[i]); // Get the length of the line
        linesLength.insert(std::pair<int, float>(i, length));
    }

    std::vector<std::pair<int, float>> sortedLengths;
    std::map<int, float>::iterator it;
    for(it = linesLength.begin(); it != linesLength.end(); it++) {
        sortedLengths.push_back(std::make_pair(it->first, it->second));
    }

    std::sort(sortedLengths.begin(), sortedLengths.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b) {
        return (a.second > b.second);
    });

    std::vector<cv::Vec4i> resultLines;
    for(int i = 0; i < amount; i++) {
        resultLines.push_back(lines[i]);
    }

    return resultLines;
}

int main() {
    cv::Mat original = cv::imread("images/drive.png");
    cv::Rect roi = cv::Rect(20, 320, original.size().width - 40, 140);
    original = original(roi);

    cv::Mat output;
    cv::cvtColor(original, output, cv::COLOR_BGR2GRAY);
    cv::Canny(output, output, 100, 300);
    cv::GaussianBlur(output, output, cv::Size(3, 3), 0);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(output, lines, 1, CV_PI/360, 50, 30, 15);

    std::vector<cv::Vec4i> positiveSlopes;
    std::vector<cv::Vec4i> negativeSlopes;

    for (int i = 0; i < lines.size(); i++) {
        cv::Vec4i l = lines[i];
        float slope = getLineSlope(l);
        (slope > 0) ? positiveSlopes.push_back(l) : negativeSlopes.push_back(l);
    }

    std::vector<cv::Vec4i> longestPositiveLine = getLongestLines(positiveSlopes, 1);
    std::vector<cv::Vec4i> longestNegativeLine = getLongestLines(negativeSlopes, 1);

    for (int i = 0; i < longestNegativeLine.size(); i++) {
        cv::line(original, cv::Point(longestNegativeLine[i][0], longestNegativeLine[i][1]), cv::Point(longestNegativeLine[i][2], longestNegativeLine[i][3]), cv::Scalar(0, 255, 0), 2);
    }

    for (int i = 0; i < longestPositiveLine.size(); i++) {
        cv::line(original, cv::Point(longestPositiveLine[i][0], longestPositiveLine[i][1]), cv::Point(longestPositiveLine[i][2], longestPositiveLine[i][3]), cv::Scalar(0, 255, 0), 2);
    }

    cv::namedWindow("Canny");

    cv::imshow("Lanes", original);
    cv::imshow("Canny", output);
    cv::waitKey(0);
    return 0;
}
