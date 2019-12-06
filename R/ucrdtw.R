#' Summarize subsequence search
#'
#' Summary method for class ucrdtw
#'
#' @param object an object of class ucrdtw
#' @param ... further arguments passed to or from methods
#'
#' @return An unlisted version of the object is returned.
#' @export
#'
summary.ucrdtw <- function(object, ...){
  print(as.data.frame.list(object), row.names=FALSE, ...)
}

#' Summarize subsequence search
#'
#' Summary method for class ucred
#'
#' @param object an object of class ucred
#' @param ... further arguments passed to or from methods
#'
#' @return An unlisted version of the object is returned.
#' @export
#'
summary.ucred <- function(object, ...){
  print(as.data.frame.list(object), row.names=FALSE, ...)
}


#' UCR DTW Algorithm matrix-vector method
#'
#' This implementation of the UCR Suite command line utility, takes an R numeric matrix as data input and an R numeric vector for the query.
#' The default behaviour differs from the other methods, in that it does not perform a sliding window search for a match.
#' Instead it is designed to find a best match for a query in a reference set of time-series of the same length as the query.
#' This is useful, for example, when comparing a time-series of undetermined class to a labelled reference set of classified time-series.
#'
#' @name ucrdtw_mv
#' @param data numeric matrix containing data
#' @param query numeric vector containing the query. This determines the query length.
#' @param dtwwindow double; Size of the warping window size (as a proportion of query length). The DTW calculation in `rucrdtw` uses a symmetric Sakoe-Chiba band. See Giorgino (2009) for a general coverage of warping window constraints.
#' @param epoch int defaults to 1e5, should be \eqn{\le} 1e6. This is the size of the data chunk that is processed at once. All cumulative values in the algorithm will be restarted after \code{epoch} iterations to reduce floating point errors in these values.
#' @param skip boolean; defaults to TRUE. If TRUE bound calculations and if necessary, distance calculations, are only performed on non-overlapping segments of the data (i.e. multiples of \code{length(query)}). This is useful if \code{data} is a set of multiple reference time series, each of length \code{length(query)}. The location returned when skipping is the index of the subsequence.
#' @param byrow logical; If TRUE rows in \code{data} represent time-series, columns time-points
#' @return a ucrdtw object. A list containing the following elements
#' \itemize{
#'   \item \strong{location:} The row or column number of the nearest neighbor of the given query in the data set.
#'   \item \strong{distance:} The DTW distance between the nearest neighbor and the query.
#'   \item \strong{prunedKim:} Percentage of subsequences that were pruned based on the LB-Kim criterion.
#'   \item \strong{prunedKeogh:} Percentage of subsequences that were pruned based on the LB-Keogh-EQ criterion.
#'   \item \strong{prunedKeogh2:} Percentage of subsequences that were pruned based on the LB-Keogh-EC criterion.
#'   \item \strong{dtwCalc:} Percentage of subsequences for which the full DTW distance was calculated.
#' }
#' @references Giorgino, Toni (2009). Computing and Visualizing Dynamic Time Warping Alignments in R: The dtw Package. Journal of Statistical Software, 31(7), 1-24, doi:\href{http://dx.doi.org/10.18637/jss.v031.i07}{10.18637/jss.v031.i07}.
#' @examples
#' #load example data
#' data("synthetic_control")
#' query <- synthetic_control[5,]
#' #run query
#' ucrdtw_mv(synthetic_control, query, 0.05, byrow = TRUE)
#' @export
ucrdtw_mv <- function(data, query, dtwwindow, epoch = 1e5, skip=TRUE, byrow=FALSE){
  if (!is.matrix(data) && !is.numeric(data)) stop("data must be a numeric matrix")
  if(byrow){
    if(ncol(data)!=length(query)) stop("data series length must match query length")
    datav <- as.vector(t(data))
  } else {
    if(nrow(data)!=length(query)) stop("data series length must match query length")
    datav <- as.vector(data)
  }
  ucrdtw_vv(data=datav, query, dtwwindow, epoch = 1e5, skip=TRUE)
}


#' UCR ED Algorithm matrix-vector method
#'
#' This implementation of the UCR Suite command line utility, takes an R numeric matrix as data input and an R numeric vector for the query.
#' The default behaviour differs from the other methods, in that it does not perform a sliding window search for a match.
#' Instead it is designed to find a best match for a query in a reference set of time-series of the same length as the query.
#' This is useful, for example, when comparing a time-series of undetermined class to a labelled reference set of classified time-series.
#'
#' @name ucred_mv
#' @param data numeric matrix containing data
#' @param query numeric vector containing the query. This determines the query length.
#' @param skip boolean; defaults to TRUE. If TRUE bound calculations and if necessary, distance calculations, are only performed on non-overlapping segments of the data (i.e. multiples of \code{length(query)}). This is useful if \code{data} is a set of multiple reference time series, each of length \code{length(query)}. The location returned when skipping is the index of the subsequence.
#' @param byrow logical; If TRUE rows in \code{data} represent time-series, columns time-points
#' @return a ucred object. A list with the following elements
#' \itemize{
#'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{qlength}, in the data. Note that location starts from 1.
#'   \item \strong{distance:} The euclidean distance between the nearest neighbor and the query.
#' }
#' @examples
#' #load example data matrix
#' data("synthetic_control")
#' #use on arbitrary row as query
#' query <- synthetic_control[5,]
#' #run query
#' ucred_mv(synthetic_control, query, byrow=TRUE)
#' @export
ucred_mv <- function(data, query, skip=TRUE, byrow=FALSE){
  if (!is.matrix(data) && !is.numeric(data)) stop("data must be a numeric matrix")
  if(byrow){
    if(ncol(data)!=length(query)) stop("data series length must match query length")
    datav <- as.vector(t(data))
  } else {
    if(nrow(data)!=length(query)) stop("data series length must match query length")
    datav <- as.vector(data)
  }
  ucred_vv(data=datav, query, skip=TRUE)
}
