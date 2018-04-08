
 PARAMETER VERSION = 2.2.0


BEGIN OS
 PARAMETER OS_NAME = standalone
 PARAMETER OS_VER = 6.5
 PARAMETER PROC_INSTANCE = microblaze_0
 PARAMETER stdin = axi_uartlite_0
 PARAMETER stdout = axi_uartlite_0
END


BEGIN PROCESSOR
 PARAMETER DRIVER_NAME = cpu
 PARAMETER DRIVER_VER = 2.6
 PARAMETER HW_INSTANCE = microblaze_0
END


BEGIN DRIVER
 PARAMETER DRIVER_NAME = PmodBT2
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = PmodBT2_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = PmodBT2
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = PmodBT2_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartlite
 PARAMETER DRIVER_VER = 3.2
 PARAMETER HW_INSTANCE = axi_uartlite_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = intc
 PARAMETER DRIVER_VER = 3.7
 PARAMETER HW_INSTANCE = microblaze_0_axi_intc
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.2
 PARAMETER HW_INSTANCE = microblaze_0_local_memory_dlmb_bram_if_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 4.2
 PARAMETER HW_INSTANCE = microblaze_0_local_memory_ilmb_bram_if_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = mig_7series
 PARAMETER DRIVER_VER = 2.1
 PARAMETER HW_INSTANCE = mig_7series_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = servo_con3
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = servo_con3_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = ssrelay
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = ssrelay_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = PmodMAXSONAR
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = PmodMAXSONAR_0
END


