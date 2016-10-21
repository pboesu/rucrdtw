## ----eval=FALSE----------------------------------------------------------
#  install.packages("devtools")
#  devtools::install_github("pboesu/rucrdtw")

## ------------------------------------------------------------------------
library("rucrdtw")

## ----load-data-----------------------------------------------------------
data("synthetic_control")

## ----plot-examples, fig.width=6------------------------------------------
par(mfrow = c(3,2),
    mar = c(1,1,1,1))
classes = c("Normal", "Cyclic", "Increasing", "Decreasing", "Upward shift", "Downward shift")
for (i in 1:6){
  plot(synthetic_control[i*100-99,], type = "l", xaxt = "n", yaxt = "n", ylab="", xlab = "", bty="n", main=classes[i])
}

## ----search-exact, fig.width=6-------------------------------------------
#reshape matrix into vector
long_series <- as.vector(t(synthetic_control))
#pick series 175 as query
query <- synthetic_control[175,]
#calculate starting location in the long series
query_start <- 175*ncol(synthetic_control)-59

#plot long series
plot(long_series, type = "l")
#overlay known position of query
lines(seq(query_start, by=1, length.out = 60), synthetic_control[175,], col="red")

#do the search using the vector-vector method
dtw_search <- ucrdtw_vv(long_series, query, length(query), 0.05)

#visualize the result
plot(long_series, type = "l", xlim = c(query_start-100, query_start+160))
abline(v=c(dtw_search$location, dtw_search$location+length(query)) , col="blue")
lines(seq(query_start, by=1, length.out = 60), synthetic_control[175,], col="red")

