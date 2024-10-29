#ifndef _object_tools_h_
#define _object_tools_h_

#include <vector>
#include <algorithm>
#include "TLorentzVector.h"

// Particle class definition
class Particle : public TLorentzVector {
  public:
    // Constructor with initialization for all member variables
    Particle(TLorentzVector p4, int charge, int id, int qualityFlags, int origRef, double puppi = 1, double unc = 0)
      : TLorentzVector(p4), charge_(charge), id_(id), qualityFlags_(qualityFlags), origRef_(origRef), puppi_(puppi), unc_(unc) {}

    // Copy constructor
    Particle(const Particle &p) 
      : TLorentzVector(p.Px(), p.Py(), p.Pz(), p.E()), charge_(p.charge_), id_(p.id_), qualityFlags_(p.qualityFlags_), origRef_(p.origRef_), puppi_(p.puppi_), unc_(p.unc_) {}

    // Accessor methods for particle properties
    double px() const    { return TLorentzVector::Px(); }
    double py() const    { return TLorentzVector::Py(); }
    double pz() const    { return TLorentzVector::Pz(); }
    double e()  const    { return TLorentzVector::E(); }
    double pt()          { return TLorentzVector::Pt(); }
    double eta()         { return TLorentzVector::Eta(); }
    double phi()         { return TLorentzVector::Phi(); }
    double energy()      { return TLorentzVector::E(); }
    double m()           { return TLorentzVector::M(); }
    double mass()        { return TLorentzVector::M(); }
    TLorentzVector p4()  { return TLorentzVector(TLorentzVector::Px(), TLorentzVector::Py(), TLorentzVector::Pz(), TLorentzVector::E()); }
    TLorentzVector momentum() { return p4(); }
    int charge()         { return charge_; }
    int id()             { return id_; }
    int qualityFlags()   { return qualityFlags_; }
    bool hasQualityFlag(int bit) { return ((qualityFlags_ >> bit) & 0x1); }
    int originalReference() { return origRef_; }
    void setOriginalReference(int origRef) { origRef_ = origRef; }
    double puppi()       { return puppi_; }
    double scaleUnc()    { return unc_; }

  private:
    int charge_, id_, qualityFlags_, origRef_;
    double puppi_;
    double unc_;
};

// Definition for tracks associated with jets
typedef std::pair<TLorentzVector, int> IdTrack;

// Jet class definition
class Jet : public TLorentzVector {
  public:
    // Constructor with flavor and index, initializes all member variables in declaration order
    Jet(TLorentzVector p4, int flavor, int idx)
      : TLorentzVector(p4), particles_(), trks_(), deepjet_(0), pumva_(0), flavor_(flavor), idx_(idx), overlap_(0), scaleUnc_(0) {}

    // Constructor with DeepJet score, initializes all member variables in declaration order
    Jet(TLorentzVector p4, float deepjet, int idx)
      : TLorentzVector(p4), particles_(), trks_(), deepjet_(deepjet), pumva_(0), flavor_(0), idx_(idx), overlap_(0), scaleUnc_(0) {}

    // Copy constructor for Jet class, corrected idx initialization and initializer list syntax
    Jet(const Jet &j) 
      : TLorentzVector(j.Px(), j.Py(), j.Pz(), j.E()),
        particles_(j.particles_),
        trks_(j.trks_),
        deepjet_(j.deepjet_),
        pumva_(j.pumva_),
        flavor_(j.flavor_),
        idx_(j.idx_),
        overlap_(j.overlap_),
        scaleUnc_(j.scaleUnc_)
    {}

    ~Jet() {}

    // Accessor and mutator methods for Jet properties
    double pt()          { return TLorentzVector::Pt(); }
    double eta()         { return TLorentzVector::Eta(); }
    TLorentzVector p4()  { return TLorentzVector(TLorentzVector::Px(), TLorentzVector::Py(), TLorentzVector::Pz(), TLorentzVector::E()); }
    TLorentzVector momentum() { return p4(); }
    std::vector<Particle>& particles() { return particles_; }
    int flavor()         { return flavor_; }
    int overlap()        { return overlap_; }

    void addParticle(Particle p) { particles_.push_back(p); }
    void setFlavor(int flavor)   { flavor_ = flavor; }
    void setOverlap(int overlap) { overlap_ = overlap; }

    void addTrack(TLorentzVector p4, int pfid) { trks_.push_back(IdTrack(p4, pfid)); }
    std::vector<IdTrack>& getTracks() { return trks_; }
    void sortTracksByPt() { std::sort(trks_.begin(), trks_.end(), sortIdTracksByPt); }

    float getPUMVA()          { return pumva_; }
    void setPUMVA(float pumva) { pumva_ = pumva; }

    float getDeepJET()        { return deepjet_; }
    void setDeepJET(float deepjet) { deepjet_ = deepjet; }

    int getJetIndex()         { return idx_; }
    void setScaleUnc(float scaleUnc) { scaleUnc_ = scaleUnc; }
    float getScaleUnc()       { return scaleUnc_; }

    // Static sorting methods for Jet properties
    static bool sortJetsByPt(Jet i, Jet j)  { return i.Pt() > j.Pt(); }
    static bool sortJetsByCSV(Jet i, Jet j) { return i.getDeepJET() > j.getDeepJET(); }

  private:
    static bool sortIdTracksByPt(IdTrack i, IdTrack j)  { return i.first.Pt() > j.first.Pt(); }

    // Member variables in the order they are declared to avoid initialization order issues
    std::vector<Particle> particles_;  // Stores particles associated with the jet
    std::vector<IdTrack> trks_;        // Stores tracks associated with the jet
    float deepjet_;                    // DeepJet b-tagging score
    float pumva_;                      // PU MVA score
    int flavor_;                       // Flavor identifier
    int idx_;                          // Index of the jet
    int overlap_;                      // Overlap flag
    float scaleUnc_;                   // Scale uncertainty
};

#endif
