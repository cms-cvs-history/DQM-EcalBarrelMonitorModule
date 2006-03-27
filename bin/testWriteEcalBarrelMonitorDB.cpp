#include "CondCore/DBCommon/interface/DBWriter.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondFormats/Calibration/interface/Pedestals.h"
#include "CondCore/IOVService/interface/IOV.h"


#include "FWCore/Framework/interface/IOVSyncValue.h"

#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalWeightRecAlgoWeights.h"
#include "CondFormats/EcalObjects/interface/EcalWeight.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"


#include "CondCore/MetaDataService/interface/MetaData.h"
#include <string>
#include <map>
#include <iostream>

using namespace std;

int main(){
  cout << "Setting up DB Connection..." << flush;
  cond::ServiceLoader* loader = new cond::ServiceLoader;
  loader->loadMessageService(cond::Error);
  //  loader->loadAuthenticationService(cond::XML);  // unneccessary?

  string contact("sqlite_file:ecalcalib.db");
  cond::DBSession* session = new cond::DBSession(contact);
  cond::MetaData* metadata_svc = new cond::MetaData(contact, *loader);
  cout << "Done." << endl;

  try {
    cout << "Making Connections..." << flush;
    session->connect(cond::ReadWriteCreate);
    metadata_svc->connect();
    cout << "Done." << endl;

    cout << "Creating Writers..." << flush;
    cond::DBWriter wgtWriter(*session, "EcalWeightRecAlgoWeights");
    cond::DBWriter pedWriter(*session, "EcalPedestals");
    cond::DBWriter iovWriter(*session, "IOV");
    cout << "Done." << endl;

    cout << "Starting Transaction." << endl;
    session->startUpdateTransaction();

    cond::IOV* wgt_iov = new cond::IOV;

    cout << "Building EcalWeightRecAlgoWeights." << endl;
    EcalWeightRecAlgoWeights* wgt = new EcalWeightRecAlgoWeights;

    typedef vector< vector<EcalWeight> > EcalWeightMatrix;
    EcalWeightMatrix& mat1 = wgt->getWeightsBeforeGainSwitch();
    EcalWeightMatrix& mat2 = wgt->getWeightsAfterGainSwitch();

    // amplitude
    {
      std::vector<EcalWeight> tv1, tv2;

      tv1.push_back( EcalWeight(-0.333) );
      tv1.push_back( EcalWeight(-0.333) );
      tv1.push_back( EcalWeight(-0.333) );
      tv1.push_back( EcalWeight( 0.025) );
      tv1.push_back( EcalWeight( 0.147) );
      tv1.push_back( EcalWeight( 0.221) );
      tv1.push_back( EcalWeight( 0.216) );
      tv1.push_back( EcalWeight( 0.176) );
      tv1.push_back( EcalWeight( 0.127) );
      tv1.push_back( EcalWeight( 0.088) );

      tv2.push_back( EcalWeight(-0.333) );
      tv2.push_back( EcalWeight(-0.333) );
      tv2.push_back( EcalWeight(-0.333) );
      tv2.push_back( EcalWeight( 0.025) );
      tv2.push_back( EcalWeight( 0.147) );
      tv2.push_back( EcalWeight( 0.221) );
      tv2.push_back( EcalWeight( 0.216) );
      tv2.push_back( EcalWeight( 0.176) );
      tv2.push_back( EcalWeight( 0.127) );
      tv2.push_back( EcalWeight( 0.088) );

      mat1.push_back(tv1);
      mat2.push_back(tv2);
    }

    // pedestal
    {
      std::vector<EcalWeight> tv1, tv2;

      tv1.push_back( EcalWeight( 0.333) );
      tv1.push_back( EcalWeight( 0.333) );
      tv1.push_back( EcalWeight( 0.333) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );

      tv2.push_back( EcalWeight( 0.333) );
      tv2.push_back( EcalWeight( 0.333) );
      tv2.push_back( EcalWeight( 0.333) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );

      mat1.push_back(tv1);
      mat2.push_back(tv2);
    }

    // jitter
    {
      std::vector<EcalWeight> tv1, tv2;

      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.000) );
      tv1.push_back( EcalWeight( 0.800) );
      tv1.push_back( EcalWeight( 0.800) );
      tv1.push_back( EcalWeight( 0.800) );
      tv1.push_back( EcalWeight( 0.200) );
      tv1.push_back( EcalWeight( 0.300) );
      tv1.push_back( EcalWeight( 0.300) );
      tv1.push_back( EcalWeight( 0.300) );

      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.000) );
      tv2.push_back( EcalWeight( 0.800) );
      tv2.push_back( EcalWeight( 0.800) );
      tv2.push_back( EcalWeight( 0.800) );
      tv2.push_back( EcalWeight( 0.200) );
      tv2.push_back( EcalWeight( 0.300) );
      tv2.push_back( EcalWeight( 0.300) );
      tv2.push_back( EcalWeight( 0.300) );

      mat1.push_back(tv1);
      mat2.push_back(tv2);
    }

    // fill the chi2 matrcies
    EcalWeightMatrix& mat3 = wgt->getChi2WeightsBeforeGainSwitch();
    EcalWeightMatrix& mat4 = wgt->getChi2WeightsAfterGainSwitch();
    for(size_t i=0; i<10; ++i) {
      vector<EcalWeight> tv1, tv2;
      for(size_t j=0; j<10; ++j) {
	tv1.push_back( EcalWeight(1000+i*10. + j) );
	tv2.push_back( EcalWeight(1000+100+i*10. + j) );
      }
      mat3.push_back(tv1);
      mat4.push_back(tv2);
    }

    cout << "Finished Building." << endl;

    cout << "Marking Weights..." << flush;
    string wgttok = wgtWriter.markWrite<EcalWeightRecAlgoWeights>(wgt);
    cout << "Done." << endl;

    cout << "Assigning IOV..." << flush;
    wgt_iov->iov.insert(make_pair(edm::IOVSyncValue::endOfTime().eventID().run(), wgttok));
    string wgtiovToken = iovWriter.markWrite<cond::IOV>(wgt_iov);
    cout << "Done." << endl;
    
    cout << "Weights written into DB with IOV" << endl;

    // Write EcalPedestals
    cout << "Building EcalPedestals." << endl;
    cond::IOV* pediov = new cond::IOV;
    int channelId;
    EcalPedestals::Item item;

    int nMaxEta = 85;
    int nMaxPhi = 20;
    EcalPedestals* ped1=new EcalPedestals;
    for(int iEta=1; iEta<=nMaxEta;++iEta) {
      for(int iPhi=1; iPhi<=nMaxPhi; ++iPhi) {
        channelId= (iEta-1)*nMaxPhi+iPhi;
        item.mean_x1  = 201.00;
        item.rms_x1   =   0.62;
        item.mean_x6  = 199.40;
        item.rms_x6   =   0.90;
        item.mean_x12 = 198.80;
        item.rms_x12  =   1.10;
	// make an EBDetId since we need EBDetId::rawId() to be used as the key for the pedestals
	EBDetId ebdetid(iEta,iPhi);
	ped1->m_pedestals.insert(make_pair(ebdetid.rawId(),item));
      }
    }

    cout << "Finished Building." << endl;

    cout << "Marking EcalPedestals..." << flush;
    string ped1tok = pedWriter.markWrite<EcalPedestals>(ped1);
    cout << "Done." << endl;

    cout << "Assigning IOV..." << flush;
    int tillrun=73000;
    pediov->iov.insert(make_pair(tillrun,ped1tok));
    cout << "Done." << endl;

    cout << "Building more EcalPedestals." << endl;
    EcalPedestals* ped2=new EcalPedestals; //the user gives up the object ownership upon send it to the writer
    for(int iEta=1; iEta<=85;++iEta) {
      for(int iPhi=1; iPhi<=20; ++iPhi) {
        channelId= (iEta-1)*nMaxPhi+iPhi;
        item.mean_x1  = 201.00;
        item.rms_x1   =   0.62;
        item.mean_x6  = 199.40;
        item.rms_x6   =   0.90;
        item.mean_x12 = 198.80;
        item.rms_x12  =   1.10;
	// make an EBDetId since we need EBDetId::rawId() to be used as the key for the pedestals
	EBDetId ebdetid(iEta,iPhi);
	ped2->m_pedestals.insert(make_pair(ebdetid.rawId(),item));
      }
    }

    cout << "Finished Building" << endl;

    cout << "Marking EcalPedestals..." << flush;
    string pedtok2 = pedWriter.markWrite<EcalPedestals>(ped2);
    cout << "Done." << endl;

    cout << "Assigning IOV..." << flush;
    tillrun=75000;
    pediov->iov.insert(make_pair(tillrun,pedtok2));
    string pediovToken = iovWriter.markWrite<cond::IOV>(pediov);
    cout << "Done." << endl;

    cout << "Pedestals written into DB with IOV" << endl;

    cout << "Committing Session..." << flush;
    session->commit();
    cout << "Done." << endl;

    // Disconnect
    cout << "Disconnection Session..." << flush;
    session->disconnect();
    delete session;
    cout << "Done." << endl;
  
    cout << "Registering to MetaData Service..." << flush;
    metadata_svc->addMapping("EcalWeightRecAlgoWeights_h4_sm5", wgtiovToken);
    metadata_svc->addMapping("EcalPedestals_2008_test", pediovToken);
    cout << "Done." << endl;

    cout << "Disconnecting MetaStata Service..." << flush;
    metadata_svc->disconnect();
    delete metadata_svc;
    cout << "Done." << endl;

    delete loader;

    cout << "All Done." << endl;
  } catch(cond::Exception &e) {
    cout << e.what() << endl;
  } catch(std::exception &e) {
    cout << e.what() << endl;
  } catch(...) {
    cout << "Unknown exception" << endl;
  }

}
