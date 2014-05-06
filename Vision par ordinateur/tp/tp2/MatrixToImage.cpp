/*
 * =====================================================================================
 *
 *       Filename:  MatrixToImage.cpp
 *
 *    Description:  j
 *
 *        Version:  1.0
 *        Created:  11/07/2013 11:11:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Van Tho (), thonv133@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>

int main(char ** argv, int argv){
    int i;
    char *inname = "images/confusionMatrixCoil";
    std::ifstream infile(inname);
    if (!infile) {
        std::cout << "There was a problem opening file "
            << inname
            << " for reading."
            << endl;
        return 0;
    }

    while (infile >> i) {
        std::cout << "Value from file is " << i << std::endl;
    }
}
}
