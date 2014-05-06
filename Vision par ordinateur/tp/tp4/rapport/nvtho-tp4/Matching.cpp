
#include "Matching.h"

#define drawCircle(img, center, r, color)\
        cv::circle(img, center, r, color)

#define drawLine(img, p1, p2, color)\
        cv::line(img, p1, p2, color)

#define debug(str)\
    std::cout<<str<<std::endl;

int main(int argc, char ** argv){
    //devoir utiliser parametre
    if(argc < 2){
        printf("Parammeter is not enough, use ./matching image1 image2");
        return 2;
    }
    double thr = 3;

    if(argc > 3){
        char *ptr;
        thr = strtod(argv[3], &ptr);
    }
    Matching matching(argv[1], argv[2], thr);
    matching.match();
    matching.heuristicMatch();
    return 0;
}

Matching::Matching(char *img1, char *img2, double thr):threshold(thr){
    image1 = cv::imread(img1, CV_LOAD_IMAGE_UNCHANGED);
    image2 = cv::imread(img2, CV_LOAD_IMAGE_UNCHANGED);
    //image identity, if found l.jpg then remove l
    std::string str = std::string(img1);
    //remove directory
    size_t lastSlash = str.find_last_of("/");
    if(lastSlash != std::string::npos){
        str.replace(str.begin(), str.begin() + lastSlash + 1, ""); 
    }
    size_t found = str.find_last_of("l.");
    //remove .jpg
    if(found == std::string::npos){
        imageIdentify = str.replace(str.end() - 4, str.end(), "");
    }else{//remove l.jpg
        imageIdentify = str.replace(str.end() - 5, str.end(), "");
    }
debug(imageIdentify);
    writeRansac.open(OUTPUT_DIR + RANSAC + TEXT_OUTPUT, std::ios::out | std::ios::app);
    writeHeuristic.open(OUTPUT_DIR + HEURISTIC + TEXT_OUTPUT, std::ios::out | std::ios::app);

    getDescriptors();
}

Matching::~Matching(){
    writeRansac.close();
    writeHeuristic.close();
}
void Matching::heuristicMatch(){
    int count = 0;
    cv::vector<cv::DMatch> matches;
    cv::vector<cv::DMatch> goodMatches;

    matcher.match(descriptors1, descriptors2, matches);

    double minDist = 10000;

    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors1.rows; i++ ){
        double dist = matches[i].distance;
        if( dist < minDist ){
            minDist = dist;
        }
    }
    minDist = minDist > DISTANCE_THRESHOLD ? minDist : DISTANCE_THRESHOLD;
    cv::Mat imageOutput = image1.clone();

    for( int i = 0; i < descriptors1.rows; i++ ){
debug(matches[i].distance);
        if( matches[i].distance <= threshold*minDist ){
            int index1 = matches[i].queryIdx;
            int index2 = matches[i].trainIdx;
            cv::Point2f point1 = keyPoints1[index1].pt;
            cv::Point2f point2 = keyPoints2[index2].pt;
            //draw circle
            cv::circle(imageOutput, point1, 3, color);
            cv::line(imageOutput, point1, point2, color, 2);
            goodMatches.push_back( matches[i]);
            count++;
        }
    }

    cv::Mat imgMatches;
    cv::drawMatches( image1, keyPoints1, image2, keyPoints2,
            goodMatches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
            std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    //cv::imshow( "Good Matches", imgMatches );
    //cv::imshow( "Sym Matches", imgMatches2 );
    //cv::imshow( "Result", imageOutput);
    
    //write results
    std::string imgMatch2images = OUTPUT_DIR + HEURISTIC + MATCH_DRAW + imageIdentify +  IMAGE_EXTENSION;
    std::string imgFinal = OUTPUT_DIR + HEURISTIC + FINAL_RESULT + imageIdentify +  IMAGE_EXTENSION;
    cv::imwrite(imgMatch2images, imgMatches); 
    cv::imwrite(imgFinal, imageOutput); 
    writeHeuristic<<imageIdentify<<" "<<count<<std::endl;
}

void Matching::match(){
    int count = 0;
    cv::vector<cv::DMatch> matches;
    cv::vector<cv::DMatch> matches2;
    cv::vector<cv::DMatch> goodMatches;

    cv::Mat imageOutput = image1.clone();

    matcher.match(descriptors1, descriptors2, matches);
    matcher.match(descriptors2, descriptors1, matches2);

    //using symetry test 
    cv::vector<cv::DMatch> symmetryMatches = getSymetryMatches(matches, matches2);

    std::vector<cv::Point2f> points1;
    std::vector<cv::Point2f> points2;

    for(int i = 0; i < symmetryMatches.size(); i++){
        int index1 = symmetryMatches[i].queryIdx; 
        int index2 = symmetryMatches[i].trainIdx; 
        points1.push_back(keyPoints1[index1].pt);
        points2.push_back(keyPoints2[index2].pt);
    }

    //fundamentalMatrix = cv::findFundamentalMat();
    std::vector<uchar> status(points1.size(),0);
    fundamentalMatrix = cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2),CV_FM_RANSAC, RANSAC_PARAM1, RANSAC_PARAM2, status); 

    //iterate over status to check whether the point is inlier or outlier
    std::vector<uchar>::const_iterator itStatus = status.begin();
    std::vector<cv::DMatch>::const_iterator itMatches = symmetryMatches.begin();
    std::vector<cv::Point2f>::const_iterator itPoints1 = points1.begin();
    std::vector<cv::Point2f>::const_iterator itPoints2 = points2.begin();
    for(; itStatus != status.end(); itStatus++, itMatches++, itPoints1++, itPoints2++){
        //status == 1
        if(*itStatus){
            cv::circle(imageOutput, *itPoints1, 3, color);
            cv::line(imageOutput, *itPoints1, *itPoints2, color, 2);
            goodMatches.push_back(*itMatches);
            count++;
        }
    }
    //Draw only "good" matches
    cv::Mat imgMatches;
    cv::drawMatches( image1, keyPoints1, image2, keyPoints2,
            goodMatches, imgMatches, cv::Scalar::all(-1), cv::Scalar::all(-1),
            std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    //cv::imshow( "Good Matches", imgMatches );
    //cv::imshow( "Sym Matches", imgMatches2 );
    //cv::imshow( "Result", imageOutput);
    
    //write results
    std::string imgMatch2images = OUTPUT_DIR + RANSAC + MATCH_DRAW + imageIdentify + IMAGE_EXTENSION;;
    std::string imgFinal = OUTPUT_DIR + RANSAC + FINAL_RESULT + imageIdentify + IMAGE_EXTENSION;
    cv::imwrite(imgMatch2images, imgMatches); 
    cv::imwrite(imgFinal, imageOutput); 
    writeRansac<<imageIdentify<<" "<<count<<std::endl;
//    while(cv::waitKey() != 27){
//
//    };
}

cv::vector<cv::DMatch> Matching::getSymetryMatches(
        const cv::vector<cv::DMatch> &matches1, const cv::vector<cv::DMatch> &matches2){

    cv::vector<cv::DMatch> symmetryMatches;
    for(int i = 0; i < matches1.size(); i++){
        for (int j = 0; j < matches2.size(); j++){
            if(matches1[i].queryIdx == matches2[j].trainIdx && matches1[i].trainIdx == matches2[j].queryIdx ){
                symmetryMatches.push_back(cv::DMatch(matches1[i].queryIdx, matches1[i].trainIdx, matches1[i].distance));
                break;
            }
        }
    }
    return symmetryMatches;
}

void Matching::getDescriptors(){
    featureDetector.detect(image1, keyPoints1);  
    featureDetector.detect(image2, keyPoints2);  

    descriptorExtractor.compute(image1, keyPoints1, descriptors1);
    descriptorExtractor.compute(image2, keyPoints2, descriptors2);
}

cv::Point2f Matching::transformPoint(cv::Point2f p2){
    cv::Point2f re;
    re.x = p2.x * image1.cols/image2.cols;
    re.y = p2.y * image1.rows/image2.rows;
    return re;
}

void Matching::combineImage(){
    int cols = image1.cols + image2.cols;
    int rows = image1.rows > image2.rows ? image1.rows : image2.rows;
    output = cv::Mat::zeros(rows, cols, image2.type());
    //image1 is heigher than image2
    if(image1.rows >= rows){
        image1.copyTo(output(cv::Rect(0, 0, image1.cols, image1.rows)));
        image2.copyTo(output(cv::Rect(image1.cols, rows - image2.rows, image2.cols, image2.rows)));
    }else{
        image1.copyTo(output(cv::Rect(0, rows - image2.rows, image1.cols, image1.rows)));
        image2.copyTo(output(cv::Rect(cols - image1.cols, 0, image2.cols, image2.rows)));
    }
}
