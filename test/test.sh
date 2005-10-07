#!/bin/sh

for FILE in H4-000008203-SM5-PEDESTAL-STD H4-000008205-SM5-TEST_PULSE-MGPA H4-000008206-SM5-COSMIC-STD ; do
  echo $FILE
  rm -f /tmp/$USER/$FILE
  rfcp /castor/cern.ch/cms/archive/ecal/h4/$FILE /tmp/$USER/
  ln -sf /tmp/$USER/$FILE /tmp/
done

cmsRun --parameter-set dump.txt
