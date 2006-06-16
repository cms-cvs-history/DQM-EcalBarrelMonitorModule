#!/bin/sh

# Configure all applications
./sendSimpleCmdToApp epsilon 1972 TA  0 Configure
./sendSimpleCmdToApp epsilon 1972 EVM 0 Configure
./sendSimpleCmdToApp epsilon 1972 RU  0 Configure
./sendSimpleCmdToApp epsilon 1972 BU  0 Configure

#Enable RU
./sendSimpleCmdToApp epsilon 1972 RU  0 Enable

#Enable EVM
./sendSimpleCmdToApp epsilon 1972 EVM 0 Enable

#Enable BU
./sendSimpleCmdToApp epsilon 1972 BU  0 Enable

#Start servicing trigger credits
./sendSimpleCmdToApp epsilon 1972 TA  0 Enable

