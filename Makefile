TARGET = motiplant

CC = arm-linux-gnueabihf-gcc

INCLUDES = -Iinclude -Iinclude/application -Iinclude/display -Iinclude/sensors -Iinclude/protocol

SRCDIR = src
OBJDIR = obj

SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/application/motiplant.c \
       $(SRCDIR)/display/lcd_1602.c \
       $(SRCDIR)/sensors/aht20.c \
       $(SRCDIR)/sensors/soil_moisture_ads.c \
       $(SRCDIR)/protocol/mqtt.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CFLAGS = -Wall -Wextra -O2 $(INCLUDES)

LDFLAGS = -static -L/usr/arm-linux-gnueabihf/lib -lpaho-mqtt3c

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)/application $(OBJDIR)/display $(OBJDIR)/sensors $(OBJDIR)/protocol

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

distclean: clean
	rm -f $(TARGET)

rebuild: clean all
