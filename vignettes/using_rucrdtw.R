## ----eval=FALSE----------------------------------------------------------
#  install.packages("devtools")
#  devtools::install_github("pboesu/rucrdtw")

## ------------------------------------------------------------------------
library("rucrdtw")

## ----random-walk---------------------------------------------------------
set.seed(123)
rwalk <- cumsum(runif(1e7, min = -0.5, max = 0.5))

## ----rw-query------------------------------------------------------------
qstart <- sample(length(rwalk), 1)
query <- rwalk[qstart:(qstart+100)]

## ----rw-search-----------------------------------------------------------
system.time(dtw_search <- ucrdtw_vv(data = rwalk, query = query, dtwwindow = 0.05))
all.equal(qstart, dtw_search$location)
system.time(ed_search <- ucred_vv(data = rwalk, query = query))
all.equal(qstart, ed_search$location)

## ----load-data-----------------------------------------------------------
data("synthetic_control")

## ----plot-examples, fig.width=6------------------------------------------
par(mfrow = c(3,2),
    mar = c(1,1,1,1))
classes = c("Normal", "Cyclic", "Increasing", "Decreasing", "Upward shift", "Downward shift")
for (i in 1:6){
  plot(synthetic_control[i*100-99,], type = "l", xaxt = "n", yaxt = "n", ylab="", xlab = "", bty="n", main=classes[i])
}

## ---- echo=TRUE, message=TRUE, warning=TRUE------------------------------
index <- 600
query <- synthetic_control[index,]

dtw_search <- ucrdtw_mv(synthetic_control[-index,], query, 0.05, byrow = TRUE)
ed_search <- ucred_mv(synthetic_control[-index,], query, byrow= TRUE)


## ----plot-search, fig.width=6--------------------------------------------
plot(synthetic_control[dtw_search$location,], type="l", ylim=c(0,55), ylab="")
lines(query, col="red")
lines(synthetic_control[ed_search$location,], col="blue", lty=3, lwd=3)
legend("topright", legend = c("query", "DTW match", "ED match"), col=c("red", "black", "blue"), lty=c(1,1,3), bty="n")

## ----dtw-comparison, message=FALSE---------------------------------------
set.seed(123)
rwalk <- cumsum(runif(5e3, min = -0.5, max = 0.5))
qstart <- 876
query <- rwalk[qstart:(qstart+99)]
library(dtw)

## ----naive-function------------------------------------------------------
naive_dtw <- function(data, query){
  n_comps <- (length(data)-length(query)+1)
  dtw_dist <- numeric(n_comps)
  for (i in 1:n_comps){
    dtw_dist[i] <- dtw(query, data[i:(i+length(query)-1)], distance.only = TRUE, window.type="sakoechiba", window.size=5)$distance
  }
  which.min(dtw_dist)
}

## ----run-benchmark, fig.width=6------------------------------------------
if(require(rbenchmark)){
benchmarks <- rbenchmark::benchmark(
  naive_1000 = naive_dtw(rwalk[1:1000], query),
  naive_2000 = naive_dtw(rwalk[1:2000], query),
  naive_5000 = naive_dtw(rwalk, query),
  ucrdtw_1000 = ucrdtw_vv(rwalk[1:1000], query, 0.05),
  ucrdtw_2000 = ucrdtw_vv(rwalk[1:2000], query, 0.05),
  ucrdtw_5000 = ucrdtw_vv(rwalk, query, 0.05),
  replications = 5)

colors <- rep(c("#33a02c","#1f78b4"), each=3)
plot(log10(benchmarks$elapsed*200) ~ benchmarks$test, cex.axis=0.7, las = 2, yaxt = "n", xlab = "", ylab = "execution time [ms]", ylim = c(0,5), medcol = colors, staplecol=colors, boxcol=colors)
axis(2, at = c(0:4), labels = 10^(0:4), cex.axis = 0.7)
legend("topright", legend = c("naive DTW", "UCR DTW"), fill = c("#33a02c","#1f78b4"), bty="n")
}

