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
  bibliography: paper.bib
---

  # Summary
  Similarity search is the core step of many time series data mining approaches, and at the same time often the computational bottleneck. 
  This is particularly true if computationally expensive similarity measures are employed.
  Dynamic time warping (DTW) provides a similarity measure based on a non-linear mapping between two time series [@berndt1994using].
  It is widely used in time series data mining applications for tasks such as classification, clustering, and anomaly detection.
  
  The [UCR Suite](http://www.cs.ucr.edu/~eamonn/UCRsuite.html) [@rakthanmanon2012searching] is a collection of C++ programs for the purpose of finding a best match for a query in a large data set. The `rucrdtw` R package makes use of `Rcpp` [@Rcpp] to provide R bindings for functions from the UCR Suite.
  
  # References
  
