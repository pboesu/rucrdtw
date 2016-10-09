

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
