#ifndef BOUNDARY_EXTRACTION_H
#define BOUNDARY_EXTRACTION_H

#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

typedef std::vector<cv::Point> Contour;

class BoundaryExtraction {
public:
  BoundaryExtraction(Mat &mask);
  ~BoundaryExtraction();
  pair<Contour, Contour> extractBoundary();
  pair<Contour, Contour> mat_boundary_to_list(cv::Mat &outer_boundary,
                                              cv::Mat &inner_boundary);
  void setContour(Contour &c);

private:
  cv::Mat mask;
};

#endif
