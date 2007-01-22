// $Id: readMaskFromDB.cpp,v 1.4 2007/01/22 16:14:14 benigno Exp $

/*!
  \file readMaskFromDB.cpp
  \brief It reads errors masks from database and writes them to an output file
  \author B. Gobbo 
  \version $Revision: 1.4 $
  \date $Date: 2007/01/22 16:14:14 $
*/


#include <iostream>
#include <string>
#include <cstdlib>

#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "DQM/EcalBarrelMonitorClient/interface/EcalErrorMask.h"

void usage( char* cp ) {
  std::cout <<
"\n\
usage: " << cp << " [-h] [-H hostname] [-l location] [-p dbpasswd] [-s sid] \n\
                 [-t run type] [-u dbuser] file\n\n\
     -h             : print this help message \n\
     -H hostname    : data base server host name \n\
     -l location    : location H4, 867-1, ...\n\
     -p dbpasswd    : data base password \n\
     -s sid         : data base sid \n\
     -t runtype     : run type \n\
     -u dbuser      : data base user name \n\n";
}

int main( int argc, char **argv ) {

  char* cp;
  std::string user   = "";
  std::string passwd = "";
  int runNb = 0;
  std::string fileName = "";
  std::string hostName = "";
  std::string sid = "";
  std::string location = "";
  std::string runType = "";

  if(( cp = (char*) strrchr( argv[0], '/' )) != NULL ) {
    ++cp;
  }
  else {
    cp = argv[0];
  }
  
  // Arguments and Options
  if( argc > 1 ) {
    int rc;
    while(( rc = getopt( argc, argv, "H:hl:p:s:t:u:" )) != EOF ) {
      switch( rc ) {
      case 'H':
	hostName = optarg;
	break;
      case 'h':
	usage(cp);
	return(0);
	break;
      case 'l':
	location = optarg;
	break;
      case 'p':
	passwd = optarg;
	break;
      case 's':
	sid = optarg;
	break;
      case 't':
	runType = optarg;
	break;
      case 'u':
	user = optarg;
	break;
      default:
	break;
      }
    }
  }	
  if( optind+1 == argc ) {
    fileName = argv[optind++];
  }
  else {
    usage(cp);
    return -1;
  }

  if( hostName == "" ) {
    std::cout << "hostname: ";
    std::cin >> hostName;
  }
  if( sid == "" ) {
    std::cout << "sid     : ";
    std::cin >> sid;
  }
  if( user == "" ) {
    std::cout << "username: ";
    std::cin >> user;
  }
  if( passwd == "" ) {
    std::cout << "password: ";
    std::cin >> passwd;
  }
  if( runNb == 0 ) {
    std::cout << "run Nb  : ";
    std::cin >> runNb;
  }
  if( location == "" ) {
    std::cout << "location: ";
    std::cin >> location;
  }
  if( runType == "" ) {
    std::cout << "run type: ";
    std::cin >> runType;
  }

  // OK, from here there's all what's needed...

  EcalCondDBInterface* eConn;
  try {
     eConn = new EcalCondDBInterface( hostName, sid, user, passwd );
  } catch( runtime_error &e ) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  
  LocationDef locdef;
  RunTypeDef rundef;
  RunTag     runtag;

  locdef.setLocation( location );
  rundef.setRunType( runType );
  runtag.setLocationDef( locdef );
  runtag.setRunTypeDef( rundef );
  
  RunIOV runiov;
  runiov.setRunTag( runtag );
  EcalErrorMask::readDB( eConn, &runiov );
  EcalErrorMask::writeFile( fileName );

  delete eConn;

  return 0;
}


