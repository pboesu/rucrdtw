#' rucrdtw: Fast time series subsequence search in R
#'
#' @description Dynamic Time Warping (DTW) methods provide algorithms to optimally map a given time series onto all or part of another time series. The remaining cumulative distance between the series after the alignement is a useful distance metric in time series data mining applications for tasks such as classification, clustering, and anomaly detection. A broad suite of DTW algorithms is implemented in R in the \strong{dtw} package (Giorgino 2009).
#'
#' Calculating a DTW alignment is computationally relatively expensive, and as a consequence DTW is often a bottleneck in time series data mining applications. The UCR Suite (Rakthanmanon et al. 2012) provides a highly optimized algorithm for best-match subsequence searches that avoids unnecessary distance computations and thereby enables fast DTW and Euclidean Distance queries even in data sets containing trillions of observations.
#'
#' The \strong{rucrdtw} package provides R bindings for the UCR Suite. In addition to queries and data stored in text files, rucrdtw also implements methods for queries and/or data that are held in memory as R objects, as well as a method to do fast similarity searches against reference libraries of time series.
#'
#' Examples of the functionality in this package are provided in the rucrdtw vignette:
#'
#' \code{vignette("using_rucrdtw", package = "rucrdtw")}
#'
#' @name rucrdtw
#' @docType package
#' @references Rakthanmanon, Thanawin, Bilson Campana, Abdullah Mueen, Gustavo Batista, Brandon Westover, Qiang Zhu, Jesin Zakaria, and Eamonn Keogh. 2012. “Searching and Mining Trillions of Time Series Subsequences Under Dynamic Time Warping.” In Proceedings of the 18th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining, 262–70. ACM. doi:\href{http://dx.doi.org/10.1145/2339530.2339576}{10.1145/2339530.2339576}.
#' @references Giorgino, Toni (2009). Computing and Visualizing Dynamic Time Warping Alignments in R: The dtw Package. Journal of Statistical Software, 31(7), 1-24, doi:\href{http://dx.doi.org/10.18637/jss.v031.i07}{10.18637/jss.v031.i07}.
#' @references UCR Suite Website: \url{http://www.cs.ucr.edu/~eamonn/UCRsuite.html}
#' @seealso \link[dtw]{dtw-package}
NULL
