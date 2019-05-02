#ifndef DEMONGROUPHANDLER_HPP
#define DEMONGROUPHANDLER_HPP

// The built in demon group defaults.
#define DEFAULT_GROUP_1 "default"
#define DEFAULT_GROUP_2 "multiuser"
#define DEFAULT_GROUP_3 "singleuser"

class DemonGroupHandler {
public:
  static int startRequiredDemons();
  static int startDefaultDemonGroup(); 
};

#endif
