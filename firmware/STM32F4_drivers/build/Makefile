LIB = libSTM32F4xx_drivers.a

CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
RANLIB = arm-none-eabi-ranlib

CFLAGS = -Wall -O2 -mthumb
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard -falign-functions=16
CFLAGS += -mcpu=cortex-m4 -fsingle-precision-constant -funsigned-char -Wundef -Wsign-compare -Wunreachable-code -Wstrict-prototypes -flto
CFLAGS += -I../inc -I../../core -DARM_MATH_CM4 -DUSE_STDPERIPH_DRIVER -DSTM32F40_41xxx -D__FPU_USED=1 -DHSE_VALUE=8000000 -DSTM32F407VG

SRCS = \
misc.c \
stm32f4xx_dac.c \
stm32f4xx_dma.c \
stm32f4xx_exti.c \
stm32f4xx_gpio.c \
stm32f4xx_fsmc.c \
stm32f4xx_i2c.c \
stm32f4xx_rcc.c \
stm32f4xx_rng.c \
stm32f4xx_sdio.c \
stm32f4xx_spi.c \
stm32f4xx_syscfg.c \
stm32f4xx_tim.c 


OBJS=$(patsubst %,.bin/%,$(SRCS:.c=.o))
DEPS=$(patsubst %,.bin/%,$(SRCS:.c=.d)) 

all: $(LIB)

$(LIB): $(OBJS) Makefile
	@echo "  AR $(LIB)"
	@$(AR) -r $(LIB) $(OBJS)
	@$(RANLIB) $(LIB)

-include $(DEPS)

.bin/%.o: ../src/%.c Makefile
	@echo "  GCC $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $<
	@$(CC) $(CFLAGS) -MM $< > $*.d.tmp
	@sed -e 's|.*:|.bin/$*.o:|' < $*.d.tmp > .bin/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> .bin/$*.d
	@rm -f $*.d.tmp

clean:
	-rm -f $(OBJS)
	-rm -f $(DEPS)
	-rm -f $(LIB)
	-rm -r .bin

.PHONY: all clean
