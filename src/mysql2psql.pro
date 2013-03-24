/*QT -= \
    core \
    gui*/

QMAKE_CXXFLAGS += \
    -std=c++0x

SOURCES += \
    main.cpp \
    system/log.cpp \
    system/config.cpp \
    types/patterns/singleton.cpp \
    types/basic/basic.cpp \
    types/types.cpp \
    types/basic/string.cpp \
    database/connection.cpp \
    application/application.cpp \
    entity/mysql_connection.cpp \
    entity/psql_connection.cpp \
    database/query_result.cpp \
    entity/mysql_query_result.cpp \
    entity/psql_query_result.cpp \
    types/threading/task.cpp \
    types/threading/system.cpp \
    types/basic/map.cpp \
    types/composite/fstream.cpp \
    types/basic/vector.cpp \
    application/migration_task.cpp

HEADERS += \
    system/log.h \
    system/config.h \
    types/patterns/singleton.h \
    database/connection.h \
    types/types.h \
    types/basic/basic.h \
    types/basic/string.h \
    entity/mysql_connection.h \
    application/application.h \
    entity/psql_connection.h \
    database/query_result.h \
    entity/mysql_query_result.h \
    entity/psql_query_result.h \
    types/threading/task.h \
    types/threading/system.h \
    types/basic/map.h \
    types/composite/fstream.h \
    types/basic/vector.h \
    application/migration_task.h

LIBS += \
    -L/usr/lib/x86_64-linux-gnu -lmysqlclient \
    -L/usr/lib -lpq

INCLUDEPATH += \
    /usr/include/postgresql \
    /usr/include/postgresql/9.2/server

OTHER_FILES += \
    ../build/log.txt \
    ../build/mysql2psql.conf \
    ../build/log.txt
