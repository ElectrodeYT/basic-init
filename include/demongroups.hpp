#ifndef DEMONGROUPS_HPP
#define DEMONGROUPS_HPP

//
// Demon groups are the equivilent of systemd targets, but are not always the same.
// There should be 3 predefined, always existing demongroups:
// S: Startup, drivers that must be loaded at all times
// singleuser:  Single user, should load a demon service for sh on console/tty1
// multiuser: Multiuser, should load a getty on at least tty1
//
// S is always started, although on simple systems it might do very little or nothing
// The init will always try to load multiuser on afterwards, unless otherwise specified
// in the config file or a error happens somewhere
// singleuser is (essentially) recovery, If a (serious) error happens or a
// required entry in the config doesnt exist the init will always try to load singleuser.
// It is not guarranteed that all demons that may have already been started will be stopped.
//
// For simplicity, it is (currently) presumed that all demons in a demon group may be started in any order.
//

#include <string>
#include <vector>
#include <demons.hpp>

#define DEMON_GROUP_LOCATIONS "/etc/basicinit/groups/"
#define DEMON_GROUP_EXTENSION ".dg"

class DemonGroup {
public:
  DemonGroup() {
    is_started = false;
  }
  // Name of Demon Group
  std::string name;
  // List of demons in this group
  std::vector<std::string> demons;
  std::vector<std::string> required_demon_groups;
  // TODO: Implement this
  std::vector<std::string> required_demons;
  // Returns is_started
  int is_running() { return is_started; }
  // Checks each demon to see if its currently running
  int is_running_strict() {
    for(int i = 0; i < demons.size(); i++) {
      if(DemonManager::operateOnDemon(demons[i], DEMONRUNNING) != true) { return false; }
    }
    return true;
  }
  // Start the demon group
  int start();
  // Stop the demon group
  int stop();
private:
  // Is true if its been started before and not stopped
  bool is_started;
};

// All the demon groups!
extern std::vector<DemonGroup> demon_groups;

// The DemonGroupManager manages demon groups; DemonGroupHandler probides a simpler interface for booting.
class DemonGroupManager {
public:
  // Look and add all demon groups and demons
  static int getAllDemonGroups();
  // Look for a specific demon group
  static int readDemonGroup(std::string name);
  // This should execute all required demon groups
  static int executeDemonGroup(std::string name);
  // Get status of demon group
  static int getDemonGroupStatus(std::string name, bool strict = false);
};


#endif
