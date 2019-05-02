#include <iostream>
#include <string>
#include <vector>
#include <demongrouphandler.hpp>
#include <demongroups.hpp>

// Starts demon group "S".
// If this fails the return value must be brought into consideration; if it wasnt found then it is probably safe to ignore.
// If it failed for other reasons, its probably best to boot in singluser mode.
int DemonGroupHandler::startRequiredDemons() {
  return DemonGroupManager::executeDemonGroup("S");
}

// Attempts to start the default demon group and its requirements.
int DemonGroupHandler::startDefaultDemonGroup() {
  if(DemonGroupManager::executeDemonGroup(DEFAULT_GROUP_1)) {
    return true;
  }
  if(DemonGroupManager::executeDemonGroup(DEFAULT_GROUP_2)) {
    return true;
  }
  if(DemonGroupManager::executeDemonGroup(DEFAULT_GROUP_3)) {
    return true;
  }
  return false;
}
