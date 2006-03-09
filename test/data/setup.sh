#!/bin/bash

eval `scramv1 ru -sh`

HOSTNAME=$(echo `/bin/hostname` | sed 's/\//\\\//g')
echo "The hostname is = $HOSTNAME"

TEST_PATH=$(echo "${PWD}" | sed 's/\//\\\//g')
echo "The current directory is = $PWD"

MWC_LIB1="${LOCALRT}/lib/slc3_ia32_gcc323/libEventFilterProcessor.so"
echo "Looking for the EventFilterProcessor library... $MWC_LIB1"
if [ ! -f $MWC_LIB1 ]; then
    echo "Not Found! Will pick it up from the release area..."
    MWC_LIB1="${CMSSW_RELEASE_BASE}/lib/slc3_ia32_gcc323/libEventFilterProcessor.so"
else 
    echo "Found!"
fi

MWC_LIB=$(echo "$MWC_LIB1" | sed 's/\//\\\//g')
echo $MWC_LIB1

if [ -e profile.xml ]; then
    rm profile.xml
fi 
if [ -e EBMonitorModule.xml ]; then
    rm EBMonitorModule.xml
fi
if [ -e startEBMonitorModule ]; then
    rm startEBMonitorModule
fi

sed -e "s/.portn/1972/g" -e "s/.host/${HOSTNAME}/g" -e "s/.pwd/${TEST_PATH}/g" -e "s/.libpath/${MWC_LIB}/g" .profile.xml > profile.xml
sed -e "s/.portn/1972/g" -e "s/.host/${HOSTNAME}/g" -e "s/.pwd/${TEST_PATH}/g" -e "s/.libpath/${MWC_LIB}/g" .EBMonitorModule.xml > EBMonitorModule.xml 
sed -e "s/.portn/1972/g" -e "s/.host/${HOSTNAME}/g" -e "s/.pwd/${TEST_PATH}/g" -e "s/.libpath/${MWC_LIB}/g" .startEBMonitorModule > startEBMonitorModule

chmod 751 profile.xml
chmod 751 EBMonitorModule.xml
chmod 751 startEBMonitorModule



