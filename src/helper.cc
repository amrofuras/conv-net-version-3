#include "helper.h"

using namespace cv;
using namespace std;


// int to string
string i2str(int num){
    stringstream ss;
    ss<<num;
    string s = ss.str();
    return s;
}

// string to int
int str2i(string str){
    return atoi(str.c_str());
}

Vec3d Scalar2Vec3d(Scalar a){
    Vec3d res(a[0], a[1], a[2]);
    return res;
}

Scalar Vec3d2Scalar(Vec3d a){
	return Scalar(a[0], a[1], a[2]);
} 


void
unconcatenateMat(const vector<Mat> &src, vector<vector<Mat> > *dst, int vsize){
    for(int i = 0; i < src.size() / vsize; i++){
        vector<Mat> tmp;
        for(int j = 0; j < vsize; j++){
            Mat img;
            src[i * vsize + j].copyTo(img);
            vector<Mat> imgs;
            split(img, imgs);
            tmp.insert(tmp.end(), imgs.begin(), imgs.end());
            imgs.clear();
//            for(int ch = 0; ch < imgs.size(); ch++){
//                tmp.push_back(imgs[ch]);
//            }
        }
        dst -> push_back(tmp);
    }
}

Mat 
concatenateMat(const vector<vector<Mat> > &vec){
    int subFeatures = vec[0][0].rows * vec[0][0].cols;
    int height = vec[0].size() * subFeatures;
    int width = vec.size();
    Mat res = Mat::zeros(height, width, CV_64FC1);
    for(int i = 0; i < vec.size(); i++){
        for(int j = 0; j < vec[i].size(); j++){
            Rect roi = Rect(i, j * subFeatures, 1, subFeatures);
            Mat subView = res(roi);
            Mat ptmat = vec[i][j].reshape(0, subFeatures);
            ptmat.copyTo(subView);
        }
    }
    return res;
}

Mat 
concatenateMat(const vector<Mat> &vec, int matcols){
    vector<vector<Mat> > temp;
    unconcatenateMat(vec, &temp, vec.size() / matcols);
    return concatenateMat(temp);
}

void 
splitChannels(vector<vector<Mat> > &vec){
    vector<vector<Mat> > res;
    for(int i = 0; i < vec.size(); i++){
        vector<Mat> tpvec;
        for(int j = 0; j < vec[i].size(); j++){
            vector<Mat> imgs;
            split(vec[i][j], imgs);
            tpvec.insert(tpvec.end(), imgs.begin(), imgs.end());
            imgs.clear();
        }
        res.push_back(tpvec);
    }
    swap(vec, res);
}

double 
getLearningRate(const Mat &data){
    // see Yann LeCun's Efficient BackProp, 5.1 A Little Theory
    int nfeatures = data.rows;
    int nsamples = data.cols;
    //covariance matrix = x * x' ./ nfeatures;
    Mat Sigma = data * data.t() / nsamples;
    SVD uwvT = SVD(Sigma);
    Sigma.release();
    return 0.9 / uwvT.w.ATD(0, 0);
}


