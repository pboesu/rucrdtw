context("match time-series against itself")

#get test data file locations
dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
firstf <- system.file("extdata/first_sc.txt", package="rucrdtw")
lastf <- system.file("extdata/last_sc.txt", package="rucrdtw")
midf <- system.file("extdata/mid_sc.txt", package="rucrdtw")
#load data
data("synthetic_control")

#get query length
qlength <- length(synthetic_control[1,])

test_that("ff method works", {
  first = ucrdtw(dataf, firstf, qlength, 0.05)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw(dataf, lastf, qlength, 0.05)
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})


test_that("fv method works", {
    first = ucrdtw_fv(dataf, synthetic_control[1,], qlength, 0.05)
    expect_equal(first$location, 1)
    expect_equal(first$distance, 0)
    last = ucrdtw_fv(dataf, synthetic_control[600,], qlength, 0.05)
    expect_equal(last$location, 36000-qlength+1)
    expect_equal(last$distance, 0)
})

#reshape data matrix to vector
datav <- as.vector(t(synthetic_control))

test_that("vv method works", {
  first = ucrdtw_vv(datav, synthetic_control[1,], qlength, 0.05)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw_vv(datav, synthetic_control[600,], qlength, 0.05)
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})

test_that("vv method works with skipping", {
  first = ucrdtw_vv(datav, synthetic_control[1,], qlength, 0.05, skip = TRUE)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw_vv(datav, synthetic_control[600,], qlength, 0.05, skip = TRUE)
  expect_equal(last$location, 600)
  expect_equal(last$distance, 0)
})
