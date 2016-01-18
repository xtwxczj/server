#!/bin/sh

#-------Warning! this is a program for cloud physical node-------


#----MYSQL for cloudfee.ihep.ac.cn----
 CLOUDFEE_DB_ADDR=192.168.83.99
 export CLOUDFEE_DB_ADDR
 CLOUDFEE_DB_USER=root
 export CLOUDFEE_DB_USER
 CLOUDFEE_DB_PWD=mysql
 export CLOUDFEE_DB_PWD
 CLOUDFEE_DB=cloudfee
 export CLOUDFEE_DB

#----running required for CLOUDFEE----
LD_LIBRARY_PATH=${PWD}/lib/
export LD_LIBRARY_PATH

#----pwd for netdb----
${PWD}/server &

