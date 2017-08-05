# hogi
Implementation of Non-texture image inpainting using histogram of oriented gradients

# Use Eigen for matrix operations

# What to do ?
1. Compute gradient maps from image
2. Compute HOG matrix (same size as the image, contains value between 1 to n)
2.1 Test by visualizing oriented gradients with the image from the paper

## One iteration
1. Use morphological operations to find one pixel thick interior and exterior
1.1 Use dilation and erosion with structuring element of size 3x3
2. For each pixels of the exterior, make a patch centered at p
3. If p is on the boundary nullify its gradient
3. Construct a 16 bin HOG with every patches
4. Propagate the dominant gradient to all its 8 connected pixels
5. Because we want every pixel to have a different weight, they use ![image](http://latex2png.com/output//latex_c8be010c27db73d5b615bacc7ec00075.png)
where d is the euclidean distance and sigma is the variance of the gaussian function
6. However there might not be any dominant gradient, to determine if there is any they normalize the histograms
