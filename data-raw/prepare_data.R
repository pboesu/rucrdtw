#prepare package data
synthetic_control <- read.table("data-raw/synthetic_control.data")
synthetic_control <- as.matrix(synthetic_control)
devtools::use_data(synthetic_control)
#write out data file as one long line
write.table(as.vector(t(synthetic_control)), "inst/extdata/col_sc.txt", row.names=FALSE, col.names=FALSE)
#write out query files
write.table(synthetic_control[1,], "inst/extdata/first_sc.txt", row.names=FALSE, col.names=FALSE)
write.table(synthetic_control[600,], "inst/extdata/last_sc.txt", row.names=FALSE, col.names=FALSE)
#pick a sequence from middle
write.table(synthetic_control[173,], "inst/extdata/mid_sc.txt", row.names=FALSE, col.names=FALSE)


#read large data
synthetic_long <- matrix(scan("data-raw/synthetic.data.gz", n=100001*10), 100001, 10, byrow=TRUE)
plot(synthetic_long[,1], type="l")
matplot(synthetic_long, type="l")
plot(as.vector(synthetic_long), type="l")
