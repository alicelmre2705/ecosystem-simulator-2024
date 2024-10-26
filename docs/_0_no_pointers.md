// Version without any pointers

// *****************************************
// Simulation.cc
// *****************************************

// For EACH update (simulation.update())
// For each scavenger in tab_scavengers

// If status == EATING (=> coral_id_target is not empty)

// Call the eat() method on the target coral
// Find the target coral inside tab_coral
for (size_t i = 0; i < tab_coral.size(); ++i)
{
  if (tab_coral[i].get_id() == coral_id_target)
  {
    scavenger.eat(tab_coral[i]);
  }
}

// *****************************************
// Lifeform.cc
// *****************************************

int Scavenger::eat(Coral &coral)
{
  int coral_segments_number = coral.get_tab_segments().size();

  // If the length of the last segment is <= delta_l
  // => remove the last segment
  if (coral.get_tab_segments().back().length <= delta_l)
  {
    coral.get_tab_segments().pop_back();
    coral_segments_number -= 1;
  }
  else
  {
    // Otherwise, the scavenger bites a piece of the target coral
    coral.change_effector_length(l_repro - l_seg_interne);
  }

  // The scavenger moves to the extremity of the last segment of the coral
  position = coral.get_tab_segments().back().extremite;

  // The scavenger grows
  radius += delta_r_sca;

  return coral_segments_number;
}
