/// \file CheckTracks.C
/// \brief Simple macro to check ITSU tracks

#if !defined(__CLING__) || defined(__ROOTCLING__)
#include <array>

#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TH2F.h>
#include <TNtuple.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TString.h>

#include "TGeoGlobalMagField.h"
#include "Field/MagneticField.h"
#include "ITSBase/GeometryTGeo.h"
#include "DataFormatsITS/TrackITS.h"
#include "SimulationDataFormat/MCTrack.h"
#include "SimulationDataFormat/MCCompLabel.h"
#include "SimulationDataFormat/TrackReference.h"
#include "DetectorsBase/Propagator.h"

#endif
#include "DataFormatsITSMFT/CompCluster.h"
#include "SimulationDataFormat/MCTruthContainer.h"

using namespace std;

struct DataFrames {
  void update(int frame, long index)
  {
    if (frame < firstFrame) {
      firstFrame = frame;
      firstIndex = index;
    }
    if (frame > lastFrame) {
      lastFrame = frame;
      lastIndex = index;
    }
    if (frame == firstFrame && index < firstIndex) {
      firstIndex = index;
    }
    if (frame == lastFrame && index > lastIndex) {
      lastIndex = index;
    }
  }

  long firstFrame = 10000;
  int firstIndex = 1;
  long lastFrame = -10000;
  int lastIndex = -1;
};

void CheckTracks(std::string tracfile = "o2trac_its.root", std::string clusfile = "o2clus_its.root", std::string kinefile = "o2sim_Kine.root", std::string grpfile = "o2sim_grp.root")
{
  using namespace o2::itsmft;
  using namespace o2::its;

  TFile* f = TFile::Open("CheckTracks.root", "recreate");
  TNtuple* nt = new TNtuple("ntt", "track ntuple",
                            //"mcYOut:recYOut:"
                            "mcZOut:recZOut:"
                            "mcPhiOut:recPhiOut:"
                            "mcThetaOut:recThetaOut:"
                            "mcPhi:recPhi:"
                            "mcLam:recLam:"
                            "mcPt:recPt:"
                            "ipD:ipZ:label");

  // Magnetic field
  o2::base::Propagator::initFieldFromGRP(grpfile.data());
  auto field = static_cast<o2::field::MagneticField*>(TGeoGlobalMagField::Instance()->GetField());
  double orig[3] = {0., 0., 0.};
  float bz = field->getBz(orig);

  // Geometry
  o2::base::GeometryManager::loadGeometry();
  auto gman = o2::its::GeometryTGeo::Instance();

  // MC tracks
  TFile* file0 = TFile::Open(kinefile.data());
  TTree* mcTree = (TTree*)gFile->Get("o2sim");
  mcTree->SetBranchStatus("*", 0); //disable all branches
  //mcTree->SetBranchStatus("MCEventHeader.*",1);
  mcTree->SetBranchStatus("MCTrack*", 1);

  std::vector<o2::MCTrack>* mcArr = nullptr;
  mcTree->SetBranchAddress("MCTrack", &mcArr);

  /*
  std::vector<o2::TrackReference>* mcTrackRefs = nullptr;
  mcTree->SetBranchAddress("TrackRefs", &mcTrackRefs);
  */

  // Clusters
  TFile::Open(clusfile.data());
  TTree* clusTree = (TTree*)gFile->Get("o2sim");
  std::vector<CompClusterExt>* clusArr = nullptr;
  clusTree->SetBranchAddress("ITSClusterComp", &clusArr);

  // Cluster MC labels
  o2::dataformats::MCTruthContainer<o2::MCCompLabel>* clusLabArr = nullptr;
  clusTree->SetBranchAddress("ITSClusterMCTruth", &clusLabArr);

  // Reconstructed tracks
  TFile* file1 = TFile::Open(tracfile.data());
  TTree* recTree = (TTree*)gFile->Get("o2sim");
  std::vector<TrackITS>* recArr = nullptr;
  recTree->SetBranchAddress("ITSTrack", &recArr);
  // Track MC labels
  std::vector<o2::MCCompLabel>* trkLabArr = nullptr;
  recTree->SetBranchAddress("ITSTrackMCTruth", &trkLabArr);

  Int_t lastEventIDcl = -1, cf = 0;
  Int_t nev = mcTree->GetEntriesFast();

  Int_t nb = 100;
  Double_t xbins[nb + 1], ptcutl = 0.01, ptcuth = 10.;
  Double_t a = TMath::Log(ptcuth / ptcutl) / nb;
  for (Int_t i = 0; i <= nb; i++)
    xbins[i] = ptcutl * TMath::Exp(i * a);
  TH1D* num = new TH1D("num", ";#it{p}_{T} (GeV/#it{c});Efficiency (fake-track rate)", nb, xbins);
  num->Sumw2();
  TH1D* fak = new TH1D("fak", ";#it{p}_{T} (GeV/#it{c});Fak", nb, xbins);
  fak->Sumw2();
  TH1D* clone = new TH1D("clone", ";#it{p}_{T} (GeV/#it{c});Clone", nb, xbins);
  clone->Sumw2();

  TH1D* den = new TH1D("den", ";#it{p}_{T} (GeV/#it{c});Den", nb, xbins);
  den->Sumw2();

  // search for MC events in frames

  cout << "Find mc events in cluster frames.. " << endl;

  int loadedEventClust = -1;

  vector<DataFrames> clusterFrames(nev);

  for (int frame = 0; frame < clusTree->GetEntriesFast(); frame++) { // Cluster frames
    if (!clusTree->GetEvent(frame))
      continue;
    loadedEventClust = frame;
    for (unsigned int i = 0; i < clusArr->size(); i++) { // Find the last MC event within this reconstructed entry
      auto lab = (clusLabArr->getLabels(i))[0];
      if (!lab.isValid() || lab.getSourceID() != 0 || lab.getEventID() < 0 || lab.getEventID() >= nev)
        continue;
      clusterFrames[lab.getEventID()].update(frame, i);
    }
  }

  cout << "Find mc events in track frames.. " << endl;

  int loadedEventTracks = -1;

  vector<DataFrames> trackFrames(nev);
  for (int frame = 0; frame < recTree->GetEntriesFast(); frame++) { // Track frames
    if (!recTree->GetEvent(frame))
      continue;
    int loadedEventTracks = frame;
    cout << "Number of tracks in frame " << frame << ": " << recArr->size() << std::endl;
    for (unsigned int i = 0; i < recArr->size(); i++) { // Find the last MC event within this reconstructed entry
      auto lab = (*trkLabArr)[i];
      if (!lab.isValid()) {
        const TrackITS& recTrack = (*recArr)[i];
        fak->Fill(recTrack.getPt());
      }
      if (!lab.isValid() || lab.getSourceID() != 0 || lab.getEventID() < 0 || lab.getEventID() >= nev) {
        continue;
      }
      trackFrames[lab.getEventID()].update(frame, i);
    }
  }

  cout << "Process mc events.. " << endl;

  Int_t statGen = 0, statGoo = 0, statFak = 0, statClone = 0;
  int nDataEvents = 0;

  for (Int_t n = 0; n < nev; n++) { // loop over MC events

    std::cout << "\nMC event " << n << '/' << nev << std::endl;
    DataFrames f = clusterFrames[n];
    if ((f.firstFrame > f.lastFrame) ||
        ((f.firstFrame == f.lastFrame) && (f.firstIndex > f.lastIndex)))
      continue;

    if (!mcTree->GetEvent(n))
      continue;
    //cout<<"mc event loaded"<<endl;

    Int_t nmc = mcArr->size();
    //Int_t nmcrefs = mcTrackRefs->size();

    std::vector<int> clusMap(nmc, 0);
    std::vector<int> clusRofMap(nmc, -1);
    std::vector<int> trackMap(nmc, -1);
    std::vector<int> mapNFakes(nmc, 0);
    std::vector<int> mapNClones(nmc, 0);

    std::vector<TrackITS> trackStore;
    trackStore.reserve(10000);

    f = trackFrames[n];
    cout << "track frames: " << f.firstFrame << ", " << f.firstIndex << " <-> " << f.lastFrame << ", " << f.lastIndex << endl;

    for (int frame = f.firstFrame; frame <= f.lastFrame; frame++) {
      if (frame != loadedEventTracks) {
        loadedEventTracks = -1;
        if (!recTree->GetEvent(frame))
          continue;
        loadedEventTracks = frame;
      }
      long nentr = recArr->size();

      long firstIndex = (frame == f.firstFrame) ? f.firstIndex : 0;
      long lastIndex = (frame == f.lastFrame) ? f.lastIndex : nentr - 1;

      for (long i = firstIndex; i <= lastIndex; i++) {
        auto lab = (*trkLabArr)[i];
        if (!lab.isValid() || lab.getSourceID() != 0 || lab.getEventID() != n)
          continue;
        int mcid = lab.getTrackID();
        if (mcid < 0 || mcid >= nmc) {
          cout << "track mc label is too big!!!" << endl;
          continue;
        }
        if (trackMap[mcid] >= 0) {
          mapNClones[mcid]++;
        } else {
          if (lab.isFake()) {
            mapNFakes[mcid]++;
          }
          trackMap[mcid] = trackStore.size();
          const TrackITS& recTrack = (*recArr)[i];
          trackStore.emplace_back(recTrack);
        }
      }
    }

    f = clusterFrames[n];
    cout << "cluster frames: " << f.firstFrame << ", " << f.firstIndex << " <-> " << f.lastFrame << ", " << f.lastIndex << endl;
    int nClusters = 0;
    for (int frame = f.firstFrame; frame <= f.lastFrame; frame++) {
      if (frame != loadedEventClust) {
        loadedEventClust = -1;
        if (!clusTree->GetEvent(frame))
          continue;
        loadedEventClust = frame;
      }
      long nentr = clusArr->size();

      long firstIndex = (frame == f.firstFrame) ? f.firstIndex : 0;
      long lastIndex = (frame == f.lastFrame) ? f.lastIndex : nentr - 1;

      for (int i = firstIndex; i <= lastIndex; i++) {
        auto lab = (clusLabArr->getLabels(i))[0];
        if (!lab.isValid() || lab.getSourceID() != 0 || lab.getEventID() != n)
          continue;

        int mcid = lab.getTrackID();
        if (mcid < 0 || mcid >= nmc) {
          cout << "cluster mc label is too big!!!" << endl;
          continue;
        }

        if (!lab.isCorrect())
          continue;

        const CompClusterExt& c = (*clusArr)[i];

        nClusters++;

        int& ok = clusMap[mcid];
        auto layer = gman->getLayer(c.getSensorID());
        float r = 0.f;
        if (layer == 0)
          ok |= 0b1;
        if (layer == 1)
          ok |= 0b10;
        if (layer == 2)
          ok |= 0b100;
        if (layer == 3)
          ok |= 0b1000;
        if (layer == 4)
          ok |= 0b10000;
        if (layer == 5)
          ok |= 0b100000;
        if (layer == 6)
          ok |= 0b1000000;
      }
    } // cluster frames

    if (nClusters > 0)
      nDataEvents++;

    Int_t nGen = 0, nGoo = 0, nFak = 0, nClone = 0;

    for (int mc = 0; mc < nmc; mc++) {

      const auto& mcTrack = (*mcArr)[mc];

      auto x = mcTrack.Vx();
      auto y = mcTrack.Vy();
      if (x * x + y * y > 1)
        continue; // Select quasi-primary particles, originating from within the beam pipe

      Int_t pdg = mcTrack.GetPdgCode();
      if (TMath::Abs(pdg) != 211)
        continue; // Select pions

      if (TMath::Abs(mcTrack.GetEta()) > 1.2)
        continue; // Select within the acceptance in eta

      if (clusMap[mc] != 0b1111111)
        continue; // Select only if all 7 layers have a cluster

      // Float_t mcYOut=-1., recYOut=-1.;
      Float_t mcZOut = -1., recZOut = -1.;
      Float_t mcPhiOut = -1., recPhiOut = -1.;
      Float_t mcThetaOut = -1., recThetaOut = -1.;
      Float_t mcPx = mcTrack.GetStartVertexMomentumX();
      Float_t mcPy = mcTrack.GetStartVertexMomentumY();
      Float_t mcPz = mcTrack.GetStartVertexMomentumZ();
      Float_t mcPhi = TMath::ATan2(mcPy, mcPx), recPhi = -1.;
      Float_t mcPt = mcTrack.GetPt(), recPt = -1.;
      Float_t mcLam = TMath::ATan2(mcPz, mcPt), recLam = -1.;
      Float_t ip[2]{0., 0.};
      Float_t label = mc;

      nGen++; // Generated tracks for the efficiency calculation
      den->Fill(mcPt);

      if (mapNClones[mc] > 0) { // Clone-track rate calculation
        nClone += mapNClones[mc];
        clone->Fill(mcPt, mapNClones[mc]);
        continue;
      }

      if (trackMap[mc] > 0) {
        if (mapNFakes[mc] > 0) { // Fake-track rate calculation
          nFak += mapNFakes[mc];
          fak->Fill(mcTrack.GetPt(), mapNFakes[mc]);
          label = -mc;
        } else { // Good found tracks for the efficiency calculation
          nGoo++;
          num->Fill(mcPt);
        }
        const TrackITS& recTrack = trackStore[trackMap[mc]];
        auto out = recTrack.getParamOut();
        // recYOut = out.getY();
        recZOut = out.getZ();
        recPhiOut = out.getPhi();
        recThetaOut = out.getTheta();
        std::array<float, 3> p;
        recTrack.getPxPyPzGlo(p);
        recPt = recTrack.getPt();
        recPhi = TMath::ATan2(p[1], p[0]);
        recLam = TMath::ATan2(p[2], recPt);
        Float_t vx = 0., vy = 0., vz = 0.; // Assumed primary vertex
        recTrack.getImpactParams(vx, vy, vz, bz, ip);
      }

      nt->Fill( // mcYOut,recYOut,
        mcZOut, recZOut, mcPhiOut, recPhiOut, mcThetaOut, recThetaOut, mcPhi, recPhi,
        mcLam, recLam, mcPt, recPt, ip[0], ip[1], label);
    }

    statGen += nGen;
    statGoo += nGoo;
    statFak += nFak;
    statClone += nClone;

    std::cout << "Good found tracks: " << nGoo;
    if (nGen > 0) {
      Float_t eff = nGoo / Float_t(nGen);
      Float_t rat = nFak / Float_t(nGen);
      Float_t clonerat = nClone / Float_t(nGen);
      cout << ",  efficiency: " << eff << ",  fake-track rate: " << rat << " clone rate " << clonerat;
    }
    std::cout << std::endl;

  } // mc events

  cout << "\nNumber of generated particles: " << statGen << endl;
  if (statGen > 0) {
    cout << "\nOverall efficiency: " << endl;
    Float_t eff = statGoo / Float_t(statGen);
    Float_t rat = statFak / Float_t(statGen);
    Float_t clonerat = statClone / Float_t(statGen);
    std::cout << "Good found tracks/event: " << statGoo / nDataEvents << ",  efficiency: " << eff << ",  fake-track rate: " << rat << " clone rate " << clonerat << std::endl;
  }

  TH1F* hg = new TH1F("hg", "Transverse impact parameter; D (cm)", 30, -0.1, 0.1);
  hg->SetLineColor(4);
  TH1F* hf = new TH1F("hf", "Transverse impact parameter; D (cm)", 30, -0.1, 0.1);
  hf->SetLineColor(2);

  // "recPt>0" means "found tracks only"
  // "label>0" means "found good tracks only"
  nt->Draw("ipD>>hg", "recPt>0 && label>0", "goff");
  auto norm = hg->Integral();
  if (norm > 0)
    hg->Scale(1. / norm);
  nt->Draw("ipD>>hf", "recPt>0 && label<0", "goff");
  norm = hf->Integral();
  if (norm > 0)
    hf->Scale(1. / norm);

  new TCanvas("d", "d", 600, 600);
  hg->Draw("hist");
  hf->Draw("histsame");

  /*
  new TCanvas;
  nt->Draw("mcLam-recLam", "recPt>0 && label>0");
  new TCanvas;
  nt->Draw("mcPt-recPt", "recPt>0 && label>0");
  new TCanvas;
  nt->Draw("mcZOut-recZOut", "recPt>0 && label>0 && abs(mcZOut-recZOut)<0.025");
  new TCanvas;
  nt->Draw("mcPhiOut-recPhiOut", "recPt>0 && label>0");
  new TCanvas;
  nt->Draw("mcThetaOut-recThetaOut", "recPt>0 && label>0");
  */
  TCanvas* c1 = new TCanvas;
  c1->SetLogx();
  c1->SetGridx();
  c1->SetGridy();
  num->Divide(num, den, 1, 1, "b");
  num->Draw("histe");
  fak->Divide(fak, den, 1, 1, "b");
  fak->SetLineColor(2);
  fak->Draw("histesame");
  clone->Divide(clone, den, 1, 1, "b");
  clone->SetLineColor(3);
  clone->Draw("histesame");
  f->Write();
  f->Close();
}
