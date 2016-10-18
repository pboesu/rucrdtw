context("match time-series against itself")

#get test data file locations
dataf <- system.file("testdata/ref_nospacer.txt", package="rucrdtw")
c1f <- system.file("testdata/c1.txt", package="rucrdtw")
c144f <- system.file("testdata/c144.txt", package="rucrdtw")

test_that("ff method works", {
  first = ucrdtw(dataf, c1f, 756, 0.05)
  expect_equal(first$location, 0)
  expect_equal(first$distance, 0)
  last = ucrdtw(dataf, c144f, 756, 0.05)
  expect_equal(last$location, 109395)
  expect_equal(last$distance, 0)
})

#read in query vectors
c1v <- read.table(c1f)$V1
c144v <-  read.table(c144f)$V1
#read in data vector
datav <- read.table(dataf)$V1

test_that("fv method works", {
  first = ucrdtw_fv(dataf, c1v, 756, 0.05)
  expect_equal(first$location, 0)
  expect_equal(first$distance, 0)
  last = ucrdtw_fv(dataf, c144v, 756, 0.05)
  expect_equal(last$location, 109395)
  expect_equal(last$distance, 0)
})

test_that("vv method works", {
  first = ucrdtw_vv(datav, c1v, 756, 0.05)
  expect_equal(first$location, 0)
  expect_equal(first$distance, 0)
  last = ucrdtw_vv(datav, c144v, 756, 0.05)
  expect_equal(last$location, 109395)
  expect_equal(last$distance, 0)
})
