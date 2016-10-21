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
  print(as.data.frame.list(object), row.names=F, ...)
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
  print(as.data.frame.list(object), row.names=F, ...)
}
