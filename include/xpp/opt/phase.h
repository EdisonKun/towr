/**
@file    phase_info.h
@author  Alexander W. Winkler (winklera@ethz.ch)
@date    Sep 3, 2016
@brief   Brief description
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_OPT_PHASE_INFO_H_
#define USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_OPT_PHASE_INFO_H_

#include "a_robot_interface.h"
#include <xpp/hyq/foothold.h>
#include <xpp/utils/eigen_std_conversions.h>
#include <iostream>
#include <vector>

namespace xpp {
namespace opt {

struct Contact {

  static int constexpr kFixedByStartStance = -1;

  Contact() {};
  Contact(int _id, EndeffectorID _ee) : id(_id), ee(_ee) {}
  int id = kFixedByStartStance; ///< a unique identifier for each contact,
  EndeffectorID ee = EndeffectorID::E0;
};

inline std::ostream& operator<<(std::ostream& out, const Contact& c)
{
  out  << c.ee << ": id: " << c.id;
  return out;
}

/** Information to represent different types of motion.
  */
class Phase {
public:
  using Foothold    = xpp::hyq::Foothold;
  using Vector2d    = Eigen::Vector2d;
  using FootholdVec = std::vector<Foothold>;
  using ContactVec  = std::vector<Contact>;

  ContactVec free_contacts_; // all the stance legs currently in contact but not fixed by start
  ContactVec swing_goal_contacts_; // what contacts the current swinglegs are swinging towards
  FootholdVec fixed_contacts_;
  int id_ = 0;
  double duration_ = 0.0;

  Phase() {};

  /** @param type     Whether this is a stance, step of flight phase.
    * @param n_completed_steps how many steps completed by the previous phases.
    * @param id       Each phase has a unique ID.
    * @param duration How many seconds this phase lasts.
    */

  // for hyq 4 legs means stance
  bool IsStep() const { return (free_contacts_.size() + fixed_contacts_.size()) != 4;  }


  // these should be ordered same as below (free before fixed)
  ContactVec GetAllContacts() const
  {
    ContactVec contacts;
    for (const auto& c_free : free_contacts_) {
      contacts.push_back(c_free);
    }

    for (const auto& c_fixed : fixed_contacts_) {
      contacts.push_back(Contact(-1, static_cast<EndeffectorID>(c_fixed.leg)));
    }

    return contacts;
  }

  // zmp_ think about caching this in CalcPhaseStampedVec for all nodes
  FootholdVec GetAllContacts(const utils::StdVecEigen2d& contacts_xy) const
  {
    FootholdVec contacts;

    for (const auto& c_free : free_contacts_) {
      Vector2d p = contacts_xy.at(c_free.id);
      double z = 0.0;
      contacts.push_back(Foothold(p.x(), p.y(), z, static_cast<hyq::LegID>(c_free.ee)));
    }

    for (const auto& c_fixed : fixed_contacts_) {
      contacts.push_back(c_fixed);
    }

    return contacts;
  }

};

inline std::ostream& operator<<(std::ostream& out, const Phase& p)
{
  out << "id: " << p.id_
      << "\t duration: " << p.duration_
      << "\n free contacts: ";

  for (auto c : p.free_contacts_)
    out << c << ";    ";

  out << "\n fixed contacts: ";
  for (auto c : p.fixed_contacts_)
    out << c << ";    ";

  return out;
}


struct PhaseInfoStamped {

  PhaseInfoStamped() : time_(0.0) {};
  double time_;
  Phase phase_;
};

using PhaseVec = std::vector<Phase>;
using PhaseStampedVec = std::vector<PhaseInfoStamped>;

} /* namespace opt */
} /* namespace xpp */


#endif /* USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_OPT_PHASE_INFO_H_ */
