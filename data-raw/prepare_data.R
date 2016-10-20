#prepare package data
synthetic_control <- read.table("data-raw/synthetic_control.data")
synthetic_control <- as.matrix(synthetic_control)
devtools::use_data(synthetic_control)
#write out data file as one long line
write.table(as.vector(t(synthetic_control)), "inst/extdata/col_sc.txt", row.names=F, col.names=F)
#write out query files
write.table(synthetic_control[1,], "inst/extdata/first_sc.txt", row.names=F, col.names=F)
write.table(synthetic_control[600,], "inst/extdata/last_sc.txt", row.names=F, col.names=F)
#pick a sequence from middle
write.table(synthetic_control[173,], "inst/extdata/mid_sc.txt", row.names=F, col.names=F)

