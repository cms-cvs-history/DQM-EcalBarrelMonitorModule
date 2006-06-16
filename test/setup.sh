#!/bin/sh

if [ "$1" != "" ]; then 
  LOCALTASK=$1
fi
if [ "$2" != "" ]; then 
  REMHOST=$2
fi
if [ "$3" != "" ]; then 
  LOCALPORT=$3
fi
if [ "$4" != "" ]; then 
  REMOTEPORT=$4
fi

# Settable parameters. If empty or commented out they'll be asked for...

#LOCALTASK="FU"
#REMHOST="pclip9.cern.ch"
#LOCALPORT=1972
#REMOTEPORT=12345

# End of settable parameters

eval `scramv1 runtime -sh`

LOCALHOST=$(echo `/bin/hostname -s` | /bin/sed 's/\//\\\//g')
echo "The local hostname is = $LOCALHOST"

PATH=$(echo "${PWD}" | /bin/sed 's/\//\\\//g')
echo "The current directory is = $PWD"

if [ "$LOCALTASK" == "" ]; then
  echo -n "Which task should be run locally (FU/BU)? "
  read LOCALTASK
fi

if [ "$LOCALTASK" == "FU" ]; then
  REMOTETASK="BU"
elif [ "$LOCALTASK" == "BU" ]; then
    REMOTETASK="FU"
else
  echo "Wrong local task choice, please try again re-running this script"
  exit
fi

if [ "$REMHOST" == "" ] ; then
  echo -n "Fine... Now tell me the name of the PC where $REMOTETASK will run (name[.domain]) "
  read REMHOST

  REMHOST=`/usr/bin/host $REMHOST | /bin/awk -F. '{print $1}'`
  if [ $REMHOST == "Host" ]; then
    echo "Uhmmm... It looks like that host does not exist, try again re-running me"
    exit
  fi
  echo "Good, your friend computer will be $REMHOST"
fi

if [ "$LOCALPORT" == "" ]; then
  echo -n "Last points, on which local port comunicates HyperDAQ? "
  read LOCALPORT
fi

if [ "$REMOTEPORT" == "" ]; then
  echo -n "... And on which remote port comunicates in $REMHOST? "
  read REMOTEPORT
fi

# End of questions, set the stuff now...

if [ $LOCALTASK == "FU" ]; then
  FUHOST="$LOCALHOST";
  BUHOST="$REMHOST";
  FUPORT="$LOCALPORT"
  BUPORT="$REMOTEPORT"
else
  BUHOST="$LOCALHOST";
  FUHOST="$REMHOST";
  BUPORT="$LOCALPORT"
  FUPORT="$REMOTEPORT"
fi

LIB1="${LOCALRT}/lib/slc3_ia32_gcc323/libEventFilterUnit.so"
echo "Looking for $LIB1"
if [ ! -f $LIB1 ]; then
  echo "Not Found! Will pick it up from the release area..."
  LIB1="${CMSSW_RELEASE_BASE}/lib/slc3_ia32_gcc323/libEventFilterUnit.so"
else 
  echo "Found!"
fi
LIB2="${LOCALRT}/lib/slc3_ia32_gcc323/libEventFilterProcessor.so"
echo "Looking for $LIB1"
if [ ! -f $LIB2 ]; then
  echo "Not Found! Will pick it up from the release area..."
  LIB2="${CMSSW_RELEASE_BASE}/lib/slc3_ia32_gcc323/libEventFilterProcessor.so"
else 
  echo "Found!"
fi
LIB3="${LOCALRT}/lib/slc3_ia32_gcc323/libSimpleI2OSender.so"
echo "Looking for $LIB3"
if [ ! -f $LIB3 ]; then
  echo "Not Found! Will pick it up from the release area..."
  LIB3="${CMSSW_RELEASE_BASE}/lib/slc3_ia32_gcc323/libSimpleI2OSender.so"
else 
  echo "Found!"
fi
LIB4="${LOCALRT}/lib/slc3_ia32_gcc323/libDQMServicesXdaqCollector.so"
echo "Looking for $4"
if [ ! -f $LIB4 ]; then
  echo "Not Found! Will pick it up from the release area..."
  LIB4="${CMSSW_RELEASE_BASE}/lib/slc3_ia32_gcc323/libDQMServicesXdaqCollector.so"
else 
  echo "Found!"
fi

LIB1=$(echo "$LIB1" | /bin/sed 's/\//\\\//g')
LIB2=$(echo "$LIB2" | /bin/sed 's/\//\\\//g')
LIB3=$(echo "$LIB3" | /bin/sed 's/\//\\\//g')
LIB4=$(echo "$LIB4" | /bin/sed 's/\//\\\//g')

if [ -e profile.xml ]; then
    /bin/rm profile.xml
fi 
if [ -e MonitorModule-FedStreamer-EVB.xml ]; then
    /bin/rm MonitorModule-FedStreamer-EVB.xml
fi
if [ -e MonitorModule-SimpleI2OSender-EVB.xml ]; then
    /bin/rm MonitorModule-SimpleI2OSender-EVB.xml
fi
if [ -e runMonitorModule.sh ]; then
    /bin/rm runMonitorModule.sh
fi
if [ -e runSimpleI2OSender.sh ]; then
    /bin/rm runSimpleI2OSender.sh
fi

/bin/sed -e "s/.pwd/${PATH}/g" .profile.xml > profile.xml
/bin/sed -e "s/.buport/$BUPORT/g" -e "s/.fuport/$FUPORT/g" -e "s/.buhost/$BUHOST/g" -e "s/.fuhost/$FUHOST/g" -e "s/.pwd/$PATH/g" -e "s/.lib1/${LIB1}/g" -e "s/.lib2/${LIB2}/g"  .MonitorModule-FedStreamer-EVB.xml > MonitorModule-FedStreamer-EVB.xml
/bin/sed -e "s/.buport/$BUPORT/g" -e "s/.fuport/$FUPORT/g" -e "s/.buhost/$BUHOST/g" -e "s/.fuhost/$FUHOST/g" -e "s/.pwd/$PATH/g" -e "s/.lib1/${LIB1}/g" -e "s/.lib2/${LIB2}/g" -e "s/.lib3/${LIB3}/g" -e "s/.lib4/${LIB4}/g"  .MonitorModule-SimpleI2OSender-EVB.xml > MonitorModule-SimpleI2OSender-EVB.xml
/bin/sed -e "s/.portn/$LOCALPORT/g" -e "s/.host/$LOCALHOST/g" .runMonitorModule.sh > runMonitorModule.sh
/bin/sed -e "s/.buport/$LOCALPORT/g" -e "s/.host/$LOCALHOST/g" .runSimpleI2OSender.sh > runSimpleI2OSender.sh

/bin/chmod 751 runMonitorModule.sh
/bin/chmod 751 runSimpleI2OSender.sh

