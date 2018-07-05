/*!
 * @file        artic.h
 * @brief       A kinematics library for articulated robot arm
 * @author      Chien-Pin Chen
 */

#ifndef RB_ARTIC_H_
#define RB_ARTIC_H_

#include "kinematic_chain.h"


namespace rb //! Robot Arm Library namespace
{
namespace kin //! Kinematics module namespace
{
// Class of kinematics of articulated (6-axis) robot arm
/*! @class Artic     artic.h
 *  @brief A kinematics class for articulated (6-axis) robot arm
 *  Artic implement the kinematics of articulated (6-axis) robot arm
 */
class Artic
{
public:
  /*! Default Constuctor */
  Artic();

  /*! Constructor with certain robot arm data. */
  Artic(
      const rb::math::Array6& a0,              //!< Link length of all links (mm)
      const rb::math::Array6& alpha0,          //!< Twist angle of all links (degree)
      const rb::math::Array6& d0,              //!< Link offset of all links (mm)
      const rb::math::Array6& ini_theta,       //!< Initial value of all joint angles
      const rb::math::Array6& uplimit0,        //!< Upper limit of all joints
      const rb::math::Array6& lowlimit0        //!< Lower limit of all joints
      );

  /*! Destructor */
  ~Artic();

  /*!
   * @brief Compute forward Kinematics for given angle, update each joint
   *  value, and return current current position and orientation of TCP.
   * @param q         An array of joint degree.
   * @return ArmPose  a structure include position and orientation of TCP.
   */
  ArmPose forwardKin(const rb::math::Array6& q);

  /*!
   * @brief Compute inverse kinematics for given position and orientation in
   *        Cartesian coordination system.
   * @return IK_RESULT  a enumerator indicates the result of inverse kinematics.
   */
  IK_RESULT inverseKin(
      const double& x,            //!< x value of the position.
      const double& y,            //!< y value of the position.
      const double& z,            //!< z value of the position.
      const double& roll,         //!< roll value of the orientation.
      const double& pitch,        //!< pitch value of the orientation.
      const double& yaw,          //!< yaw value of the orientation.
      rb::math::Array6& joints,   //!< the best fittest IK solution.
      ArmAxisValue& all_sols      //!< a data structure to store all possible solutions.
      );

  /*!
   * @brief Compute inverse kinematics for two links (pitch-pitch) structure
   *        by given first joint.
   * @param th1_rad  angle (radian) of first joint.
   * @param p0       the position of wrist point wrt joint 0 (base)
   * @param config   a vector of 3 bits to indicator 8 configuration (solutions) of robot arm.
   * @param all_sols
   * @return
   */
  void solvePitchPitchIK(const double& th1_rad, const rb::math::Vector4& p0,
      const std::vector<bool>& config,
      ArmAxisValue& all_sols);

  /*!
   * @brief Compute inverse kinematics for wrist mechanism (row-pitch-row) structure
   *        by given first joint.
   * @param th1_rad  angle (radian) of first joint.
   * @param config   a vector of 3 bits to indicator 8 configuration (solutions) of robot arm.
   * @param flange_tr a H.T. Matrix of robot flange with respect to it base.
   * @param all_sols a structure to storage all possible solutions of inverse kinematics.
   */
  void solveRowPitchRowIK(const double& th1_rad, const std::vector<bool>& config,
      const rb::math::Matrix4& flange_tr,
      ArmAxisValue& all_sols);

  /*!
   * @brief Find the solutions in all_sols that is most fit to previous joints value
   * @param sols a structure to storage all possible solutions of inverse kinematics.
   */
  IK_RESULT solutionCheck(ArmAxisValue& sols);

  /*!
   * @brief Check the value of individual joint depend join limits. Will map
   *        map the value to corrected range if possible.
   * @param njoint    index of joint indicate which joint is examined.
   * @param rad       the value of joint will be check and update as output.
   */
  void preCheck(const int& njoint, double& rad);

  /*!
   * @brief Compute homogeneous transformation matrix for given link properties,
   *   and return the matrix.
   * @param A         Given link length.
   * @param alpha     Given link twist.
   * @param D         Given link offset.
   * @param theta     Given joint angle.
   * @return  Homogeneous transformation matrix of given link properties.
   */
  rb::math::Matrix4 homoTrans(double& A, double& alpha, double& D, const double& theta);

  /*!
   * @brief Get the position and orientation of the robot arm.
   * @return  ArmPose
   */
  ArmPose getArmPose(void);

  /*!
   * @brief Set the HT matrix of the offset b/w the arm flange and
   *        equipped tool or end-effector.
   * @param tool_offset
   * @return bool     Check if setting success
   */
  bool setToolOffset(const ArmPose& tool_offset);

  /*!
   * @brief Set the Homogeneous Transformation matrix of the working base of the robot arm.
   * @param base  a Homogeneous Transformation matrix from World -> the robot base.
   * @return
   */
  void setBase(rb::math::Matrix4& base);

  /*!
   * @brief Get the HT matrix of the working base of the robot arm.
   * @return rb::math::Matrix4
   */
  rb::math::Matrix4 getBase(void);

  /*!
   * @brief Get the link length `a` value between each joint of the robot arm.
   * @return rb::math::Array6
   */
  rb::math::Array6 getA(void);

  /*!
   * @brief Get the link twist \f$\alpha\f$  value between each joint of the robot arm.
   * @return rb::math::Array6
   */
  rb::math::Array6 getAlpha(void);

  /*!
   * @brief Get the link offset `d` value between each joint of the robot arm.
   * @return rb::math::Array6
   */
  rb::math::Array6 getD(void);

  /*!
   * @brief Get the joint angle \f$\theta\f$ value of each joint of the robot arm.
   * @return rb::math::Array6
   */
  rb::math::Array6 getTheta(void);

  /*!
   * @brief Set the upper limits of joint angles for the robot arm.
   * @param up_lim    An rb::math::Array6 contain the upper limit of all joints.
   */
  void setUpLimit(rb::math::Array6& up_lim);

  /*!
   * @brief Get the upper limits of joint angles of the robot arm.
   * @return rb::math::Array6
   */
  rb::math::Array6 getUpLimit(void);

  /*!
   * @brief Set the lower limits of joint angles for the robot arm.
   * @param low_lim   An rb::math::Array6 contain the lower limit of all joints.
   */
  void setLowLimit(rb::math::Array6& low_lim);

  /*!
   * @brief Get the lower limits of joint angles of the robot arm.
   * @return rb::math::Array6
   */
  rb::math::Array6 getLowLimit(void);


protected:
  /*! Link length data member of modified D-H parameter for robot arm */
  rb::math::Array6 a;                          //!< Link length (mm)
  /*! Link twist data member of modified D-H parameter for robot arm */
  rb::math::Array6 alpha;                      //!< Link twist angle (degree)
  /*! Link offset data member of modified D-H parameter for robot arm */
  rb::math::Array6 d;                          //!< Link offset (mm)
  /*! Joint angle data member of modified D-H parameter for robot arm */
  rb::math::Array6 theta;                      //!< Joint angle (degree)
  rb::math::Array6 uplimit;                    //!< Upper limit of all joints
  rb::math::Array6 lowlimit;                   //!< Lower limit of all joints
  rb::math::Matrix4 work_base_T;               //!< Homogeneous transformation matrix of robot base with respect to world
  rb::math::Matrix4 work_base;                 //!< Position & Orientation of TCP in world coordination

private:
  rb::math::Array6 m_ini_theta;            //!< Storage initialize 6 joint angle
  rb::math::Array6 m_pre_theta;            //!< Storage previous set of 6 joint angle, this will use to precheck result of IK
  rb::math::Matrix4 m_T_act;               //!< HT matrix of TCP with respected to work base
  rb::math::Matrix4 m_tool_T;              //!< HT matrix of TCP with respected to 6th joint (last joint coordination)
  ArmPose m_pos_act;                       //!< position & orientation (x,y,z,a,b,c) of TCP, and HT matrix of each joint & work base
  int pre_fit_solution;                    //!< The index of configuration of previous IK

  // private functions
  // TODO: could move functions for matrix manipulating, such as rpy2tr & tr2rpy to math.h
  void tr2rpy(const rb::math::Matrix4& m, double& roll_z, double& pitch_y, double& yaw_x);
  void rpy2tr(double& roll_z, double& pitch_y, double& yaw_x, rb::math::Matrix4& tool_mat);
  rb::math::Matrix4 rotateX(const double& deg);
  rb::math::Matrix4 rotateY(const double& deg);
  rb::math::Matrix4 rotateZ(const double& deg);
};

}       // namespace kin
}       // namespace rb

#endif  // RB_ARTIC_H_
