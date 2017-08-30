#include "hogi/boundary_extraction.hpp"

BoundaryExtraction::BoundaryExtraction(cv::Mat mask) {
  this->mask = mask;
  this->se = getStructuringElement(MORPH_RECT, Size(3, 3));
}

BoundaryExtraction::~BoundaryExtraction() {}

void BoundaryExtraction::extract_boundaries(pair<Contour, Contour> &contours) {
  Mat dilated;
  Mat eroded;
  Mat outer_boundary;
  Mat inner_boundary;

  contours.first.clear();
  contours.second.clear();

  cv::erode(this->mask, dilated, this->se);
  subtract(this->mask, dilated, outer_boundary);
  cv::dilate(this->mask, eroded, this->se);
  subtract(eroded, this->mask, inner_boundary);

  cv::imwrite("outer_boundary.png", outer_boundary);
  cv::imwrite("inner_boundary.png", inner_boundary);

  mat_boundary_to_list(outer_boundary, inner_boundary, contours);
}

void BoundaryExtraction::mat_boundary_to_list(
    cv::Mat &outer_boundary, cv::Mat &inner_boundary,
    pair<Contour, Contour> &contours) {
  for (int row = 0; row < outer_boundary.rows; row++)
    for (int col = 0; col < outer_boundary.cols; col++) {
      if (outer_boundary.at<uchar>(row, col) != MASK_TARGET)
        contours.first.push_back(Point(col, row));
      if (inner_boundary.at<uchar>(row, col) != MASK_TARGET)
        contours.second.push_back(Point(col, row));
    }
  cout << "Size of outer contour " << contours.first.size() << endl;
  cout << "Size of inner contour " << contours.second.size() << endl;
}

void BoundaryExtraction::update_mask(Point &p) {
  this->mask.at<uchar>(p.y, p.x) = MASK_SOURCE;
}

cv::Mat BoundaryExtraction::mask_patch(cv::Rect &roi) {
  return this->mask(roi);
}

bool BoundaryExtraction::is_target(cv::Point &p) {
  return this->mask.at<uchar>(p.y, p.x) == MASK_TARGET;
}
