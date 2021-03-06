/******************************************************************************
 ** File Name:        sdram_sc7710g2.c
 ** Author:           henry.he
 ** DATE:             11/03/2013
 ** Copyright:        2013 Spreatrum, Incoporated. All Rights Reserved.
 ** Description:
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History
 **-------------------------------------------------------------------------
 ** DATE          NAME            DESCRIPTION
 ** 11/03/2013                    Create.
 ******************************************************************************/

#include <common.h>
#include <asm/arch/sci_types.h>
#include <asm/arch/arm_reg.h>
#include <asm/arch/sc_reg.h>


#include <asm/arch/sdram_sc7710g2.h>
#include <asm/arch/emc_config.h>

#ifdef   __cplusplus
extern   "C"
{
#endif



//#define SDRAM_CLK   (EMC_CLK/2)              // 96MHz
//#define SDRAM_T     (1000000000/SDRAM_CLK)   // ns
#define SDRAM_T (1000000000/EMC_CLK_400MHZ)  // ns

/*******************************************************************************
                           Variable and Array definiation
*******************************************************************************/


LOCAL CONST EMC_PARAM_T s_emc_parm = 
// arm_clk          emc_clk        ddr driver strength   dqs_drv / dat_drv / ctl_drv / clk_drv / clk_wr
//{CHIP_CLK_1000MHZ, EMC_CLK_133MHZ, DDR_DRV_STR_TR_Q,        2,		1,          0,      3,          15};    //EVB
//{CHIP_CLK_1000MHZ, EMC_CLK_400MHZ, DDR_DRV_STR_TR_Q,        2,		2,          0,      2,          15};  // PCB_V1.0.0
{CHIP_CLK_1000MHZ, EMC_CLK_400MHZ, DDR_DRV_STR_TR_Q,        2,		3,          1,      2,          12};  // 4+2 nandmcp
//{CHIP_CLK_1000MHZ, EMC_CLK_333MHZ, DDR_DRV_STR_TR_Q,        1,		1,          1,      2,          19};  // openphone



LOCAL CONST SDRAM_TIMING_PARA_T s_sdram_timing_param =
//  ms    ns   ns    		ns      ns    	ns     	  ns   	ns  	clk   clk
// tREF,tRP,tRCD, tWR/tRDL/tDPL,tRFC,	tXSR,     tRAS,	tRRD,	tMRD, tWTR(wtr is only for ddr)
#if defined(CHIP0_HYNIX_DDR_H8BCS0RJ0MCP)
{7800,   30,  30,   	15,         110, 	140,      50,  	15,  	2,    1   };
#elif defined(CHIP1_TOSHIBA_SDR_TY9000A800JFGP40)
{7800,   23,  23,  2*SDRAM_T,   	80,  	120,      50,  	15,  	2,    0   };
#elif defined(CHIP2_ST_SDR_M65K)
{7800,   24,  18,   	15,         80,  	18,       60,  	18,  	2,    0   };
#elif defined(CHIP3_SAMSUNG_SDR_K5D1G13DCA)
{7800,   24,  18,   	15,         80,  	18,       60,  	18,  	2,    0   };
#elif defined(CHIP4_SAMSUNG_SDR_K5D5657DCBD090)
{7800,   24,  18,   	15,         80,  	18,       60,  	18,  	2,    0   };
#elif defined(CHIP5_HYNIX_SDR_HYC0SEH0AF3P)
{7800,   29,  29,  2*SDRAM_T,   	80,  2*SDRAM_T,	  60,  	19,  	2,    0   };
#elif defined(CHIP6_SAMSUNG_SDR_K5D1257ACFD090)
{7800,   27,  27,   	15,         80,  	120,      50,  	18,  	2,    0   };
#elif defined(CHIP7_HYNIX_SDR_H8ACUOCEOBBR)
{7800,   27,  27,   	15,         80,  	120,      60,  	19,  	2,    0   };
#elif defined(CHIP8_HYNIX_SDR_H8ACS0EJ0MCP)
{7800,   27,  27,   	15,         80,  	120,      60,  	19,  	2,    0   };
#elif defined(CHIP9_HYNIX_SDR_H8AES0SQ0MCP)
{7800,   27,  27,   	15,         80,  	120,      60,  	19,  	2,    0   };
#elif defined(CHIP10_HYNIX_SDR_HYC0SEH0AF3P)
{7800,   27,  27,   	15,         80,  	120,      60,  	19,  	2,    0   };
#elif defined(CHIP11_MICRON_SDR_MT48H)
{7800,   20,  20,   	15,         100, 	120,      60, 2*SDRAM_T,2,    0   };
#elif defined(CHIP12_HYNIX_DDR_H9DA4GH4JJAMCR4EM)
{7800,   30,  30,   	15,         90, 	140,      50,  	15,  	2,    2   };
#elif defined(CHIP13_HYNIX_SDR_H8ACS0PH0MCP)
{7800,   27,  27,   	15,         80,  	120,      60,  	19,  	2,    0   };
#elif defined(CHIP14_HYNIX_DDR_H9DA4GH2GJAMCR)
{7800,   30,  30,   	15,         90, 	140,      50,  	15,  	2,    2   };
#elif defined(CHIP15_SAMSUNG_DDR_K522H1HACF)
{7800,   21,  15,   	12,         80, 	120,      40,  	10,  	2,    2   };
#else
{7800,   30,  30,   	15,         110, 	140,      50,  	15,  	2,    2   };
#endif

#ifndef SDRAM_AUTODETECT_SUPPORT

LOCAL CONST SDRAM_CFG_INFO_T s_sdram_config_info =
#if defined(CHIP0_HYNIX_DDR_H8BCS0RJ0MCP)
{ROW_MODE_13, COL_MODE_10, DATA_WIDTH_16, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     DDR_SDRAM,   EMC_ONE_CS_MAP_1GBIT};//for sc7702 emc 16bit, actually this ddr is 32bit
#elif defined(CHIP1_TOSHIBA_SDR_TY9000A800JFGP40)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_32, BURST_LEN_1_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     SDR_SDRAM,   EMC_ONE_CS_MAP_512MBIT};
#elif defined(CHIP2_ST_SDR_M65K)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP3_SAMSUNG_SDR_K5D1G13DCA)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP4_SAMSUNG_SDR_K5D5657DCBD090)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_16, BURST_LEN_8_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP5_HYNIX_SDR_HYC0SEH0AF3P)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_32, BURST_LEN_8_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP6_SAMSUNG_SDR_K5D1257ACFD090)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_16, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP7_HYNIX_SDR_H8ACUOCEOBBR)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_16, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP8_HYNIX_SDR_H8ACS0EJ0MCP)
{ROW_MODE_13, COL_MODE_10, DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_1GBIT};
#elif defined(CHIP9_HYNIX_SDR_H8AES0SQ0MCP)
{ROW_MODE_13, COL_MODE_10, DATA_WIDTH_16, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     SDR_SDRAM,   EMC_ONE_CS_MAP_1GBIT};
#elif defined(CHIP10_HYNIX_SDR_HYC0SEH0AF3P)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_16, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP11_MICRON_SDR_MT48H)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     SDR_SDRAM,   EMC_ONE_CS_MAP_256MBIT};
#elif defined(CHIP12_HYNIX_DDR_H9DA4GH4JJAMCR4EM)
{ROW_MODE_14, COL_MODE_10, DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     DDR_SDRAM,   EMC_ONE_CS_MAP_2GBIT};
#elif defined(CHIP13_HYNIX_SDR_H8ACS0PH0MCP)
{ROW_MODE_13, COL_MODE_9,  DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_INVALID, SDR_SDRAM,   EMC_ONE_CS_MAP_512MBIT};
#elif defined(CHIP14_HYNIX_DDR_H9DA4GH2GJAMCR)
{ROW_MODE_14, COL_MODE_10, DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     DDR_SDRAM,   EMC_ONE_CS_MAP_2GBIT};
#elif defined(CHIP15_SAMSUNG_DDR_K522H1HACF)
{ROW_MODE_14, COL_MODE_10, DATA_WIDTH_16, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     DDR_SDRAM,   EMC_ONE_CS_MAP_1GBIT};
#else
{ROW_MODE_14, COL_MODE_10, DATA_WIDTH_32, BURST_LEN_2_WORD, CAS_LATENCY_3, SDRAM_EXT_MODE_REG,     DDR_SDRAM,   EMC_ONE_CS_MAP_1GBIT};
#endif

#endif

#ifndef SDRAM_AUTODETECT_SUPPORT

LOCAL CONST SDRAM_CHIP_FEATURE_T s_sdram_feature =
#if defined(CHIP0_HYNIX_DDR_H8BCS0RJ0MCP)
{                   SDRAM_FEATURE_CL_3,                    SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_1G_BIT  };//for sc7702 emc 512m, actually this ddr is 1g
#elif defined(CHIP1_TOSHIBA_SDR_TY9000A800JFGP40)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_512M_BIT};
#elif defined(CHIP2_ST_SDR_M65K)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP3_SAMSUNG_SDR_K5D1G13DCA)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP4_SAMSUNG_SDR_K5D5657DCBD090)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP5_HYNIX_SDR_HYC0SEH0AF3P)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP6_SAMSUNG_SDR_K5D1257ACFD090)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP7_HYNIX_SDR_H8ACUOCEOBBR)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP8_HYNIX_SDR_H8ACS0EJ0MCP)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_1G_BIT  };
#elif defined(CHIP9_HYNIX_SDR_H8AES0SQ0MCP)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_1G_BIT  };
#elif defined(CHIP10_HYNIX_SDR_HYC0SEH0AF3P)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP11_MICRON_SDR_MT48H)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3,                    SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_256M_BIT};
#elif defined(CHIP12_HYNIX_DDR_H9DA4GH4JJAMCR4EM)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3,                    SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_4G_BIT  };
#elif defined(CHIP13_HYNIX_SDR_H8ACS0PH0MCP)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3, SDRAM_FEATURE_BL_1|SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_512M_BIT};
#elif defined(CHIP14_HYNIX_DDR_H9DA4GH2GJAMCR)
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3,                    SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_2G_BIT  };
#elif defined(CHIP15_SAMSUNG_DDR_K522H1HACF)
{                   SDRAM_FEATURE_CL_3,                    SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_1G_BIT  };
#else
{SDRAM_FEATURE_CL_2|SDRAM_FEATURE_CL_3,                    SDRAM_FEATURE_BL_2|SDRAM_FEATURE_BL_4|SDRAM_FEATURE_BL_8, CAP_1G_BIT  };
#endif

#endif

CONST EMC_PHY_L1_TIMING_T EMC_PHY_TIMING_L1_INFO[EMC_PHYL1_TIMING_MATRIX_MAX] =
{
//data_ie, data_oe, dqs_pst_gate, dqs_pre_gate, dqs_ie, dqs_oe
#ifdef SDR_SDRAM_SUPPORT
	{0x20,		1,		0,				0,			0,		0},		//sdram cas_latency = 2
	{0x40,		1,		0,				0,			0,		0},  	//sdram cas_latency = 3
#endif
	{0xf0,		0xe,	0x10,			0x8,		0xf0,	0xe}, 	//ddram cas_latency = 2
	{0xf0,		0xe,	0x20,			0x10,		0xf0,	0xe},  	//ddram cas_latency = 3
};

CONST EMC_PHY_L2_TIMING_T EMC_PHY_TIMING_L2_INFO[EMC_PHYL2_TIMING_MATRIX_MAX] =
{
//emc_dl3,   4,     5,     6,     7,     8,     9,     10,    11,    12,    13,    14,    15,    16,    17,    18,    19
	//{L2_PAR, L2_PAR/2,L2_PAR/2,L2_PAR/2,L2_PAR/2,L2_PAR/2,L2_PAR/2,L2_PAR/2,L2_PAR/2,L2_PAR,L2_PAR,L2_PAR,L2_PAR,L2_PAR,L2_PAR,L2_PAR,L2_PAR},	//DLL_OFF
	{12,12,12,12,	6,	6,	6,	6,	6,	6,	6,	6,	12,	12,	12,	12,	12,	12,	12,	12},	//DLL_OFF emc=400mhz
	{0x8040,0x8040,0x8040,0x8040,0x8020,0x8020,0x8020,0x8020,0x8020,0x8020,0x8020,0x8020,0x8040,0x8040,0x8040,0x8040,0x8040,0x8040,0x8040,0x8040}	//DLL_ON
};

#ifdef SDRAM_AUTODETECT_SUPPORT

CONST SDRAM_MODE_T sdram_mode_table[] =
{
    {CAP_6G_BIT, EMC_ONE_CS_MAP_4GBIT, ROW_MODE_14, ROW_MODE_15_6G,   DATA_WIDTH_32},
    {CAP_6G_BIT, EMC_ONE_CS_MAP_4GBIT, ROW_MODE_14, COL_MODE_11_6G,   DATA_WIDTH_32},
    {CAP_4G_BIT, EMC_ONE_CS_MAP_2GBIT, ROW_MODE_14, COL_MODE_10,      DATA_WIDTH_32},
    {CAP_2G_BIT, EMC_ONE_CS_MAP_2GBIT, ROW_MODE_14, COL_MODE_10,      DATA_WIDTH_32},
    {CAP_1G_BIT, EMC_ONE_CS_MAP_1GBIT, ROW_MODE_14, COL_MODE_9,       DATA_WIDTH_32},
    {CAP_1G_BIT, EMC_ONE_CS_MAP_1GBIT, ROW_MODE_13, COL_MODE_10,      DATA_WIDTH_32},

    {CAP_2G_BIT, EMC_ONE_CS_MAP_2GBIT, ROW_MODE_14, COL_MODE_11,      DATA_WIDTH_16},    
    {CAP_1G_BIT, EMC_ONE_CS_MAP_1GBIT, ROW_MODE_14, COL_MODE_10,      DATA_WIDTH_16},
    {CAP_ZERO, EMC_ONE_CS_MAP_DEFAULT, SDRAM_MIN_ROW, SDRAM_MIN_COLUMN, DATA_WIDTH_16}
};

PUBLIC SDRAM_MODE_PTR SDRAM_GetModeTable(void)
{
    return (SDRAM_MODE_PTR)sdram_mode_table;
}

SDRAM_CFG_INFO_T s_sdram_config_info = {
    SDRAM_MAX_ROW, 
    SDRAM_MAX_COLUMN, 
    DATA_WIDTH_32, 
    BURST_LEN_2_WORD, 
    CAS_LATENCY_3, 
    SDRAM_EXT_MODE_REG,     
    DDR_SDRAM,   
    EMC_ONE_CS_MAP_4GBIT
};

#endif


LOCAL EMC_CHL_INFO_T s_emc_chl_info[] =
{// emc_chl_num       axi_chl_wr_pri  axi_chl_rd_pri      ahb_chl_pri
    {EMC_AXI_ARM,       EMC_CHL_PRI_2,  EMC_CHL_PRI_2,  EMC_CHL_NONE},
    {EMC_AXI_GPU,       EMC_CHL_PRI_0,  EMC_CHL_PRI_0,  EMC_CHL_NONE},
    {EMC_AXI_DISPC,     EMC_CHL_PRI_0,  EMC_CHL_PRI_3,  EMC_CHL_NONE},
    {EMC_AHB_CP_MTX,    EMC_CHL_NONE,   EMC_CHL_NONE,   EMC_CHL_PRI_1},
    {EMC_AHB_MST_MTX,   EMC_CHL_NONE,   EMC_CHL_NONE,   EMC_CHL_PRI_3},
    {EMC_AHB_LCDC,      EMC_CHL_NONE,   EMC_CHL_NONE,   EMC_CHL_PRI_0},
    {EMC_AHB_DCAM,      EMC_CHL_NONE,   EMC_CHL_NONE,   EMC_CHL_PRI_2},
    {EMC_AHB_VSP,       EMC_CHL_NONE,   EMC_CHL_NONE,   EMC_CHL_PRI_1},
    {EMC_CHL_MAX,       EMC_CHL_NONE,   EMC_CHL_NONE,   EMC_CHL_NONE}
};


PUBLIC EMC_PARAM_PTR EMC_GetPara(void)
{
    return (EMC_PARAM_PTR)&s_emc_parm;
}

PUBLIC SDRAM_CFG_INFO_T_PTR SDRAM_GetCfg(void)
{
    return (SDRAM_CFG_INFO_T_PTR)&s_sdram_config_info;
}


PUBLIC SDRAM_TIMING_PARA_T_PTR SDRAM_GetTimingPara(void)
{
    return (SDRAM_TIMING_PARA_T_PTR)&s_sdram_timing_param;
}

#ifndef SDRAM_AUTODETECT_SUPPORT

PUBLIC SDRAM_CHIP_FEATURE_T_PTR SDRAM_GetFeature(void)
{
    return (SDRAM_CHIP_FEATURE_T_PTR)&s_sdram_feature;
}
#endif

PUBLIC EMC_CHL_INFO_PTR EMC_GetChlInfo(void)
{
    return (EMC_CHL_INFO_PTR)&s_emc_chl_info;
}

#ifdef   __cplusplus
}
#endif

