#include "CondCore/DBCommon/interface/DBWriter.h"
#include "CondCore/IOVService/interface/IOV.h"
#include "CondCore/MetaDataService/interface/MetaData.h"
#include "FWCore/Framework/interface/IOVSyncValue.h"
#include "SealKernel/Service.h"
#include "POOLCore/POOLContext.h"
#include "SealKernel/Context.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalWeightRecAlgoWeights.h"
#include "CondFormats/EcalObjects/interface/EcalWeight.h"

using namespace std;

int main(){
  //std::string contact("oracle://devdb10/cms_xiezhen_dev");
  std::string contact("sqlite_file:ecalcalib.db");
  pool::POOLContext::loadComponent( "SEAL/Services/MessageService" );
  pool::POOLContext::setMessageVerbosityLevel( seal::Msg::Error );
  const std::string userNameEnv = "POOL_AUTH_USER=cms_xiezhen_dev";
  ::putenv( const_cast<char*>( userNameEnv.c_str() ) );
  const std::string passwdEnv = "POOL_AUTH_PASSWORD=xiezhen123";
  ::putenv( const_cast<char*>( passwdEnv.c_str() ) );

  cond::DBWriter w(contact);
  w.startTransaction();
  cond::IOV* wgt_iov=new cond::IOV;

  EcalWeightRecAlgoWeights* wgt=new EcalWeightRecAlgoWeights;

  typedef std::vector< std::vector<EcalWeight> > EcalWeightMatrix;
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
    std::vector<EcalWeight> tv1, tv2;
    for(size_t j=0; j<10; ++j) {
      tv1.push_back( EcalWeight(1000+i*10. + j) );
      tv2.push_back( EcalWeight(1000+100+i*10. + j) );
    }
    mat3.push_back(tv1);
    mat4.push_back(tv2);
  }

  std::string wgttok=w.write<EcalWeightRecAlgoWeights>(wgt, "EcalWeightRecAlgoWeights");

  //assign IOV
  wgt_iov->iov.insert(std::make_pair(edm::IOVSyncValue::endOfTime().eventID().run(),wgttok));

  std::string wgtiovToken=w.write<cond::IOV>(wgt_iov,"IOV");
  std::cout << "weights written into db with IOV" << std::endl;

  cond::IOV* pediov=new cond::IOV;
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
      ped1->m_pedestals.insert(std::make_pair(channelId,item));
    }
  }
  std::string ped1tok=w.write<EcalPedestals>(ped1, "EcalPedestals");//pool::Ref takes the ownership of ped1
  //assign IOV
  int tillrun=73000;
  pediov->iov.insert(std::make_pair(tillrun,ped1tok));

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
      ped2->m_pedestals.insert(std::make_pair(channelId,item));
    }
  }

  std::string pedtok2=w.write<EcalPedestals>(ped2,"EcalPedestals");

  //assign IOV
  tillrun=75000;
  pediov->iov.insert(std::make_pair(tillrun,pedtok2));
  std::string pediovToken=w.write<cond::IOV>(pediov,"IOV");  
  std::cout << "pedestals written into db with IOV" << std::endl;

  w.commitTransaction();
  std::cout << "committed transaction" << std::endl;

  //register the iovToken to the metadata service
  cond::MetaData metadata_svc(contact);
  metadata_svc.addMapping("EcalWeightRecAlgoWeights_h4_sm5", wgtiovToken);
  metadata_svc.addMapping("EcalPedestals_2008_test", pediovToken);
}
