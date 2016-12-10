TARGET=/home/box/mtexe

all: $(TARGET).c
	gcc -o $(TARGET) -std=c11 $(TARGET).c

clean:
	rm -f $(TARGET)
