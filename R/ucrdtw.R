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


#' UCR DTW Algorithm vector-matrix method
#'
#' This implementation of the UCR Suite command line utility, takes an R numeric matrix as data input and an R numeric vector for the query.
#' The default behaviour differs from the other methods, in that it does not perform a sliding window search for a match.
#' Instead it is designed to find a best match for a query in a reference set of time-series of the same length as the query.
#' This is useful, for example, when comparing a time-series of undetermined class to a labelled reference set of classified time-series.
#'
#' @name ucrdtw_vm
#' @param data numeric matrix containing data
#' @param query numeric vector containing the query
#' @param qlength int length of query (n data points)
#' @param dtwwindow double warping window size (as a proportion of query length)
#' @param epoch int defaults to 1e5, should be \eqn{\le} 1e6. This is the size of the data chunk that is processed at once. All cumulative values in the algorithm will be restarted after \code{epoch} iterations to reduce floating point errors in these values.
#' @param skip bool defaults to TRUE. If TRUE bound calculations and if necessary, distance calculations, are only performed on non-overlapping segments of the data (i.e. multiples of \code{qlength}). This is useful if \code{data} is a set of multiple reference time series, each of length \code{qlength}. The location returned when skipping is the index of the subsequence.
#' @param byrow logical If TRUE rows represent time-series, columns time-points
#' @return a ucrdtw object, note that the field \code{location} indicates the row or column number of the reference set
#' @export
ucrdtw_vm <- function(data, query, qlength, dtwwindow, epoch = 1e5, skip=TRUE, byrow=FALSE){
  if (class(data)!="matrix" && class(data[1])!="numeric") stop("data must be a numeric matrix")
  if(byrow){
    if(ncol(data)!=qlength) stop("data series length must match query length")
    datav <- as.vector(t(data))
  } else {
    if(nrow(data)!=qlength) stop("data series length must match query length")
    datav <- as.vector(data)
  }
  ucrdtw_vv(data=datav, query, qlength, dtwwindow, epoch = 1e5, skip=TRUE)
}


#' UCR ED Algorithm vector-matrix method
#'
#' This implementation of the UCR Suite command line utility, takes an R numeric matrix as data input and an R numeric vector for the query.
#' The default behaviour differs from the other metjods, in that it does not perform a sliding window search for a match.
#' Instead it is designed to find a best match for a query in a reference set of time-series of the same length as the query.
#' This is useful, for example, when comparing a time-series of undetermined class to a labelled reference set of classified time-series.
#'
#' @name ucred_vm
#' @param data numeric matrix containing data
#' @param query numeric vector containing the query
#' @param qlength int length of query (n data points)
#' @param skip bool defaults to TRUE. If TRUE bound calculations and if necessary, distance calculations, are only performed on non-overlapping segments of the data (i.e. multiples of \code{qlength}). This is useful if \code{data} is a set of multiple reference time series, each of length \code{qlength}. The location returned when skipping is the index of the subsequence.
#' @param byrow logical If TRUE rows represent time-series, columns time-points
#' @return a ucrdtw object, note that the field \code{location} indicates the row or column number of the reference set
#' @export
ucred_vm <- function(data, query, qlength, skip=TRUE, byrow=FALSE){
  if (class(data)!="matrix" && class(data[1])!="numeric") stop("data must be a numeric matrix")
  if(byrow){
    if(ncol(data)!=qlength) stop("data series length must match query length")
    datav <- as.vector(t(data))
  } else {
    if(nrow(data)!=qlength) stop("data series length must match query length")
    datav <- as.vector(data)
  }
  ucred_vv(data=datav, query, qlength, skip=TRUE)
}
