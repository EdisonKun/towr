/**
 @file    a_linear_constraint.cc
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    May 26, 2016
 @brief   Brief description
 */

#include <xpp/zmp/a_linear_constraint.h>
#include <xpp/zmp/optimization_variables.h>

namespace xpp {
namespace opt {

ALinearConstraint::ALinearConstraint ()
{
}

void
ALinearConstraint::Init (const MatVec& linear_equation)
{
  linear_equation_ = linear_equation;
}

ALinearConstraint::VectorXd
ALinearConstraint::EvaluateConstraint () const
{
  return linear_equation_.M*x_; // linear part respected in bounds
}

LinearEqualityConstraint::VecBound
LinearEqualityConstraint::GetBounds () const
{
  VecBound bounds;
  for (int i=0; i<linear_equation_.v.rows(); ++i) {
    Bound bound(-linear_equation_.v[i],-linear_equation_.v[i]);
    bounds.push_back(bound);
  }
  return bounds;
}

LinearInequalityConstraint::VecBound
LinearInequalityConstraint::GetBounds () const
{
  VecBound bounds;
  for (int i=0; i<linear_equation_.v.rows(); ++i) {
    Bound bound(-linear_equation_.v[i], kNoBound_.upper_);
    bounds.push_back(bound);
  }
  return bounds;
}

void
LinearSplineEqualityConstraint::UpdateVariables (const OptimizationVariables* opt_var)
{
  x_ = opt_var->GetVariables(VariableNames::kSplineCoeff);
}

LinearSplineEqualityConstraint::Jacobian
LinearSplineEqualityConstraint::GetJacobianWithRespectTo (std::string var_set) const
{
  Jacobian jac; // empy matrix

  if (var_set == VariableNames::kSplineCoeff) {
    // careful, .sparseView is only valid when the Jacobian is constant, e.g.
    // the constraints are all linear w.r.t. the decision variables.
    jac = linear_equation_.M.sparseView();
  }

  return jac;
}

} /* namespace zmp */
} /* namespace xpp */

