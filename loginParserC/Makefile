CC = gcc
CFLAGS = -I/usr/include/mysql -g -fPIC -std=c99
LDFLAGS = -L/usr/lib/mysql -lmysqlclient

SOURCES = login.c
EXECUTABLE = loginSecurityLib
CONNECTOR_SRC = connector.h
CONNECTOR_LIB = connector.so
LIBCONNECTOR = libconnector.so

# 기본 타겟
all: $(EXECUTABLE) connector

# 기존 C 프로그램 빌드
$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# connector.so 생성
connector: $(CONNECTOR_SRC)
	$(CC) -shared -o $(CONNECTOR_LIB) $(CONNECTOR_SRC) $(CFLAGS) $(LDFLAGS)
	cp $(CONNECTOR_LIB) $(LIBCONNECTOR)

# 정리
clean:
	rm -f $(EXECUTABLE) $(CONNECTOR_LIB) $(LIBCONNECTOR) 
