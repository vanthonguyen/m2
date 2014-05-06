/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>


using namespace cv;
using namespace std;

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, vector<string>& nameImg, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
	    nameImg.push_back(path);
            labels.push_back(atoi(classlabel.c_str()));
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

static double getRecall(const std::vector<std::vector<int> > &cm, int position){
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

int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if (argc < 2) {
        cout << "usage: " << argv[0] << " <csv.ext> <output_folder> " << endl;
        exit(1);
    }
    // Get the path to your CSV.
    string fn_csv = string(argv[1]);
    string fn_csv1 = string(argv[2]);
    unsigned int methode = atoi(argv[3]);
    int nbPers = 40;

    int n = 0;                                                                                                                                                  
    if(argc > 4){
        n = atoi(argv[4]);
    }

    //open file for writing confusion matrix
    std::string confusionFile("cf" + std::string(argv[3]));
    std::ofstream ofs(confusionFile.c_str());  
// Pour calculer le temps de reconnaissance
    struct timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
    
    
    // These vectors hold the images and corresponding labels.
    vector<Mat> imagesApp;
    vector<int> labelsApp;
    
    vector<Mat> imagesTest;
    vector<int> labelsTest;
    vector<string> nameImgTest;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv(fn_csv, imagesApp, labelsApp);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(imagesApp.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    
    try {
        read_csv(fn_csv1, imagesTest, labelsTest, nameImgTest);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv1 << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(imagesTest.size() <= 1) {
        string error_message = "This test needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    Ptr<FaceRecognizer> model;
    
    // 	Calcul de la matrice de confusion
    vector< vector<int> > MatriceConfusion;
     ofs.precision(0);
     ofs.unsetf ( std::ios::floatfield );
    // Initialisation de la matrice de confusion
    for (unsigned int i = 0; i < nbPers; i++) {
        vector<int> row; // Create an empty row
        for (unsigned int j = 0; j < nbPers; j++) {
            row.push_back(0); // Add an element (column) to the row
        }
        MatriceConfusion.push_back(row); // Add the row to the main vector
    }
    
    cout<<endl<<endl<<"Reconnaissance en utilisant la methode "<<methode<<endl;
    // Trois methodes
    if(methode==1)  model= cv::createEigenFaceRecognizer(n);
    else if(methode==2)  model= createFisherFaceRecognizer();
    else if(methode==3)  model= createLBPHFaceRecognizer();
    else exit(0);

    model->train(imagesApp, labelsApp);

    if(methode == 2){
        cv::Mat eigenvalues = model->getMat("eigenvalues");
        std::cout<<eigenvalues.size()<<std::endl;
    }
    int good=0, bad=0;

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    double trainingTime = t2-t1;

    for(unsigned int i=0; i<imagesTest.size(); i++){
        // The following line predicts the label of a given
        // test image:
        int predictedLabel = model->predict(imagesTest[i]);


        //
        // To get the confidence of a prediction call the model with:
        // 	    int predictedLabel = -1;
        // 	    double confidence = 0.0;
        // 	    model->predict(imagesTest[i], predictedLabel, confidence);
        //
        MatriceConfusion[labelsTest[i]-1][predictedLabel-1]++;
        // 	string result_message = format("Predicted class = %d / Actual class = %d / Confidence = %f.",predictedLabel, labelsTest[i], confidence);
        // 	cout << "Image name = "<<nameImgTest[i]<<" / " <<result_message << endl;
        //ofs << predictedLabel << " "<< labelsTest[i]<<std::endl;	
        if(predictedLabel==labelsTest[i]) good++;
        else{
            bad++;
            string result_message = format("Predicted class = %d / Actual class = %d.",predictedLabel, labelsTest[i]);
            cout << "Image name = "<<nameImgTest[i]<<" / " <<result_message << endl;
        }
    }

    //faire qch
    gettimeofday(&tim, NULL);
    double t3=tim.tv_sec+(tim.tv_usec/1000000.0);
    double classificationTime = t3-t2;
    
    cout<<"Temps d'apprentissage : " << trainingTime<< "s" << std::endl;
    cout<<"Temps de classification: " << classificationTime<< "s" << std::endl;
    cout<<"Temps de calcul: " << t3 - t1<< "s" << std::endl;
    cout<<"Nombre de reconnaissances bonnes : "<<good<<endl;
    cout<<"Nombre de reconnaissances mauvaises : "<<bad<<endl;
    cout<<"Taux de reconnaissance : "<<(double)good*100.0/(double)(good+bad)<<endl;
    
    //Affichage de la matrice de confusion
    cout << endl<<"Matrice de confusion"<<endl;
    //header line of matrix
    ofs<<"&"<<std::fixed;
    for (unsigned i = 0; i < nbPers; i++) {
        ofs<<i + 1<<"&";
    }

    ofs<<"r\\\\"<< std::endl<<"\\hline"<<std::endl;
    for (unsigned i = 0; i < nbPers; i++) {
        //first column
        ofs<<i + 1<<"&";
        for (unsigned int j = 0; j < nbPers; j++) {
            // cout<<MatriceConfusion[i][j]<<"\t";
            ofs<<MatriceConfusion[i][j]<<"&";
        }
        ofs<<getRecall(MatriceConfusion, i)<<"\\\\"<< std::endl<<"\\hline"<<std::endl;
        //ofs<<"\\\\"<<std::endl;
        //cout << endl;
    }

    ofs<<"p&"<<std::fixed;
    //print precision
    for (unsigned i = 0; i < nbPers; i++) {
        ofs<<getPrecision(MatriceConfusion, i)<<"&";
    }

    ofs<<"\\\\"<< std::endl<<"\\hline"<<std::endl;
    cout<<endl;
    ofs.close();
    return 0;
}
