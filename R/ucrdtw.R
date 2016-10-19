

#' UC Riverside DTW Algorithm
#'
#' Description of this function
#'
#' @name ucrdtw
#' @param ... arguments to appropriate ucrdtw method
#'
#'
#' @useDynLib rucrdtw
#' @importFrom Rcpp sourceCpp
#' @export

ucrdtw <- function(...){
  ucrdtw_ff(...)
}


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
  unlist(object)
}
