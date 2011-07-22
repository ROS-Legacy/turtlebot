#ifndef _ESTIMATE_KINECT_LINK_
#define _ESTIMATE_KINECT_LINK_

#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry> 
#include <Eigen/StdVector>
#include <cminpack.h>

typedef std::vector<Eigen::Vector3f,Eigen::aligned_allocator<Eigen::Vector3f> > PointVector;

const double pi = std::acos(-1.0);

class ObjectPose
{
  public:
    ObjectPose() : translation(0,0,0), orientation(1,0,0,0) { }
    ObjectPose(Eigen::Vector3f trans, Eigen::Quaternionf orient): translation(trans), orientation(orient) { }
    ObjectPose(Eigen::Quaternionf orient) : translation(0,0,0),  orientation(orient) { }
    ObjectPose(Eigen::Vector3f trans) : translation(trans), orientation(1,0,0,0) { }
    
    Eigen::Vector3f translation;
    Eigen::Quaternionf orientation;
    
    Eigen::Transform<float,3,Eigen::Affine> transform()
    {
      Eigen::Transform<float,3,Eigen::Affine> newtrans(orientation);
      newtrans.pretranslate(translation);
      
      return newtrans;
    }
};

enum EstimatorMask
{
  ESTIMATOR_MASK_ALL, ESTIMATOR_MASK_ROTATION, ESTIMATOR_MASK_XYZ, ESTIMATOR_MASK_XY
};

class EstimateKinectTransform
{
  public:
    EstimateKinectTransform() : result_transform(Eigen::Translation3f(0,0,0)), mask(ESTIMATOR_MASK_ALL) {}
  
    
  
    void addData(ObjectPose base_pose_, PointVector target_points_);
    
    void computeTransform(Eigen::Transform<float,3,Eigen::Affine> guess);
    double computeTotalCost();
    Eigen::Transform<float, 3, Eigen::Affine> getTransform();
    
    static int functionToOptimize (void *p, int m, int n, const double *x, double *fvec, int iflag);
    double computeError(int m, Eigen::Transform<float, 3, Eigen::Affine> t_base_kinect);
    double computeTotalCost(Eigen::Transform<float, 3, Eigen::Affine> transform);
  
    void setMask(EstimatorMask mask_);
    void transformFromX(const double* x, Eigen::Transform<float, 3, Eigen::Affine>& transform);
    void xFromTransform(const Eigen::Transform<float, 3, Eigen::Affine>& transform, double* x);
  
  public:
  //private:
    double reprojectionError(Eigen::Vector3f point_obs, Eigen::Vector3f point_calc);
  
    std::vector<ObjectPose,Eigen::aligned_allocator<ObjectPose> > base_pose;
    std::vector<PointVector> target_points;
    Eigen::Transform<float, 3, Eigen::Affine> result_transform;
    Eigen::Transform<float, 3, Eigen::Affine> input_transform;
    EstimatorMask mask;
    
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif
