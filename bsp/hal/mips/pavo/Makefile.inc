
SRCS:=	        mips/arch/locore.S \
                mips/arch/cache.S \
                mips/arch/copy.S \
                mips/arch/cpufunc.S \
                mips/arch/cpu.c \
                mips/arch/trap.c \
                mips/arch/context.c \
                mips/arch/spl.c \
                mips/pavo/interrupt.c \
                mips/pavo/clock.c \
                mips/pavo/machdep.c

ifeq ($(CONFIG_MMU),y)
SRCS+=	        mips/arch/mmu.c
endif
ifeq ($(DEBUG),1)
SRCS+=	        mips/pavo/diag.c
endif
