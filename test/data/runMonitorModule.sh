#!/bin/sh

./sendSimpleCmdToApp pclip1.cern.ch 1972 FilterUnitFramework   0 Configure
./sendSimpleCmdToApp pclip1.cern.ch 1972 evf::FUEventProcessor 0 Configure
./sendSimpleCmdToApp pclip1.cern.ch 1972 FilterUnitFramework   0 Enable
./sendSimpleCmdToApp pclip1.cern.ch 1972 evf::FUEventProcessor 0 Enable
