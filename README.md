# Data-Analysis-and-ML

My projects and checkpoints for the University of Edinburgh data analysis and machine learning (DAML) graduate course

In total, I worked on 3 projects and 14 checkpoints. The projects were focused on exploring key stages of the particle physics analysis pipeline and are in the form of Jupyter Notebook reports. 
The checkpoints were based on self-contained analysis tasks that were explored along with the lectures. 

**Report2: Dark machines anomaly detection challenge**

- Designed an autoencoder (AE) for unsupervised anomaly detection
- Performed hyperparameter search
- Chose AE with the highest average ROC area under the curve for all BSM samples
- Found anomaly thresholds maximising the significance improvement

**Report4: 1 TeV Higgs search**

- Manual cuts followed by NN classification on weakly correlated variables with invariant mass, avoiding background sculpting
- Curve fitting using Minuit optimiser: finding best distribution shape, polynomial (H0) vs gaussian+polynomial (H1)
- Fitting polynomial coefficients, gaussian width and signal fraction using the invariant mass
- Obtaining chi-squared difference between H0 and H1 and converting this into statistical significance (final result 7.29 sigma)


