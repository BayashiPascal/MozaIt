// ============ MOZAIT.H ================

#ifndef MOZAIT_H
#define MOZAIT_H

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "gset.h"
#include "tgapaint.h"

// ================= Define ==================


// ================= Data structures ===================

// Types of background
typedef enum MozaItBackground {
  // Solid color
  MozaItBackgroundSolid,
  // Average color
  MozaItBackgroundAvg
} MozaItBackground;

// MozaIt options
typedef struct MozaItOpt {
  // Orders defining the size min and max of the tiles
  // size = pow(golden ratio, order)
  int _orders[2];
  // Margin around the tile, in pixel
  float _margin;
  // Threshold in [0.0, 1.0] (0.0: strict, 1.0: loose)
  float _threshold;
  // Type of background
  MozaItBackground _typeBg;
  // Background color in case of solid background
  unsigned char _rgbaBg[4];
} MozaItOpt;

// MozaIt
typedef struct MozaIt {
  // MozaIt options
  MozaItOpt _option;
} MozaIt;

// ================ Functions declaration ====================

// Create a new MozaIt with default options' value:
// _orders = {1, 10}
// _margin = 2
// _threshold = sqrt(0.1)
// _typeBg = MozaItBackgroundAvg
// Return NULL if we couldn't create the MozaIt
MozaIt* MozaItCreate(void);

// Free the memory used by a MozaIt
// Do nothing if arguments are invalid
void MozaItFree(MozaIt **moz);

// Set the min and max orders (in [0, 12])
// Do nothing if arguments are invalid
void MozaItSetMinMaxOrder(MozaIt *moz, int *orders);

// Set the margin
// Do nothing if arguents are invalid
void MozaItSetMargin(MozaIt *moz, float margin);

// Set the threshold
// Do nothing if arguents are invalid
void MozaItSetThreshold(MozaIt *moz, float threshold);

// Set the background type to solid
// Do nothing if arguents are invalid
void MozaItSetSolidBackground(MozaIt *moz, unsigned char *rgba);

// Set the background type to average
// Do nothing if arguents are invalid
void MozaItSetAvgBackground(MozaIt *moz);

// Process the TGA 'src' and return the resulting TGA
// Return NULL if arguments are invalid or couldn't
// process the image
TGA* MozaItProcess(MozaIt *moz, TGA *src);

#endif
