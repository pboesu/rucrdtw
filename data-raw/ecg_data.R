#ECG example
# This makes use of data from the
#the data acquisition requires a working copy of the WFDB software which is available at https://physionet.org/physiotools/wfdb.shtml

#download sample data
rdsamp -r mitdb/100 -p -v > 100.txt
rdsamp -r mitdb/119 -p -v > 119.txt
rdsamp -r mitdb/121 -p -v > 121.txt
rdsamp -r mitdb/230 -p -v > 230.txt
#download annotations
rdann -r mitdb/100 -a atr -v >100.ann
rdann -r mitdb/119 -a atr -v >119.ann
rdann -r mitdb/121 -a atr -v >121.ann
rdann -r mitdb/230 -a atr -v >230.ann

download_ecg <- function(ecg_id){
  system(paste("rdsamp -r mitdb/", ecg_id, " -p -v > data-raw/", ecg_id, ".txt", sep=""))
  system(paste("rdann -r mitdb/", ecg_id, " -a atr -v > data-raw/", ecg_id, ".ann", sep=""))
}

explore_ecg <- function(ecg_id, start_sec, duration_sec){
  ecg <- read.table(paste("data-raw/", ecg_id,".txt", sep=""), header =FALSE, skip=2)
  startrow <- which(ecg[,1]==start_sec)
  endrow <- which(ecg[,1]==start_sec+duration_sec)
  range <- startrow:endrow
  plot(ecg[range,1], ecg[range,2], type="l")
  lines(ecg[range,1], ecg[range,3], col='blue')
}

download_ecg(200)
explore_ecg(200, 13*60+45, 6)

ecg1 <- read.table("data-raw/106.txt", header =FALSE, skip=2)
ecg1_ann <- read.table("data-raw/119.ann", header=FALSE, skip=1,fill=TRUE, col.names = c("Time","Sample","Type","Sub","Chan","Num","Aux"))
ecg1_ann[ecg1_ann$Type=="V",]$Sample

plot(ecg1[,1], ecg1[,2], type="l")
range <- (7*60):(7*60+2000)
plot(ecg1[range,1], ecg1[range,2], type="l")
lines(ecg1[range,1], ecg1[range,3], col='blue')
range_pvc <- (503+30):(503+130)
lines(ecg1[range_pvc,1], ecg1[range_pvc,2], type="l",col="red")

pvc <- ecg1[range_pvc,2]

ecg2 <- read.table("data-raw/230.txt", header =FALSE, skip=2)
ecg2_ann <- read.table("data-raw/230.ann", header=FALSE, skip=1,fill=TRUE, col.names = c("Time","Sample","Type","Sub","Chan","Num","Aux"))
v2_ann <- ecg2_ann[ecg2_ann$Type=="V",]$Sample

hit <- ucrdtw_vv(ecg2[,2], pvc, length(pvc), dtwwindow = 0.15)

range_v2 <- (v2_ann-1000):(v2_ann+1000)
range_hit <- (hit$location-1000):(hit$location+1000)
plot(ecg2[range_v2,1], ecg2[range_v2,2], type="l")
plot(ecg2[range_hit,1], ecg2[range_hit,2], type="l")
