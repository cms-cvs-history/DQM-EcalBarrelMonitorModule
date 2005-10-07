#ifndef EBMonitorModule_H
#define EBMonitorModule_H

/*
 * \file EBMonitorModule.h
 *
 * $Date: 2005/08/05 14:34:03 $
 * $Revision: 1.2 $
 *
*/

#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>
#include <DataFormats/EcalDetId/interface/EBDetId.h>

#include <EcalMonitor/EBMonitorTasks/interface/EBPedestalTask.h>
#include <EcalMonitor/EBMonitorTasks/interface/EBTestPulseTask.h>
#include <EcalMonitor/EBMonitorTasks/interface/EBLaserTask.h>
#include <EcalMonitor/EBMonitorTasks/interface/EBCosmicTask.h>

#include <EcalMonitor/EBMonitorTasks/interface/EBHtmlTask.h>

#include "TROOT.h"
#include "TFile.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace cms;
using namespace std;

class EBMonitorModule: public edm::EDAnalyzer{

public:

/// Constructor
EBMonitorModule(const edm::ParameterSet& ps);

/// Destructor
virtual ~EBMonitorModule();

protected:

/// Analyze digis out of raw data
void analyze(const edm::Event& e, const edm::EventSetup& c);

private:

int ievt;

TFile* rootFile;

TH1F* hEbarrel;

EBPedestalTask* pedestal_task;
EBTestPulseTask* testpulse_task;
EBLaserTask* laser_task;
EBCosmicTask* cosmic_task;

EBHtmlTask* html_task;

ofstream logFile;

};

DEFINE_FWK_MODULE(EBMonitorModule)

#endif
