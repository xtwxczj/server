CC =g++
CFLAGS=-Wall
COMPILE=$(CC) $(CFLAGS) -c
BUILD=$(CC) $(CFLAGS)
PWD=$(shell pwd)
BIN_DIR=$(PWD)/bin
OBJ_DIR=$(PWD)/obj
SRC_DIR=$(PWD)/src
HEADER_DIR=$(PWD)/header



all:$(BIN_DIR)/server
$(BIN_DIR)/server:$(OBJ_DIR)/main.o $(OBJ_DIR)/serversocket.o $(OBJ_DIR)/mysqlc.o
	$(BUILD) -o $(BIN_DIR)/server $(OBJ_DIR)/main.o $(OBJ_DIR)/serversocket.o $(OBJ_DIR)/mysqlc.o -I /usr/local/mysql/include -ljsoncpp -lmysqlclient -l curl
$(OBJ_DIR)/main.o:$(SRC_DIR)/main.cpp
	$(COMPILE) -o $(OBJ_DIR)/main.o -g -I /usr/local/mysql/include -ljsoncpp -lmysqlclient $(SRC_DIR)/main.cpp
$(OBJ_DIR)/serversocket.o:$(SRC_DIR)/serversocket.cpp $(HEADER_DIR)/serversocket.h
	$(COMPILE) -o $(OBJ_DIR)/serversocket.o -g -I /usr/local/mysql/include -ljsoncpp -lmysqlclient $(SRC_DIR)/serversocket.cpp
$(OBJ_DIR)/mysqlc.o:$(SRC_DIR)/mysqlc.cpp $(HEADER_DIR)/mysqlc.h
	$(COMPILE) -o $(OBJ_DIR)/mysqlc.o -g -I /usr/local/mysql/include -lmysqlclient $(SRC_DIR)/mysqlc.cpp
clean:
	-rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/server
