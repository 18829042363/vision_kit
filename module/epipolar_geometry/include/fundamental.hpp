#ifndef _FUNDAMENTAL_HPP_
#define _FUNDAMENTAL_HPP_

#include <vector>
#include <opencv2/core/core.hpp>

namespace vk{

enum FundamentalType {
    FM_7POINT = 1,
    FM_8POINT = 2,
    FM_RANSAC = 4,
};
/**
 * [findFundamentalMat find fundamental matrix by matched points]
 * @param  pts_prev   [points in the first image]
 * @param  pts_next   [points in the second image]
 * @param  type       [type of algrithm]
 * @param  sigma      [sigma of Gaussian distribution of points]
 * @param  iterations [iteration times fot RANSAC. If set to -1, use self-adaptive iteration]
 * @return            [fundamental matrix]
 */
cv::Mat findFundamentalMat(const std::vector<cv::Point2f>& pts_prev, const std::vector<cv::Point2f>& pts_next, FundamentalType type=FM_8POINT,
    float sigma=1, int iterations=-1);

/**
 * Fundametal matrix to find
 */
class Fundamental
{
public:
    Fundamental(const std::vector<cv::Point2f>& pts_prev, const std::vector<cv::Point2f>& pts_next, FundamentalType type, float sigma=1, int iterations=-1);

    ~Fundamental();

/**
 * [slove find the fundamental matrix]
 * @return [3*3 fundamental matrix]
 */
    cv::Mat slove();

/**
 * [getInliers get inliers by Fundamental maxtix]
 * @return [inliers Mat, is inlier if set to 1]
 */
    cv::Mat getInliers();

/**
 * [getErrors get the distance from point to its epipolar line]
 * @param p1   [point in the first image]
 * @param p2   [point in the second image]
 * @param F    [3*3 array, fundamental matrix of the two images]
 * @param err1 [distance of p1 to its epipolar line]
 * @param err2 [distance of p2 to its epipolar line]
 */
    static inline void getErrors(const cv::Point2f& p1, const cv::Point2f& p2, const float* F, float& err1, float& err2);

private:
/**
 * [Normalize normalize points by isotropic scaling]
 * @param points      [points input for mormalizing]
 * @param points_norm [normalized points]
 * @param T           [Transform matrix of normalizing]
 */
    void Normalize(const std::vector<cv::Point2f>& points, std::vector<cv::Point2f>& points_norm, cv::Mat& T);

/**
 * [run8points 8-points algrothm]
 * @param  pts_prev [normalized points in the first image]
 * @param  pts_next [normalized points in the second image]
 * @param  T1       [transform matrix by normalizing in the first image]
 * @param  T2       [transform matrix by normalizing in the second image]
 * @return          [3*3 fundamental matrix]
 */
    cv::Mat run8points(const std::vector<cv::Point2f>& pts_prev, const std::vector<cv::Point2f>& pts_next, cv::Mat& T1, cv::Mat& T2);
/**
 * [runRANSAC RANSCA for 8-points algrothm]
 * @param  pts_prev [normalized points in the first image]
 * @param  pts_next [normalized points in the second image]
 * @param  T1       [transform matrix by normalizing in the first image]
 * @param  T2       [transform matrix by normalizing in the second image]
 * @param  inliners [output inliners]
 * @return          [3*3 fundamental matrix]
 */
    cv::Mat runRANSAC(const std::vector<cv::Point2f>& pts_prev, const std::vector<cv::Point2f>& pts_next, cv::Mat& T1, cv::Mat& T2, cv::Mat& inliners);

private:
    const FundamentalType run_type_;
    std::vector<cv::Point2f> pts_prev_;
    std::vector<cv::Point2f> pts_next_;
    std::vector<cv::Point2f> pts_prev_norm_;
    std::vector<cv::Point2f> pts_next_norm_;

    cv::Mat inliners_;
    cv::Mat T1_, T2_;
    cv::Mat F_;
    float sigma2_;
    int iterations_;
};


}//! vk

#endif