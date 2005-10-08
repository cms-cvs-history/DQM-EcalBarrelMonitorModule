#include "TROOT.h"
#include "TFile.h"
#include "TKey.h"

TObject* search( TDirectory *d, string objname ) {

  TObject* obj = d->Get( objname.c_str() );
  if( obj != NULL ) {
    return obj;
  }
  else { 
    TIter i( d->GetListOfKeys() );
    TKey *k;
    while( (k = (TKey*)i())) {
      if( k->GetClassName() == TString( "TDirectory" ) ) {
        TDirectory *d = (TDirectory*)k->ReadObj();
        d->cd();
        obj = search( d, objname );      
        if( obj != NULL ) return( obj );
      }
    }
  }
  return obj;
}

int monitor(int t = 0, string S = "01") {

// 0   = pedestal
// 1,2 = test pulse amplitude, shape
// 3,4 = laser shape
// 5,6 = cosmic

  delete gROOT->GetListOfFiles()->FindObject("EcalBarrelMonitorModule.root");
  delete gROOT->GetListOfCanvases()->FindObject("c1");
  delete gROOT->GetListOfCanvases()->FindObject("c2");
  delete gROOT->GetListOfCanvases()->FindObject("c3");

  TFile* file = new TFile("EcalBarrelMonitorModule.root");
  TCanvas *c1 = new TCanvas("c1","EcalBarrel - Canvas 1", 70,  0,500,400);
  TCanvas *c2 = new TCanvas("c2","EcalBarrel - Canvas 2",600,  0,500,400);
  TCanvas *c3 = new TCanvas("c3","EcalBarrel - Canvas 3",600,450,500,400);

  TObject* h1 = new TObject();
  TObject* h2 = new TObject();
  TObject* h3 = new TObject();

  string T;

  switch ( t ) {

    case 0:
      T = "EBPT pedestal";
      break;
    case 1:
      T = "EBTT amplitude";
      break;
    case 2:
      T = "EBTT shape";
      break;
    case 3:
      T = "EBLT amplitude";
      delete c3;
      break;
    case 4:
      T = "EBLT shape";
      delete c3;
      break;
    case 5:
      T = "EBCT amplitude (cut)";
      delete c2;
      delete c3;
      break;
    case 6:
      T = "EBCT amplitude (sel)";
      delete c2;
      delete c3;
      break;
  }

  string title0, title1;
  title0 = T + " SM" + S;

  cout << title0 << endl;

  title1 = title0;

  if ( t <= 2 ) title1 = title0 + " G01";
  if ( t == 3 || t == 4 ) title1 = title0 + " L1";
  h1 = search( file, title1);

  if ( t <= 2 ) title1 = title0 + " G06";
  if ( t == 3 || t == 4 ) title1 = title0 + " L2";
  h2 = search( file, title1);

  if ( t <= 2 ) title1 = title0 + " G12";
  if ( t == 3 || t == 4 ) title1 = title0 + " L3";
  h3 = search( file, title1);

  if ( c1 ) c1->cd();
  if ( h1 ) h1->Draw("lego");

  if ( c2 ) c2->cd();
  if ( h2 ) h2->Draw("lego");

  if ( c3 ) c3->cd();
  if ( h3 ) h3->Draw("lego");

}

