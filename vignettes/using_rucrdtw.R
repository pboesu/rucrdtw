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
#microbenchmark::microbenchmark(
dtw_search = ucrdtw_mv(synthetic_control[-index,], query, 0.05, byrow = TRUE)
ed_search = ucred_mv(synthetic_control[-index,], query, byrow= TRUE)
#times=50)

## ----plot-search, fig.width=6--------------------------------------------
plot(synthetic_control[dtw_search$location,], type="l", ylim=c(0,55), ylab="")
lines(query, col="red")
lines(synthetic_control[ed_search$location,], col="blue", lty=3, lwd=3)
legend("topright", legend = c("query", "DTW match", "ED match"), col=c("red", "black", "blue"), lty=c(1,1,3), bty="n")

