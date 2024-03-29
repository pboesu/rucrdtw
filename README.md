# rucrdtw - R Bindings for the UCR Suite

<!-- badges: start -->

[![R-CMD-check](https://github.com/pboesu/rucrdtw/workflows/R-CMD-check/badge.svg)](https://github.com/pboesu/rucrdtw/actions)
[![codecov](https://codecov.io/gh/pboesu/rucrdtw/branch/master/graph/badge.svg)](https://app.codecov.io/gh/pboesu/rucrdtw)
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/rucrdtw)](https://cran.r-project.org/package=rucrdtw)
[![Peer_Reviewed_Software](https://img.shields.io/badge/Peer%20Reviewed-%E2%9C%93-green.svg)](http://doi.org/10.21105/joss.00100)
[![DOI:10.21105/joss.00100](https://img.shields.io/badge/DOI-10.21105%2Fjoss.00100-blue.svg)](http://doi.org/10.21105/joss.00100)
<!-- badges: end -->

R bindings for functions from the [UCR
Suite](http://www.cs.ucr.edu/~eamonn/UCRsuite.html) (Thanawin
Rakthanmanon et al. 2012 SIGKDD), which enables ultrafast subsequence
search under both Dynamic Time Warping and Euclidean Distance. UCR DTW
is several orders of magnitudes faster than naive DTW searches.

![](inst/img/dtw-comparison-1.png)

## Installation

Install `rucrdtw` from GitHub:

``` r
install.packages("devtools")
devtools::install_github("pboesu/rucrdtw")
```

## Examples

Examples are contained in the vignette `rucrdtw`:

``` r
library("rucrdtw")
vignette("using_rucrdtw")
```

## Citation

To cite rucrdtw in publications, please use:

Boersch-Supan (2016). rucrdtw: Fast time series subsequence search in R.
The Journal of Open Source Software <http://doi.org/10.21105/joss.00100>

Rakthanmanon et al. (2012). Searching and mining trillions of time
series subsequences under dynamic time warping. SIGKDD
<http://doi.org/10.1145/2339530.2339576>

## Bug reports and contributions

Please file a github issue at <https://github.com/pboesu/rucrdtw/issues>
if you find any problems or have feature suggestions. Contributions (via
pull requests or otherwise) are welcome. Read more about how to
contribute and the code of conduct for contributions
[here](https://github.com/pboesu/rucrdtw/blob/master/CONTRIBUTING.md).
If you don’t like github you can contact the package maintainer at
<pboesu@gmail.com>.

## License

COPYRIGHT 2012: Thanawin Rakthanmanon, Bilson Campana, Abdullah Mueen,
Gustavo Batista and Eamonn Keogh. (UCR Suite source code)

COPYRIGHT 2016: Philipp Boersch-Supan (R bindings)

Licensed under the Apache License, Version 2.0 (the “License”); you may
not use this file except in compliance with the License. You may obtain
a copy of the License at

<https://www.apache.org/licenses/LICENSE-2.0>

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an “AS IS” BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
