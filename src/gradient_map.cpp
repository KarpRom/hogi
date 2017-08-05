#include <hogi/gradient_map.hpp>

GradientMap::GradientMap(cv::Mat &im) {
  assert(im.type() == CV_8UC1);
  // Init vectors size
  this->gx = Mat(im.rows, im.cols, CV_16SC1, Scalar(0));
  this->gy = Mat(im.rows, im.cols, CV_16SC1, Scalar(0));
  this->oriented_matrix = Mat(im.rows, im.cols, CV_8UC1, Scalar(0));
  this->gradients_map(im);
  this->orientation_matrix();

  cv::imwrite("out/in.png", im);
}

GradientMap::~GradientMap() {}

void GradientMap::gradients_map(cv::Mat &im) {
  // Compute gradients gx and gy
  for (unsigned int row = 0; row < im.rows - 1; row++)
    for (unsigned int col = 0; col < im.cols - 1; col++) {
      gx.at<short>(row, col) =
          im.at<uchar>(row, col + 1) - im.at<uchar>(row, col);
      gy.at<short>(row, col) =
          im.at<uchar>(row + 1, col) - im.at<uchar>(row, col);
    }
}

void GradientMap::orientation_matrix() {
  for (size_t row = 0; row < this->gx.rows; row++)
    for (size_t col = 0; col < this->gy.cols; col++) {
      int orientation_value = this->get_orientation_value(
          this->gx.at<short>(row, col), this->gy.at<short>(row, col));
      if (orientation_value > 0)
        oriented_matrix.at<uchar>(row, col) = orientation_value;
      else
        oriented_matrix.at<uchar>(row, col) = NUM_ORIENTATION;
    }
}

int GradientMap::get_orientation_value(short int gx, short int gy) {
  return round((atan(float(gx) / float(gy)) / M_PI + 0.5f) *
               float(NUM_ORIENTATION));
}

void GradientMap::write_orientation_matrix() {
  for (size_t orientation = 1; orientation <= NUM_ORIENTATION; orientation++) {
    cv::Mat im_orientation =
        Mat(this->gx.rows, this->gx.cols, CV_8UC1, Scalar::all(0));
    for (size_t row = 0; row < im_orientation.rows; row++)
      for (size_t col = 0; col < im_orientation.cols; col++)
        if (this->oriented_matrix.at<uchar>(row, col) == orientation)
          im_orientation.at<uchar>(row, col) = 255;

    std::string output_path =
        "out/orientation_" + to_string(orientation) + ".png";
    cout << "Write orientation image " << output_path << endl;
    cv::imwrite(output_path, im_orientation);
  }
}

void GradientMap::write_gradient_maps() {
  Mat gmX(this->gx.rows, this->gx.cols, CV_8UC1, Scalar(0));
  Mat gmY(this->gy.rows, this->gy.cols, CV_8UC1, Scalar(0));

  int base = 127;
  for (int row = 0; row < this->gx.rows; row++) {
    for (int col = 0; col < this->gx.cols; col++) {
      gmX.at<uchar>(row, col) = base + int(this->gx.at<short>(row, col) / 2);
      gmY.at<uchar>(row, col) = base + int(this->gy.at<short>(row, col) / 2);
    }
  }
  cv::imwrite("out/gradient_x.png", gmX);
  cv::imwrite("out/gradient_y.png", gmY);
}

cv::Mat &get_orientation_matrix() { return this->oriented_matrix; }
