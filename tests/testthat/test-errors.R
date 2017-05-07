context("test failures on inputs")

#get test data file locations
dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
firstf <- system.file("extdata/first_sc.txt", package="rucrdtw")
#load data
data("synthetic_control")
#get query length
qlength <- length(synthetic_control[1,])
#cut off some columns
wrong_ref <- synthetic_control[,1:55]

test_that("error message is displayed on unsuitable input", {
  #input not a numeric matrix
  expect_error(ucrdtw_mv(dataf, firstf, 0.05), "data must be a numeric matrix")
  expect_error(ucred_mv(dataf, firstf), "data must be a numeric matrix")
  expect_error(ucrdtw_ff("missing.file", firstf, 60, 0.05), "ERROR : File not Found!!!")
  expect_error(ucrdtw_ff(dataf, "missing.file", 60, 0.05), "ERROR : File not Found!!!")
  expect_error(ucred_ff("missing.file", firstf, 60), "ERROR : File not Found!!!")
  expect_error(ucred_ff(dataf, "missing.file", 60), "ERROR : File not Found!!!")
  expect_error(ucrdtw_fv("missing.file", synthetic_control[1,], 0.05), "ERROR : File not Found!!!")
  expect_error(ucred_fv("missing.file", synthetic_control[1,]), "ERROR : File not Found!!!")
})

test_that("dtw vm method errors on dimension mismatch", {
    query <- synthetic_control[123,]
    expect_error(ucrdtw_mv(wrong_ref, query, 0.05, byrow = TRUE), "data series length must match query length")
    expect_error(ucrdtw_mv(wrong_ref, query, 0.05, byrow = FALSE), "data series length must match query length")
})

test_that("dtw ed method works errors on dimension mismatch", {
  query <- synthetic_control[123,]
  expect_error(ucred_mv(wrong_ref, query, byrow = TRUE), "data series length must match query length")
  expect_error(ucred_mv(wrong_ref, query, byrow = FALSE), "data series length must match query length")
})
