#
# Copyright (C) 2011 OpenWrt.org
#

preinit_set_switch() {

	. /lib/ralink.sh

	case $(ralink_board_name) in
	hg255d |\
	hg256)
 		configEsw WLLLL
		;;
	mw305r)
 		configEsw WLLLL
		;;
	dir620 | \
	wr8305rt | \
	pbx20)
 		configEsw LLLLW
		;;
	rt-n13u)
 		configEsw LLLLW
		;;
	*)
 		configEsw WLLLL
		;;
	esac

}

configEsw()
{
	local cpu_name
	cpu_name=$(awk 'BEGIN{FS="[ \t]+:[ \t]"} /system type/ {print $2}' /proc/cpuinfo)
	
	case "$cpu_name" in
	*"Ralink RT3052")
		config_rt305x_esw $1
		;;
	*"Ralink RT3050")
		config_rt305x_esw $1
		;;
	*"Ralink RT3350")
		config_rt305x_esw $1
		;;
	*"Ralink RT3352")
		config_rt305x_esw $1
		;;
	*"Ralink RT5350")
		config_rt305x_esw $1
		;;
	*"Ralink RT3883")
		config_rt305x_esw $1
		;;
	*"MediaTek MT7620")
		config_mt76xx_esw $1
		;;
	*)
		config_rt305x_esw $1
		;;
	esac
}

config_rt305x_esw()
{

echo   "##### Ralink RT305x ESW VLAN Partition($1) #####"

	switch reg w 14 405555
	switch reg w 50 2001
	switch reg w 98 7f3f
if [ "$CONFIG_RAETH_SPECIAL_TAG" == "y" ]; then
	switch reg w e4 40043f  
else
	switch reg w e4 3f
fi

	if [ "$1" = "LLLLW" ]; then
if [ "$CONFIG_RAETH_SPECIAL_TAG" == "y" ]; then
		switch reg w 40 7007
		switch reg w 44 7007
		switch reg w 48 7008
		switch reg w 70 48444241
		switch reg w 74 50ef6050
else
		switch reg w 40 1001
		switch reg w 44 1001
		switch reg w 48 1002
		switch reg w 70 ffff506f
fi
	elif [ "$1" = "WLLLL" ]; then
if [ "$CONFIG_RAETH_SPECIAL_TAG" == "y" ]; then
		switch reg w 40 7008
		switch reg w 44 7007
		switch reg w 48 7007
		switch reg w 70 48444241
		switch reg w 74 41fe6050
else
		switch reg w 40 1002
		switch reg w 44 1001
		switch reg w 48 1001
		switch reg w 70 ffff417e
fi
	elif [ "$1" = "W1234" ]; then
		switch reg w 40 1005
		switch reg w 44 3002
		switch reg w 48 1004
		switch reg w 70 50484442
		switch reg w 74 ffffff41
	elif [ "$1" = "12345" ]; then
		switch reg w 40 2001
		switch reg w 44 4003
		switch reg w 48 1005
		switch reg w 70 7e7e7e41
		switch reg w 74 ffffff7e
	elif [ "$1" = "GW" ]; then
		switch reg w 40 1001
		switch reg w 44 1001
		switch reg w 48 2001
		switch reg w 70 ffff605f
	elif [ "$1" = "G01234" ]; then
		switch reg w 40 2001
		switch reg w 44 4003
		switch reg w 48 6005
		switch reg w 70 48444241
		switch reg w 74 ffff6050
	elif [ "$1" = "LLLLL" ]; then
		switch reg w 0xe4 0x1f
		switch reg w  0x40 0x2001
		switch reg w  0x44 0x4003
		switch reg w 0x48 0x1005
		switch reg w  0x70 0x48444241
		switch reg w  0x74 0xffffff50 
	fi

	#clear mac table if vlan configuration changed
	switch clear
}

config_mt76xx_esw()
{
echo   "##### MediaTek MT76xx ESW VLAN Partition($1) #####"

	#LAN/WAN ports as security mode
	switch reg w 2004 ff0003 #port0
	switch reg w 2104 ff0003 #port1
	switch reg w 2204 ff0003 #port2
	switch reg w 2304 ff0003 #port3
	switch reg w 2404 ff0003 #port4
	switch reg w 2504 ff0003 #port5
	#LAN/WAN ports as transparent port
	switch reg w 2010 810000c0 #port0
	switch reg w 2110 810000c0 #port1
	switch reg w 2210 810000c0 #port2
	switch reg w 2310 810000c0 #port3	
	switch reg w 2410 810000c0 #port4
	switch reg w 2510 810000c0 #port5
	#set CPU/P7 port as user port
	switch reg w 2610 81000000 #port6
	switch reg w 2710 81000000 #port7

	switch reg w 2604 20ff0003 #port6, Egress VLAN Tag Attribution=tagged
	switch reg w 2704 20ff0003 #port7, Egress VLAN Tag Attribution=tagged
	if [ "$CONFIG_RAETH_SPECIAL_TAG" == "y" ]; then
	    echo "Special Tag Enabled"
		switch reg w 2610 81000020 #port6
		#VLAN member port
		switch vlan set 0 1 1000001
		switch vlan set 1 2 0100001
		switch vlan set 2 3 0010001
		switch vlan set 3 4 0001001
		switch vlan set 4 5 0000101
		switch vlan set 5 6 0000011
		switch vlan set 6 7 0000011
	else
	    echo "Special Tag Disabled"
		switch reg w 2610 81000000 #port6
	fi

	if [ "$1" = "LLLLW" ]; then
		if [ "$CONFIG_RAETH_SPECIAL_TAG" == "y" ]; then
		#set PVID
		switch reg w 2014 10007 #port0
		switch reg w 2114 10007 #port1
		switch reg w 2214 10007 #port2
		switch reg w 2314 10007 #port3
		switch reg w 2414 10008 #port4
		switch reg w 2514 10007 #port5
		#VLAN member port
		switch vlan set 0 1 10000011
		switch vlan set 1 2 01000011
		switch vlan set 2 3 00100011
		switch vlan set 3 4 00010011
		switch vlan set 4 5 00001011
		switch vlan set 5 6 00000111
		switch vlan set 6 7 11110111
		switch vlan set 7 8 00001011
		else
		#set PVID
		switch reg w 2014 10001 #port0
		switch reg w 2114 10001 #port1
		switch reg w 2214 10001 #port2
		switch reg w 2314 10001 #port3
		switch reg w 2414 10002 #port4
		switch reg w 2514 10001 #port5
		#VLAN member port
		switch vlan set 0 1 11110111
		switch vlan set 1 2 00001011
		fi
	elif [ "$1" = "WLLLL" ]; then
		if [ "$CONFIG_RAETH_SPECIAL_TAG" == "y" ]; then
		#set PVID
		switch reg w 2014 10008 #port0
		switch reg w 2114 10007 #port1
		switch reg w 2214 10007 #port2
		switch reg w 2314 10007 #port3
		switch reg w 2414 10007 #port4
		switch reg w 2514 10007 #port5
		#VLAN member port
		switch vlan set 4 5 10000011
		switch vlan set 0 1 01000011
		switch vlan set 1 2 00100011
		switch vlan set 2 3 00010011
		switch vlan set 3 4 00001011
		switch vlan set 5 6 00000111
		switch vlan set 6 7 01111111
		switch vlan set 7 8 10000011
		else
		#set PVID
		switch reg w 2014 10002 #port0
		switch reg w 2114 10001 #port1
		switch reg w 2214 10001 #port2
		switch reg w 2314 10001 #port3
		switch reg w 2414 10001 #port4
		switch reg w 2514 10001 #port5
		#VLAN member port
		switch vlan set 0 1 01111111
		switch vlan set 1 2 10000011
		fi
	elif [ "$1" = "W1234" ]; then
		echo "W1234"
		#set PVID
		switch reg w 2014 10005 #port0
		switch reg w 2114 10001 #port1
		switch reg w 2214 10002 #port2
		switch reg w 2314 10003 #port3
		switch reg w 2414 10004 #port4
		switch reg w 2514 10006 #port5
		#VLAN member port
		switch vlan set 4 5 10000011
		switch vlan set 0 1 01000011
		switch vlan set 1 2 00100011
		switch vlan set 2 3 00010011
		switch vlan set 3 4 00001011
		switch vlan set 5 6 00000111
	    
	elif [ "$1" = "12345" ]; then
		echo "12345"
		#set PVID
		switch reg w 2014 10001 #port0
		switch reg w 2114 10002 #port1
		switch reg w 2214 10003 #port2
		switch reg w 2314 10004 #port3
		switch reg w 2414 10005 #port4
		switch reg w 2514 10006 #port5
		#VLAN member port
		switch vlan set 0 1 10000011
		switch vlan set 1 2 01000011
		switch vlan set 2 3 00100011
		switch vlan set 3 4 00010011
		switch vlan set 4 5 00001011
		switch vlan set 5 6 00000111
	elif [ "$1" = "GW" ]; then
		echo "GW"
		#set PVID
		switch reg w 2014 10001 #port0
		switch reg w 2114 10001 #port1
		switch reg w 2214 10001 #port2
		switch reg w 2314 10001 #port3
		switch reg w 2414 10001 #port4
		switch reg w 2514 10002 #port5
		#VLAN member port
		switch vlan set 0 1 11111011
		switch vlan set 1 2 00000111
	fi

	#clear mac table if vlan configuration changed
	switch clear
}

boot_hook_add preinit_main preinit_set_switch
