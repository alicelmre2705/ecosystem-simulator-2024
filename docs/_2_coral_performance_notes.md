// Performance notes: iterating over the coral vector

// Duplicating state in memory is always risky
// <= risk of forgetting to update all related fields
// Example: bool available
// Remember to update it at the same time as coral_target and coral_id_target
// => 3 pieces of information that must stay in sync

// coral_id_target: we must keep it (required for save/export)
// However, we can avoid a dedicated bool available by keeping a vector of pointers to "available dead corals"

// To improve performance further, we can also keep a vector of pointers to living corals
// => avoids scanning the whole tab_coral vector to check whether a coral is ALIVE or DEAD.

// *****************************************
// Simulation.h
// *****************************************
class Simulation
{
  // ...
private:
  int number_update = 0;
  vector<Algae> tab_algae;
  vector<unique_ptr<Coral>> tab_coral;
  vector<Scavenger> tab_scavenger;

  vector<Coral *> tab_coral_alive;
  vector<Coral *> tab_coral_available;
};

// *****************************************
// Simulation.cc
// *****************************************

// READING

unique_ptr<Coral> ptr_coral = make_unique<Coral>(line);

if (number_segments >= 1)
{
  std::string subline1;
  for (int i = 0; i < number_segments; ++i)
  {
    getline(file >> std::ws, subline1);

    if (ptr_coral->read_segment_invalid(subline1))
    {
      return true;
    }
  }
}

if (ptr_coral->is_invalid(tab_coral))
{
  return true;
}

tab_coral.push_back(std::move(ptr_coral));
++count;

if (tab_coral.back()->get_status() == ALIVE)
{
  // If the coral is alive => store its address in tab_coral_alive
  tab_coral_alive.push_back(tab_coral.back().get());
}

// Reading scavengers
Scavenger scavenger(line);

if (scavenger.is_invalid(tab_coral))
{
  return true;
}

tab_scavenger.push_back(scavenger);
++count;

// UPDATE

void Simulation::update_corals()
{
  // Update only ALIVE corals
  int tab_coral_size = tab_coral_alive.size();
  for (int i = 0; i < tab_coral_size; ++i)
  {
    if (tab_coral_alive[i]->get_age() >= max_life_cor)
    {
      tab_coral_alive[i]->change_status_to_dead();

      // Store its address in tab_coral_available (no scavenger on it yet)
      tab_coral_available.push_back(tab_coral_alive[i]);

      // Remove it from tab_coral_alive
      swap(tab_coral_alive[i], tab_coral_alive.back());
      tab_coral_alive.pop_back();
    }
    else
    {
      Segment old_effector = tab_coral_alive[i]->get_last_segment();

      if (old_effector.length < l_repro)
      {
        update_coral_effector_inferior_lrepro(*tab_coral_alive[i], old_effector);
      }
      else
      {
        update_coral_effector_superior_lrepro(*tab_coral_alive[i]);
      }
    }
  }
}
