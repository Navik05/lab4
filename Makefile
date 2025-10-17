# Компилятор
CC = g++

# Флаги компиляции
CFLAGS = -Wall -g

# Целевой исполняемый файл
TARGET = scanner

# Исходные файлы
SOURCES = main.cpp Scaner.cpp Diagram.cpp

# Объектные файлы
OBJECTS = $(SOURCES:.cpp=.o)

# Цель по умолчанию
all: $(TARGET)

# Связывание объектных файлов в исполняемый файл
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Компиляция исходных файлов в объектные
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка сгенерированных файлов
clean:
	rm -f $(OBJECTS) $(TARGET)