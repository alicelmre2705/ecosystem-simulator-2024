// Version using raw pointers only (no unique_ptr)
// Advantage of unique_ptr: if there is an ownership error, the compiler usually catches it (won't compile)
// Risk with raw pointers: it may compile, but crash at runtime (segfault) and is harder to debug

// *****************************************
// Simulation.cc
// *****************************************

// READING

Coral *ptr_coral = new Coral(line);

if (number_segments >= 1)
{
  std::string subline1;
  for (int i = 0; i < number_segments; ++i)
  {
    getline(file >> std::ws, subline1);

    if (ptr_coral->read_segment_invalid(subline1))
      return true;
  }
}

if (ptr_coral->is_invalid(tab_coral))
  return true;

tab_coral.push_back(ptr_coral);

void Simulation::update_eating_scavenger(int i)
{
  S2d old_position = tab_scavenger[i].get_position();
  int number_segment_left = tab_scavenger[i].eat_coral();

  if (tab_scavenger[i].get_radius() >= r_sca_repro)
  {
    cout << "Scavenger index " << i << " is reproducting" << endl;
    tab_scavenger.push_back(tab_scavenger[i].reproduction(old_position));
  }

  cout << "Number of segments left for eaten coral: " << number_segment_left << endl;
  if (number_segment_left == 0)
  {
    // delete tab_scavenger[i].get_coral_target();

    for (int j = 0; j < tab_coral.size(); ++j)
    {
      // No need to use .get() to compare raw pointers
      if (tab_coral[j] == tab_scavenger[i].get_coral_target())
      {
        tab_scavenger[i].reinit_coral_target();
        cout << "Coral id " << tab_coral[j]->get_id() << " DISAPPEARS" << endl;
        swap(tab_coral[j], tab_coral.back());
        tab_coral.pop_back();
      }
    }

    tab_scavenger[i].change_status_scavenger();
    cout << "Scavenger index " << i << " is now FREE" << endl;
  }
}

// *****************************************
// Note: you CANNOT do the following, because the coral variable is destroyed when leaving the reading function
// => the object is destroyed, so the pointer stored in tab_coral would point to a non-existing object

Coral coral = Coral(line);

// ... test if coral is invalid
// tab_coral.push_back(&coral);

// *****************************************
// Lifeform.cc
// *****************************************

Coral *Coral::reproduction(int new_id)
{
  // Create a new coral with a first segment
  Coral *ptr_new_coral = new Coral(segments.back(), new_id);

  // Update the parent coral which detaches its last segment
  segments.pop_back();
  change_effector_length(l_repro / 2);

  return ptr_new_coral;
}

// No need to use .get() to copy a raw pointer into a raw pointer
bool Scavenger::id_target_invalid(vector<Coral *> &tab_coral)
{
  if (status_scavenger == 1)
  {
    for (size_t i = 0; i < tab_coral.size(); ++i)
    {
      if (tab_coral[i]->get_id() == coral_id_target)
      {
        coral_target = tab_coral[i];
        tab_coral[i]->change_disponibility();
        return false;
      }
    }
    cout << message::lifeform_invalid_id(coral_id_target) << endl;
    return true;
  }
  return false;
}
