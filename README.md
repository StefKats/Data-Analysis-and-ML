# Data-Analysis-and-ML

My projects and checkpoints for the University of Edinburgh data analysis and machine learning (DAML) graduate course

In total, I worked on 3 projects and 14 checkpoints. The projects were focused on exploring key stages of the particle physics analysis pipeline and are in the form of Jupyter Notebook reports. 
The checkpoints were based on self-contained analysis tasks that were explored along with the lectures. The projects contain all of the output files needed to interpret the results. Only the initial datasets are not included due to their size. 

### Report2: Dark machines anomaly detection challenge

- Designed an autoencoder (AE) for unsupervised anomaly detection
- Performed hyperparameter search
- Chose AE with the highest average ROC area under the curve for all BSM samples
- Found anomaly thresholds maximising the significance improvement

### Report3: Geant4 custom detector build, detection and classification analysis

**Base set-up**

- Fired truth electrons, protons, photons and neutrons at homogeneous electromagnetic calorimeter (10 cm lead-glass) and hadronic sampling calorimeter (3cm lead, 30 cm liquid argon, 5 stacks)
- 5000 particles of each type fired with energy incrementing by 5 MeV each time, starting at 200 MeV and finishing at 25 GeV. 20000 events in total. Simulation precision tuned to speed up computation. 
- Processed detector hits and used them to train a neural network to classify between the four particles. The current detector cannot distinguish the charge of particles so protons-neutrons and electrons-photons get mistagged between the classes. The best val_loss is 0.5818. It does better than guessing which is equivalent to 0.25.
- Calibration quality is very good for electrons and photons as they are completely stopped. Protons and neutrons are not fully contained in the detector so they have poor calibration quality. 
  
**Enhanced set-up**

- **Improvements**: Add silicon tracking layer (1cm) before ECAL, increase the lead to liquid argon ratio (5:25), increase the depth of HCAL (from 5 to 8 layers) and add 0.05 Tesla magnetic field in the x direction.
- **Effect**: Distinguish between charged and non-charged particles, increase the stopping power of the HCAL, increase the detector material to contain the particles better, and distinguish the charge between particles.
- Repeat the event simulation for another 20000 events, process the detector read-out and train a neural network. 
- Best val_loss is now 0.9984. The symmetry in the detector activation between electrons-photons and neutrons-protons is now removed and the network can easily discriminate between the four particles.
- Calibration quality and energy resolution for neutrons and protons have significantly improved due to the improved stopping power of the HCAL.
- Classification can be improved with some pre-selection criteria. 

  
### Report4: 1 TeV Higgs search

- Manual cuts followed by NN classification on weakly correlated variables with invariant mass, avoiding background sculpting
- Curve fitting using Minuit optimiser: finding best distribution shape, polynomial (H0) vs gaussian+polynomial (H1)
- Fitting polynomial coefficients, gaussian width and signal fraction using the invariant mass
- Obtaining chi-squared difference between H0 and H1 and converting this into statistical significance (final result 7.29 sigma)


