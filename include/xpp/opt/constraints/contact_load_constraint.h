/**
 @file    contact_load_constraint.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Mar 27, 2017
 @brief   Brief description
 */

#ifndef XPP_XPP_OPT_INCLUDE_XPP_OPT_CONTACT_LOAD_CONSTRAINT_H_
#define XPP_XPP_OPT_INCLUDE_XPP_OPT_CONTACT_LOAD_CONSTRAINT_H_

#include <xpp/constraint.h>
#include <xpp/opt/endeffector_load.h>
#include <xpp/opt/contact_schedule.h>


namespace xpp {
namespace opt {

class ContactLoadConstraint : public Constraint {
public:
  using EELoadPtr   = std::shared_ptr<EndeffectorLoad>;
  using ContactSchedulePtr = std::shared_ptr<ContactSchedule>;

  ContactLoadConstraint (const OptVarsPtr& opt_vars_container,
                         const ContactSchedulePtr&, const EELoadPtr&);
  virtual ~ContactLoadConstraint ();

  void UpdateConstraintValues () override;
  void UpdateBounds () override;

private:
  EELoadPtr ee_load_;
  ContactSchedulePtr contact_schedule_;
  std::vector<EndeffectorID> ee_ids_;
};

} /* namespace opt */
} /* namespace xpp */

#endif /* XPP_XPP_OPT_INCLUDE_XPP_OPT_CONTACT_LOAD_CONSTRAINT_H_ */
