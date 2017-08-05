#include <hogi/gradientMap.hpp>

GradientMap::GradientMap(cv::Mat &im) {
  // Init vectors size
  this->gradients = Mat(im.rows, im.cols, CV_32FC2, Scalar::all(0.0f));
  this->oriented_matrix = Mat(im.rows, im.cols, CV_8UC1, Scalar::all(0));
  this->gradients_map(im);
  this->orientation_matrix();
}

void GradientMap::gradients_map(cv::Mat &im) {
  // Compute gradients gx and gy
  for (unsigned int row = 0; row < im.rows - 1; row++)
    for (unsigned int col = 0; col < im.cols - 1; col++) {
      cv::Vec2f &elem = this->gradients.at<Vec2f>(row, col);
      elem[GX] = im.at<uchar>(row, col + 1) - im.at<uchar>(row, col);
      elem[GY] = im.at<uchar>(row + 1, col) - im.at<uchar>(row, col);
    }
}

void GradientMap::orientation_matrix() {
  for (size_t row = 0; row < this->gradients.rows; row++)
    for (size_t col = 0; col < this->gradients.cols; col++) {
      cv::Vec2f &elem = this->gradients.at<Vec2f>(row, col);
      int orientation_value = this->get_orientation_value(elem[GX], elem[GY]);
      if (orientation_value > 0)
        oriented_matrix.at<uchar>(row, col) = orientation_value;
      else
        oriented_matrix.at<uchar>(row, col) = NUM_ORIENTATION;
    }
}

int GradientMap::get_orientation_value(float gx, float gy) {
  return round((atan(gx / gy) / M_PI + 1 / 2) * NUM_ORIENTATION);
}

void GradientMap::write_orientation_matrix() {
  for (size_t orientation = 0; orientation < NUM_ORIENTATION; orientation++) {
    cv::Mat im_orientation = Mat(this->gradients.rows, this->gradients.cols,
                                 CV_8UC1, Scalar::all(0));
    for (size_t row = 0; row < im_orientation.rows; row++)
      for (size_t col = 0; col < im_orientation.cols; col++)
        if (this->oriented_matrix.at<uchar>(row, col) == orientation)
          im_orientation.at<uchar>(row, col) = 255;

    std::string output_path = "orientation_" + to_string(orientation) + ".png";
    cout << "Write orientation image " << output_path << endl;
    cv::imwrite(output_path, im_orientation);
  }
}
