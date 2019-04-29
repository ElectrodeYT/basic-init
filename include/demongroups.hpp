#ifndef DEMONGROUPS_HPP
#define DEMONGROUPS_HPP

// Demon groups are the equivilent of targets
// There should be 3 predefined, always existing demongroups:
// S: Startup, drivers that must be loaded at all times
// singleuser:  Single user, should load a demon service for sh on console/tty1
// multiuser: Multiuser, should load a getty on at least tty1
//
// S is always started, although on simple systems it might do very little or nothing
// The kernel will always try to load multiuser on afterwards, unless otherwise specified
// in the config file or a error happens somewhere
// singleuser is bassicly recovery, If S or multiuser/whatever is specified in the config doesnt load
// (missing dependencies, etc..) singleuser will always be called. if it gets loaded because of an Error
// the state of the demons already loaded is not specified!

#include <string>
#include <vector>

class DemonGroup {
public:
  // List of demons in this group
  std::vector<int> demons;
};

// All the demon groups!
extern std::vector<DemonGroup> demon_groups;

class DemonGroupManager {
public:
  // Look and add all demon groups and demons
  static int getAllDemonGroups();
  // Look for a specific demon group
  static int readDemonGroup(std::string name);
  // This should execute all required demon groups
  static int executeDemonGroup(std::string name);
};


#endif
