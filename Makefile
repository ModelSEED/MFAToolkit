TOP_DIR = ../..
include $(TOP_DIR)/tools/Makefile.common
CWD= $(shell pwd)
# CC
CC = g++
# LIBS
GLPKLIB = -lglpk
LIBS = -lpthread -lz ${GLPKLIB}
# INCS
MFATK_INC = -I${CWD}/include
ENV_INC   = ${INC}
INCS = ${MFATK_INC} ${ENV_INC}
# FLAGS
OFLAGS = -DNDEBUG -DIL_STD -DILOSTRICTPOD -DNOSAFEMEM -DNOBLOCKMEM -O3
FFLAGS = -fPIC -fexceptions
# CCFLAGS
CCFLAGS = ${INCS} ${OFLAGS} ${FFLAGS}
LDFLAGS = ${GLPKLIB}

CPLEXAPI = CPLEXapiEMPTY.cpp
SRCDIR=${CWD}/src

SRCFILES = ${SRCDIR}/driver.cpp ${SRCDIR}/MFAProblem.cpp ${SRCDIR}/${CPLEXAPI} ${SRCDIR}/SCIPapi.cpp ${SRCDIR}/GLPKapi.cpp ${SRCDIR}/LINDOapiEMPTY.cpp ${SRCDIR}/SolverInterface.cpp ${SRCDIR}/Species.cpp ${SRCDIR}/Data.cpp ${SRCDIR}/InterfaceFunctions.cpp ${SRCDIR}/Identity.cpp ${SRCDIR}/Reaction.cpp ${SRCDIR}/GlobalFunctions.cpp ${SRCDIR}/AtomCPP.cpp ${SRCDIR}/UtilityFunctions.cpp ${SRCDIR}/AtomType.cpp ${SRCDIR}/Gene.cpp ${SRCDIR}/GeneInterval.cpp ${SRCDIR}/stringDB.cpp
OBJFILES = $(SRCFILES:.cpp=.o)

mfatoolkit: $(OBJFILES)
	${CC} ${CCFLAGS} -o bin/mfatoolkit $(^) ${LDFLAGS}

%.o: %.cpp
	${CC} ${CCFLAGS} -c $<; mv *.o ${SRCDIR}

deploy-mfatoolkit:
	if [ !-d ${TARGET}/etc/MFAToolkit ]; then mkdir -p ${TARGET}/etc/MFAToolkit; fi
	cp -r etc/ ${TARGET}/etc/
	cp bin/mfatoolkit ${TARGET}/bin

deploy: mfatoolkit deploy-mfatoolkit

clean:
	rm -f ${SRCDIR}/*.o bin/mfatoolkit
