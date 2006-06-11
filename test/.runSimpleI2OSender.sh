#!/bin/sh

# Configure all applications
./sendSimpleCmdToApp .host 1972 TA  0 Configure
./sendSimpleCmdToApp .host 1972 EVM 0 Configure
#./sendSimpleCmdToApp .host 1972 RUI 0 Configure
./sendSimpleCmdToApp .host 1972 RU  0 Configure
./sendSimpleCmdToApp .host 1972 BU  0 Configure
#./sendSimpleCmdToApp .host 1972 FU  0 Configure

#Enable RU
./sendSimpleCmdToApp .host 1972 RU  0 Enable

#Enable EVM
./sendSimpleCmdToApp .host 1972 EVM 0 Enable

#Enable BU
./sendSimpleCmdToApp .host 1972 BU  0 Enable

#Start generation of dummy super-fragments
#./sendSimpleCmdToApp .host 1972 RUI 0 Enable

#Start servicing trigger credits
./sendSimpleCmdToApp .host 1972 TA  0 Enable

#Start filtering events
#./sendSimpleCmdToApp .host 1972 FU  0 Enable

