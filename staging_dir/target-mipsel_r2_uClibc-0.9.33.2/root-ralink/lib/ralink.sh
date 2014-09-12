#!/bin/sh
#
# Copyright (C) 2010 OpenWrt.org
#


ralink_board_name() {
	local machine
	local name

	machine=$(awk 'BEGIN{FS="[ \t]+:[ \t]"} /machine/ {print $2}' /proc/cpuinfo)

	case "$machine" in
	*"Edimax 3g-6200n")
		name="3g-6200n"
		;;
	*"Allnet ALL0256N")
		name="all0256n"
		;;
	*"ARC FreeStation5")
		name="freestation5"
		;;
	*"Argus ATP-52B")
		name="argus-atp52b"
		;;
	*"Asus WL-330N")
		name="wl-330n"
		;;
	*"Asus WL-330N3G")
		name="wl-330n3g"
		;;
	*"Aztech HW550-3G")
		name="hw550-3g"
		;;
	*"DIR-300 B1")
		name="dir-300-b1"
		;;
	*"DIR-600 B1")
		name="dir-600-b1"
		;;
	*"DIR-600 B2")
		name="dir-600-b2"
		;;
	*"ESR-9753")
		name="esr-9753"
		;;
	*"F5D8235 v1")
		name="f5d8235-v1"
		;;
	*"F5D8235 v2")
		name="f5d8235-v2"
		;;
	*"La Fonera 2.0N")
		name="fonera20n"
		;;
	*"ASUS RT-N13")
		name="rt-n13"
		;;
	*"HuaWei HG255D")
		name="hg255d"
		;;
	*"Mercury MW305R")
		name="mw305r"
		;;
	*"SpeedyTel PBX20")
		name="pbx20"
		;;
	*"D-Link DIR-620 B2")
		name="dir620"
		;;
	*"ZBT WR8305RT")
		name="wr8305rt"
		;;
	*"HuaWei HG256")
		name="hg256"
		;;
	*"ZyXEL WAP120NF")
		name="wap120nf"
		;;
	*"ZBT AP8100RT")
		name="ap8100rt"
		;;
	*"MoFi Network MOFI3500-3GN")
		name="mofi3500-3gn"
		;;
	*"NBG-419N")
		name="nbg-419n"
		;;
	*"NexAira BC2")
		name="bc2"
		;;
	*"NW718")
		name="nw718"
		;;
	*"Omnima MiniEMBWiFi")
		name="omni-emb"
		;;
	*"Petatel PSR-680W"*)
		name="psr-680w"
		;;
	*"PWH2004")
		name="pwh2004"
		;;
	*"RT-G32 B1")
		name="rt-g32-b1"
		;;
	*"RT-N10+")
		name="rt-n10-plus"
		;;
	*"RT-N15")
		name="rt-n15"
		;;
	*"RT-N56U")
		name="rt-n56u"
		;;
	*"Skyline SL-R7205"*)
		name="sl-r7205"
		;;
	*"Sparklan WCR-150GN")
		name="wcr-150gn"
		;;
	*"V22RW-2X2")
		name="v22rw-2x2"
		;;
	*"W502U")
		name="w502u"
		;;
	*"WHR-G300N")
		name="whr-g300n"
		;;
	*"Sitecom WL-341 v3")
		name="wl341v3"
		;;
	*"Sitecom WL-351 v1 002")
		name="wl-351"
		;;
	*"Tenda W306R V2.0")
		name="w306r-v20"
		;;
	*"Ralink V11ST-FE")
		name="v11st-fe"
		;;
	*"WLI-TX4-AG300N")
		name="wli-tx4-ag300n"
		;;
	*"WZR-AGL300NH")
		name="wzr-agl300nh"
		;;
	*"WR512-3GN-like router")
		name="wr512-3gn"
		;;
	*"UR-336UN Wireless N router")
		name="ur-336un"
		;;
	*"AWB WR6202")
		name="wr6202"
		;;
	*"XDX RN502J")
		name="xdxrn502j"
		;;
	*)
		name="generic"
		;;
	esac

	echo $name
}
