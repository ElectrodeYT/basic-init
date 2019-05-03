#include <iostream>
#include <filesystem>
#include <demons.hpp>
#include <helper.hpp>
#include <demongroups.hpp>
#include <config.hpp>

// List of demon groups.
std::vector<DemonGroup> demon_groups;


int DemonGroupManager::executeDemonGroup(std::string name) {
  int id = -1;
  for(int i = 0; i < demon_groups.size(); i++) {
    if(demon_groups[i].name == name) {
      id = i;
      break;
    }
  }
  // Pretend S was found
  if(id == -1 && name == "S") { return 0; }
  if(id == -1) { DODEBUG(std::cout << "DemonGroupManager::executeDemonGroup failed to find group " << name << "\n";) return -1; }
  // Start that demon groups requirements
  for(int i = 0; i < demon_groups[id].required_demon_groups.size(); i++) {
    if(DemonGroupManager::getDemonGroupStatus(demon_groups[id].required_demon_groups[i]) != true) {
      if(!DemonGroupManager::executeDemonGroup(demon_groups[id].required_demon_groups[i])) {
        DODEBUG(std::cout << "DemonGroupManager::executeDemonGroup required demon group " << demon_groups[i].required_demon_groups[i] << "failed\n");
        return -1;
      }
    }
  }
  DODEBUG(std::cout << "DemonGroupManager::executeDemonGroup starting " << demon_groups[id].name << "\n";)
  return demon_groups[id].start();
}

int DemonGroupManager::readDemonGroup(std::string name) {
  DODEBUG(std::cout << "DemonGroupManager::readDemonGroup adding demongroup " << name << "\n");
  // Get the config file
  std::string file = DEMON_GROUP_LOCATIONS + name + DEMON_GROUP_EXTENSION;
  ConfigFile config = Config::readConfigFile(file);
  if(config.config_names[0] == "failed" && config.config_values[0] == "true") {
    DODEBUG(std::cout << "DemonGroupManager::readDemonGroup config returned failed");
    return -1;
  }
  DemonGroup dgroup;
  // Loop through config file
  for(int i = 0; i < config.config_names.size(); i++) {
    if(config.config_names[i] == "name") {
      dgroup.name = config.config_values[i];
    }
    if(config.config_names[i] == "required_demon") {
      dgroup.required_demons.push_back(config.config_values[i]);
    }
    if(config.config_names[i] == "required_demon_group") {
      dgroup.required_demon_groups.push_back(config.config_values[i]);
    }
    if(config.config_names[i] == "demon") {
      dgroup.demons.push_back(config.config_values[i]);
    }
  }
  // Sanity checks
  if(dgroup.name == "" || dgroup.demons.size() == 0) {
    DODEBUG(std::cout << "DemonGroupManager::readDemonGroup ignoring empty demon group";)
    return 0;
  }
  // Add the demon group!
  demon_groups.push_back(dgroup);
  return 0;
}

// Really simple function
int DemonGroupManager::getDemonGroupStatus(std::string name, bool strict) {
  for(int i = 0; i < demon_groups.size(); i++) {
    if(demon_groups[i].name == name) {
      if(strict) {
        // Check every single demon to see if its running
        return demon_groups[i].is_running_strict();
      } else {
        // Just see if start has been called
        return demon_groups[i].is_running();
      }
    }
  }
  // If S wasnt found, pretend that it is started
  if(name == "S") { return true; }
  // Not found
  return -1;
}

int DemonGroupManager::getAllDemonGroups() {
  std::vector<std::string> files;
  std::filesystem::directory_iterator iter(DEMON_GROUP_LOCATIONS);
  std::filesystem::directory_iterator end;
  while(iter != end) {
    std::error_code err_code;
    if(!std::filesystem::is_directory(iter->path())) {
      if(iter->path().extension() == DEMON_GROUP_EXTENSION) {
        DemonGroupManager::readDemonGroup(iter->path().stem());
      }
    }
    iter.increment(err_code);
    if(err_code) {
      DODEBUG(std::cout << "DemonGroupManager::getAllDemonGroups failed to iterate directory with error: " << err_code.message() << "\n";)
    }
  }
  return 0;
}

int DemonGroup::start() {
  DODEBUG(std::cout << "DemonGroup::start starting demon group " << name << "\n";)
  for(int i = 0; i < demons.size(); i++) {
    DODEBUG(std::cout << "DemonGroup::start starting demon " << demons[i] << "\n";)
    DemonManager::operateOnDemon(demons[i], DEMONSTART);
  }
  is_started = true;
  return 0;
}

int DemonGroup::stop() {
  // TODO: implemnt this
  return 0;
}
