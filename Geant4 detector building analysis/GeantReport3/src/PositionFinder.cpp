#include "PositionFinder.h"

#include "g4csv.hh"
#include "G4RunManager.hh"

PositionFinder::PositionFinder( const G4String& name, const G4int id ) : G4VSensitiveDetector( name )
{
  // Set which ntuple to use
  m_ID = id;
}

PositionFinder::~PositionFinder()
{
}

void PositionFinder::Initialize( G4HCofThisEvent* )
{
}

// Analyse anything that hit the detector
G4bool PositionFinder::ProcessHits( G4Step* step, G4TouchableHistory* )
{
  // Tracking detectors only sense charged particles
  if ( step->GetTrack()->GetParticleDefinition()->GetPDGCharge() != 0.0 )
  {
    // Get the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Fill ntuple with my ID number
    analysisManager->FillNtupleIColumn( m_ID, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() ); // Column 0 - event number
    analysisManager->FillNtupleDColumn( m_ID, 1, step->GetTrack()->GetPosition().phi() );                          // Column 1 - phi coordinate of hit
    analysisManager->FillNtupleDColumn( m_ID, 2, step->GetTrack()->GetPosition().theta() );                        // Column 2 - theta coordinate of hit
    analysisManager->AddNtupleRow( m_ID ); // Row complete
  }

  // provide 0 theta and 0 phi for when no charged particles are detected
  else{

  // Get the analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Fill ntuple with my ID number
  analysisManager->FillNtupleIColumn( m_ID, 0, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()  ); // Column 0 - event number
  analysisManager->FillNtupleDColumn( m_ID, 1, 0 );                          // Column 1 - phi coordinate of hit
  analysisManager->FillNtupleDColumn( m_ID, 2, 0 );                        // Column 2 - theta coordinate of hit
  analysisManager->AddNtupleRow( m_ID ); // Row complete

  }

  return true;
}

void PositionFinder::EndOfEvent( G4HCofThisEvent* )
{
}
