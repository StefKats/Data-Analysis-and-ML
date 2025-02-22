#include "DetectorConstruction.h"
#include "PositionFinder.h"
#include "EnergyCounter.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

// Set number of detector layers
G4int const nLayers = 8;

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::m_magneticFieldMessenger = 0;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

// Here we define the actual experiment that we want to perform
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Materials
  // http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* vacuum = nistManager->FindOrBuildMaterial( "G4_Galactic" );
  G4Material* lead = nistManager->FindOrBuildMaterial( "G4_Pb" );
  G4Material* liquidArgon = nistManager->FindOrBuildMaterial( "G4_lAr" );
  G4Material* lead_glass = nistManager->FindOrBuildMaterial( "G4_PbWO4" ); // lead glass very good em cal, very high density
  G4Material* silicon = nistManager->FindOrBuildMaterial( "G4_Si" );



  // Sizes of the principal geometrical components (solids)

  // Tracker half-length
  G4double tracker1Length = 0.5*cm;

  // EM homogenous calorimeter half-length
  G4double EcalThickness = 5.0*cm;


  // Hadronic sampling calorimeter
  G4double absorberThickness = 2.5*cm;
  G4double absorberRadius = 50.0*cm;
  G4double detectorThickness = 12.5*cm;
  G4double detectorRadius = 50.0*cm;
  G4double worldLength = 300.0*cm;

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // WORLD: Solid (cube)
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent( worldLength );
  G4Box* worldS = new G4Box(
                 "World",         // its name
                 worldLength,
                 worldLength,
                 worldLength );   // its size (in half-lengths)

  // WORLD: Logical volume (how to treat it)
  G4LogicalVolume* worldLV = new G4LogicalVolume(
                 worldS,          // its solid
                 vacuum,          // its material
                 "World" );       // its name

  // WORLD: Physical volume (where is it)
  // Must place the World Physical volume unrotated at (0,0,0).
  G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(0.0, 0.0, 0.0), // in the centre
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother volume
                 false,           // no boolean operations
                 0,               // copy number
                 true );          // checking overlaps

  // TRACKER: Solid (tube)
  G4Tubs* tracker1S = new G4Tubs(
                "Tracker1",        // its name
                0, // Hollow cylinder
                40*cm,
                tracker1Length,    // how far along the beam axis it extends
                0.0*deg,           // starting angle
                360.0*deg );       // ending angle (i.e. it's a full circle)

  // TRACKER: Logical volume (how to treat it)
  G4LogicalVolume* tracker1LV = new G4LogicalVolume(
                tracker1S,         // its solid
                silicon,           // its material
                "Tracker1",        // its name
                0, 0, 0 );         // Modifiers we don't use

  // TRACKER: Physical volume (where is it)
  G4VPhysicalVolume* tracker1PV = new G4PVPlacement(
                0,                 // no rotation
                G4ThreeVector(0.0, 0.0, -55*cm), // in the centre
                tracker1LV,        // its logical volume
                "Tracker1",        // its name
                worldLV,           // its mother volume
                false,             // no boolean operations
                0,                 // copy number
                true );            // checking overlaps

  // TRACKER: Quit if there's an overlap
  if ( tracker1PV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;


  // ECAL: Solid (cone)
  G4Cons* EcalS = new G4Cons(
                "ECAL",      // its name
                0.0,               // inner radius start point 0, so it's a solid cylinder (not a hollow tube)
                40.0*cm,               // outer radius endpoint 0
                0.0,              // inner radius start start point
                50.0*cm,              // outer radius endpoint
                EcalThickness, // how much material in the beam path (half length)
                0.0*deg,           // starting angle
                360.0*deg );       // ending angle (i.e. it's a full circle)

  // ECAL: Logical volume (how to treat it)
  G4LogicalVolume* EcalLV = new G4LogicalVolume(
                EcalS,         // its solid
                lead_glass,              // its material
                "ECAL",      // its name
                0, 0, 0 );         // Modifiers we don't use

  // ECAL: Physical volume (where is it)
  G4VPhysicalVolume* EcalPV = new G4PVPlacement(
                0,                 // no rotation
                G4ThreeVector(0.0, 0.0, -30.0*cm),  // placed infront of Hcal
                EcalLV,        // its logical volume
                "ECAL",      // its name
                worldLV,           // its mother volume
                false,             // no boolean operations
                0,                 // copy number
                true );            // checking overlaps

  // ECAL: Quit if there's an overlap
  if ( EcalPV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;

  // Make multiple layers
  G4double zPosition = 0.0;
  for ( unsigned int layerIndex = 1; layerIndex <= nLayers; ++layerIndex )
  {
    // ABSORBER: Layer properties
    std::string absorberName = "Absorber" + std::to_string( layerIndex );
    zPosition += absorberThickness;
    G4ThreeVector absorberPosition( 0, 0, zPosition );
    zPosition += absorberThickness;

    // ABSORBER: Solid (tube)
    G4Tubs* absorberS = new G4Tubs(
                   absorberName,      // its name
                   0.0,               // inner radius 0, so it's a solid cylinder (not a hollow tube)
                   absorberRadius,    // outer radius
                   absorberThickness, // how much material in the beam path (half length)
                   0.0*deg,           // starting angle
                   360.0*deg );       // ending angle (i.e. it's a full circle)

    // ABSORBER: Logical volume (how to treat it)
    G4LogicalVolume* absorberLV = new G4LogicalVolume(
                   absorberS,         // its solid
                   lead,              // its material
                   absorberName,      // its name
                   0, 0, 0 );         // Modifiers we don't use

    // ABSORBER: Physical volume (where is it)
    G4VPhysicalVolume* absorberPV = new G4PVPlacement(
                   0,                 // no rotation
                   absorberPosition,  // where is it
                   absorberLV,        // its logical volume
                   absorberName,      // its name
                   worldLV,           // its mother volume
                   false,             // no boolean operations
                   0,                 // copy number
                   true );            // checking overlaps

    // ABSORBER: Quit if there's an overlap
    if ( absorberPV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;

    // DETECTOR: Layer properties
    std::string detectorName = "Detector" + std::to_string( layerIndex );
    zPosition += detectorThickness;
    G4ThreeVector detectorPosition( 0, 0, zPosition );
    zPosition += detectorThickness;

    // DETECTOR: Solid (tube)
    G4Tubs* detectorS = new G4Tubs(
                   detectorName,      // its name
                   0.0,               // inner radius 0, so it's a solid cylinder (not a hollow tube)
                   detectorRadius,    // outer radius
                   detectorThickness, // how much material in the beam path (half length)
                   0.0*deg,           // starting angle
                   360.0*deg );       // ending angle (i.e. it's a full circle)

    // DETECTOR: Logical volume (how to treat it)
    G4LogicalVolume* detectorLV = new G4LogicalVolume(
                   detectorS,         // its solid
                   liquidArgon,       // its material
                   detectorName,      // its name
                   0, 0, 0 );         // Modifiers we don't use

    // DETECTOR: Physical volume (where is it)
    G4VPhysicalVolume* detectorPV = new G4PVPlacement(
                   0,                 // no rotation
                   detectorPosition,  // where is it
                   detectorLV,        // its logical volume
                   detectorName,      // its name
                   worldLV,           // its mother volume
                   false,             // no boolean operations
                   0,                 // copy number
                   true );            // checking overlaps

    // DETECTOR: Warn if there's an overlap
    if ( detectorPV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;
  }

  // Always return the physical world
  return worldPV;
}

// Set up the magnetic field
void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  m_magneticFieldMessenger = new G4GlobalMagFieldMessenger( fieldValue );

  // Register the field messenger for deleting
  G4AutoDelete::Register( m_magneticFieldMessenger );

  // The silicon layer is for tracking
  auto tracker1 = new PositionFinder( "Tracker1", 1 );
  G4SDManager::GetSDMpointer()->AddNewDetector( tracker1 );
  this->SetSensitiveDetector( "Tracker1", tracker1 );

  auto EcalDetector = new EnergyCounter( "ECAL", 1 );
  G4SDManager::GetSDMpointer()->AddNewDetector( EcalDetector );
  this->SetSensitiveDetector( "ECAL", EcalDetector );

  // Make "sensitive detectors" for the liquid argon layers
  for ( unsigned int layerIndex = 1; layerIndex <= nLayers; ++layerIndex )
  {
    std::string detectorName = "Detector" + std::to_string( layerIndex );

    auto argonDetector = new EnergyCounter( detectorName, layerIndex+1 );
    G4SDManager::GetSDMpointer()->AddNewDetector( argonDetector );
    this->SetSensitiveDetector( detectorName, argonDetector );
  }
}
