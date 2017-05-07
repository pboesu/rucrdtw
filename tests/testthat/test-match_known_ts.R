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
  first = ucrdtw_ff(dataf, firstf, qlength, 0.05)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw_ff(dataf, lastf, qlength, 0.05)
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})


test_that("fv method works", {
    first = ucrdtw_fv(dataf, synthetic_control[1,], 0.05)
    expect_equal(first$location, 1)
    expect_equal(first$distance, 0)
    last = ucrdtw_fv(dataf, synthetic_control[600,], 0.05)
    expect_equal(last$location, 36000-qlength+1)
    expect_equal(last$distance, 0)
})

#reshape data matrix to vector
datav <- as.vector(t(synthetic_control))

test_that("vv method works", {
  first = ucrdtw_vv(datav, synthetic_control[1,], 0.05)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw_vv(datav, synthetic_control[600,], 0.05)
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})

test_that("vv method works with skipping", {
  first = ucrdtw_vv(datav, synthetic_control[1,], 0.05, skip = TRUE)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw_vv(datav, synthetic_control[600,], 0.05, skip = TRUE)
  expect_equal(last$location, 600)
  expect_equal(last$distance, 0)
})

test_that("vm method works", {
  for (index in c(1, 234, 600)){
  query <- synthetic_control[index,]
  dtw_search = ucrdtw_mv(synthetic_control, query, 0.05, byrow = TRUE)
  expect_equal(dtw_search$location, index)
  expect_equal(dtw_search$distance, 0)
  dtw_search = ucrdtw_mv(t(synthetic_control), query, 0.05, byrow = FALSE)
  expect_equal(dtw_search$location, index)
  expect_equal(dtw_search$distance, 0)
  }
})

test_that("vv method works with epoch < data", {
  first = ucrdtw_vv(datav, synthetic_control[1,], 0.05, epoch = 10000)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucrdtw_vv(datav, synthetic_control[600,], 0.05, epoch = 10000)
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})
#ed methods

test_that("ed_ff method works", {
  first = ucred_ff(dataf, firstf, qlength)
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucred_ff(dataf, lastf, qlength)
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})

test_that("ed_fv method works", {
  first = ucred_fv(dataf, synthetic_control[1,])
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucred_fv(dataf, synthetic_control[600,])
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})

test_that("ed_vv method works", {
  first = ucred_vv(datav, synthetic_control[1,])
  expect_equal(first$location, 1)
  expect_equal(first$distance, 0)
  last = ucred_vv(datav, synthetic_control[600,])
  expect_equal(last$location, 36000-qlength+1)
  expect_equal(last$distance, 0)
})

test_that("ed_mv method works", {
  for (index in c(1, 234, 600)){
    query <- synthetic_control[index,]
    #microbenchmark::microbenchmark(
    ed_search = ucred_mv(synthetic_control, query, byrow = TRUE)
    expect_equal(ed_search$location, index)
    expect_equal(ed_search$distance, 0)
    #same with columnwise input
    ed_search = ucred_mv(t(synthetic_control), query, byrow = FALSE)
    expect_equal(ed_search$location, index)
    expect_equal(ed_search$distance, 0)
  }
})

