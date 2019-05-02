# Basic Init
A Basic Linux Init.
The goal is to have enough functionality without any other programs or init scripts to start a full linux system.
# Current Status
At the moment the Init is capable of launching gettys on the ttys.
I am also implementing "DemonGroups", similar to systemd targets but simpler.


# Demons
I called them Demons after i had a large amount of problems implementing them, intstead of Daemons.
They are simpler then SystemD Services; 1 DemonGroup is a list of Demons to be started at once, although im not sure how to implement the ordering on Demons yet.

# Demon Groups
There are currently three always defined Groups:
* multiuser
* singleuser
* S
If S does not exists, the init will pretend it does and do nothing if it is called upon.

In the following order the init will attempt to start the following Groups:
1. default
2. multiuser
3. singleuser
