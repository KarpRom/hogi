#include <hogi/dominant_gradient.hpp>

int DominantGradient::get_dominant_gradient(cv::Mat &patch, cv::Mat &mask,
                                            cv::Point origin, cv::Point center,
                                            Contour &contour) {
  assert(patch.type() == CV_8UC1);
  assert(mask_patch.type() == CV_8UC1);
  assert(patch.cols == mask_patch.cols);
  assert(patch.rows == mask_patch.rows);

  // cout << "Histogram of point " << center.x << " , " << center.y << endl;
  // cv::imwrite("out/patch" + to_string(it) + ".png", patch);
  // cv::imwrite("out/mask" + to_string(it) + ".png", mask);
  this->init_histogram();
  this->compute_histogram(patch, mask, origin, center, contour);
  this->normalize_histogram();
  return this->get_max_value();
}

void DominantGradient::init_histogram() {
  this->histogram.resize(HISTOGRAM_SIZE);
  fill(this->histogram.begin(), this->histogram.end(), 0.f);
}

void DominantGradient::compute_histogram(cv::Mat &patch, cv::Mat &mask,
                                         cv::Point origin, cv::Point center,
                                         Contour &contour) {
  for (int row = 0; row < patch.rows; row++)
    for (int col = 0; col < patch.cols; col++)
      if (mask.at<uchar>(row, col) == MASK_SOURCE) { // not target region
        // Not the actual contour point
        Point pc(origin.x + col, origin.y + row);
        if (find(contour.begin(), contour.end(), pc) == contour.end()) {
          int orientation = int(patch.at<uchar>(row, col));
          if (orientation > 0) {
            float ed = square_euclidean_distance(center, pc);
            float val = exp(-ed / (2.f * SIGMA_2));
            this->histogram[orientation - 1] += val;
          }
        }
      }
}

float DominantGradient::get_histogram_sum() {
  float sum = 0;
  for (size_t i = 0; i < this->histogram.size(); i++)
    sum += this->histogram[i];
  return sum;
}

void DominantGradient::normalize_histogram() {
  float sum = get_histogram_sum();
  for (size_t i = 0; i < this->histogram.size(); i++)
    this->histogram[i] /= sum;
}

int DominantGradient::get_max_value() {
  float max = 0.f;
  int max_index = 0;
  for (size_t i = 0; i < this->histogram.size(); i++)
    if (this->histogram[i] > max) {
      max = this->histogram[i];
      max_index = i + 1;
    }
  int ret = max >= BETA ? max_index : 0;
  // this->print_histogram();
  // cout << "Dominant orientation of it " << it << "= > " << ret << endl;
  it++;
  return ret;
}

void DominantGradient::print_histogram() {
  for (size_t i = 0; i < this->histogram.size(); i++)
    cout << " [" << i << "] " << this->histogram[i];
  cout << endl;
}
