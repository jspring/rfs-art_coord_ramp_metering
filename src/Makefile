# Makefile
#
# Copyright (c) 2013   Regents of the University of California
#
include $(CAPATH_MK_DEFS)

METER_LIB = $(OBJDIR)/libmeter_lib.a
METER_LIB_OBJS = $(OBJDIR)/meter_lib.o

PATH_LDFLAGS += -L./lnx

PATH_CFLAGS += -I$(ATSC_INC_DIR) -pg -Wall
PATH_OBJS += $(OBJDIR)/read_data.o
PATH_OBJS += $(OBJDIR)/interface.o
PATH_OBJS += $(OBJDIR)/control_compute.o
PATH_OBJS += $(OBJDIR)/init.o
PATH_OBJS += $(OBJDIR)/temp_global.o
PATH_OBJS += $(OBJDIR)/variables2.o
PATH_OBJS += $(OBJDIR)/variables.o
PATH_OBJS += $(OBJDIR)/ac_rm_table.o
PATH_OBJS += $(OBJDIR)/meter_lib.o

PATH_LDFLAGS += -L$(ATSC_LIB_DIR)
PATH_LIBS += -lset_min_max_green_lib

EXEC = $(OBJDIR)/ac_rm_algo $(OBJDIR)/wrfiles_ac_rm $(OBJDIR)/clt_vars

all: 	$(OBJDIR) $(PATH_OBJS) $(EXEC)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	\rm -f $(OBJDIR)/*.[oa] $(EXEC)
