---
title: 'rucrdtw: Fast time series subsequence search in R'
tags:
    - R
    - Rcpp
    - Data mining
    - Time series
authors:
   - name: Philipp H Boersch-Supan
     orcid: 0000-0001-6723-6833
     affiliation: Department of Geography and Emerging Pathogens Institute, University of Florida
date: 2016-10-23
bibliography: vignettes/rucrdtw.bib
---

# Summary
  Similarity search is a common task in time series data mining, and usually has a computational bottleneck in the step that calculates the similarity between the query and each data subsequence. 
  This is particularly true for computationally expensive similarity measures such as dynamic time warping (DTW), which provides a similarity measure based on a non-linear mapping between two time series [@berndt1994using].
  DTW is widely used in time series data mining applications for tasks such as classification, clustering, and anomaly detection.
  
  The `rucrdtw` R package makes use of `Rcpp` [@Rcpp] to provide R bindings for the [UCR Suite](http://www.cs.ucr.edu/~eamonn/UCRsuite.html) [@rakthanmanon2012searching], a collection of C++ algorithms which enables ultrafast sliding-window subsequence search under both DTW and Euclidean Distance for queries and data stored in text files. 
  `rucrdtw` additionally implements methods for querys and/or data that are held in memory as R objects, as well as a method to do fast similarity searches against reference libraries of time series.
  The package is available on github (https://github.com/pboesu/rucrdtw) and submission to CRAN is planned following peer review.
   
  
# References
  
