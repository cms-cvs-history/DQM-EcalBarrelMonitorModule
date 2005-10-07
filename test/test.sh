#!/bin/sh

rfcp /castor/cern.ch/cms/archive/ecal/h4/H4-000008203-SM5-PEDESTAL-STD /tmp/$USER/
ln -s /tmp/$USER/H4-000008203-SM5-PEDESTAL-STD .
rfcp /castor/cern.ch/cms/archive/ecal/h4/H4-000008205-SM5-TEST_PULSE-MGPA /tmp/$USER/
ln -s /tmp/$USER/H4-000008205-SM5-TEST_PULSE-MGPA .
rfcp /castor/cern.ch/cms/archive/ecal/h4/H4-000008206-SM5-COSMIC-STD /tmp/$USER/
ln -s /tmp/$USER/H4-000008206-SM5-COSMIC-STD .
cmsRun --parameter-set config.txt
