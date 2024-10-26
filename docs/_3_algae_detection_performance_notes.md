// Performance notes: detecting algae to eat

// *****************************************
// Lifeform.h
// *****************************************

class Coral : public Lifeform
{
  // ....
private:
  int id;
  CoralStatus status;
  CoralRotationDirection rotation_dir;
  CoralDevelopmentStatus dev_status;
  std::vector<Segment> segments;
  vector<shared_ptr<Algae>> nearby_algae;
};

// *****************************************
// Simulation.cc
// *****************************************

// READING
// For each coral, fill nearby_algae
// with algae located inside a circle of radius l_repro around the base of its effector

// UPDATE

// For each newly created algae:
// iterate through tab_coral_alive and add this algae if it is within the effector radius

// For each coral:
// - If nearby_algae is not empty, iterate over it to check if an algae is in reach.
//   If yes, eat it and remove it from nearby_algae.
// - If nearby_algae is empty, iterate through tab_algae to check if an algae is in reach,
//   and at the same time populate nearby_algae with algae within the effector radius.

// If the coral creates a new segment to extend (EXTEND):
// clear nearby_algae

// If the coral reproduces:
// keep nearby_algae intact (because the coral keeps the same effector base)
// populate nearby_algae for the new coral
