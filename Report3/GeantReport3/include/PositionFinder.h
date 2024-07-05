#ifndef PositionFinder_h
#define PositionFinder_h 1

#include "G4VSensitiveDetector.hh"


class PositionFinder : public G4VSensitiveDetector
{
  public:
    PositionFinder( const G4String& name, const G4int id );
    ~PositionFinder() override;

    void Initialize( G4HCofThisEvent* ) override;
    G4bool ProcessHits( G4Step* step, G4TouchableHistory* ) override;
    void EndOfEvent( G4HCofThisEvent* ) override;

  private:
    G4int m_ID;
};

#endif
