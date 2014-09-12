/*
 * Ralink machine types
 *
 * Copyright (C) 2010 Joonas Lahtinen <joonas.lahtinen@gmail.com>
 * Copyright (C) 2009 Gabor Juhos <juhosg@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include <asm/mips_machine.h>

enum ralink_mach_type {
	RALINK_MACH_GENERIC,
	/* RT2880 based machines */
	RALINK_MACH_F5D8235_V1,		/* Belkin F5D8235 v1 */
	RALINK_MACH_RT_N15,		/* Asus RT-N15 */
	RALINK_MACH_V11ST_FE,		/* Ralink V11ST-FE */
	RALINK_MACH_WLI_TX4_AG300N,	/* Buffalo WLI-TX4-AG300N */
	RALINK_MACH_WZR_AGL300NH,	/* Buffalo WZR-AGL300NH */

	/* RT3050 based machines */
	RALINK_MACH_3G_6200N,		/* Edimax 3G-6200N */
	RALINK_MACH_ALL0256N,		/* Allnet ALL0256N */
	RALINK_MACH_DIR_300_B1,		/* D-Link DIR-300 B1 */
	RALINK_MACH_DIR_600_B1,		/* D-Link DIR-600 B1 */
	RALINK_MACH_DIR_600_B2,		/* D-Link DIR-600 B2 */
	RALINK_MACH_DIR_615_D,		/* D-Link DIR-615 D */
	RALINK_MACH_RT_G32_B1,		/* Asus RT-G32 B1 */
	RALINK_MACH_RT_N10_PLUS,	/* Asus RT-N10+ */
	RALINK_MACH_NW718,		/* Netcore NW718 */
	RALINK_MACH_WL_330N,		/* Asus WL-330N */
	RALINK_MACH_WL_330N3G,		/* Asus WL-330N3G */
	RALINK_MACH_AP8100RT,		/* ZBT AP8100RT */

	/* RT3052 based machines */
	RALINK_MACH_ARGUS_ATP52B,	/* Argus ATP-52B */
	RALINK_MACH_BC2,		/* NexAira BC2 */
	RALINK_MACH_ESR_9753,		/* Senao / EnGenius ESR-9753*/
	RALINK_MACH_F5D8235_V2,         /* Belkin F5D8235 v2 */
	RALINK_MACH_FONERA20N,		/* La Fonera 2.0N */
	RALINK_MACH_HG255D,		/* HuaWei HG255D */
	RALINK_MACH_HG256,		/* HuaWei HG256 */
	RALINK_MACH_RT_N13U,		/* ASUS RT-N13U */
	RALINK_MACH_FREESTATION5,	/* ARC Freestation5 */
	RALINK_MACH_HW550_3G,		/* Aztech HW550-3G */
	RALINK_MACH_MOFI3500_3GN,	/* MoFi Network MOFI3500-3GN */
	RALINK_MACH_NBG_419N,		/* ZyXEL NBG-419N */
	RALINK_MACH_OMNI_EMB,         	/* Omnima MiniEMBWiFi */
	RALINK_MACH_PSR_680W,		/* Petatel PSR-680W Wireless 3G Router*/
	RALINK_MACH_PWH2004,		/* Prolink 2004H / Abocom 5205 */
	RALINK_MACH_SL_R7205,		/* Skylink SL-R7205 Wireless 3G Router*/
	RALINK_MACH_V22RW_2X2,		/* Ralink AP-RT3052-V22RW-2X2 */
	RALINK_MACH_W306R_V20,	        /* Tenda W306R_V20 */
	RALINK_MACH_W502U,		/* ALFA Networks W502U */
	RALINK_MACH_WCR150GN,		/* Sparklan WCR-150GN */
	RALINK_MACH_WHR_G300N,		/* Buffalo WHR-G300N */
	RALINK_MACH_WL341V3,		/* Sitecom WL-341 v3 */
	RALINK_MACH_WL351,		/* Sitecom WL-351 v1 002 */
	RALINK_MACH_WR512_3GN,		/* SH-WR512NU/WS-WR512N1-like 3GN*/
	RALINK_MACH_WR6202,		/* Accton WR6202 */
	RALINK_MACH_XDXRN502J,          /* unknown XDX-RN502J */
	RALINK_MACH_UR_336UN,		/* UPVEL ROUTER */
	
	/* RT3352 based machines */
	RALINK_MACH_WAP120NF,

	/* RT3662 based machines */
	RALINK_MACH_RT_N56U,		/* Asus RT-N56U */

	/* RT5350 based machines */
	RALINK_MACH_FWR601,
	RALINK_MACH_AP8101RT,
	
	/* MT7620 based machines */
	RALINK_MACH_MW305R,
	RALINK_MACH_PBX20,
	RALINK_MACH_CREDO,
	RALINK_MACH_DIR_620_B2,
	RALINK_MACH_WR8305RT,
};
