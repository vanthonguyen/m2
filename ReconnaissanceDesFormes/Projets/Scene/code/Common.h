/*
 * =====================================================================================
 *       Filename:  Common.h
 *    Description:  
 *        Version:  1.0
 *        Created:  01/04/2014 06:10:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 * =====================================================================================
 */

#include <vector>
#include <iostream>

#include <fstream>
#include <sstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define DICTIONARY_SIZE 1000
#define VOCABULARY_FILE "vocabulary.xml" 
#define SVM_FILE "svm.xml" 


void readCsv(char *fileName, std::vector<cv::Mat>& images, cv::Mat& labels, char separator = ';') {                                                  
    std::ifstream file(fileName, std::ifstream::in);
    if (!file) {
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line)) {
        std::stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void readCsv(char *fileName, std::vector<cv::Mat>& images, cv::Mat& labels, std::vector<std::string> &imageNames ,char separator = ';') {                                                  
    std::ifstream file(fileName, std::ifstream::in);
    if (!file) {
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line)) {
        std::stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE));
            labels.push_back(atoi(classlabel.c_str()));
            imageNames.push_back(path);
        }
    }
}


static double getPrecision(const std::vector<std::vector<int> > &cm, int position){
      int fp = 0;
      int tp = cm[position][position];
      int n = cm[0].size();
      for (int i = 0; i < n; i++){
          if(i != position){
              fp += cm[i][position];
          }
      }
      return 100.0*tp/(tp + fp);
  }
  
double getRecall(const std::vector<std::vector<int> > &cm, int position){
      int fn = 0;
      int tp = cm[position][position];
      int n = cm[0].size();
      for (int i = 0; i < n; i++){
          if(i != position){
              fn += cm[position][i];
          }
      }
      return 100.0*tp/(tp + fn);
  }

void printConfusionMatrix(const std::vector<std::vector<int> > &cm){
debug("cf size");
debug(cm.size());
    std::ofstream ofs("cf");  
    ofs.precision(1);
    ofs.unsetf ( std::ios::floatfield );
      //Affichage de la matrice de confusion
    std::cout << std::endl<<"Matrice de confusion"<<std::endl;
      //header line of matrix
      ofs<<"&"<<std::fixed;
    for (unsigned i = 0; i < cm.size(); i++) {
          ofs<<i + 1<<"&";
      }
  
      ofs<<"r\\\\"<< std::endl<<"\\hline"<<std::endl;
    for (unsigned i = 0; i < cm.size(); i++) {
          //first column
          ofs<<i + 1<<"&";
        for (unsigned int j = 0; j < cm.size(); j++) {
              // cout<<MatriceConfusion[i][j]<<"\t";
              ofs<<cm[i][j]<<"&";
          }
          ofs<<getRecall(cm, i)<<"\\\\"<< std::endl<<"\\hline"<<std::endl;
          //ofs<<"\\\\"<<std::endl;
          //cout << endl;
      }
  
      ofs<<"p&"<<std::fixed;
      //print precision
    for (unsigned i = 0; i < cm.size(); i++) {
          ofs<<getPrecision(cm,i)<<"&";
      }
  
      ofs<<"\\\\"<< std::endl<<"\\hline"<<std::endl;
      std::cout<<std::endl;
      ofs.close();
}
