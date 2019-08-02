rucrdtw - R Bindings for the UCR Suite
======================================

[![Travis-CI Build Status](https://api.travis-ci.org/pboesu/rucrdtw.svg?branch=master)](https://travis-ci.org/pboesu/rucrdtw) [![codecov](https://codecov.io/gh/pboesu/rucrdtw/branch/master/graph/badge.svg)](https://codecov.io/gh/pboesu/rucrdtw) [![CRAN\_Status\_Badge](http://www.r-pkg.org/badges/version/deBInfer)](https://cran.r-project.org/package=deBInfer) ![Peer Reviewed Software](https://img.shields.io/badge/Peer%20Reviewed-%E2%9C%93-green.svg) [![DOI:10.1111/2041-210X.12679](https://img.shields.io/badge/DOI-10.1111%2F2041--210X.12679-blue.svg)](http://doi.org/10.1111/2041-210X.12679)

R bindings for functions from the [UCR Suite](http://www.cs.ucr.edu/~eamonn/UCRsuite.html) (Thanawin Rakthanmanon et al. 2012 SIGKDD), which enables ultrafast subsequence search under both Dynamic Time Warping and Euclidean Distance. UCR DTW is several orders of magnitudes faster than naive DTW searches.

![](inst/img/dtw-comparison-1.png)

Installation
------------

Install `rucrdtw` from GitHub:

``` r
install.packages("devtools")
devtools::install_github("pboesu/rucrdtw")
```

Examples
--------

Examples are contained in the vignette `rucrdtw`:

``` r
library("rucrdtw")
vignette("using_rucrdtw")
```

Bug reports and contributions
-----------------------------

Please file a github issue at <https://github.com/pboesu/rucrdtw/issues> if you find any problems or have feature suggestions. Contributions (via pull requests or otherwise) are welcome. If you don't like github you can contact the package maintainer at <pboesu@gmail.com>.

License
-------

COPYRIGHT 2012: Thanawin Rakthanmanon, Bilson Campana, Abdullah Mueen, Gustavo Batista and Eamonn Keogh. (UCR Suite source code)

COPYRIGHT 2016: Philipp Boersch-Supan (R bindings)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

<http://www.apache.org/licenses/LICENSE-2.0>

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
