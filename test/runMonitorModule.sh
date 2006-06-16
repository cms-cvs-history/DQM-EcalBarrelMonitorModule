#!/bin/sh

./sendSimpleCmdToApp epsilon 12345 XdaqCollector         0 Configure

./sendSimpleCmdToApp epsilon 12345 FilterUnitFramework   0 Configure

./sendSimpleCmdToApp epsilon 12345 evf::FUEventProcessor 0 Configure

./sendSimpleCmdToApp epsilon 12345 XdaqCollector         0 Enable

./sendSimpleCmdToApp epsilon 12345 FilterUnitFramework   0 Enable

./sendSimpleCmdToApp epsilon 12345 evf::FUEventProcessor 0 Enable

