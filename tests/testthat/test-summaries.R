context("test summary output")

#get test data file locations
dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
firstf <- system.file("extdata/first_sc.txt", package="rucrdtw")

test_that("ucrdtw summary method works", {
  first = ucrdtw_ff(dataf, firstf, 60, 0.05)
  x <- summary(first)
  expect_equal(class(x), "data.frame")
  expect_equal(ncol(x), length(first))
})


test_that("ucred summary method works", {
  first = ucred_ff(dataf, firstf, 60)
  x <- summary(first)
  expect_equal(class(x), "data.frame")
  expect_equal(ncol(x), length(first))
})
