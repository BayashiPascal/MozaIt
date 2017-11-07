// ============ MOZAIT.C ================

// ================= Include ==================

#include "mozait.h"

// ================= Define ==================

#define rnd() (float)(rand())/(float)(RAND_MAX)
#define MOZAIT_GOLDENRATIO 1.61803

// ================ Functions declaration ====================

// Check if a tile of siwe 'size' centered at 'pos' is possible for 
// image 'src' and mask 'mask'
// Return true if possible, false else
bool MozaItCheckPos(MozaIt *moz, VecShort *pos, float size, 
  bool *mask, TGA *src);

// Draw one tile of siwe 'siwe' centered at 'pos' in TGA 'tga' and update
// the mask 'mask'
void MozaItDrawTile(MozaIt *moz, VecShort *pos, float size, bool *mask, 
  TGA *src, TGA *tga);
  
// ================ Functions implementation ====================

// Create a new MozaIt with default options' value:
// _orders = {1, 10}
// _margin = 2
// _threshold = 0.1
// _typeBg = MozaItBackgroundAvg
// Return NULL if we couldn't create the MozaIt
MozaIt* MozaItCreate(void) {
  // Allocate memory
  MozaIt *ret = (MozaIt*)malloc(sizeof(MozaIt));
  // If we could allocate memory
  if (ret != NULL) {
    // Set the default options
    ret->_option._orders[0] = 1;
    ret->_option._orders[1] = 9;
    ret->_option._margin = 2.0;
    ret->_option._threshold = sqrt(0.1);
    ret->_option._typeBg = MozaItBackgroundAvg;
    ret->_option._rgbaBg[0] = 255;
    ret->_option._rgbaBg[1] = 255;
    ret->_option._rgbaBg[2] = 255;
    ret->_option._rgbaBg[3] = 255;
  }
  // Return the MozaIt
  return ret;
}

// Free the memory used by a MozaIt
// Do nothing if arguments are invalid
void MozaItFree(MozaIt **moz) {
  // Check the arguments
  if (moz == NULL || *moz == NULL)
    return;
  // Free memory
  free(*moz);
  *moz = NULL;
}

// Set the min and max orders (in [0, 12])
// Do nothing if arguents are invalid
void MozaItSetMinMaxOrder(MozaIt *moz, int *orders) {
  // Check the arguments
  if (moz == NULL || orders == NULL || orders[0] > orders[1] || 
    orders[0] < 0 || orders[1] > 12)
    return;
  // Set the dimensions
  for (int iDim = 0; iDim < 2; ++iDim)
    moz->_option._orders[iDim] = orders[iDim];
}

// Set the margin
// Do nothing if arguents are invalid
void MozaItSetMargin(MozaIt *moz, float margin) {
  // Check the arguments
  if (moz == NULL || margin < 0.0)
    return;
  // Set the margin
  moz->_option._margin = margin;
}

// Set the threshold
// Do nothing if arguents are invalid
void MozaItSetThreshold(MozaIt *moz, float threshold) {
  // Check the arguments
  if (moz == NULL || threshold < 0.0 || threshold > 1.0)
    return;
  // Set the threshold
  moz->_option._threshold = threshold;
}

// Set the background type to solid
// Do nothing if arguents are invalid
void MozaItSetSolidBackground(MozaIt *moz, unsigned char *rgba) {
  // Check the arguments
  if (moz == NULL || rgba == NULL)
    return;
  // Set the background
  moz->_option._typeBg = MozaItBackgroundSolid;
  for (int iRGB = 0; iRGB < 4; ++iRGB)
    moz->_option._rgbaBg[iRGB] = rgba[iRGB];
}

// Set the background type to average
// Do nothing if arguents are invalid
void MozaItSetAvgBackground(MozaIt *moz) {
  // Check the arguments
  if (moz == NULL)
    return;
  // Set the background
  moz->_option._typeBg = MozaItBackgroundAvg;
}

// Process the TGA 'src' and return the resulting TGA
// Return NULL if arguments are invalid or couldn't
// process the image
TGA* MozaItProcess(MozaIt *moz, TGA *src) {
  // Check the arguments
  if (moz == NULL || src == NULL)
    return NULL;
  // Declare a variable to memorize the dimensions of the TGA
  VecShort *dim = VecShortCreate(2);
  if (dim == NULL)
    return NULL;
  VecSet(dim, 0, src->_header->_width);
  VecSet(dim, 1, src->_header->_height);
  // Declare a variable to memorize the background color
  TGAPixel *pixel = NULL;
  // Set the background color
  if (moz->_option._typeBg == MozaItBackgroundSolid) {
    pixel = TGAGetWhitePixel();
    if (pixel != NULL)
      for (int iRGB = 0; iRGB < 4; ++iRGB)
        pixel->_rgba[iRGB] = moz->_option._rgbaBg[iRGB];
  } else if (moz->_option._typeBg == MozaItBackgroundAvg) {
    pixel = TGAGetAverageColor(src);
  } else {
    pixel = TGAGetWhitePixel();
  }
  // If we coudln't create the pixel for the background color
  if (pixel == NULL) {
    // Stop here
    VecFree(&dim);
    return NULL;
  }
  // Create the result TGA
  TGA *tga = TGACreate(dim, pixel); 
  // If we couldn't create the tga
  if (tga == NULL) {
    // Stop here
    VecFree(&dim);
    TGAPixelFree(&pixel);
    return NULL;
  }
  // Declare a variable to memorize pixels painted
  bool *mask = (bool*)malloc(sizeof(bool) * VecGet(dim, 0) * VecGet(dim, 1));
  // If we couldn't allocate memory for the mask
  if (mask == NULL) {
    // Free memory
    TGAPixelFree(&pixel);
    TGAFree(&tga);
    VecFree(&dim);
    // Stop here
    return NULL;
  }
  // Initialize the mask
  for (int i = VecGet(dim, 0) * VecGet(dim, 1); i--;)
    mask[i] = false;
  // Declare a pencil to paint on the result TGA
  TGAPencil *pen = TGAGetPencil();
  // If we couldn't allocate memory for the pen
  if (pen == NULL) {
    // Free memory
    TGAPixelFree(&pixel);
    TGAFree(&tga);
    VecFree(&dim);
    // Stop here
    return NULL; 
  }
  // Declare a variable to memorize the min and max sizes of the tiles
  float sizes[2];
  for (int iSize = 0; iSize < 2; ++iSize)
    sizes[iSize] = pow(MOZAIT_GOLDENRATIO, moz->_option._orders[iSize]);
  // Declare a variable to memorize the order in which we move through
  // the image (abciss first or ordinate first)
  // By giving priority to the longest axis it speed up the process
  // thanks to the skipping of pixels in intersection when we achieve
  // a tile placement
  int axis[2];
  if (VecGet(dim, 0) > VecGet(dim, 1)) {
    axis[0] = 0;
    axis[1] = 1;
  } else {
    axis[0] = 1;
    axis[1] = 0;
  }
  // Fill the TGA starting with big tiles down to smallest tiles
  for (float size = sizes[1]; size >= sizes[0]; 
    size /= MOZAIT_GOLDENRATIO) {
    // Declare a variable to memorize the number of tiles found
    int nbTile = 0;
    // Declare a variable to memorize the size of the tile with margin
    short r = (short)floor(size + moz->_option._margin);
    // For each possible position of this tile
    // Step by 2 to speed up the process without decreasing the quality
    // of the result
    VecShort *pos = VecShortCreate(2);
    if (pos == NULL) {
      // Free memory
      TGAPixelFree(&pixel);
      TGAFree(&tga);
      VecFree(&dim);
      TGAPencilFree(&pen);
      // Stop here
      return NULL; 
    }
    for (VecSet(pos, axis[1], r); 
      VecGet(pos, axis[1]) <= VecGet(dim, axis[1]) - r; 
      VecSet(pos, axis[1], VecGet(pos, axis[1]) + 2)) { 
      for (VecSet(pos, axis[0], r); 
        VecGet(pos, axis[0]) <= VecGet(dim, axis[0]) - r; 
        VecSet(pos, axis[0], VecGet(pos, axis[0]) + 2)) { 
        // Get the index of this position
        int index = VecGet(pos, 1) * src->_header->_width + 
          VecGet(pos, 0);
        // If this position is free
        if (mask[index] == false && MozaItCheckPos(moz, pos, size, mask, src) == true) {
          // Draw the tile
          MozaItDrawTile(moz, pos, size, mask, src, tga);
          // Update the number of tiles
          ++nbTile;
          // Skip the next pixels which are in intersection with the tile
          VecSet(pos, axis[0], VecGet(pos, axis[0]) + 
            (short)floor(size * 2));
        }
      }
    }
    VecFree(&pos);
  }
  // Free memory
  TGAPixelFree(&pixel);
  VecFree(&dim);
  free(mask);
  mask = NULL;
  TGAPencilFree(&pen);
  // Return the result TGA
  return tga;
}

// Check if a tile of radius 'size' centered at 'pos' is possible for 
// image 'src' and mask 'mask'
// Return true if possible, false else
bool MozaItCheckPos(MozaIt *moz, VecShort *pos, float size, bool *mask, 
  TGA *src) { 
  // Declare a variable to memorize the size of the square covering the 
  // tile
  short r = (short)floor(size + moz->_option._margin);
  // Declare variables to calculate the average delta of color
  float avg[3] = {0.0};
  float sum = 0.0;
  // Declare a variable to memorize if the tile touch the mask
  bool maskOk = true;
  // Get the pixel in the source image at center of tile
  TGAPixel *pixelSrc = TGAGetPix(src, pos);
  // For each pixel in the square covering the tile
  VecShort *p = VecShortCreate(2);
  if (p == NULL)
    return false;
  for (VecSet(p, 0, VecGet(pos, 0) - r); 
    VecGet(p, 0) <= VecGet(pos, 0) + r && maskOk == true; 
    VecSet(p, 0, VecGet(p, 0) + 1)) {
    for (VecSet(p, 1, VecGet(pos, 1) - r); 
      VecGet(p, 1) <= VecGet(pos, 1) + r && maskOk == true; 
      VecSet(p, 1, VecGet(p, 1) + 1)) {
      // Get the index of this position
      int index = VecGet(p, 1) * src->_header->_width + VecGet(p, 0);
      // Calculate the distance to the center
      float d = sqrt(pow(VecGet(pos, 0) - VecGet(p, 0), 2.0) + pow(VecGet(pos, 1) - VecGet(p, 1), 2.0));
      // If this pos is in the margin
      if (d <= r) {
        // Check the mask
        if (mask[index] == true)
          maskOk = false;
        // Else, if this pos is in the tile
        else if (d <= size) {
          // Get the pixel in the source image at current pos
          TGAPixel *pixel = TGAGetPix(src, p);
          // Update the average delta of color
          // The delta is weighted with the square of the distance
          // to counteract the fact that as the circle get bigger 
          // delta of color of one pixel gets relatively less and
          // less significant 
          sum += d * d;
          for (int iRGB = 3; iRGB--;)
            avg[iRGB] += 
              fabs((float)(pixel->_rgba[iRGB]) - 
              (float)(pixelSrc->_rgba[iRGB])) / 255.0 *
              d * d;
        }
      }
    }
  }
  VecFree(&p);
  // Complete the calculation of the average delta of color
  for (int iRGB = 0; iRGB < 3; ++iRGB)
    avg[iRGB] /= sum;
  // Calculate the distance of the average delta of color in the tile
  float dColor = sqrt(pow(avg[0], 2.0) + pow(avg[1], 2.0) + 
    pow(avg[2], 2.0));
  // If the tile doesn't touch the mask and the average color respects
  // the threshold
  if (maskOk == true && dColor <= pow(moz->_option._threshold, 2.0)) {
    return true;
  // Else, the tile touches the mask or the average color doesn't respect
  // the threshold
  } else {
    return false;
  }
}

// Draw one tile of siwe 'siwe' centered at 'pos' in TGA 'tga' and update
// the mask 'mask'
void MozaItDrawTile(MozaIt *moz, VecShort *pos, float size, bool *mask, 
  TGA *src, TGA *tga) {
  // Declare a variable to memorize the size of the square covering the 
  // tile
  short r = (short)floor(size);
  // Get the pixel in the source image at center of tile
  TGAPixel *pixel = TGAGetPix(src, pos);
  // For each pixel in the square covering the tile
  VecShort *p = VecShortCreate(2);
  for (VecSet(p, 0, VecGet(pos, 0) - r); 
    VecGet(p, 0) <= VecGet(pos, 0) + r; 
    VecSet(p, 0, VecGet(p, 0) + 1)) {
    for (VecSet(p, 1, VecGet(pos, 1) - r); 
      VecGet(p, 1) <= VecGet(pos, 1) + r; 
      VecSet(p, 1, VecGet(p, 1) + 1)) {
      // Get the index of this position
      int index = VecGet(p, 1) * src->_header->_width + VecGet(p, 0);
      // Calculate the distance to the center
      float d = sqrt(pow(VecGet(pos, 0) - VecGet(p, 0), 2.0) + 
        pow(VecGet(pos, 1) - VecGet(p, 1), 2.0));
      // If this pos is in the tile
      if (d <= size) {
        // Get the pixel in the result image at current pos
        TGAPixel *pixelOut = TGAGetPix(tga, p);
        // Update the color
        for (int iRGB = 3; iRGB--;)
          pixelOut->_rgba[iRGB] = pixel->_rgba[iRGB];
        // Update the mask
        mask[index] = true;
      }
    }
  }
}
