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
FINAL_IMAGE=${DISTDIR}/sched.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/sched.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=main.c ../../../../../MPLABXProjects/Lib.X/scheduler.c ../../../../../MPLABXProjects/Lib.X/adc.c ../../../../../MPLABXProjects/Lib.X/timer.c ../../../../../MPLABXProjects/Lib.X/task.c ../../../../../MPLABXProjects/Lib.X/pwm.c ../../../../../MPLABXProjects/Lib.X/spi.c ../../../../../MPLABXProjects/Lib.X/uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/896853193/scheduler.o ${OBJECTDIR}/_ext/896853193/adc.o ${OBJECTDIR}/_ext/896853193/timer.o ${OBJECTDIR}/_ext/896853193/task.o ${OBJECTDIR}/_ext/896853193/pwm.o ${OBJECTDIR}/_ext/896853193/spi.o ${OBJECTDIR}/_ext/896853193/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/_ext/896853193/scheduler.o.d ${OBJECTDIR}/_ext/896853193/adc.o.d ${OBJECTDIR}/_ext/896853193/timer.o.d ${OBJECTDIR}/_ext/896853193/task.o.d ${OBJECTDIR}/_ext/896853193/pwm.o.d ${OBJECTDIR}/_ext/896853193/spi.o.d ${OBJECTDIR}/_ext/896853193/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/896853193/scheduler.o ${OBJECTDIR}/_ext/896853193/adc.o ${OBJECTDIR}/_ext/896853193/timer.o ${OBJECTDIR}/_ext/896853193/task.o ${OBJECTDIR}/_ext/896853193/pwm.o ${OBJECTDIR}/_ext/896853193/spi.o ${OBJECTDIR}/_ext/896853193/uart.o

# Source Files
SOURCEFILES=main.c ../../../../../MPLABXProjects/Lib.X/scheduler.c ../../../../../MPLABXProjects/Lib.X/adc.c ../../../../../MPLABXProjects/Lib.X/timer.c ../../../../../MPLABXProjects/Lib.X/task.c ../../../../../MPLABXProjects/Lib.X/pwm.c ../../../../../MPLABXProjects/Lib.X/spi.c ../../../../../MPLABXProjects/Lib.X/uart.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/sched.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/f0540bf3b68d24d0941b08f2ea528ba4f491f3ab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/scheduler.o: ../../../../../MPLABXProjects/Lib.X/scheduler.c  .generated_files/flags/default/9795898de5286bec13ecbd0c5ea610068eb2891d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/scheduler.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/scheduler.c  -o ${OBJECTDIR}/_ext/896853193/scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/scheduler.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/adc.o: ../../../../../MPLABXProjects/Lib.X/adc.c  .generated_files/flags/default/5b62d0ba56614b853a547543478c76974b114a7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/adc.c  -o ${OBJECTDIR}/_ext/896853193/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/adc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/timer.o: ../../../../../MPLABXProjects/Lib.X/timer.c  .generated_files/flags/default/fd92d2cf6c00b8155f3700b32791c0830f85560f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/timer.c  -o ${OBJECTDIR}/_ext/896853193/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/timer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/task.o: ../../../../../MPLABXProjects/Lib.X/task.c  .generated_files/flags/default/d7f7f2971e95417eb48c6c462351707716466256 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/task.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/task.c  -o ${OBJECTDIR}/_ext/896853193/task.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/task.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/pwm.o: ../../../../../MPLABXProjects/Lib.X/pwm.c  .generated_files/flags/default/8ce40d7c3201438c4013b35af9c3ab74039aa2cb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/pwm.c  -o ${OBJECTDIR}/_ext/896853193/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/pwm.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/spi.o: ../../../../../MPLABXProjects/Lib.X/spi.c  .generated_files/flags/default/70e20449baecf9139079a124e6b65ca691e012f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/spi.c  -o ${OBJECTDIR}/_ext/896853193/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/spi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/uart.o: ../../../../../MPLABXProjects/Lib.X/uart.c  .generated_files/flags/default/1715b6da0e8ee4fbdf8420828175c02498a8749b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/uart.c  -o ${OBJECTDIR}/_ext/896853193/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/77e9512bb1d550cbc2b6e2741f7c568c7a552c0b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/scheduler.o: ../../../../../MPLABXProjects/Lib.X/scheduler.c  .generated_files/flags/default/4b273e471e87cdb07c73931aeee0e2f15b76275f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/scheduler.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/scheduler.c  -o ${OBJECTDIR}/_ext/896853193/scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/scheduler.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/adc.o: ../../../../../MPLABXProjects/Lib.X/adc.c  .generated_files/flags/default/6a99993a79124e51c77123560219ca8dfb57fe28 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/adc.c  -o ${OBJECTDIR}/_ext/896853193/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/adc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/timer.o: ../../../../../MPLABXProjects/Lib.X/timer.c  .generated_files/flags/default/dea1047cd01f5b1326dd4429941e15e78310382f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/timer.c  -o ${OBJECTDIR}/_ext/896853193/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/task.o: ../../../../../MPLABXProjects/Lib.X/task.c  .generated_files/flags/default/c91a77821fd8b5fd8204295d86d9f41ed80eb63f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/task.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/task.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/task.c  -o ${OBJECTDIR}/_ext/896853193/task.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/task.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/pwm.o: ../../../../../MPLABXProjects/Lib.X/pwm.c  .generated_files/flags/default/af01e63b889cf055f2a3bd6b68270949136371ab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/pwm.c  -o ${OBJECTDIR}/_ext/896853193/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/pwm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/spi.o: ../../../../../MPLABXProjects/Lib.X/spi.c  .generated_files/flags/default/a0dd09c8ab32e5f97ad0bca76150153c9182d5ad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/spi.c  -o ${OBJECTDIR}/_ext/896853193/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/896853193/uart.o: ../../../../../MPLABXProjects/Lib.X/uart.c  .generated_files/flags/default/c3da18ae89f6a8f8cd3b599ea25fde2669b18830 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/896853193" 
	@${RM} ${OBJECTDIR}/_ext/896853193/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/896853193/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../MPLABXProjects/Lib.X/uart.c  -o ${OBJECTDIR}/_ext/896853193/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/896853193/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
${DISTDIR}/sched.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/sched.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X"     -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/sched.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/sched.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../../../../../MPLABXProjects/Lib.X" -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/sched.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
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
