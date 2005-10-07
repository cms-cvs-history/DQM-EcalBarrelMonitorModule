#!/bin/sh

rfcp /castor/cern.ch/cms/archive/ecal/h4/H4-000008203-SM5-PEDESTAL-STD /tmp/dellaric/
rfcp /castor/cern.ch/cms/archive/ecal/h4/H4-000008205-SM5-TEST_PULSE-MGPA /tmp/dellaric/
rfcp /castor/cern.ch/cms/archive/ecal/h4/H4-000008206-SM5-COSMIC-STD /tmp/dellaric/
cmsRun --parameter-set config.txt
