void read_csv(const string& filename, vector<cv::Mat>& images,
	      cv::Mat& labels, char separator = ';')
{
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    int j = 0;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
	    int type = atoi(classlabel.c_str());
	    cv::Mat image = imread(path, 0);
	    images.push_back(image);
	    labels.at<float>(0, j++) = (double)type;
        }
    }
}

void train(const string file_name)
{
    vector<cv::Mat> images;
    cv::Mat labels(1, 1300, CV_32FC1);
    read_csv(file_name, images, labels);
    /*
      Get labels and images correspondance here
     */
}
