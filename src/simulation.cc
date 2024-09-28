/**
 * @file simulation.cc
 * @author LEMAIRE Alice 0.5, CAPDEVILLE Maud 0.5
 * @version 0.3
 */

#include "simulation.h"

#include <fstream>
#include <random>
#include <sstream>
#include <string>

#include "lifeform.h"
#include "message.h"
#include "shape.h"

using namespace std;

enum read_state
{
  ALGUE,
  CORAIL,
  SCAVENGER
};

static uniform_real_distribution<double> distribution(1, 255);
static default_random_engine e;
static const double p(alg_birth_rate);

Simulation::Simulation() {}

bool Simulation::read_contain_invalid_data(std::string nom_fichier)
{
  e.seed(1);
  destroy_lifeforms();
  std::string line;
  std::ifstream fichier(nom_fichier);
  if (!fichier.is_open())
  {
    return true;
  }

  int number_algue, number_coral, number_scavenger, number_segments, count(0);
  bool count_initialized = false;
  read_state state = ALGUE;

  while (getline(fichier >> std::ws, line))
  {
    if (line[0] == '#')
    {
      continue;
    }
    if (line.empty())
    {
      continue;
    }
    std::istringstream data(line);
    switch (state)
    {
    case ALGUE:
    {
      if (!count_initialized)
      {
        data >> number_algue;
        count_initialized = true;
        if (number_algue == 0)
        {
          state = CORAIL;
          count_initialized = false;
          count = 0;
        }
        break;
      }
      Algue algue(line);

      if (algue.is_invalid())
      {
        return true;
      }

      tab_algue.push_back(algue);
      ++count;

      if (count == number_algue)
      {
        state = CORAIL;
        count_initialized = false;
        count = 0;
      }
      break;
    }
    case CORAIL:
    {
      if (!count_initialized)
      {
        data >> number_coral;
        count_initialized = true;
        if (number_coral == 0)
        {
          state = SCAVENGER;
          count_initialized = false;
          count = 0;
        }
        break;
      }
      // Parse the 8 coral fields from the line to reach number_segments (last field).
      {
        double px, py;
        int age_val, id_val, sc, dr, sd;
        data >> px >> py >> age_val >> id_val >> sc >> dr >> sd >> number_segments;
      }

      unique_ptr<Corail> ptr_corail = make_unique<Corail>(line);

      if (number_segments >= 1)
      {
        std::string subline1;
        for (int i = 0; i < number_segments; ++i)
        {
          getline(fichier >> std::ws, subline1);

          if (ptr_corail->read_segment_invalid(subline1))
          {
            return true;
          }
        }
      }

      if (ptr_corail->is_invalid(tab_corail))
      {
        return true;
      }

      tab_corail.push_back(std::move(ptr_corail));
      ++count;

      if (count == number_coral)
      {
        state = SCAVENGER;
        count_initialized = false;
        count = 0;
      }
      break;
    }
    case SCAVENGER:
    {
      if (!count_initialized)
      {
        data >> number_scavenger;
        count_initialized = true;
        if (number_scavenger == 0)
        {
          count_initialized = false;
          count = 0;
        }
        break;
      }

      Scavenger scavenger(line);

      if (scavenger.is_invalid(tab_corail))
      {
        return true;
      }

      tab_scavenger.push_back(scavenger);
      ++count;

      if (count == number_scavenger)
      {
        break;
      }
      break;
    }
    }
  }

  std::cout << message::success();
  return false;
}

void Simulation::save(ofstream &myfile)
{
  myfile.imbue(std::locale("C"));
  myfile << get_number_algue() << endl;
  for (size_t i = 0; i < tab_algue.size(); ++i)
  {
    const Algue &algue = tab_algue[i];
    myfile << "  " << algue.get_position().x << " " << algue.get_position().y << " "
           << algue.get_age() << endl;
  }
  myfile << endl;
  myfile << get_number_coral() << endl;
  for (size_t i = 0; i < tab_corail.size(); ++i)
  {
    const Corail &corail = *tab_corail[i];
    vector<Segment> corailSegments = corail.get_tab_segments();
    myfile << "  " << corail.get_position().x << " " << corail.get_position().y << " "
           << corail.get_age() << " " << corail.get_id() << " "
           << static_cast<int>(corail.get_status_coral()) << " "
           << static_cast<int>(corail.get_direction_rotation()) << " "
           << static_cast<int>(corail.get_status_dev()) << " "
           << corailSegments.size() << endl;

    for (size_t j = 0; j < corailSegments.size(); ++j)
    {
      myfile << "    " << corailSegments[j].angle << " " << corailSegments[j].length
             << endl;
    }
  }
  myfile << endl;
  myfile << get_number_scavenger() << endl;
  for (size_t i = 0; i < tab_scavenger.size(); ++i)
  {
    const Scavenger &scavenger = tab_scavenger[i];
    std::string cible;
    Statut_sca status_scavenger = scavenger.get_statut_sca();
    if (status_scavenger == MANGE)
      cible = " " + std::to_string(scavenger.get_id_target());
    myfile << "  " << scavenger.get_position().x << " " << scavenger.get_position().y
           << " " << scavenger.get_age() << " " << scavenger.get_radius() << " "
           << static_cast<int>(status_scavenger) << cible << endl;
  }
  myfile << endl;
}

void Simulation::destroy_lifeforms()
{
  tab_algue.clear();
  tab_corail.clear();
  tab_scavenger.clear();
}

void Simulation ::draw_simulation(World &monde,
                                  const Cairo::RefPtr<Cairo::Context> &cr, int height,
                                  int width)
{
  for (int i = 0; i < get_number_algue(); ++i)
  {
    tab_algue[i].draw(monde, cr, height, width);
  }

  for (int i = 0; i < get_number_coral(); ++i)
  {
    tab_corail[i]->draw(monde, cr, height, width);
  }

  for (int i = 0; i < get_number_scavenger(); ++i)
  {
    tab_scavenger[i].draw(monde, cr, height, width);
  }
}

void Simulation::update(bool checkbutton_seaweed)
{
  increment_number_update();

  update_algues(checkbutton_seaweed);

  update_corals();

  update_scavengers();
}

void Simulation::update_algues(bool checkbutton_seaweed)
{
  for (int i = 0; i < static_cast<int>(tab_algue.size()); ++i)
  {
    tab_algue[i].increment_age();
    if (tab_algue[i].get_age() >= static_cast<int>(max_life_alg))
    {
      swap(tab_algue[i], tab_algue.back());
      tab_algue.pop_back();
      --i; // Re-check the swapped element now at position i
    }
  }

  if (checkbutton_seaweed)
    create_seaweed();
}

void Simulation::update_corals()
{
  // Snapshot the size so newly reproduced corals are not updated this step.
  const int tab_corail_size = static_cast<int>(tab_corail.size());
  for (int i = 0; i < tab_corail_size; ++i)
  {
    if (tab_corail[i]->get_status_coral() == ALIVE)
    {
      tab_corail[i]->increment_age();
      if (tab_corail[i]->get_age() >= static_cast<int>(max_life_cor))
      {
        tab_corail[i]->change_status_to_dead();
      }
      else
      {
        Segment old_effector = tab_corail[i]->get_last_segment();

        if (old_effector.length < l_repro)
        {
          update_coral_effector_inferior_lrepro(*tab_corail[i], old_effector);
        }
        else
        {
          update_coral_effector_superior_lrepro(*tab_corail[i]);
        }
      }
    }
  }
}

void Simulation::update_coral_effector_inferior_lrepro(Corail &coral,
                                                       Segment old_effector)
{
  int index_closest_algue = 0;
  bool index_algue_initialized = false;
  double angle_effector_closest_algue = 0.;
  find_closest_algue(coral, old_effector, index_closest_algue, index_algue_initialized,
                     angle_effector_closest_algue);

  if (index_algue_initialized)
  {
    coral_eat_algue(coral, old_effector, index_closest_algue,
                    angle_effector_closest_algue, index_closest_algue);
  }
  else
  {
    coral_turn(coral, old_effector);
  }
}

void Simulation::find_closest_algue(Corail &coral, Segment old_effector,
                                    int &index_closest_algue,
                                    bool &index_algue_initialized,
                                    double &angle_effector_closest_algue)
{
  for (int j = 0; j < static_cast<int>(tab_algue.size()); ++j)
  {
    double distance_effector_algue =
        distance(old_effector.base, tab_algue[j].get_position());

    if (distance_effector_algue <= old_effector.length)
    {
      double angle_effector_algue =
          angular_deviation_point_segment(tab_algue[j].get_position(), old_effector);

      if (abs(angle_effector_algue) <= delta_rot)
      {
        if ((coral.get_direction_rotation() == TRIGO && angle_effector_algue >= 0) ||
            (coral.get_direction_rotation() == INVTRIGO && angle_effector_algue < 0))
        {
          if (!index_algue_initialized)
          {
            index_closest_algue = j;
            index_algue_initialized = true;
            angle_effector_closest_algue = angle_effector_algue;
          }
          else
          {
            // ON A OUBLIE d'utiliser les valeurs aboslues pour comparer les ecarts angulaires
            if (abs(angle_effector_algue) < abs(angle_effector_closest_algue))
            {
              index_closest_algue = j;
              angle_effector_closest_algue = angle_effector_algue;
            }
          }
        }
      }
    }
  }
}

void Simulation::coral_eat_algue(Corail &coral, Segment old_effector,
                                 int index_algue_initialized,
                                 double angle_effector_closest_algue,
                                 int index_closest_algue)
{
  Segment futur_effector;
  futur_effector.base = old_effector.base;
  futur_effector.angle = norm_pi(old_effector.angle + angle_effector_closest_algue);
  futur_effector.length = old_effector.length + delta_l;
  futur_effector.extremity.x =
      futur_effector.base.x + futur_effector.length * cos(futur_effector.angle);
  futur_effector.extremity.y =
      futur_effector.base.y + futur_effector.length * sin(futur_effector.angle);

  coral.replace_effector(futur_effector);

  if (coral.collision(epsil_zero, tab_corail, old_effector))
  {
    coral.replace_effector(old_effector);
    coral.change_direction_rotation();
  }
  else
  {
    swap(tab_algue[index_closest_algue], tab_algue.back());
    tab_algue.pop_back();
  }
}

void Simulation::coral_turn(Corail &coral, Segment old_effector)
{
  Segment futur_effector;

  futur_effector.base = old_effector.base;
  futur_effector.length = old_effector.length;

  if (coral.get_direction_rotation() == TRIGO)
  {
    futur_effector.angle = norm_pi(old_effector.angle + delta_rot);
  }
  else
  {
    futur_effector.angle = norm_pi(old_effector.angle - delta_rot);
  }

  futur_effector.extremity.x =
      futur_effector.base.x + futur_effector.length * cos(futur_effector.angle);
  futur_effector.extremity.y =
      futur_effector.base.y + futur_effector.length * sin(futur_effector.angle);

  coral.replace_effector(futur_effector);
  if (coral.collision(epsil_zero, tab_corail, old_effector))
  {
    coral.replace_effector(old_effector);
    coral.change_direction_rotation();
  }
}

void Simulation::update_coral_effector_superior_lrepro(Corail &coral)
{
  if (coral.get_status_dev() == REPRO)
  {
    coral.add_segment();
    int id_new_coral = get_max_coral_id() + 1;
    tab_corail.push_back(coral.reproduction(id_new_coral));
    coral.change_status_dev(EXTEND);
  }
  else // EXTEND
  {
    coral.add_segment();
    coral.change_status_dev(REPRO);
  }
}

void Simulation::update_scavengers()
{
  // Re-evaluate size each iteration: handle_dead_scavenger may shrink the vector.
  for (int i = 0; i < static_cast<int>(tab_scavenger.size()); ++i)
  {
    update_scavenger_age_and_life(i);
  }
}

void Simulation::update_scavenger_age_and_life(int i)
{
  tab_scavenger[i].increment_age();
  if (tab_scavenger[i].get_age() >= static_cast<int>(max_life_sca))
  {
    handle_dead_scavenger(i);
  }
  else
  {
    if (tab_scavenger[i].get_statut_sca() == MANGE)
    {
      update_eating_scavenger(i);
    }
    else // LIBRE: move toward the nearest available dead coral
    {
      update_free_scavenger(i);
    }
  }
}

void Simulation::handle_dead_scavenger(int i)
{
  if (tab_scavenger[i].get_coral_target())
  {
    tab_scavenger[i].get_coral_target()->change_disponibility();
  }

  swap(tab_scavenger[i], tab_scavenger.back());
  tab_scavenger.pop_back();
}

void Simulation::update_eating_scavenger(int i)
{
  S2d old_position = tab_scavenger[i].get_position();

  int number_segment_left = tab_scavenger[i].eat_coral();

  if (tab_scavenger[i].get_radius() >= static_cast<int>(r_sca_repro))
  {
    Scavenger new_sca = tab_scavenger[i].reproduction(old_position);
    tab_scavenger.push_back(new_sca);
  }

  if (number_segment_left == 0)
  {
    for (int j = 0; j < static_cast<int>(tab_corail.size()); ++j)
    {
      if (tab_corail[j].get() == tab_scavenger[i].get_coral_target())
      {
        tab_scavenger[i].reinit_coral_target();
        swap(tab_corail[j], tab_corail.back());
        tab_corail.pop_back();
        break;
      }
    }

    tab_scavenger[i].change_status_scavenger();
  }
}

void Simulation::update_free_scavenger(int i)
{
  int index_closest_coral = 0;
  bool index_initialized = false;
  double distance_scavenger_closest_effector = 0.;

  find_closest_coral(i, index_closest_coral, index_initialized,
                     distance_scavenger_closest_effector);

  if (index_initialized) // there is an available dead coral
  {
    if (distance_scavenger_closest_effector <= delta_l)
    {
      tab_scavenger[i].set_position(
          tab_corail[index_closest_coral]->get_tab_segments().back().extremity);

      tab_scavenger[i].change_status_scavenger();
      tab_scavenger[i].set_coral_target(*tab_corail[index_closest_coral]);
      tab_corail[index_closest_coral]->change_disponibility();
    }
    else
    {
      tab_scavenger[i].move(*tab_corail[index_closest_coral]);
    }
  }
}

void Simulation::find_closest_coral(int i, int &index_closest_coral,
                                    bool &index_initialized,
                                    double &distance_scavenger_closest_effector)
{
  for (int j = 0; j < static_cast<int>(tab_corail.size()); ++j)
  {
    if (tab_corail[j]->get_status_coral() == DEAD &&
        tab_corail[j]->get_disponibility())
    {
      double distance_scavenger_effector =
          distance(tab_scavenger[i].get_position(),
                   tab_corail[j]->get_tab_segments().back().extremity);

      if (!index_initialized ||
          distance_scavenger_effector < distance_scavenger_closest_effector)
      {
        index_closest_coral = j;
        distance_scavenger_closest_effector = distance_scavenger_effector;
        index_initialized = true;
      }
    }
  }
}

void Simulation ::create_seaweed()
{
  bernoulli_distribution b(p);

  if (b(e))
  {
    double random_between_x = distribution(e);
    double random_between_y = distribution(e);

    Algue algue((int)random_between_x, (int)random_between_y);
    tab_algue.push_back(algue);
  }
}

int Simulation::get_number_update() const { return number_update; }

void Simulation::increment_number_update() { number_update += 1; }

int Simulation::get_number_algue() const { return tab_algue.size(); }

int Simulation::get_number_coral() const { return tab_corail.size(); }

int Simulation::get_number_scavenger() const { return tab_scavenger.size(); }

int Simulation::get_max_coral_id() const
{
  int maxId = 0;
  for (int i(0); i < get_number_coral(); ++i)
  {
    if (tab_corail[i]->get_id() > maxId)
    {
      maxId = tab_corail[i]->get_id();
    }
  }
  return maxId;
}
