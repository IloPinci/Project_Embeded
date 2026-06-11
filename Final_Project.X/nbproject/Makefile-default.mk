#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=project_main.c ../libraries/adc.c ../libraries/pwm.c ../libraries/spi.c ../libraries/uart.c ../libraries/timer.c ../libraries/scheduler.c ../libraries/tasks.c ../libraries/system.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/project_main.o ${OBJECTDIR}/_ext/754011402/adc.o ${OBJECTDIR}/_ext/754011402/pwm.o ${OBJECTDIR}/_ext/754011402/spi.o ${OBJECTDIR}/_ext/754011402/uart.o ${OBJECTDIR}/_ext/754011402/timer.o ${OBJECTDIR}/_ext/754011402/scheduler.o ${OBJECTDIR}/_ext/754011402/tasks.o ${OBJECTDIR}/_ext/754011402/system.o
POSSIBLE_DEPFILES=${OBJECTDIR}/project_main.o.d ${OBJECTDIR}/_ext/754011402/adc.o.d ${OBJECTDIR}/_ext/754011402/pwm.o.d ${OBJECTDIR}/_ext/754011402/spi.o.d ${OBJECTDIR}/_ext/754011402/uart.o.d ${OBJECTDIR}/_ext/754011402/timer.o.d ${OBJECTDIR}/_ext/754011402/scheduler.o.d ${OBJECTDIR}/_ext/754011402/tasks.o.d ${OBJECTDIR}/_ext/754011402/system.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/project_main.o ${OBJECTDIR}/_ext/754011402/adc.o ${OBJECTDIR}/_ext/754011402/pwm.o ${OBJECTDIR}/_ext/754011402/spi.o ${OBJECTDIR}/_ext/754011402/uart.o ${OBJECTDIR}/_ext/754011402/timer.o ${OBJECTDIR}/_ext/754011402/scheduler.o ${OBJECTDIR}/_ext/754011402/tasks.o ${OBJECTDIR}/_ext/754011402/system.o

# Source Files
SOURCEFILES=project_main.c ../libraries/adc.c ../libraries/pwm.c ../libraries/spi.c ../libraries/uart.c ../libraries/timer.c ../libraries/scheduler.c ../libraries/tasks.c ../libraries/system.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/c4b5474946e00747b0403261a97718a022b77cdc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/14cc7504cb3c6d6c78c0891f056c801de8a139fa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/1f8b072796e5226d490fd8bc286cfaabaaf72512 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/1b5a1b64f57dcd9b4a24434fcd887cf0224b9133 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/e50359d6b5b12acdda46cf1261b6fd7b286a8d3d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer.o: ../libraries/timer.c  .generated_files/flags/default/268886e7bc02fb9e6a702c27f8424aeabe838b08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer.c  -o ${OBJECTDIR}/_ext/754011402/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/scheduler.o: ../libraries/scheduler.c  .generated_files/flags/default/a058f24a9a2f2caf5c47d5187e9ea3ad8b824360 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/scheduler.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/scheduler.c  -o ${OBJECTDIR}/_ext/754011402/scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/scheduler.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/tasks.o: ../libraries/tasks.c  .generated_files/flags/default/110a7a56fa40b6d83a2b5dc05e3146098586dc74 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/tasks.c  -o ${OBJECTDIR}/_ext/754011402/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/tasks.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/system.o: ../libraries/system.c  .generated_files/flags/default/87e311d4effc5091d2dfb445acac2732f2e06027 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/system.c  -o ${OBJECTDIR}/_ext/754011402/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/project_main.o: project_main.c  .generated_files/flags/default/68a9ce04e3f8cccb435027d0d608b96fb980e4e5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/project_main.o.d 
	@${RM} ${OBJECTDIR}/project_main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  project_main.c  -o ${OBJECTDIR}/project_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/project_main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/adc.o: ../libraries/adc.c  .generated_files/flags/default/3abe19032851b32b22afb46b896ce7ebffc03c3e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/adc.c  -o ${OBJECTDIR}/_ext/754011402/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/adc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/pwm.o: ../libraries/pwm.c  .generated_files/flags/default/e25000256cc7cbaf64ef2ecafe1feb769cf9fbda .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/pwm.c  -o ${OBJECTDIR}/_ext/754011402/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/pwm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/spi.o: ../libraries/spi.c  .generated_files/flags/default/528c7b38ae2fd2a1bdc387b54f475399926be6f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/spi.c  -o ${OBJECTDIR}/_ext/754011402/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/uart.o: ../libraries/uart.c  .generated_files/flags/default/7fde5ad40870c49893810ad3ac6d14c7c76f1950 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/uart.c  -o ${OBJECTDIR}/_ext/754011402/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/timer.o: ../libraries/timer.c  .generated_files/flags/default/cf2d8201b7a7a20ccbe04b30deb088778510b6ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/timer.c  -o ${OBJECTDIR}/_ext/754011402/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/scheduler.o: ../libraries/scheduler.c  .generated_files/flags/default/316c75dcc78531680814d049bdc3891b03fe6ec9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/scheduler.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/scheduler.c  -o ${OBJECTDIR}/_ext/754011402/scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/scheduler.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/tasks.o: ../libraries/tasks.c  .generated_files/flags/default/838f312c996a44a5cccc3b64f900d7b4fc7e0b7a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/tasks.c  -o ${OBJECTDIR}/_ext/754011402/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/tasks.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/754011402/system.o: ../libraries/system.c  .generated_files/flags/default/e305e38f10e8154eea8bc2d2426a9c36456dc2af .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/754011402" 
	@${RM} ${OBJECTDIR}/_ext/754011402/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/754011402/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../libraries/system.c  -o ${OBJECTDIR}/_ext/754011402/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/754011402/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries"     -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../libraries" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Final_Project.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
