/**
 * @file projet.cc
 * @author LEMAIRE Alice 0.5, CAPDEVILLE Maud 0.5
 * @version 0.3
 */

#include <cstdlib>
#include <string>

#include "gui.h"

int main(int argc, char *argv[])
{
  Simulation simulation;

  if (argc == 2)
  {
    std::string fichier = argv[1];
    if (simulation.read_contain_invalid_data(fichier))
      simulation.destroy_lifeforms();
  }

  // Optional headless mode for automated tests and CI.
  // Set MICRORECIF_HEADLESS=1 to skip the GUI entirely.
  const char *headless = std::getenv("MICRORECIF_HEADLESS");
  if (headless != nullptr && std::string(headless) == "1")
  {
    return 0;
  }

  // Redirect verbose simulation output to a log file when running the GUI,
  // to keep the terminal clean during interactive use.
  std::ofstream outFile("output.log");
  std::cout.rdbuf(outFile.rdbuf());

  auto app = Gtk::Application::create();
  return app->make_window_and_run<MyEvent>(1, argv, simulation);
}
