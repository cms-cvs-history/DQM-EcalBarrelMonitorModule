#!/bin/sh

./sendSimpleCmdToApp .host .portn XdaqCollector         0 Configure

./sendSimpleCmdToApp .host .portn FilterUnitFramework   0 Configure

./sendSimpleCmdToApp .host .portn evf::FUEventProcessor 0 Configure

./sendSimpleCmdToApp .host .portn XdaqCollector         0 Enable

./sendSimpleCmdToApp .host .portn FilterUnitFramework   0 Enable

./sendSimpleCmdToApp .host .portn evf::FUEventProcessor 0 Enable

