#include <iostream>
#include <hogi/gradient_map.hpp>
#include <hogi/boundary_extraction.hpp>
#include <hogi/dominant_gradient.hpp>
#include <hogi/weights.hpp>

#include <hogi/utils.hpp>

int weighted_average(cv::Mat weights, cv::Mat input, cv::Mat mask_patch) {
  assert(weights.cols == input.cols);
  assert(weights.rows == input.rows);

  float sum = 0.f;
  float weight_sum = 0.f;

  for (int i = 0; i < weights.cols; i++)
    for (int j = 0; j < weights.rows; j++) {
      if (int(mask_patch.at<uchar>(j, i) == MASK_SOURCE)) {
        sum += float(weights.at<double>(j, i)) * float(input.at<uchar>(j, i));
        weight_sum += float(weights.at<double>(j, i));
      }
    }
  if (sum == 0) {
    cout << "Sum is null" << endl;
    cout << "weights " << endl << weights << endl;
    cout << "input " << endl << input << endl;
    cout << "mask " << endl << mask_patch << endl;
    imwrite("patch.png", input);
    imwrite("mask.png", mask_patch);
  }
  return int(sum / weight_sum);
}

cv::Mat inpaint(cv::Mat im, cv::Mat mask) {
  GradientMap gm(im, mask);
  BoundaryExtraction be(mask);
  DominantGradient dg;
  vector<Point> assigned_points;
  pair<Contour, Contour> contours;
  Contour inner, outer;
  int iteration = 0;
  int np = 0;

  // 1. Compute the orientation matrix
  cv::Mat orientation_matrix = gm.get_orientation_matrix();
  gm.write_orientation_matrix();

  int window_search_shift = (SEARCH_SIZE - 1) / 2;
  int window_fill_shift = (FILL_SIZE - 1) / 2;

  // 2. While omega is not empty do
  while (true) {
    // 2.1 Identify the interior and exteriror boundary regions
    be.extract_boundaries(contours);
    outer = contours.first;
    inner = contours.second;

    cout << "Inner size " << inner.size() << endl;
    cout << "Outer size " << outer.size() << endl;

    // If there are no more boundaries
    if (inner.size() == 0) {
      cout << "Inner size is null" << endl;
      break; // end reconstruction
    }

    if (iteration == 0)
      for (Point &p : outer)
        orientation_matrix.at<uchar>(p.y, p.x) = 0;

    // 2.2 For each point of outer boundary
    cout << "Set gradients" << endl;
    for (Point p : outer) {
      // 2.2.a Make patch with regard to BETA
      cv::Point origin(p.x - window_search_shift, p.y - window_search_shift);
      cv::Rect roi(origin.x, origin.y, SEARCH_SIZE, SEARCH_SIZE);
      cv::Mat patch(orientation_matrix, roi);
      cv::Mat mask_patch = be.mask_patch(roi);

      // 2.2.b Normalized 16 bin histogram of oriented gradients from patch
      // 2.2.c Compute maximum gradient O
      int cdg = dg.get_dominant_gradient(patch, mask_patch, origin, p, outer);

      // 2.2.d Propagate O into p and all its 8-connected neighborhood pixels
      orientation_matrix.at<uchar>(p.y, p.x) = cdg;
      if (cdg > 0) {
        for (int i = -1; i <= 1; i++)
          for (int j = -1; j <= 1; j++) {
            Point cp(p.x + i, p.y + j);
            if (cp.x >= 0 && cp.x < orientation_matrix.cols && cp.y >= 0 &&
                cp.y < orientation_matrix.rows)
              if (be.is_target(cp))
                if (find(assigned_points.begin(), assigned_points.end(), cp) ==
                    assigned_points.end()) {
                  orientation_matrix.at<uchar>(cp) = cdg;
                  assigned_points.push_back(cp);
                }
          }
      }
    }

    cv::imwrite("orientation_matrix" + to_string(iteration) + ".png",
                orientation_matrix);

    // 2.3 For each point of inner boundary
    cout << "Inpainting points " << endl;
    for (Point p : inner) {
      // 2.3.a Specify the weight matrix according on the value of O
      int teta = orientation_matrix.at<uchar>(p.y, p.x);
      cv::Mat weight_matrix = get_weight_matrix(teta);
      cv::Rect roi = Rect(p.x - window_fill_shift, p.y - window_fill_shift,
                          FILL_SIZE, FILL_SIZE);
      cv::Mat patch = im(roi);
      cv::Mat mask_patch = be.mask_patch(roi);

      // 2.3.b Fill I

      float val = weighted_average(weight_matrix, patch, mask_patch);

      if (val == 0) {
        cout << "Update point p " << p << endl;
        return im;
      }
      im.at<uchar>(p.y, p.x) = val;

      // 2.3.c Update target mask
      be.update_mask(p);
    }

    cv::imwrite("Inpaint_" + to_string(iteration) + ".png", im);
    iteration++;
    // break;
  }
  return im;
}

int main(int argc, char **argv) {
  cv::Mat im = imread(argv[1], 0);
  cv::Mat mask = imread(argv[2], 0);
  cv::Mat out;

  out = inpaint(im, mask);
  cv::imwrite("reconstructed.png", out);

  return 0;
}
