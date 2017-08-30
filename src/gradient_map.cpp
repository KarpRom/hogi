#include <hogi/gradient_map.hpp>

GradientMap::GradientMap(cv::Mat &im, cv::Mat &mask) {
  assert(im.type() == CV_8UC1);

  this->gx = Mat(im.rows, im.cols, CV_32FC1, Scalar(0));
  this->gy = Mat(im.rows, im.cols, CV_32FC1, Scalar(0));
  this->oriented_matrix = Mat(im.rows, im.cols, CV_8UC1, Scalar(0));
  this->mask = mask;

  this->compute_gradients(im);
  this->orientation_matrix();
}

GradientMap::~GradientMap() {}

void GradientMap::compute_gradients(cv::Mat &im) {
  for (int row = 1; row < im.rows - 1; row++)
    for (int col = 1; col < im.cols - 1; col++) {
      gx.at<float>(row, col) = (float(im.at<unsigned char>(row, col + 1)) -
                                float(im.at<unsigned char>(row, col - 1))) /
                               2.f;
      gy.at<float>(row, col) = (float(im.at<unsigned char>(row + 1, col)) -
                                float(im.at<unsigned char>(row - 1, col))) /
                               2.f;
    }
}

void GradientMap::orientation_matrix() {
  for (int row = 0; row < this->gx.rows; row++)
    for (int col = 0; col < this->gy.cols; col++) {
      if (this->mask.at<uchar>(row, col) == MASK_SOURCE) {
        int orientation_value = this->get_orientation_value(
            this->gx.at<float>(row, col), this->gy.at<float>(row, col));
        oriented_matrix.at<unsigned char>(row, col) = orientation_value;
      }
    }
}

int GradientMap::get_orientation_value(float gx, float gy) {
  float inner = (atan(gx / gy) / float(M_PI)) + 0.5f;
  int orientation = round(inner * float(NUM_ORIENTATION));
  return orientation; // > 0 ? orientation : NUM_ORIENTATION;
}

cv::Mat &GradientMap::get_orientation_matrix() { return this->oriented_matrix; }

void GradientMap::write_orientation_matrix() {
  cv::Mat im_orientation =
      Mat(this->gx.rows, this->gx.cols, CV_8UC1, Scalar::all(0));
  for (int row = 0; row < im_orientation.rows; row++)
    for (int col = 0; col < im_orientation.cols; col++)
      im_orientation.at<uchar>(row, col) =
          (oriented_matrix.at<unsigned char>(row, col) - 1) * 16;

  cv::imwrite("out/orientations.png", im_orientation);
}

void GradientMap::write_gradient_maps() {
  Mat gmX(this->gx.rows, this->gx.cols, CV_8UC1, Scalar(0));
  Mat gmY(this->gy.rows, this->gy.cols, CV_8UC1, Scalar(0));

  int base = 127;
  for (int row = 0; row < this->gx.rows; row++) {
    for (int col = 0; col < this->gx.cols; col++) {
      gmX.at<uchar>(row, col) = base + int(this->gx.at<float>(row, col) / 2.f);
      gmY.at<uchar>(row, col) = base + int(this->gy.at<float>(row, col) / 2.f);
    }
  }
  cv::imwrite("out/gradient_x.png", gmX);
  cv::imwrite("out/gradient_y.png", gmY);
}
