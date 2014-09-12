#!/bin/sh
#
# by lintel@gmail.com, hoowa.sun@gmail.com
#
append DRIVERS "ralink"

prepare_config() {
	local num=1
	echo "We now in prepare_config()"
	local device=$1
	
	echo  "device="$device
	config_get channel $device channel
	echo  "channel="$channel
	config_get vifs "$device" vifs
	echo  "vifs="$vifs
#	config_get ssid1 private ssid
	config_get mode $device mode
	config_get WmmCapable $device WmmCapable
	config_get txpower $device txpower
	config_get ht $device ht
	config_get country $device country
	config_get hidessid $device hidessid
	
	for vif in $vifs; do
	echo "num="$num
	echo "vif="$vif
#	config_get ssid "$vif" ssid
	case $num in
	1)
		config_get ssid1 "$vif" ssid
		echo "ssid1="$ssid1
		;;
	2)
		config_get ssid2 "$vif" ssid
		echo "ssid2="$ssid2
		;;
	3)
		config_get ssid3 "$vif" ssid
		echo "ssid3="$ssid3
		;;
	4)
		config_get ssid4 "$vif" ssid
		echo "ssid4="$ssid4
		;;
	*)
		echo "to much vif"
		;;
	esac
#	$((i++))
	let num+=1
	done
	
	echo "Found "$((num-=1))"SSID Configure!"
	
	[ ! -z "$ssid2" ] 
	# By default, let the driver select 20 or 40 Mhz mode. 40Mhz
	# mode is really only available in n-only mode ($mode = 6), so
	# we only allow disabling it then.
	HT=1
    #[ "$mode" = "6" -a "$ht" = "20" ] && HT=0
    # by hoowa: below remark not like code
    [ "$ht" = "20" ] && HT=0

	# In HT40 mode, a second channel is used. If EXTCHA=0, the extra
	# channel is $channel + 4. If EXTCHA=1, the extra channel is
	# $channel - 4. If the channel is fixed to 1-4, we'll have to
	# use + 4, otherwise we can just use - 4.
	EXTCHA=0
	[ "$channel" != auto ] && [ "$channel" -lt "5" ] && EXTCHA=1

    # if channel = auto
    [ "$channel" == "auto" ] && {
        channel=11
        AutoChannelSelect=2
    }

    # hidessid set
    if [ "$hidessid" == "1" ]; then
        HideSSID=1
    else
        HideSSID=0
    fi

	cat > /tmp/RT2860.dat<<EOF
#The word of "Default" must not be removed
Default
CountryRegion=0
CountryRegionABand=7
CountryCode=${country:-US}
BssidNum=${num:-1}
SSID1=${ssid1:-DreamBox_SSID1}
SSID2=${ssid2:-DreamBox_SSID2}
SSID3=${ssid3:-DreamBox_SSID3}
SSID4=${ssid4:-DreamBox_SSID4}
SSID5=
SSID6=
SSID7=
SSID8=
WirelessMode=${mode:-9}
FixedTxMode=
TxRate=0
Channel=${channel:-11}
BasicRate=15
BeaconPeriod=100
DtimPeriod=1
TxPower=${txpower:-100}
DisableOLBC=0
BGProtection=0
TxAntenna=
RxAntenna=
TxPreamble=1
RTSThreshold=2347
FragThreshold=2346
TxBurst=1
PktAggregate=1
AutoProvisionEn=0
FreqDelta=0
TurboRate=0
WmmCapable=${WmmCapable:-0}
APAifsn=3;7;1;1
APCwmin=4;4;3;2
APCwmax=6;10;4;3
APTxop=0;0;94;47
APACM=0;0;0;0
BSSAifsn=3;7;2;2
BSSCwmin=4;4;3;2
BSSCwmax=10;10;4;3
BSSTxop=0;0;94;47
BSSACM=0;0;0;0
AckPolicy=0;0;0;0
APSDCapable=0
DLSCapable=0
NoForwarding=0
NoForwardingBTNBSSID=0
HideSSID=${HideSSID:-0}
ShortSlot=1
AutoChannelSelect=${AutoChannelSelect:-0}
IEEE8021X=0
IEEE80211H=0
CarrierDetect=0
ITxBfEn=0
PreAntSwitch=
PhyRateLimit=0
DebugFlags=0
ETxBfEnCond=0
ITxBfTimeout=0
ETxBfTimeout=0
ETxBfNoncompress=0
ETxBfIncapable=0
FineAGC=0
StreamMode=0
StreamModeMac0=
StreamModeMac1=
StreamModeMac2=
StreamModeMac3=
CSPeriod=6
RDRegion=
StationKeepAlive=0
DfsLowerLimit=0
DfsUpperLimit=0
DfsOutdoor=0
SymRoundFromCfg=0
BusyIdleFromCfg=0
DfsRssiHighFromCfg=0
DfsRssiLowFromCfg=0
DFSParamFromConfig=0
FCCParamCh0=
FCCParamCh1=
FCCParamCh2=
FCCParamCh3=
CEParamCh0=
CEParamCh1=
CEParamCh2=
CEParamCh3=
JAPParamCh0=
JAPParamCh1=
JAPParamCh2=
JAPParamCh3=
JAPW53ParamCh0=
JAPW53ParamCh1=
JAPW53ParamCh2=
JAPW53ParamCh3=
FixDfsLimit=0
LongPulseRadarTh=0
AvgRssiReq=0
DFS_R66=0
BlockCh=
GreenAP=0
PreAuth=0
AuthMode=OPEN
EncrypType=NONE
WapiPsk1=0123456789
WapiPsk2=
WapiPsk3=
WapiPsk4=
WapiPsk5=
WapiPsk6=
WapiPsk7=
WapiPsk8=
WapiPskType=0
Wapiifname=
WapiAsCertPath=
WapiUserCertPath=
WapiAsIpAddr=
WapiAsPort=
RekeyMethod=DISABLE
RekeyInterval=3600
PMKCachePeriod=10
MeshAutoLink=0
MeshAuthMode=
MeshEncrypType=
MeshDefaultkey=0
MeshWEPKEY=
MeshWPAKEY=
MeshId=
WPAPSK1=12345678
WPAPSK2=
WPAPSK3=
WPAPSK4=
WPAPSK5=
WPAPSK6=
WPAPSK7=
WPAPSK8=
DefaultKeyID=1
Key1Type=0
Key1Str1=
Key1Str2=
Key1Str3=
Key1Str4=
Key1Str5=
Key1Str6=
Key1Str7=
Key1Str8=
Key2Type=0
Key2Str1=
Key2Str2=
Key2Str3=
Key2Str4=
Key2Str5=
Key2Str6=
Key2Str7=
Key2Str8=
Key3Type=0
Key3Str1=
Key3Str2=
Key3Str3=
Key3Str4=
Key3Str5=
Key3Str6=
Key3Str7=
Key3Str8=
Key4Type=0
Key4Str1=
Key4Str2=
Key4Str3=
Key4Str4=
Key4Str5=
Key4Str6=
Key4Str7=
Key4Str8=
HSCounter=0
HT_HTC=1
HT_RDG=1
HT_LinkAdapt=0
HT_OpMode=0
HT_MpduDensity=5
HT_EXTCHA=${EXTCHA}
HT_BW=${HT}
HT_AutoBA=1
HT_BADecline=0
HT_AMSDU=0
HT_BAWinSize=64
HT_GI=1
HT_STBC=1
HT_MCS=33
HT_TxStream=2
HT_RxStream=2
HT_PROTECT=1
HT_DisallowTKIP=1
HT_BSSCoexistence=1
GreenAP=0
WscConfMode=0
WscConfStatus=1
WCNTest=0
AccessPolicy0=0
AccessControlList0=
AccessPolicy1=0
AccessControlList1=
AccessPolicy2=0
AccessControlList2=
AccessPolicy3=0
AccessControlList3=
AccessPolicy4=0
AccessControlList4=
AccessPolicy5=0
AccessControlList5=
AccessPolicy6=0
AccessControlList6=
AccessPolicy7=0
AccessControlList7=
WdsEnable=0
WdsPhyMode=
WdsEncrypType=NONE
WdsList=
Wds0Key=
Wds1Key=
Wds2Key=
Wds3Key=
RADIUS_Server=
RADIUS_Port=1812
RADIUS_Key1=
RADIUS_Key2=
RADIUS_Key3=
RADIUS_Key4=
RADIUS_Key5=
RADIUS_Key6=
RADIUS_Key7=
RADIUS_Key8=
RADIUS_Acct_Server=
RADIUS_Acct_Port=1813
RADIUS_Acct_Key=
own_ip_addr=
Ethifname=
EAPifname=
PreAuthifname=
session_timeout_interval=0
idle_timeout_interval=0
WiFiTest=0
TGnWifiTest=0
ApCliEnable=0
ApCliSsid=
ApCliBssid=
ApCliAuthMode=
ApCliEncrypType=
ApCliWPAPSK=
ApCliDefaultKeyID=0
ApCliKey1Type=0
ApCliKey1Str=
ApCliKey2Type=0
ApCliKey2Str=
ApCliKey3Type=0
ApCliKey3Str=
ApCliKey4Type=0
ApCliKey4Str=
RadioOn=1
SSID=
WPAPSK=0123456789
Key1Str=
Key2Str=
Key3Str=
Key4Str=
EOF

	echo "We now leave prepare_config()"
}

scan_ralink() {
	local device="$1"
}

disable_ralink() {
	local device="$1"
	config_get vifs "$device" vifs
	for vif in $vifs; do
		config_get ifname $vif ifname
		ifconfig $ifname down
	done

	# kill any running ap_clients
	killall ap_client || true
}

enable_ralink() {
	echo "Now we are in enable_ralink()"
	local device="$1" dmode
	echo  "device="$device
	prepare_config $device
	config_get dmode $device mode
	echo  "dmode="$dmode
	config_get vifs "$device" vifs
	echo  "vifs="$vifs
	config_get disabled "$device" disabled
	[ "$disabled" = "1" ] && return
	for vif in $vifs; do
		local ifname encryption key ssid mode
		config_get ifname $vif ifname
		config_get encryption $vif encryption
		config_get key $vif key
		config_get ssid $vif ssid
		config_get wpa_crypto $vif wpa_crypto
		config_get mode $vif mode
		config_get wps $vif wps
		config_get isolate $vif isolate
		
		echo  "vif="$vif
		echo  "ifname="$ifname
		echo  "encryption="$encryption
		echo  "key="$key
		echo  "ssid="$ssid
		# Skip this interface if no SSID was configured
		[ "$ssid" -a "$ssid" != "off" ] || continue

		[ "$mode" == "sta" ] && {
			case "$encryption" in
				WEP|wep|wep-open|wep-shared)
					# $key contains the key index,
					# so lookup the actual key to
					# use
					config_get key $vif "key$key"
					;;
			esac
			ap_client "$ssid" "$key" >/dev/null 2>/dev/null </dev/null &
		}
		[ "$mode" == "sta" ] || {
			[ "$key" = "" -a "$vif" = "private" ] && {
				logger "no key set serial"
				key="AAAAAAAAAA"
			}
			[ "$dmode" == "6" ] && wpa_crypto="aes"
			ifconfig $ifname up
			case "$encryption" in
				wpa*|psk*|WPA*|Mixed|mixed)
					local enc
					case "$encryption" in
						WPA|WPA1|wpa|wpa1|psk) enc=WPAPSK;;
						WPA2|wpa2|psk2) enc=WPA2PSK;;
						Mixed|mixed|psk+psk2) enc=WPAPSKWPA2PSK;;
					esac
					local crypto="AES"
					case "$wpa_crypto" in
						aes|AES) crypto=AES;;
						tkip+aes|TKIP+AES) crypto=TKIPAES;;
						tkip|TKIP) 
						crypto=TKIP
						echo Warring!!! TKIP not support in 802.11n 40Mhz!!!
						;;
					esac
					iwpriv $ifname set AuthMode=$enc
					iwpriv $ifname set EncrypType=$crypto
#					iwpriv $ifname set EncrypType=AES
					iwpriv $ifname set IEEE8021X=0
					iwpriv $ifname set "SSID=${ssid}"
					iwpriv $ifname set "WPAPSK=${key}"
					iwpriv $ifname set DefaultKeyID=2
					iwpriv $ifname set "SSID=${ssid}"
					if [ "$wps" == "1" ]; then
						iwpriv $ifname set WscConfMode=7
					else
						iwpriv $ifname set WscConfMode=0
					fi
					;;
				WEP|wep|wep-open|wep-shared)
					iwpriv $ifname set AuthMode=WEPAUTO
					iwpriv $ifname set EncrypType=WEP
					iwpriv $ifname set IEEE8021X=0
					for idx in 1 2 3 4; do
						config_get keyn $vif key${idx}
						[ -n "$keyn" ] && iwpriv $ifname set "Key${idx}=${keyn}"
					done
					iwpriv $ifname set DefaultKeyID=${key}
					iwpriv $ifname set "SSID=${ssid}"
					iwpriv $ifname set WscConfMode=0
					;;
				none|open)
					iwpriv $ifname set AuthMode=OPEN
					iwpriv $ifname set WscConfMode=0
					iwpriv $ifname set EncrypType=NONE
					;;
			esac
		}

		local net_cfg bridge
		net_cfg="$(find_net_config "$vif")"
		[ -z "$net_cfg" ] || {
			bridge="$(bridge_interface "$net_cfg")"
			config_set "$vif" bridge "$bridge"
			start_net "$ifname" "$net_cfg"
		}

		# If isolation is requested, disable forwarding between
		# wireless clients (both within the same BSSID and
		# between BSSID's, though the latter is probably not
		# relevant for our setup).
		
#		iwpriv $ifname set NoForwarding="${isolate:-0}"
#		iwpriv $ifname set NoForwardingBTNBSSID="${isolate:-0}"

		set_wifi_up "$vif" "$ifname"
	done
}

first_enable() {

ifconfig ra0 down

	cat > /tmp/RT2860.dat<<EOF
#The word of "Default" must not be removed
Default
CountryRegion=0
CountryRegionABand=7
CountryCode=US
BssidNum=1
SSID1=DreamBox
WirelessMode=9
FixedTxMode=
TxRate=0
Channel=11
BasicRate=15
BeaconPeriod=100
DtimPeriod=1
TxPower=100
DisableOLBC=0
BGProtection=0
TxAntenna=
RxAntenna=
TxPreamble=1
RTSThreshold=2347
FragThreshold=2346
TxBurst=1
PktAggregate=1
AutoProvisionEn=0
FreqDelta=0
TurboRate=0
WmmCapable=1
APAifsn=3;7;1;1
APCwmin=4;4;3;2
APCwmax=6;10;4;3
APTxop=0;0;94;47
APACM=0;0;0;0
BSSAifsn=3;7;2;2
BSSCwmin=4;4;3;2
BSSCwmax=10;10;4;3
BSSTxop=0;0;94;47
BSSACM=0;0;0;0
AckPolicy=0;0;0;0
APSDCapable=0
DLSCapable=0
NoForwarding=0
NoForwardingBTNBSSID=0
HideSSID=0
ShortSlot=1
AutoChannelSelect=0
IEEE8021X=0
IEEE80211H=0
CarrierDetect=0
ITxBfEn=0
PreAntSwitch=
PhyRateLimit=0
DebugFlags=0
ETxBfEnCond=0
ITxBfTimeout=0
ETxBfTimeout=0
ETxBfNoncompress=0
ETxBfIncapable=0
FineAGC=0
StreamMode=0
StreamModeMac0=
StreamModeMac1=
StreamModeMac2=
StreamModeMac3=
CSPeriod=6
RDRegion=
StationKeepAlive=0
DfsLowerLimit=0
DfsUpperLimit=0
DfsOutdoor=0
SymRoundFromCfg=0
BusyIdleFromCfg=0
DfsRssiHighFromCfg=0
DfsRssiLowFromCfg=0
DFSParamFromConfig=0
FCCParamCh0=
FCCParamCh1=
FCCParamCh2=
FCCParamCh3=
CEParamCh0=
CEParamCh1=
CEParamCh2=
CEParamCh3=
JAPParamCh0=
JAPParamCh1=
JAPParamCh2=
JAPParamCh3=
JAPW53ParamCh0=
JAPW53ParamCh1=
JAPW53ParamCh2=
JAPW53ParamCh3=
FixDfsLimit=0
LongPulseRadarTh=0
AvgRssiReq=0
DFS_R66=0
BlockCh=
GreenAP=0
PreAuth=0
AuthMode=OPEN
EncrypType=NONE
WapiPsk1=0123456789
WapiPsk2=
WapiPsk3=
WapiPsk4=
WapiPsk5=
WapiPsk6=
WapiPsk7=
WapiPsk8=
WapiPskType=0
Wapiifname=
WapiAsCertPath=
WapiUserCertPath=
WapiAsIpAddr=
WapiAsPort=
RekeyMethod=DISABLE
RekeyInterval=3600
PMKCachePeriod=10
MeshAutoLink=0
MeshAuthMode=
MeshEncrypType=
MeshDefaultkey=0
MeshWEPKEY=
MeshWPAKEY=
MeshId=
WPAPSK1=12345678
WPAPSK2=
WPAPSK3=
WPAPSK4=
WPAPSK5=
WPAPSK6=
WPAPSK7=
WPAPSK8=
DefaultKeyID=1
Key1Type=0
Key1Str1=
Key1Str2=
Key1Str3=
Key1Str4=
Key1Str5=
Key1Str6=
Key1Str7=
Key1Str8=
Key2Type=0
Key2Str1=
Key2Str2=
Key2Str3=
Key2Str4=
Key2Str5=
Key2Str6=
Key2Str7=
Key2Str8=
Key3Type=0
Key3Str1=
Key3Str2=
Key3Str3=
Key3Str4=
Key3Str5=
Key3Str6=
Key3Str7=
Key3Str8=
Key4Type=0
Key4Str1=
Key4Str2=
Key4Str3=
Key4Str4=
Key4Str5=
Key4Str6=
Key4Str7=
Key4Str8=
HSCounter=0
HT_HTC=1
HT_RDG=1
HT_LinkAdapt=0
HT_OpMode=0
HT_MpduDensity=5
HT_EXTCHA=0
HT_BW=0
HT_AutoBA=1
HT_BADecline=0
HT_AMSDU=0
HT_BAWinSize=64
HT_GI=1
HT_STBC=1
HT_MCS=33
HT_TxStream=2
HT_RxStream=2
HT_PROTECT=1
HT_DisallowTKIP=1
HT_BSSCoexistence=1
GreenAP=0
WscConfMode=0
WscConfStatus=1
WCNTest=0
AccessPolicy0=0
AccessControlList0=
AccessPolicy1=0
AccessControlList1=
AccessPolicy2=0
AccessControlList2=
AccessPolicy3=0
AccessControlList3=
AccessPolicy4=0
AccessControlList4=
AccessPolicy5=0
AccessControlList5=
AccessPolicy6=0
AccessControlList6=
AccessPolicy7=0
AccessControlList7=
WdsEnable=0
WdsPhyMode=
WdsEncrypType=NONE
WdsList=
Wds0Key=
Wds1Key=
Wds2Key=
Wds3Key=
RADIUS_Server=
RADIUS_Port=1812
RADIUS_Key1=
RADIUS_Key2=
RADIUS_Key3=
RADIUS_Key4=
RADIUS_Key5=
RADIUS_Key6=
RADIUS_Key7=
RADIUS_Key8=
RADIUS_Acct_Server=
RADIUS_Acct_Port=1813
RADIUS_Acct_Key=
own_ip_addr=
Ethifname=
EAPifname=
PreAuthifname=
session_timeout_interval=0
idle_timeout_interval=0
WiFiTest=0
TGnWifiTest=0
ApCliEnable=0
ApCliSsid=
ApCliBssid=
ApCliAuthMode=
ApCliEncrypType=
ApCliWPAPSK=
ApCliDefaultKeyID=0
ApCliKey1Type=0
ApCliKey1Str=
ApCliKey2Type=0
ApCliKey2Str=
ApCliKey3Type=0
ApCliKey3Str=
ApCliKey4Type=0
ApCliKey4Str=
RadioOn=1
SSID=
WPAPSK=0123456789
Key1Str=
Key2Str=
Key3Str=
Key4Str=
EOF

ifconfig ra0 up
}
detect_ralink() {
	local i=-1

	cd /sys/module/
	[ -d rt2860v2_ap ] || return
	
	while grep -qs "^ *ra$((++i)):" /proc/net/dev; do
		config_get type ra${i} type
		[ "$type" = ralink ] && continue

	first_enable
	
		cat <<EOF
config wifi-device  ra${i}
	option type     ralink
	option channel  11
  	
	# REMOVE THIS LINE TO ENABLE WIFI:
	# option disabled 0
	
config wifi-iface
	option device   ra${i}
	option ifname	ra0
	option network	lan
	option mode     ap
	option ssid     PandoraBox${i#0}_$(cat /sys/class/net/ra${i}/address|awk -F ":" '{print $4""$5""$6 }'| tr a-z A-Z)
	option encryption none
#	option encryption wpa2
#	option key 0123456789

EOF

	ifconfig ra0 down 
	done
	
}


