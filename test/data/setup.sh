#!/bin/sh

if [ "$1" != "" ]; then 
  LOCALTASK=$1
fi
if [ "$2" != "" ]; then 
  REMOTEHOST=$2
fi
if [ "$3" != "" ]; then 
  LOCALPORT=$3
fi
if [ "$4" != "" ]; then 
  REMOTEPORT=$4
fi

# Settable parameters. If empty or commented out they'll be asked for...

#LOCALTASK="FU"
#REMOTEHOST="pclip9.cern.ch"
#LOCALPORT=1972
#REMOTEPORT=12345

# End of settable parameters

eval `scramv1 runtime -sh`

LOCALHOST=$(echo `/bin/hostname` | /bin/sed 's/\//\\\//g')
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

if [ "$REMOTEHOST" == "" ] ; then
  echo -n "Fine... Now tell me the name of the PC where $REMOTETASK will run (name[.domain]) "
  read REMOTEHOST

  REMOTEHOST=`/usr/bin/host $REMOTEHOST | /bin/awk '{print $1}'`
  if [ $REMOTEHOST == "Host" ]; then
    echo "Uhmmm... It looks like that host does not exist, try again re-running me"
    exit
  fi
  echo "Good, your friend computer will be $REMOTEHOST"
fi

if [ "$LOCALPORT" == "" ]; then
  echo -n "Last points, on which local port comunicates HyperDAQ? "
  read LOCALPORT
fi

if [ "$REMOTEPORT" == "" ]; then
  echo -n "... And on which remote port comunicates in $REMOTEHOST? "
  read REMOTEPORT
fi

# End of questions, set the stuff now...

if [ $LOCALTASK == "FU" ]; then
  FUHOST="$LOCALHOST";
  BUHOST="$REMOTEHOST";
  FUPORT="$LOCALPORT"
  BUPORT="$REMOTEPORT"
else
  BUHOST="$LOCALHOST";
  FUHOST="$REMOTEHOST";
  BUPORT="$LOCALPORT"
  FUPORT="$REMOTEPORT"
fi

LIBS="libEventFilterProcessor.so libEventFilterProcessor.so"
MW_LIB=""

for n in $LIBS 
do 
  SEARCH="${LOCALRT}/lib/slc3_ia32_gcc323/$n"
  echo "Looking for $SEARCH"
  if [ ! -f $SEARCH ]; then
    echo "Not Found! Will pick it up from the release area..."
    SEARCH="${CMSSW_RELEASE_BASE}/lib/slc3_ia32_gcc323/$n"
  else 
    echo "Found!"
  fi
  if [ "$MW_LIB" == "" ]; then
    MW_LIB="$SEARCH"
  else
    MW_LIB="$MW_LIB $SEARCH"
  fi
done

MWC_LIB=$(echo "$MWC_LIB" | /bin/sed 's/\//\\\//g')
echo $MWC_LIB

if [ -e profile.xml ]; then
    /bin/rm profile.xml
fi 
if [ -e MonitorModule-FedStreamer-EVB.xml ]; then
    /bin/rm MonitorModule-FedStreamer-EVB.xml
fi
if [ -e MonitorModule-SimpleI2OSender-EVB.xml ]; then
    /bin/rm MonitorModule-SimpleI2OSender-EVB.xml
fi
if [ -e startEBMonitorModule ]; then
    /bin/rm startEBMonitorModule
fi

/bin/sed -e "s/.pwd/${PATH}/g" .profile.xml > profile.xml
/bin/sed -e "s/.buport/$BUPORT/g" -e "s/.fuport/$FUPORT/g" -e "s/.buhost/$BUHOST/g" -e "s/.fuhost/$FUHOST/g" -e "s/.pwd/$PATH/g" -e "s/.libpath/${MWC_LIB}/g"  .MonitorModule-FedStreamer-EVB.xml > MonitorModule-FedStreamer-EVB.xml
/bin/sed -e "s/.buport/$BUPORT/g" -e "s/.fuport/$FUPORT/g" -e "s/.buhost/$BUHOST/g" -e "s/.fuhost/$FUHOST/g" -e "s/.pwd/$PATH/g" -e "s/.libpath/${MWC_LIB}/g"  .MonitorModule-SimpleI2OSender-EVB.xml > MonitorModule-SimpleI2OSender-EVB.xml
/bin/sed -e "s/.portn/$LOCALPORT/g" -e "s/.host/$LOCALHOST/g" .startMonitorModule-FedStreamer-EVB > startMonitorModule-FedStreamer-EVB
/bin/sed -e "s/.portn/$LOCALPORT/g" -e "s/.host/$LOCALHOST/g" .startMonitorModule-SimpleI2OSender-EVB > startMonitorModule-SimpleI2OSender-EVB

/bin/chmod 751 profile.xml
/bin/chmod 751 MonitorModule-FedStreamer-EVB.xml
/bin/chmod 751 MonitorModule-SimpleI2OSender-EVB.xml
/bin/chmod 751 startMonitorModule-FedStreamer-EVB
/bin/chmod 751 startMonitorModule-SimpleI2OSender-EVB



