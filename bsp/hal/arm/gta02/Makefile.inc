
SRCS:=          arm/arch/locore.S \
                arm/arch/cpufunc.S \
                arm/arch/cpu.c \
                arm/arch/trap.c \
                arm/arch/context.c \
                arm/gta02/interrupt.c \
                arm/gta02/clock.c \
                arm/gta02/machdep.c \
                arm/gta02/speed.c

ifeq ($(CONFIG_MMU),y)
SRCS+=          arm/arch/mmu.c
endif
ifeq ($(DEBUG),1)
SRCS+=          arm/gta02/diag.c
endif
