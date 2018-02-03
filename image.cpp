
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <Magick++.h>
#include "image.h"
#include "maze.h"

using namespace Magick;

using std::vector;

Image read_image(string filename) {
  Image img(filename);
  img.modifyImage(); // get ready to modify image
  img.threshold(QuantumRange * 0.5); // convert to black and white
  img.trim(); // trim off any space around the edges
  if (img.size().width() == 0 || img.size().height() == 0) {
    throw std::invalid_argument("Image is blank or nil sized");
  }
  return img;
}

maze to_maze(const Image & img) {
  auto width = img.size().width();
  auto height = img.size().height();
  maze res(width, vector<bool>(height, false));
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      if (img.pixelColor(x, y) == ColorMono(false)) {
        res[x][y] = true;
      }
    }
  }

  // Draw in diagonal walls starting from each corner, stopping each
  // one once they hit anything
  bool tl, tr, bl, br; // t = top, b = bottom, l = left, r = right
  tl = tr = bl = br = true;
  for (int i = 0; i < std::min(width, height) && (tl || tr || bl || br); ++i) {
    if (bl) {
      if (res[i][i]) {
        bl = false;
      }
      res[i][i] = true;
    }
    if (br) {
      if (res[width - i - 1][i]) {
        br = false;
      }
      res[width - i - 1][i] = true;
    }
    if (tl) {
      if (res[i][height - i - 1]) {
        tl = false;
      }
      res[i][height - i - 1] = true;
    }
    if (tr) {
      if (res[width - i - 1][height - i - 1]) {
        tr = false;
      }
      res[width - i - 1][height - i - 1] = true;
    }
  }
  return res;
}
