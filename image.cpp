
#include <iostream>
#include <Magick++.h>
#include "image.h"

using namespace Magick;

Image read_image(string filename) {
  Image img(filename);
  img.modifyImage(); // get ready to modify image
  img.threshold(QuantumRange * 0.5); // convert to black and white
  img.trim(); // trim off any space around the edges
  return img;
}

maze to_maze(Image & img) {
  maze res(img.size().width(), vector<bool>(img.size().height(), false));
  for (int x = 0; x < img.size().width(); ++x) {
    for (int y = 0; y < img.size().height(); ++y) {
      if (img.pixelColor(x, y) == ColorMono(false)) {
        res[x][y] = true;
      }
    }
  }
  return res;
}
