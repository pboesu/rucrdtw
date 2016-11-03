context("test failures on inputs")

#get test data file locations
dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
firstf <- system.file("extdata/first_sc.txt", package="rucrdtw")
#load data
data("synthetic_control")
#cut off some columns
wrong_ref <- synthetic_control[,1:55]

test_that("ucrdtw_vm error message is displayed on unsuitable input", {
  #input not a numeric matrix
  expect_error(ucrdtw_vm(dataf, firstf, 60, 0.05), "data must be a numeric matrix")
  #input dimensions wrong
})

test_that("vm method works", {
    query <- synthetic_control[123,]
    #microbenchmark::microbenchmark(
    expect_error(ucrdtw_vm(wrong_ref, query, length(query), 0.05, byrow = TRUE), "data series length must match query length")
    expect_error(ucrdtw_vm(wrong_ref, query, length(query), 0.05, byrow = FALSE), "data series length must match query length")

})
