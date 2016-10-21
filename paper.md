---
  title: 'rucrdtw: Fast time series subsequence search under dynamic time warping in R'
  tags:
    - R
    - C++
    - Data mining
    - Time series
  authors:
   - name: Philipp H Boersch-Supan
     orcid: 0000-0001-6723-6833
     affiliation: Department of Geography and Emerging Pathogens Institute, University of Florida
  date: October 2016
  bibliography: vignettes/rucrdtw.bib
---

  # Summary
  Similarity search is a common task in time series data mining, and usually has a computational bottleneck in the step that calculates the similarity between the query and each data subsequence. 
  This is particularly true for computationally expensive similarity measures such as dynamic time warping (DTW), which provides a similarity measure based on a non-linear mapping between two time series [@berndt1994using].
  DTW is widely used in time series data mining applications for tasks such as classification, clustering, and anomaly detection.
  
  The `rucrdtw` R package makes use of `Rcpp` [@Rcpp] to provide R bindings for the [UCR Suite](http://www.cs.ucr.edu/~eamonn/UCRsuite.html) [@rakthanmanon2012searching], a collection of C++ algorithms which enables ultrafast subsequence search under both DTW and Euclidean Distance.
  
  # References
  
