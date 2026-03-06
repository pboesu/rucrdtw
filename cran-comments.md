## Release summary
This is a minor update of the previous version it addresses:
* a request by the Rcpp maintainers to protect Rf_error calls from macro expansion by building the package with Rcpp 1.1.1.
* a CRAN check warning on Windows R-devel, presumably caused by stricter GCC analysis of array bounds in the Windows R-devel GCC toolchain

## Test environments
* Windows Server 2022 x64 (build 26100) R version 4.5.2 (2025-10-31 ucrt), R-devel 
* macOS Sequoia 15.7.4 R version 4.5.2 (2025-10-31)
* Ubuntu 24.04.3 LTS R version 4.4.3 (2025-02-28), 4.5.2 (2025-10-31), and R-devel (2026-03-04 r89535)

## R CMD check results
There were no ERRORs, WARNINGs, or NOTEs.
