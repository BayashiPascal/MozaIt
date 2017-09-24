// ============ MAIN.C ================

// ================= Include ==================

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "mozait.h"

// ================= Main function ==================

int main(int argc, char **argv) {
  // Create the MozaIt
  MozaIt *moz = MozaItCreate();
  // If we couldn't create the MozaIt
  if (moz == NULL) {
    // Display a message and stop
    fprintf(stdout, "Couldn't create the MozaIt\n");
    return 1;
  }
  // Declare variables to process the arguments
  char *fileNameIn = NULL;
  char *fileNameOut = NULL;
  // Declare a variable to memorize the usage
  char *usage = "Usage: mozait <input.tga> <output.tga> \
[-order <min> <max> (in [0,12])] \
[-margin <margin> (in pixel)] \
[-threshold <threshold> (in [0.0,1.0]) \
[solid <r> <g> <b> <a> (in [0, 255]) \
[-average]\n";
  // If the number of arguments is invalid
  if (argc < 3) {
    // Display the usage
    fprintf(stdout, "%s", usage);
    // Free memory
    MozaItFree(&moz);
    // Stop here
    return 2;
  // Else, the number of argument is valid
  } else {
    // Process the arguments
    fileNameIn = argv[1];
    fileNameOut = argv[2];
    // For each optional argument
    for (int iArg = 3; iArg < argc; ++iArg) {
      if (strcmp(argv[iArg], "-order") == 0 && iArg + 2 < argc) {
        int o[2];
        o[0] = atoi(argv[iArg + 1]);
        o[1] = atoi(argv[iArg + 2]);
        MozaItSetMinMaxOrder(moz, o);
        iArg += 2;
      } else if (strcmp(argv[iArg], "-margin") == 0 && iArg + 1 < argc) {
        float m = atof(argv[iArg + 1]);
        MozaItSetMargin(moz, m);
        iArg += 1;
      } else if (strcmp(argv[iArg], "-threshold") == 0 && 
        iArg + 1 < argc) {
        float t = atof(argv[iArg + 1]);
        MozaItSetThreshold(moz, t);
        iArg += 1;
      } else if (strcmp(argv[iArg], "-solid") == 0 && iArg + 4 < argc) {
        unsigned char r[4];
        r[0] = atoi(argv[iArg + 1]);
        r[1] = atoi(argv[iArg + 2]);
        r[2] = atoi(argv[iArg + 3]);
        r[3] = atoi(argv[iArg + 4]);
        MozaItSetSolidBackground(moz, r);
        iArg += 4;
      } else if (strcmp(argv[iArg], "-average") == 0) {
        MozaItSetAvgBackground(moz);
      } else {
        // Display the usage
        fprintf(stdout, "%s", usage);
        // Free memory
        MozaItFree(&moz);
        // Stop here
        return 3;
      }
    }
  }
  // If the user gave proper arguments
  if (fileNameIn != NULL && fileNameOut != NULL) {
    // Open the input TGA
    TGA *tgaIn = NULL;
    int ret = TGALoad(&tgaIn, fileNameIn);
    // If we couldn't load the input TGA
    if (ret != 0) {
      // Display a message
      fprintf(stdout, "Couldn't load %s (%d)\n", fileNameIn, ret);
      // Free memory
      MozaItFree(&moz);
      // Stop here
      return 4;
    }
    // Check the size of the source image
    /*if (tgaIn->_header->_width > 500 || tgaIn->_header->_height > 500) {
      // Display a message
      fprintf(stdout, "Source image too big\n");
      // Free memory
      MozaItFree(&moz);
      // Stop here
      return 7;
    }*/
    // Process the input TGA
    TGA *tgaOut = MozaItProcess(moz, tgaIn);
    // If we couldn't process the TGA
    if (tgaOut == NULL) {
      // Display a message
      fprintf(stdout, "Couldn't process the TGA\n");
      // Free memory
      TGAFree(&tgaIn);
      MozaItFree(&moz);
      // Stop here
      return 5;
    }
    // Save the output TGA
    ret = TGASave(tgaOut, fileNameOut);
    // If we couldn't load the input TGA
    if (ret != 0) {
      // Display a message
      fprintf(stdout, "Couldn't save %s (%d)\n", fileNameOut, ret);
      // Free memory
      TGAFree(&tgaIn);
      MozaItFree(&moz);
      // Stop here
      return 6;
    }
    // Free memory
    TGAFree(&tgaIn);
    TGAFree(&tgaOut);
  }
  // Free memory
  MozaItFree(&moz);
  // Return the success code
  return 0;
}

