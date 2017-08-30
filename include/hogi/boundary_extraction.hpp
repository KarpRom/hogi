#ifndef BOUNDARY_EXTRACTION_H
#define BOUNDARY_EXTRACTION_H

#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

#include <hogi/constants.hpp>
#include <hogi/types.hpp>

using namespace std;
using namespace cv;

class BoundaryExtraction {
public:
  BoundaryExtraction(Mat mask);
  ~BoundaryExtraction();
  void extract_boundaries(pair<Contour, Contour> &contours);
  void mat_boundary_to_list(cv::Mat &outer_boundary, cv::Mat &inner_boundary,
                            pair<Contour, Contour> &contours);
  void update_mask(Point &p);
  cv::Mat mask_patch(cv::Rect &roi);
  bool is_target(cv::Point &p);

private:
  cv::Mat mask;
  cv::Mat se;

  int boundary_id;
};

#endif
