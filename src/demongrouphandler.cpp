#include <iostream>
#include <string>
#include <vector>
#include <demongrouphandler.hpp>
#include <demongroups.hpp>
#include <helper.hpp>

// Starts demon group "S".
// If this fails the return value must be brought into consideration; if it wasnt found then it is probably safe to ignore.
// If it failed for other reasons, its probably best to boot in singluser mode.
int DemonGroupHandler::startRequiredDemons() {
  return DemonGroupManager::executeDemonGroup("S");
}

// Attempts to start the default demon group and its requirements.
int DemonGroupHandler::startDefaultDemonGroup() {/*
  if(DemonGroupManager::executeDemonGroup(DEFAULT_GROUP_1) == 0) {
    return true;
  }
  */
  DODEBUG(std::cout << "DemonGroupHandler::startDefaultDemonGroup failed to start default, trying multiuser\n";)
  if(DemonGroupManager::executeDemonGroup(DEFAULT_GROUP_2) == 0) {
    return true;
  }
  DODEBUG(std::cout << "DemonGroupHandler::startDefaultDemonGroup failed to start multiuser, trying singleuser\n";)
  if(DemonGroupManager::executeDemonGroup(DEFAULT_GROUP_3) == 0) {
    return true;
  }
  DODEBUG(std::cout << "DemonGroupHandler::startDefaultDemonGroup system fucked\n";)
  return false;
}
