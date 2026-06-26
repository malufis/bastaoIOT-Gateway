# A76XX Series AT Command Manual V1.09



## Página 1

A76XXSeries_
ATCommandManual
SIMComWirelessSolutionsLimited
SIMCom HeadquartersBuilding,Building 3,No.289 Linhong
Road,ChangningDistrict,Shanghai P.R.China
Tel:86-21-31575100
support@simcom.com
www.simcom.com
LTEModule



## Página 2

A76XX Series_AT Command Manual_V1.09
www.simcom.com 1/652
DocumentTitle: A76XXSeries_ATCommand Manual
Version: 1.09
Date: 2023.04.27
Status: Released
GENERALNOTES
SIMCOM OFFERS THIS INFORMATION AS A SERVICE TO ITS CUSTOMERS, TO SUPPORT
APPLICATION AND ENGINEERING EFFORTS THAT USE THE PRODUCTS DESIGNED BY SIMCOM.
THE INFORMATION PROVIDED IS BASED UPON REQUIREMENTS SPECIFICALLY PROVIDED TO
SIMCOM BY THE CUSTOMERS. SIMCOM HAS NOT UNDERTAKEN ANY INDEPENDENT SEARCH
FORADDITIONALRELEVANTINFORMATION,INCLUDINGANYINFORMATIONTHATMAYBEINTHE
CUSTOMER’SPOSSESSION.FURTHERMORE,SYSTEMVALIDATIONOFTHISPRODUCTDESIGNED
BY SIMCOM WITHIN A LARGER ELECTRONIC SYSTEM REMAINS THE RESPONSIBILITY OF THE
CUSTOMER OR THE CUSTOMER’S SYSTEM INTEGRATOR. ALL SPECIFICATIONS SUPPLIED
HEREINARESUBJECTTOCHANGE.
COPYRIGHT
THISDOCUMENTCONTAINSPROPRIETARYTECHNICALINFORMATIONWHICHISTHEPROPERTY
OFSIMCOMWIRELESSSOLUTIONSLIMITEDCOPYING,TOOTHERSANDUSINGTHISDOCUMENT,
ARE FORBIDDEN WITHOUT EXPRESS AUTHORITY BY SIMCOM. OFFENDERS ARE LIABLE TO THE
PAYMENT OF INDEMNIFICATIONS. ALL RIGHTS RESERVED BY SIMCOM IN THE PROPRIETARY
TECHNICAL INFORMATION,INCLUDING BUT NOT LIMITED TO REGISTRATION GRANTING OF A
PATENT,AUTILITYMODELORDESIGN.ALLSPECIFICATIONSUPPLIEDHEREINARESUBJECTTO
CHANGE WITHOUTNOTICEATANY TIME.
SIMComWireless Solutions Limited
SIMCom HeadquartersBuilding,Building 3,No.289 Linhong Road, ChangningDistrict,ShanghaiP.R.
China
Tel:+8621 31575100
Email:simcom@simcom.com
Formore information, pleasevisit:
https://www.simcom.com/download/list-863-en.html
Fortechnical support,ortoreportdocumentationerrors, please visit:
https://www.simcom.com/ask/oremailto: support@simcom.com
Copyright©2023 SIMCom Wireless SolutionsLimitedAllRights Reserved.


## Página 3

A76XX Series_AT Command Manual_V1.09
www.simcom.com 2/652
VersionHistory
Version Date Chapter Description
V1.00 2019.5.15 Newversion
V1.01
2019.5.20 5.2.9AT+CGEQREQ
5.2.10AT+CGEQMIN Modifythesecommands
2019.5.22
9.2.9AT+CNMI
14.2.1AT+CHTPSERV
14.2.3AT+CNTP
Modifythesecommands
2019.6.5 2.2.15AT&W
2.2.16ATZ Modifythesecommands
2019.6.17
2.2.10AT&V
9.2.4AT+CSCA
9.2.6AT+CSMP
8.2.4AT+CPBW
8.2.5AT+CNUM
5.2.14AT+CGCLASS
14.2.1AT+CHTPSERV
14.2.3AT+CNTP
Modifythesecommands
2019.7.8 8.2.5AT+CNUM
14.2.3AT+CNTP Modifythesecommands
2019.8.9 5.2.17AT+CPING Addthis command
2019.8.9
2.2.8ATI
2.2.14ATX
2.2.18AT+CGMM
2.2.19AT+CGMR
4.2.2AT+COPS
5.2.4AT+CGACT
5.2.5AT+CGDCONT
5.2.7AT+CGTFT
5.2.9AT+CGEQREQ
5.2.11AT+CGEQMIN
12.2.1AT+FSCD
13.2.1AT+CFTRANRX
13.2.2AT+CFTRANTX
Modifythesecommands
2019.12.11 4.2.2AT+COPS Modifythiscommand
2019.12.25 8.2.5AT+CNUM Modifythiscommand
2020.1.7 4.2.7AT+CNMP Modifythiscommand
2020.3.6 5.2.11AT+CGEQMIN Modifythiscommand


## Página 4

A76XX Series_AT Command Manual_V1.09
www.simcom.com 3/652
2020.3.11 4.2.7AT+CNMP Modifythiscommand
2020.3.23
15ATCommands forTCPIP
16ATCommands forHTTP(S)
17ATCommands forFTP(S)
18ATCommands forMQTT(S)
19ATCommands forSSL
20ATCommands forTTS
21ATCommands forAMR
22ATCommands forSFOTA
Add Chapter
15/16/17/18/19/21/22/23 and
reconstructthe chapters
2020.4.1 7ATCommandsforCallControl Add Chapter 7 and reconstruct
the chapters
2020.4.10 8.2.5AT+CNUM Modifythiscommand
2020.4.20 5.2.7AT+CGTFT Modifythiscommand
2020.4.20 5.2.1AT+CGREG Modifythiscommand
2020.4.20
4.2.2AT+COPS
4.2.3AT+CUSD
4.2.4AT+CSSN
4.2.7AT+CNMP
4.2.10AT+CNSMOD
4.2.11AT+CTZU
4.2.12AT+CTZR
Modifythesecommands
2020.4.26
18.2.3AT+CMQTTACCQ
18.2.8AT+CMQTTCONNECT
18.2.9AT+CMQTTDISC
18.2.10AT+CMQTTTOPIC
18.2.11AT+CMQTTPAYLOAD
18.2.14AT+CMQTTSUB
18.2.16AT+CMQTTUNSUB
18.2.17AT+CMQTTCFG
Modifythesecommands
2020.4.26
12.2.5AT+FSDEL
12.2.6AT+FSRENAME
12.2.8AT+FSMEM
12.2.9AT+FSCOPY
Modifythesecommands
2020.4.26 10.2.10AT+CFGRI Modifythiscommand
2020.4.26 10.2.11AT+CURCD Modifythiscommand
2020.4.26 21.2.1AT+CCMXPLAY Modifythiscommand
2020.4.27 3.2.4AT+CSQDELTA Modifythiscommand
2020.4.16
12ATCommands forFileSystem
13AT Commands for File
Transmission
AddNotes tothese chapters
2020.4.29
10.2.10AT+CFGRI
17.2.2AT+CFTPSSTOP
17.2.9AT+CFTPSPWD
17.2.14AT+CFTPSPUT
17.2.15AT+CFTPSSINGLEIP
Modifythesecommands


## Página 5

A76XX Series_AT Command Manual_V1.09
www.simcom.com 4/652
17.2.17AT+FTPSTYPE
2020.5.6 21.2.1AT+CCMXPLAY
21.2.2AT+CCMXSTOP Modifythesecommands
2020.5.8 12.2 Detailed Description of AT
CommandsforFileSystem Adddescriptionto thissection
2020.5.11
3.2.1AT+CFUN
3.2.3AT+AUTOCSQ
3.2 4AT+CSQDELTA
3.3.10AT+CCLK
3.3.11AT+CMEE
4.2.7AT+CNMP
4.2.9AT+CPSI
5.2.2AT+CEREG
5.2.3AT+CGATT
5.2.4AT+CGACT
5.2.5AT+CGDCONT
5.2.6AT+CGDSCONT
5.2.8AT+CGQREQ
5.2.9AT+CGEQREQ
5.2.10AT+CGQMIN
5.2.11AT+CGEQMIN
5.2.15AT+CGEREP
5.2.16AT+CGAUTH
8.2.5AT+CNUM
Modifythesecommands
2020.5.12
2.2.5ATS0
2.2.21AT+CSCS
6.2.4AT+CPWD
6.2.7AT+CRSM
19.2.2AT+CCERTDOWN
19.2.12AT+CCHOPEN
Modifythesecommands
2020.5.13
14.2.1AT+CHTPSERV
14.2.2AT+CHTPUPDATE
14.2.3AT+CNTP
18.2.1AT+CMQTTSTART
Modifythesecommands
2020.5.18
2.2.5ATS0
9.2.1AT+CSMS
9.2.2AT+CPMS
9.2.3AT+CMGF
9.2.5AT+CSCB
9.2.7AT+CSDH
9.2.9AT+CNMI
9.2.10AT+CGSMS
9.2.11AT+CMGL
9.2.12AT+CMGR
9.2.13AT+CMGS
Modifythesecommands


## Página 6

A76XX Series_AT Command Manual_V1.09
www.simcom.com 5/652
9.2.14AT+CMSS
9.2.15AT+CMGW
9.2.16AT+CMGD
9.2.18AT+CMVP
9.2.19AT+CMGRD
2020.5.19
8.2.1AT+CPBS
8.2.2AT+CPBR
8.2.4AT+CPBW
Modifythesecommands
.2020.5.19 6.2.11AT+UIMHOTSWAPON
6.2.11AT+UIMHOTSWAPLEVEL Addthese commands
2020.5.20 18ATCommands forMQTT(S) Modifythischapter
2020.5.20
3.2.11AT+CMEE
4.2.4AT+CSSN
4.2.6AT+COPN
5.2.7AT+CGTFT
5.2.15AT+CGEREP
5.2.16AT+CGAUTH
5.2.17AT+CGPIN
8.2.5AT+CNUM
17.2.16AT+CFTPSSIZE
Modifythesecommands
2020.5.22
15.2.1AT+NETOPEN
15.2.3AT+CIPOPEN
15.2.5AT+CIPRXGET
15.2.8AT+CIPHEAD
15.2.9AT+CIPSRIP
15.2.10AT+CIPMODE
15.2.11AT+CIPSENDMODE
Modifythesecommands
2020.5.26 21.ATCommands forAudio Modifythischapter
2020.5.26 22.2.3AT+CREC Addthis command
2020.5.27 17.2.16AT+CFTPSSIZE Modifythiscommand
2020.5.28 5.2.1AT+CGREG
5.2.2AT+CEREG Modifythesecommands
2020.5.29
6.2.2AT+CPIN
6.2.3AT+CLCK
6.2.4AT+CPWD
Modifythesecommands
2020.5.29
12.2.9AT+FSCOPY
13.2.1AT+CFTRANRX
13.2.2AT+CFTRANTX
Modifythesecommands
2020.5.29 18.2.9AT+CMQTTDISC Modifythiscommand
2020.6.1
3.2.1AT+CFUN
3.2.2AT+CSQ
3.2.3AT+AUTOCSQ
4.2.1AT+CREG
4.2.3AT+CUSD
4.2.5AT+CPOL
Modifythesecommands


## Página 7

A76XX Series_AT Command Manual_V1.09
www.simcom.com 6/652
4.2.11AT+CTZU
4.2.12AT+CTZR
2020.6.1
9.2.1AT+CSMS
9.2.5AT+CSCB
9.2.7AT+CSDH
9.2.8AT+CNMA
9.2.10AT+CGSMS
9.2.11AT+CMGL
9.2.13AT+CMGS
9.2.16AT+CMGD
9.2.18AT+CMVP
Modifythesecommands
2020.6.2 21.2.1AT+CCMXPLAY Modifythiscommand
2020.6.3
11.2.1AT+CVALARM
11.2.2AT+CVAUXS
11.2.3AT+CVAUXV
11.2.6AT+CMTE
11.2.7AT+CPMVT
11.2.8AT+CRIIC
11.2.10AT+CBC
Modifythesecommands
2020.6.3
4.2.11AT+CTZU
5.2.5AT+CGDCONT
5.2.17AT+CPING
8.2.2AT+CPBR
Modifythesecommands
2020.6.3 16.2.8AT+HTTPPOSTFILE
16.2.9AT+HTTPREADFILE Modifythesecommands
2020.6.4
12.2.3AT+FSRMDIR
12.2.4AT+FSLS
12.2.8AT+FSMEM
12.2.9AT+FSCOPY
13.2.1AT+CFTRANRX
Modifythesecommands
2020.6.10
5.2.13AT+CGPADDR
15.2.3AT+CIPOPEN
15.2.6AT+CIPCLOSE
15.2.9AT+CIPSRI
Modifythesecommands
2020.6.19 All
V1.02
2020.6.28 21.2.3AT+CREC Modifythiscommand
2020.6.28 17.2.14AT+CFTPSPUT Modifythiscommand
2020.6.28 23.3Summary ofCME ERRORcodes Modifythissection
2020.6.29 17.2.16AT+CFTPSSIZE Modifythiscommand
2020.6.29 19.2.7AT+CCHSTART Modifythiscommand
2020.7.1 21.2.3AT+CREC Modifythiscommand
2020.7.2 9.1Overview of AT Commands for
SMS Modifythissection
2020.7.6 16.2.8AT+HTTPPOSTFILE Modifythiscommand


## Página 8

A76XX Series_AT Command Manual_V1.09
www.simcom.com 7/652
2020.7.10 23ATCommands forGPS Addthis chapter
2020.7.10
10.2.2AT&C
7.2.10AT+CLIP
7.2.12AT+COLP
2.2.11ATV
2.2.13ATQ
2.2.14ATX
2.2.21AT+CSCS
Modifythesecommands
2020.7.15 21.2.1AT+CCMXPLAY
21.2.3AT+CREC Modifythesecommands
2020.7.21 16.2.8AT+HTTPREAD Modifythiscommand
2020.8.11
12.2.1AT+FSCD
12.2.2AT+FSMKDIR
12.2.3AT+FSRMDIR
13.2.1AT+CFTRANRX
13.2.2AT+CFTRANTX
3.2.10AT+CCLK
5.2.2AT+CEREG
5.2.4AT+CGACT
4.2.6AT+COPN
4.2.4AT+CSSN
18.2.7AT+CMQTTWILLMSG
5.2.16AT+CGAUTH
3.2.2AT+CSQ
3.2.3AT+AUTOCSQ
3.2.5AT+CPOF
3.2.6AT+CRESET
3.2.7AT+CACM
15.2.7AT+IPADDR
15.2.10AT+CIPMODE
3.2.12AT+CPAS
15.2.2AT+NETCLOSE
15.2.3AT+CIPOPEN
15.2.4AT+CIPSEND
15.2.6AT+CIPCLOSE
15.2.15AT+SERVERSTOP
15.2.16AT+CIPACK
18.2.9AT+CMQTTDISC
18.2.15AT+CMQTTUNSUBTOPIC
4.2.2AT+CGDCONT
3.2.8AT+CAMM
3.2.9AT+CPUC
5.2.4AT+CGACT
4.2.2AT+COPS
5.2.15AT+CGEREP
Modifythesecommands


## Página 9

A76XX Series_AT Command Manual_V1.09
www.simcom.com 8/652
10.2.3AT+IPR
10.2.4AT+IPREX
10.2.10AT+CFGRI
23.2.8AT+CGNSSNMEA
23.2.9AT+CGPSNMEARATE
23.2.13AT+CGNSSCMD
6.2.4AT+CPWD
4.2.3AT+CUSD
10.2.2AT&C
3.2.12AT+CPAS
7.2.20AT+CMICGAIN
7.2.13AT+VTS
7.2.14AT+VTD
18.2.1AT+CMQTTSTART
18.2.5AT+CMQTTSSLCFG
13.2.1AT+CFTRANRX
13.2.2AT+CFTRANTX
23ATCommands forGNSS
5.2.8AT+CGQREQ
5.2.6AT+CGDSCONT
20.2.1AT+CTTS
20.2.2AT+CTTSPARAM
7.2.20AT+CMICGAIN
7.2.20AT+COUTGAIN
19.2.1AT+CSSLCFG
5.2.17AT+CPING
15.2.17AT+CDNSGIP
15.2.16AT+CIPACK
15.2.6AT+CIPCLOSE
2020.8.11
23.2.1AT+CGNSSPWR
23.2.2AT+CGPSCOLD
23.2.4AT+HOT
4.2.9AT+CPSI
5.2.7AT+CGTFT
5.2.6AT+CGDSCONT
Addthese commands
2020.8.13 23.2.10AT+CGPSFTM Modifythiscommand
2020.8.14 4.2.7AT+CNMP
9.2.2AT+CPMS Modifythesecommands
2020.8.18 23.2.5AT+CCONFSAVE Deletethis command
2020.8.18 15.2.4AT+CIPSEND Addanote
2020.8.19 20.2.1AT+CTTS Modifythiscommand
2020.8.19
24.1 Overview of AT Commands for
GNSS
24.2.5AT+CGNSSIPR
24.2.7AT+CGNSSNMEA
Modifythesesections


## Página 10

A76XX Series_AT Command Manual_V1.09
www.simcom.com 9/652
24.2.8AT+CGPSNMEARATE
2020.8.21 5.2.7AT+CGTFT
5.2.5AT+CGDCONT Modifythesecommands
2020.8.24 20.2.9AT+CTTSPARAM Modifythiscommand
2020.8.25 22ATCommands forFOTA Addthis chapter
2020.8.25 5.2.4AT+CGACT Modifythiscommand
2020.8.25 7.2.5AT+CRC
9.2.20AT+CMGSEX Modifythesecommands
2020.8.31 3.2.13AT+SIMEI
16.6.26AT+HTTPREAD Modifythesecommands
2020.9.1 25ATCommands forWIFI Addthis chapter
2020.9.1 19.2.11AT+CCHCFG Modifythiscommand
2020.9.2 16.2.7AT+HTTPDATA Modifythiscommand
2020.9.2 17.4 Summary of Unsolicited Result
Codes Addthis section
2020.9.2 10.2.3AT+IPR
10.2.4AT+IPREX Deletethese commands
2020.9.3 5.2.13AT+CGPADDR Modifythiscommand
2020.9.4 12.2.7AT+FSATTRI Modifythiscommand
2020.9.7 4.2.4AT+CSSN
4.2.8AT+CNBP Modifythesecommands
2020.9.8 16.2.8AT+HTTPPOSTFILE Modifythiscommand
2020.9.11 22.2.2AT+LFOTA Addthis command
2020.9.11 11.2.8AT+CWIIC
11.2.8AT+CRIIC Modifythesecommands
2020.9.14
7.2.4AT+CRLP
7.2.20AT+CMICGAIN
7.2.21AT+COUTGAIN
2.2.3ATA
5.2.4AT+CGACT
9.2.8AT+CNMA
9.2.20AT+CMGSEX
2.2.14ATX
Modifythesecommands
2020.9.15 9.2.5AT+CSCB
9.2.16AT+CMGD Modifythesecommands
2020.9.16 22.2.2AT+LFOTA Modifythiscommand
2020.9.25 20.2.2AT+CTTSPARAM Modifythiscommand
2020.9.25 16.2.3AT+HTTPPARA
14.2.1AT+HTPSERV Modifythesecommands
2020.9.29 24.2.13AT+CGNSSPORTSWITCH Addthis command
2020.10.10 16.2.6AT+HTTPREAD
16.2.8AT+HTTPPOSTFILE Modifythesecommands
2020.10.15 18.2.15AT+CMQTTUNSUBTOPIC Modifythiscommand


## Página 11

A76XX Series_AT Command Manual_V1.09
www.simcom.com 10/652
2020.10.20 16.2.6AT+HTTPREAD
18.2.17AT+CMQTTCFG Modifythesecommand
2020.10.21 25.2.2AT+CWSTASCANEX Addthis command
2020.10.29 12.2.9AT+FSCOPY Modifythiscommand
2020.10.30 2.2.1A/ Deletethis command
2020.11.2 11.2.15Unsolicitedresult codes Modifythissection
2020.11.2
16.2.3AT+HTTPPARA
5.2.17AT+CPING
16.2.8AT+HTTPPOSTFILE
4.2.2AT+COPS
4.2.8AT+CNBP
7.2.8AT+CCWA
7.2.2AT+CHUP
7.2.11AT+CLIR
22.2.2AT+LFOTA
11.2.7AT+CPMVT
5.2.14AT+CGCLASS
15.2.4AT+CIPSEND
15.2.5AT+CIPRXGET
15.2.6AT+CIPCLOSE
15.2.16AT+CIPACK
15.2.17AT+CDNSGIP
23.2.2AT+CSCFOTA
Modifythesecommands
2020.11.3
18.2.9AT+CMQTTDISC
19.2.13AT+CCHCLOSE
10.2.6AT+IFC
24.2.5AT+CGNSSIPR
24.2.6AT+CGNSSMODE
24.2.10AT+CGPSINFO
25.2.1AT+CWSTASCAN
25.2.2AT+CWSTASCANEX
16.2.3AT+HTTPPARA
20.2.1AT+CTTS
22.2.1AT+CFOTA
19.2.2AT+CCERTDOWN
24.2.13AT+CGNSSPORTSWITCH
Modifythesecommands
2020.11.7
17.2.3AT+CFTPSLOGIN
17.2.12AT+CFTPSPUTFILE
17.2.17AT+CFTPSTYPE
17.2.18AT+CFTPSSLCFG
21.2.3AT+CREC
20.2.1AT+CTTS
20.2.2AT+CTTSPARAM
21.1ATCommands forAudio
15.2.4AT+CIPSEND
Modifythesecommands


## Página 12

A76XX Series_AT Command Manual_V1.09
www.simcom.com 11/652
24.2.1AT+CGNSSPWR
7.2.12AT+COLP
7.2.11AT+CLIR
3.3.7AT+CACM
3.3.8AT+CAMM
4.2.9AT+CPSI
5.2.14AT+CGCLASS
15.2.15AT+SERVERSTOP
24.2.13AT+CGNSSPORTSWITCH
2020.11.13 15.2.18AT+CSOCKSETPN
24.2.14AT+CAGPS Addthese commands
2020.11.13
15.2.5AT+CIPRXGET
15.2.3AT+CIPOPEN
8.2.1AT+CPBS
7.2.11AT+VTS
7.2.12AT+COLP
7.2.11AT+CLIP
10.2.5AT+ICF
9.2.20AT+CMGSEX
6.2.3AT+CLCK
6.2.4AT+CPWD
23.2.1AT+CAPFOTA
17.2.2AT+CFTPSSTOP
9.2.9AT+CNMI
9.2.12AT+CMGR
6.2.7AT+CRSM
Modifythesecommands
2020.11.15
24.2.13AT+CGNSSPORTSWITCH
17.2.12AT+CFTPSPUTFILE
17.2.18AT+CFTPSSLCFG
20.2.2AT+CTTSPARAM
4.2.9AT+CPSI
15.2.18AT+CSOCKSETPN
15.2.5AT+CIPRXGET
9.2.12AT+CMGR
Modifythesecommands
2020.11.16
3.2.10AT+CCLK
9.2.8AT+CNMA
16.2.4AT+HTTPACTION
16.2.9AT+HTTPREADFILE
15.2.14AT+SERVERSTART
5.2.1AT+CGREG
4.2.7AT+CNMP
2.2.1ATD
16.2.8AT+HTTPPOSTFILE
3.2.4AT+CSQDELTA
3.2.10AT+CCLK
Modifythesecommands


## Página 13

A76XX Series_AT Command Manual_V1.09
www.simcom.com 12/652
V1.03
2020.11.25 24.2.7AT+CGNSSNMEA
10.2.6AT+IFC Modifythesecommands
2020.11.30
7.2.13AT+VTS
7.2.3AT+CBST
6.2.3AT+CLCK
Modifythesecommands
2020.12.1
7.2.3AT+CRLP
2.2.2ATA
4.2.3AT+CUSD
Modifythesecommands
2020.12.3 6.2.3AT+CLCK Modifythiscommand
2020.12.4 16.2.5AT+HTTPHEAD Modifythiscommand
2020.12.7 10.2.10AT+CFGRI Modifythiscommand
2020.12.14 24.2.13AT+CGNSSPORTSWITCH Modifythiscommand
2020.12.21
4.2.3AT+CUSD
7.2.3AT+CBST
10.2.11AT+CURCD
Modifythesecommands
2020.12.22 20.2.2AT+CTTSPARAM Modifythiscommand
2020.12.28
1.4 Definitions and Conventions
26.2Response StringofAT+CEER
26.3SummaryofCMEERRORCodes
Modifythesesections
2020.12.28
9.2.1AT+CSMS
9.2.2AT+CPMS
7.2.13AT+VTS
4.2.11AT+CTZU
Modifythesecommands
2020.12.31 20.2.3AT+CDTAM Addthis command
2020.12.31 21.2.1AT+CCMXPLAY Modifythiscommand
2021.1.7
4.2.1AT+CREG
4.2.4AT+CSSN
4.2.9AT+CPSI
5.2.15AT+CGEREP
Modifythesecommands
2021.1.8 18.2.8AT+CMQTTCONNECT Modifythiscommand
2021.1.12 7.2.9AT+CCFC Modifythiscommand
2021.1.14 11.2.5AT+CADC2
7.2.21AT+COUTGAIN Modifythesecommands
2021.1.15
9.2.20AT+CMGSEX
15.2.18AT+CSOCKSETPN
15.2.6AT+CIPCLOSE
5.2.8AT+CGQREQ
Modifythesecommands
2021.1.29
11.2.7AT+CPMVT
8.2.4AT+CPBW
9.2.5AT+CSCB
9.2.21AT+CMSSEX
9.2.6AT+CSMP
Modifythesecommands


## Página 14

A76XX Series_AT Command Manual_V1.09
www.simcom.com 13/652
21.2.3AT+CREC
2021.2.3
All AddsupportonA7678Series
3.2.10AT+CCLK
7.2.12AT+COLP Modifythesecommands
24ATCommands forGNSS modify this chapter for A7678
Series
2021.3.12 18.2.15AT+CMQTTUNSUBTOPIC Modifythiscommand
2021.2.4 12.2Detailed Description of AT
CommandsforFileSystem Modifythis command
2021.3.5 14.2.1AT+CHTPSERV Modifythiscommand
2021.3.16
11.2.7AT+CPMVT Low and high
voltagePower Off
4.2.2AT+COPSOperatorselection
15.2.6AT+CIPCLOSE Close TCP or
UDPSocket
9.2.15AT+CMGW Write message to
memory
14.2.3AT+CNTP rectify parameter
range
Modifythiscommand
2021.3.10
13.2.3AT+CFTRXBUF
13.1AT+CFTRANRX
13.1AT+CFTRANTX
13.1AT+CFTRXBUF
Addthis command
2021.3.26
4.2.4AT+CSSN
25.2.1AT+CWSTASCAN
25.2.2AT+CWSTASCANEX
2.2.9AT&V
Modifythiscommand
2021.3.29 14.2.3AT+CCLK Modifythiscommand
2021.3.30 5.2.2AT+CEREG EPS network
registrationstatus Modifythiscommand
2021.3.30 9.2.18AT+CMVP Set message valid
period Modifythiscommand
2021.3.30 9.2.10 AT+CGSMS Select service for
MOSMS messages Modifythiscommand
2021.3.30 10.2.7AT+CSCLK Modifythiscommand
2021.3.30 15.2.8AT+CSOCKSETPN Modifythiscommand
2021.3.30 22.2.1AT+CFOTA Modifythiscommand
2021.3.31 9.2.16AT+CMGD Modifythiscommand
2021.3.31 16.2.8AT+HTTPPOSTFILE Modifythiscommand
2021.4.1 26Added BleAt Addthis chapter
2021.4.14 25.2.2AT+CWSTASCANEX Modifythiscommand


## Página 15

A76XX Series_AT Command Manual_V1.09
www.simcom.com 14/652
2021.4.22 7.2.3AT+CBST Modifythiscommand
2021.4.22
26.2.19 +BLESRREQ
26.2.20+BLESWREQ
26.2.21+BLESCON
Addthis command
2021.4.22 26.2.9AT+BLESLSTART
26.2.11AT+BLEADV Modifythiscommand
2021.4.27 20.2.3AT+CDTAM Modifythiscommand
2021.4.27
16.2.1AT+HTTPINIT
16.2.2AT+HTTPTERM
16.2.3AT+HTTPPARA
16.2.5AT+HTTPHEAD
16.2.6AT+HTTPREAD
16.2.7AT+HTTPDATA
17.2.1AT+CFTPSSTART
17.2.2AT+CFTPSSTOP
17.2.5AT+CFTPSLIST
17.2.9AT+CFTPSPWD
AddthereTestcommands
2021.4.28 26.2.1AT+BLESREG
26.2.21+BLESCONN Modifythiscommand
2021.4.28 16.2.3AT+HTTPPARA
16.2.8AT+HTTPPOSTFILE Modifythiscommand
2021.4.29 20.2.1AT+CTTS Modifythiscommand
2021.4.30 9.2.21AT+CMSSEX Modifythiscommand
2021.4.30
2.2.4ATS0Automaticanswerincoming
call
22.3UnsolicitedResult Codes
16.2.5AT+HTTPHEAD
Modifythe word
2021.5.7 23.2.2AT+CSCFOTA Modifythe word
2021.5.8 11.2.7AT+CPMVT Modifythe word
2021.5.10 21.2.2AT+CCMXSTOP Modifythiscommand
2021.5.10
17.2.14AT+CFTPSPUT
26.2.1AT+BLEPOWER
26.2.10AT+BLESSCRM
26.2.12AT+BLESSDRM
Modifythiscommand
2021.5.13
26.2.1AT+BLEPOWER
26.2.5AT+BLESREG
26.2.7AT+BLESSAD
Modifythiscommand


## Página 16

A76XX Series_AT Command Manual_V1.09
www.simcom.com 15/652
26.2.9AT+BLESSCAD
26.2.11AT+BLESSDAD
2021.5.14 7.2.11AT+CLIR
7.2.12AT+COLP Modifythesecommands
2021.5.17 14.2.1AT+CHTPSERV Modifythe word
2021.5.18 9.2.15AT+CMGW
9.2.20AT+CMGSEX Modifythesecommands
2021.5.19
26.2.2AT+BLESTATUS
26.2.3AT+BLEHOST
26.2.4AT+BLEADDRESS
26.2.5AT+BLESREG
26.2.6AT+BLESDREG
10.2.10AT+CFGRI
24.2.6AT+CGNSSIPR
24.2.9AT+CGPSNMEARATE
24.2.7AT+CGNSSMODE
21.2.3AT+CREC
Modifythesecommands
2021.5.31 21.2.3AT+CREC Modifythiscommands
2021.6.1 12.1Overview of AT Commands for
FileSystem
Modify the length of actual fully
qualifiednames
2021.6.1 12.2.6AT+FSRENAME Deletethe Note
2021.6.2 5.2.4AT+CGACT
7.2.12AT+COLP Modifythesecommands
2021.6.16
6.2.12AT+SWITCHSIM
6.2.13AT+DUALSIM
6.2.14AT+BINDSIM
Addthese commands
2021.6.17 21.2.3AT+CREC Modifythiscommands
2021.6.29 22.2.1AT+CFOTA ModifyURCreport
2021.6.29
2.2.1ATD
2.2.2ATA
2.2.3ATH
7.2.2AT+CHUP
7.2.11AT+CLIR
7.2.12AT+COLP
7.2.15AT+CSTA
Modifythesecommands


## Página 17

A76XX Series_AT Command Manual_V1.09
www.simcom.com 16/652
2021.6.29 21.2.3AT+CREC Modifycomment
2021.6.30
4.2.1AT+CREG
5.2.1AT+CGREG
5.2.2AT+CEREG
Addstate
2021.6.30
14.2.1AT+CHTPSERV
17.2Overview of AT Commands for
FTP.
Modifyparameter
2021.6.30 20.2.1AT+CTTS Modifycomment
2021.7.7 21.2.3AT+CREC Modifycomment
2021.7.8 24.2.6AT+CGNSSIPR Addnotes
V1.04
2021.7.13 15.2.18AT+CSOCKSETPN
21.2.3AT+CREC
Modifycomment
2021.7.13
6.2.12AT+SWITCHSIM
6.2.13AT+DUALSIM
6.2.14AT+BINDSIM
Modifythesecommands
2021.7.19
7.2.7AT+CEER
7.2.13AT+VTS
7.2.15AT+CSTA
7.2.16AT+CMOD
Modifythesecommands
2021.7.23
16.2.5AT+HTTPHEAD
16.2.6AT+HTTPREAD
16.2.7AT+HTTPDATA
16.2.8AT+HTTPPOSTFILE
Modifythesecommands
2021.7.23 17.2.5AT+CFTPSLIST
17.2.16AT+CFTPSSIZE Modifycomment
2021.7.28
15.2.2AT+NETCLOSE
15.2.18AT+CSOCKSETPN
24.2.3AT+CGPSCOLD
24.2.4AT+CGPSWARM
24.2.5AT+CGPSHOT
25.2.1AT+CWSTASCAN
25.2.2AT+CWSTASCANEX
Modifythesecommands
2021.7.28 24.2.16AT+CGNSSPROD Addthis command
2021.7.29
14.2.1AT+CHTPSERV
11.2.1AT+CVALARM
11.2.7AT+CPMVT
Modifythesecomment
2021.7.30 4.2.11AT+CTZU Modifycomment


## Página 18

A76XX Series_AT Command Manual_V1.09
www.simcom.com 17/652
2021.7.30 24.2.2AT+CGNSSTST
10.2.11AT+CURCD Modifythesecommands
V1.05
2021.8.11 17.2.9AT+CFTPSPWD
17.2.10AT+CFPSDELE Modifythesecommands
2021.8.13 18.2.2AT+CMQTTSTOP Modifycommand
2021.8.16 6.2.10AT+UIMHOTSWAPON Modifycommand
2021.8.18 27.2.1AT+CTBURST Addthis command
2021.8.19 17.2.3AT+CFTPSLOGIN Addreadcommand
2021.8.23
25.2.1AT+CWSTASCAN
3.2.7AT+CACM
3.2.8AT+CAMM
Change the font
2021.8.24 3.2.2AT+CSQ Change the definedvalues
2021.8.27 18.2.4AT+CMQTTREL
18.4UnsolicitedResultCodes Modifycommand
2021.8.27 15.2.18 AT+CSOCKSETPN
16.2.4 AT+HTTPACTION Modifycommand
2021.9.1
5.2.1 AT+CGREG Network
registrationstatus
5.2.2 AT+CEREG EPS network
registrationstatus
Modifycommand
2021.9.1 15.2.18 AT+CSOCKSETPN Modifycommand
2021.9.2 17.2.9AT+CFTPSPWD
17.4 UnsolicitedResultcodes Modifycommand
2021.9.3 19.2.1AT+CSSLCFG
19.2.15AT+CCHRECV Modifycommand
2021.9.8
17.2.6AT+CFTPSMKD
17.2.7AT+CFTPSRMD
17.2.8AT+CFTPSCWD
17.2.10AT+CFTPSDELE
17.2.11AT+CFTPSGETFILE
17.2.12AT+CFTPSPUTFILE
17.2.13AT+CFTPSGET
Modifycommand
2021.9.9 19.2.2AT+CCERTDOWN Modifycommand
2021.9.23 11.2.7AT+CPMVT Modifycommand


## Página 19

A76XX Series_AT Command Manual_V1.09
www.simcom.com 18/652
2021.9.27 9.2.21 AT+CMSSEX
13.2.3AT+CFTRXBUF Modifycommand
2021.9.27 9.2.16AT+CMGD
24.2.1AT+CGNSSPWR Modifythesecommands
2021.9.28 18.2.3AT+CMQTTACCQ
18.2.4AT+CMQTTREL Modifycommand
2021.9.29 17.2.15AT+CFTPSSINGLEIP Modifycommand
2021.9.29 10.2.6AT+IFC Modifycommand
2021.9.29 5.2.9AT+CGEQREQ Modifycommand
2021.9.29 11.2.9AT+CWIIC Modifycommand
2021.10.8 15.2.4AT+CIPSEND Modifycommand
2021.10.8 15.2.3AT+CIPOPEN Modifycommand
2021.10.8 15.2.5AT+CIPRXGET Modifycommand
2021.10.9 19.4 UnsolicitedResult Codes AddURC
V1.06
2021.10.18 17.2.3AT+CFTPSLOGIN Modifycommand
2021.10.27 12.2.4AT+FSLS Modifycommand
2021.10.27 16.2.8AT+HTTPPOSTFILE Modifycommand
2021.10.29 6.2.12AT+SWITCHSIM
6.2.13AT+DUALSIM Modifycommands
2021.11.3 18.2.14AT+CMQTTSUB
18.2.8AT+CMQTTCONNECT Modifycommands
2021.11.9 11.2.2AT+CVAUXS
11.2.2AT+CVAUXV Modifycommands
2021.11.11 15.2.4AT+CIPSEND Modifycommands
2021.11.15 6.2.6AT+CSIM Modifycommands
2021.11.17 12.2.5AT+FSRENAME AddNotes
2021.11.19 4.2.11AT+CTZU Modifycommands
2021.11.22 3.2.13AT+SIMEI AddNotes
2021.11.25 10.2.10AT+CFGRI Modifycommands
V1.07 2021.11.23 15.2.18AT+CSOCKSETPN Modifycommands


## Página 20

A76XX Series_AT Command Manual_V1.09
www.simcom.com 19/652
2021.11.26 15.2.2AT+NETCLOSE Modifycommands
2021.11.29 24.2.12AT+CGNSSINFO Modify the comment of this
command.
2021.12.01 12.2.6AT+FSRENAME Modify the note of this
command.
2021.12.02 11.2.2AT+CVAUXS
11.2.3AT+CVAUXV
Modify the note of this
command.
2021.12.02 29ATCommands forWEBSOCKET Addcommand
2021.12.02 15.2.7AT+IPADDR Modifycommands
2021.12.03 21.2.4AT+CRTSWITCH Addcommand
2021.12.08 25.2.1AT+CWSTASCNA Modifycommands
2021.12.09 11.2.7AT+CPMVT Modifycommands
2021.12.13 15.2.18AT+CSOCKSETPN Modifycommands
2021.12.13 27.3.1AT+CTBURST Modifythe commanddescription
2021.12.13 14.2.3AT+CNTP Modifyexamples
2021.12.13 4.2.12AT+CTZR Modify the comment of this
command.
2021.12.14 20.2.2AT+CTTSPARAM Modify the comment of this
command.
2021.12.14 15.2.3AT+CIPOPEN Modify the comment of this
command.
2021.12.16 19.2.15AT+CCHRECV Modify the comment of this
command.
2021.12.22 9.2.17AT+CGSMS Modify the comment of this
command.
2021.12.23 4.2.10AT+CPSITD Addcommand
2021.12.24
30.2.1AT+LWSTART
30.2.2AT+LWSTOP
30.2.3AT+LWCNF
30.2.4AT+LWOPEN
30.2.5AT+LWCLOSE
30.2.6AT+LWADDOBJ
30.2.7AT+LWDELOBJ
30.2.8AT+LWREADRSP
30.2.9AT+LWWRITERSP
30.2.10AT+LWEXECUTERSP
12.2.10AT+FSPRESET
Addcommand


## Página 21

A76XX Series_AT Command Manual_V1.09
www.simcom.com 20/652
2021.12.31 15.2.3AT+CIPOPEN Modify the comment of this
command.
2021.12.31 9.2.5AT+CSCB Modify the comment of this
command.
2022.01.11 25.2.3AT+CWSTASCANSYN Addthe command.
2022.01.12 15.2.5AT+CIPRXGET Modify the comment of this
command.
2022.01.14 15.2.19AT+CTCPKA Addthe commands
2022.1.21
18.1 Overview of AT Commands for
MQTT(S) Modify the comment of this
command.
2022.1.21 27.2 Detailed Description of AT
CommandsforCTBURST(CAT4)
Modifythischapter
2022.1.21
8.2.4 AT+CPBW Write phonebook
entry Modifythiscommand
2022.1.25 6.2.15 AT+CENPRX Enable SIM2
URCprefix Addthe command
2022.02.11 15.2.20AT+CDNSCFG
15.2.21AT+CSOC Addthese commands
2022.02.17 9.2.21AT+CMSSEX Modifythiscommand
2022.02.21 4.2.8AT+CNBP Modifythiscommand
2022.02.25 21.2.5AT+CRINGSET Addthis command
2022.03.03 4.2.1AT+CREG Modifythiscommand
2022.03.04
6.1.26 AT+DUALSIMURC Dual card
reportingcontrol Addthe command
2022.03.09 4.2.10AT+CPSI Modifythiscommand
2022.03.17 25.2.2AT+CWSTASCANEX Modifyfontcolor
2022.03.22 16.2.8AT+HTTPPOSTFILE Modifythiscommand
2022.3.23 18.2.9AT+CMQTTDISC
18.2.12AT+CMQTTPUB Modifythiscommand
2022.03.24
31.2.1AT+COAPSTART
31.2.2AT+COAPSTOP
31.2.3AT+COAPOPEN
31.2.4AT+COAPCLOSE
31.2.5AT+COAPHEAD
Addcommand


## Página 22

A76XX Series_AT Command Manual_V1.09
www.simcom.com 21/652
31.2.6AT+COAPOPTION
31.2.7AT+COAPSEND
31.2.8AT+COAPSENDTX
2022.03.28 15.2.21AT+CSOC Modifythiscommand
2022.03.30 13.2.1AT+CFTRANRX Modifythiscommand
2022.03.31
12.2.11AT+FSOPEN
12.2.12AT+FSREAD
12.2.13AT+FSWRITE
12.2.14AT+FSSEEK
12.2.15AT+FSPOSITION
12.2.16AT+FSCLOSE
Addthese commands
2022.04.01 6.2.16AT+DUALSIMURC Mdify this command
2022.04.06 15.1 Modifydirectory Modifydirectory
2022.04.07 6.2.15AT+CENPRX Mdify this command
2022.04.11
25.2.3AT+CWSTASCANSYN
26.2.15AT+BLESSETADVDATA
26.2.17AT+BLESSETADVPARAM
26.2.24AT+BLESRSP
26.2.37AT+BLECGC
26.2.41AT+BLECRD
Mdify this command
2022.04.15 32.2.1AT+HWVER
32.2.2AT+AUTOREGCFG Addthese commands
2022.04.19 15.2.19AT+CTCPKA Mdify this command
2022.04.21 17.2.16AT+CFTPSSIZE Mdify this command
2022.04.24 15.2.20AT+CDNSCFG Mdify this command
2022.5.1 7.2.3AT+CBST Mdify this command
2022.5.7 21.1.6AT+CCODESWITCH Addnewcommand
2022.5.14 2.2.11AT&F Mdify this command
2022.5.18
26.3.1AT+BTPOWER
26.3.2AT+BTHOST
26.3.3AT+BTADDR
26.3.4AT+BTSCAN
26.3.5AT+BTIOCAP
26.3.6AT+BTPAIR
26.3.7AT+BTUNPAIR
Addnewcommand


## Página 23

A76XX Series_AT Command Manual_V1.09
www.simcom.com 22/652
26.3.8AT+BTPAIRED
26.3.9AT+BTSPPSRV
26.3.10AT+BTSPPPROF
26.3.11AT+BTSPPCONN
26.3.12AT+BTSPPSEND
26.3.13 +BTSPPRECV
2022.5.18
25.2.4AT+CWMAP
25.2.5AT+CWSSID
25.2.6AT+CWAUTH
25.2.7AT+CWMOCH
25.2.8AT+CWISO
Addnewcommand
2022.5.19 25.1AT+CWSTASCAN Addnewcommand
2022.5.20 21.2.5AT+CRINGSET Modifythiscommand
2022.5.25 21.2.5AT+CRINGSET Modifythiscommand
2022.5.26 25.2.9AT+CWMACADDR
25.2.10AT+CWCLICNT Addnewcommand
2022.5.26 5.2.16AT+CGAUTH Modifythiscommand
2022.5.31
26.3.2AT+BTHOST
26.3.3AT+BTADDR
26.3.4AT+BTSCAN
26.3.6AT+BTPAIR
26.3.7AT+BTUNPAIR
26.3.8AT+BTPAIRED
26.3.10AT+BTSPPPROF
26.3.11AT+BTSPPCONN
26.3.12AT+BTSPPSEND
26.3.13 +BTSPPRECV
Modifythiscommand
2022.5.31 6.2.15AT+CENPRX Deletethis command
2022.5.31 6.2.15AT+DUALSIMURC Modifythiscommand
2022.6.6
26.2.11AT+BLESSDAD
26.2.13AT+BLESSSTART
26.2.14AT+BLESSSTOP
26.2.18AT+BLESLSTART
26.2.19AT+BLESLSTOP
Modifythiscommand
2022.6.22
26.3.1AT+BTPOWER
26.3.2AT+BTHOST
26.3.3AT+BTADDR
26.3.4AT+BTSCAN
26.3.5AT+BTIOCAP
26.3.6AT+BTPAIR
Modifythiscommand


## Página 24

A76XX Series_AT Command Manual_V1.09
www.simcom.com 23/652
26.3.7AT+BTUNPAIR
26.3.8AT+BTPAIRED
26.3.9AT+BTSPPSRV
26.3.10AT+BTSPPPROF
26.3.11AT+BTSPPCONN
26.3.12AT+BTSPPSEND
26.3.13AT+BTSPPRECV
2022.7.6 15.2.5AT+CIPRXGET Modifythiscommand
2022.7.6 4.2.8AT+CNBP
27.3.1AT+CTBURST Modifythiscommand
2022.7.7 16.2.3AT+HTTPPARA Modifythiscommand
2022.7.7 15.2.19AT+CTCPKA Modifythiscommand
2022.7.8 26.2.7AT+BLESSAD Modifythiscommand
2022.7.11 7.2.13AT+CHLD Addthis command
2022.7.13 26.2.40AT+BLECWC
26.2.42AT+BLECWD Modifythiscommand
2022.7.15 19.2.1AT+CSSLCFG Modifythiscommand
2022.7.19
34.2.1AT*COMCFG
34.2.2AT+CPSMS
34.2.3AT+MEDCR
35.2.1AT+DIALMODE
35.2.2AT$MYCONFIG
35.2.3AT+USBNETIP
35.2.4AT+USBNETMAC
Addthese commands
2022.7.19 4.2.8AT+CNBP Modifythiscommand
2022.7.19 15.2.21AT+CSOC Modifythiscommand
2022.7.21 19.2.16AT+CCERTMOVE Modifythiscommand
2022.7.25
26.3.3AT+BTADDR
26.3.4AT+BTSCAN
26.3.11AT+BTSPPCONN
Modifythiscommand
V1.08
2022.8.3 19.1 Modifydirectory
19.2.1 AT+CSSLCFG
Modifydirectory
Modifythiscommand
2022.8.4 36.2.1AT+SJDR
36.2.2AT+SJDCFG Addjdcommands
2022.8.8
17.2.11AT+CFTPSGETFILE
17.2.12AT+CFTPSPUTFILE
17.2.19AT+CFTPSMODE
Modifythiscommand
Modifythiscommand
Addthis command


## Página 25

A76XX Series_AT Command Manual_V1.09
www.simcom.com 24/652
2022.8.9 6.2.10AT+UIMHOTSWAPON Modifythiscommand
2022.8.22 22.2.2AT+LFOTA Modifythiscommand
2022.8.23 17FTP(S) Modifythiscommand
2022.8.26 17.2.11AT+CFTPSGETFILE
19.2.16AT+CCERTMOVE Modifythiscommand
2022.8.30 21.2.7AT+SIMTONE Addthis command
2022.8.31 17.2.11AT+CFTPSGETFILE Modifythiscommand
2022.9.5 29.2websocket(S)
15.2.2AT+NETCLOSE Modifythiscommand
2022.9.8 26.2.17AT+BLESSETADVPARAM Modifythiscommand
2022.9.14
17.2.5 AT+CFTPSLIST
17.2.9 AT+CFTPSPWD
17.2.16 AT+CFTPSSIZE
17.2.19 AT+CFTPSMODE
17.4UnsolicitedResult codes
19.2.1 AT+CSSLCFG
Modifythiscommand
2022.9.19 15.2.13 AT+CIPCCFG Modifythiscommand
2022.9.27 4.2.10 AT+CPSITD Modifythiscommand
2022.9.29 24.2.13 AT+CGNSSCMD Modifythiscommand
2022.9.29
16.2.5AT+HTTPHEAD
16.2.6AT+HTTPREAD
16.2.8AT+HTTPPOSTFILE
Modifythiscommand
2022.9.29
25.2.1AT+CWSTASCAN
25.2.2AT+CWSTASCANEX
25.2.3AT+CWSTASCANSYN
26.2.4AT+BLEADDR
26.2.20AT+BLEADV
Modifythiscommand
V1.09
2022.10.13 21.2.1AT+CCMXPLAY Modifythiscommand
2022.10.13 18.2.3AT+CMQTTDISC Modifythiscommand
2022.10.19
26.3.6AT+BTPAIR
26.3.7AT+BTUNPAIR
26.3.11AT+BTSPPCONN
Modifythiscommand
2022.10.20 26.3.11AT+BTSPPCONN Modifythiscommand


## Página 26

A76XX Series_AT Command Manual_V1.09
www.simcom.com 25/652
2022.11.11
6.2.13AT+SWITCHSIM
6.2.14AT+DUALSIM
6.2.15AT+BINDSIM
6.2.16AT+DUALSIMURC
AddNote
2022.11.21 26.2.3AT+BLEHOST
26.3.2AT+BTHOST Modifythesecommands
2022.12.26 6.2.8AT+SPIC Modifythiscommand
2023.01.11 4.2.2AT+COPS AddNote
2023.01.11 3.2.1AT+CFUN Modifythiscommand
2023.02.07 27.3AT+CTBURST Modifythischapter
2023.02.09 4.2AT+CUSD Modifythischapter
2023.02.10
15.2.3AT+CIPOPEN
15.2.5AT+CIPRXGET
15.2.13AT+CIPCCFG
15.2.22AT+CIPCFG
Modifythesecommands
Modifythesecommands
Modifythesecommands
Addthis command
2023.02.14 3.2.1AT+CFUN Modifythischapter forexample
2023.02.20 27.3.1AT+CTBURST Modifythischapter forexample
2023.03.08 9.2.20 AT+CMGSEX Modifythischapter
2023.03.14 7.2.6AT+CLCC Modifythiscommand
2023.03.14 6.2.15AT+DUALSIMURC Modifythe Defined Values
2023-03-15 9.2.22AT+CCONCINDEX Addthis command
2023-3-17 9.2.10AT+CGSMS Modifythiscommand
2023.3.23 15.2.17AT+CDNSGIP Modifythiscommand
2023.3.23 2.2.20AT+CSCS Modifythiscommand
2023.3.23 2.2.16AT+CGMI Modifythiscommand
2023.3.27 16.2.8AT+HTTPPOSTFILE Modifythiscommand
2023.3.27 15.2.23AT+CIPSENDSTR Addthis command
2023.3.29 12.2.12AT+FSREAD
12.2.13AT+FSWRITE Modifythiscommand


## Página 27

A76XX Series_AT Command Manual_V1.09
www.simcom.com 26/652
12.2.14AT+FSSEEK
2023.4.4 18.2.17AT+CMQTTCFG Modifythiscommand
2023.4.6 4.2.3AT+CUSD Modifythiscommand
2023.4.10 9.2.22AT+CCONCINDEX Modifythiscommand
2023.4.17 4.2.11 AT+CNSMOD Show
networksystem mode Modifythiscommand
2023.4.18 17.2.13AT+CFTPSGET Modifythiscommand
2023.4.19 2.2.20 AT+CSCS Select TE
characterset Modifythiscommand
2023.4.21 28.4 Modify340error meaning Modify340errormeaning
2023.4.23 4.2.8AT+CNBP
6.2.15AT+DUALSIMURC Modifythiscommand
2023.4.24 7.2.6AT+CLCC AddNOTE


## Página 28

A76XX Series_AT Command Manual_V1.09
www.simcom.com 27/652
Contents
VersionHistory.......................................................................................................................2
Contents...............................................................................................................................27
Introduction.......................................................................................................................40
1.1 Scope ofthe document.....................................................................................................................40
1.2 Related documents............................................................................................................................40
1.3TermsandAbbreviations...................................................................................................................41
1.4 Definitions andConventions.............................................................................................................42
1.5ATInterfaceSynopsis........................................................................................................................43
1.5.1 InterfaceSettings...............................................................................................................43
1.5.2 ATCommands Syntax.......................................................................................................43
1.5.3 Supportedcharactersets..................................................................................................45
ATCommandsAccordingtoV.25TER.............................................................................46
2.1 OverviewofATCommandsAccording toV.25TER..........................................................................46
2.2 Detailed DescriptionofATCommands forV.25TER........................................................................46
2.2.1 ATDMobile originated calltodial anumber...................................................................47
2.2.2 ATA Callanswer..............................................................................................................48
2.2.3 ATH Disconnectexistingcall..........................................................................................49
2.2.4 ATS0 Automatic answer incomingcall...........................................................................50
2.2.5 +++ Switchfromdata modetocommand mode............................................................51
2.2.6 ATO Switch from commandmodeto datamode...........................................................51
2.2.7 ATI Display productidentification information................................................................52
2.2.8 ATE Enable command echo...........................................................................................53
2.2.9 AT&V Display currentconfiguration................................................................................54
2.2.10 ATV Setresultcodeformat mode..................................................................................55
2.2.11 AT&F Setallcurrentparameters tomanufacturerdefaults...........................................56
2.2.12 ATQ SetResultCodePresentation Mode......................................................................56
2.2.13 ATX SetCONNECTResultCode Format......................................................................57
2.2.14 AT&W Save the usersettingtoME................................................................................58
2.2.15 ATZ Restorethe usersettingfrom ME...........................................................................59
2.2.16 AT+CGMI Request manufactureridentification..............................................................60
2.2.17 AT+CGMM Requestmodel identification.......................................................................61
2.2.18 AT+CGMR Request revisionidentification.....................................................................61
2.2.19 AT+CGSN Requestproductserial numberidentification...............................................62
2.2.20 AT+CSCS SelectTEcharacterset.................................................................................63
2.2.21 AT+GCAP Request overallcapabilities..........................................................................64
ATCommandsforStatusControl....................................................................................66


## Página 29

A76XX Series_AT Command Manual_V1.09
www.simcom.com 28/652
3.1 OverviewofATCommands forStatus Control.................................................................................66
3.2 Detailed DescriptionofATCommands forStatusControl................................................................66
3.2.1 AT+CFUN Setphonefunctionality..................................................................................66
3.2.2 AT+CSQ Querysignalquality.........................................................................................68
3.2.3 AT+AUTOCSQ SetCSQreport......................................................................................69
3.2.4 AT+CSQDELTA Set RSSIdeltachangethreshold.........................................................70
3.2.5 AT+CPOF Powerdown the module...............................................................................71
3.2.6 AT+CRESET Resetthe module.....................................................................................72
3.2.7 AT+CACM Accumulatedcallmeter................................................................................73
3.2.8 AT+CAMM Accumulated callmetermaximum...............................................................74
3.2.9 AT+CPUC Price perunitandcurrencytable..................................................................75
3.2.10 AT+CCLK Real timeclock management........................................................................77
3.2.11 AT+CMEE Reportmobile equipmenterror....................................................................78
3.2.12 AT+CPAS Phoneactivity status......................................................................................79
3.2.13 AT+SIMEI Setthe IMEIforthe module..........................................................................80
ATCommandsforNetwork..............................................................................................82
4.1 OverviewofATCommands forNetwork...........................................................................................82
4.2 Detailed DescriptionofATCommands forNetwork.........................................................................82
4.2.1 AT+CREG Networkregistration......................................................................................82
4.2.2 AT+COPS Operatorselection.........................................................................................84
4.2.3 AT+CUSD Unstructuredsupplementaryservice data...................................................86
4.2.4 AT+CSSN Supplementary service notifications.............................................................88
4.2.5 AT+CPOL Preferredoperatorlist....................................................................................90
4.2.6 AT+COPN Readoperator names...................................................................................92
4.2.7 AT+CNMP Preferred modeselection.............................................................................93
4.2.8 AT+CNBP Preferredband selection...............................................................................94
4.2.9 AT+CPSI Inquiring UEsysteminformation....................................................................97
4.2.10 AT+CPSITD Inquiring UELTEsysteminformation........................................................99
4.2.11 AT+CNSMOD Shownetwork systemmode.................................................................100
4.2.12 AT+CTZU Automatic timeand timezoneupdate.........................................................101
4.2.13 AT+CTZR Timeand timezonereporting.....................................................................103
ATCommandsforPacketDomain.................................................................................105
5.1 OverviewofATCommands forPacketDomain.............................................................................105
5.2 Detailed DescriptionofATCommands forPacketDomain............................................................105
5.2.1 AT+CGREG Networkregistrationstatus......................................................................105
5.2.2 AT+CEREG EPSnetwork registrationstatus...............................................................107
5.2.3 AT+CGATT Packetdomain attachordetach...............................................................109
5.2.4 AT+CGACT PDPcontext activate ordeactivate..........................................................110
5.2.5 AT+CGDCONT DefinePDPcontext.............................................................................112
5.2.6 AT+CGDSCONT DefineSecondary PDPContext.......................................................115
5.2.7 AT+CGTFT TrafficFlowTemplate................................................................................117
5.2.8 AT+CGQREQ Qualityof serviceprofile (requested)...................................................120
5.2.9 AT+CGEQREQ 3Gqualityof serviceprofile (requested)............................................123
5.2.10 AT+CGQMIN Qualityof serviceprofile (minimumacceptable)...................................128
5.2.11 AT+CGEQMIN 3Gqualityof serviceprofile (minimumacceptable)............................130


## Página 30

A76XX Series_AT Command Manual_V1.09
www.simcom.com 29/652
5.2.12 AT+CGDATA Enterdata state......................................................................................135
5.2.13 AT+CGPADDR ShowPDPaddress.............................................................................137
5.2.14 AT+CGCLASS GPRSmobile station class..................................................................138
5.2.15 AT+CGEREP GPRSevent reporting...........................................................................140
5.2.16 AT+CGAUTH Settype ofauthenticationforPDP-IPconnectionsof GPRS...............147
5.2.17 AT+CPING Pingdestination address.........................................................................149
ATCommandsforSIMCard...........................................................................................152
6.1 OverviewofATCommands forSIM Card.......................................................................................152
6.2 Detailed DescriptionofATCommands forSIM Card.....................................................................152
6.2.1 AT+CICCID Read ICCID from SIM card......................................................................152
6.2.2 AT+CPIN Enter PIN......................................................................................................153
6.2.3 AT+CLCK Facility lock..................................................................................................155
6.2.4 AT+CPWD Change password......................................................................................157
6.2.5 AT+CIMI Requestinternationalmobilesubscriberidentity..........................................158
6.2.6 AT+CSIM Generic SIMaccess.....................................................................................159
6.2.7 AT+CRSM Restricted SIM access................................................................................160
6.2.8 AT+SPIC Timesremain toinputSIM PIN/PUK............................................................165
6.2.9 AT+CSPN Getservice providernamefrom SIM..........................................................166
6.2.10 AT+UIMHOTSWAPON SetUIM Hotswap Function On...............................................167
6.2.11 AT+UIMHOTSWAPLEVEL SetUIM Card Detection Level..........................................168
6.2.12 AT+SWITCHSIM Switch masterSIM...........................................................................169
6.2.13 AT+DUALSIM Set dual-sim mode................................................................................170
6.2.14 AT+BINDSIM BindATPto SIM1 orSIM2.....................................................................171
6.2.15 AT+DUALSIMURCDual cardreporting control..............................................................173
ATCommandsforCallControl......................................................................................175
7.1 OverviewofATCommands forCall Control...................................................................................175
7.2 Detailed DescriptionofATCommands forCallControl..................................................................175
7.2.1 AT+CVHU Voicehang up control.................................................................................176
7.2.2 AT+CHUP Hang upcall................................................................................................177
7.2.3 AT+CBST Selectbearer servicetype...........................................................................178
7.2.4 AT+CRLP Radio linkprotocol.......................................................................................180
7.2.5 AT+CRC Cellularresult codes......................................................................................182
7.2.6 AT+CLCC Listcurrentcalls..........................................................................................183
7.2.7 AT+CEER Extendederrorreport..................................................................................186
7.2.8 AT+CCWA Callwaiting.................................................................................................187
7.2.9 AT+CCFC Callforwarding numberandconditions......................................................189
7.2.10 AT+CLIP Calling lineidentification presentation..........................................................191
7.2.11 AT+CLIR Calling line identification restriction..............................................................193
7.2.12 AT+COLP Connectedline identificationpresentation..................................................195
7.2.13 AT+CHLD Callrelated supplementaryservices...........................................................196
7.2.14 AT+VTS DTMF and tone generation............................................................................197
7.2.15 AT+VTD Tone duration.................................................................................................199
7.2.16 AT+CSTA Selecttype of address.................................................................................200
7.2.17 AT+CMOD Call mode...................................................................................................201
7.2.18 AT+VMUTE Speakermute control...............................................................................203


## Página 31

A76XX Series_AT Command Manual_V1.09
www.simcom.com 30/652
7.2.19 AT+CMUT Microphone mutecontrol............................................................................204
7.2.20 AT+CSDVC Switchvoice channel device....................................................................205
7.2.21 AT+CMICGAIN Adjustmicgain....................................................................................206
7.2.22 AT+COUTGAIN Adjustoutgain...................................................................................207
ATCommandsforPhonebook.......................................................................................209
8.1 OverviewofATCommands forPhonebook....................................................................................209
8.2 Detailed DescriptionofATCommands forPhonebook..................................................................209
8.2.1 AT+CPBS Selectphonebookmemorystorage............................................................209
8.2.2 AT+CPBR Read phonebookentries.............................................................................211
8.2.3 AT+CPBF Find phonebookentries...............................................................................212
8.2.4 AT+CPBW Writephonebook entry...............................................................................214
8.2.5 AT+CNUM Subscribernumber.....................................................................................215
ATCommandsforSMS...................................................................................................217
9.1 OverviewofATCommands forSMS..............................................................................................217
9.2 Detailed DescriptionofATCommands forSMS.............................................................................218
9.2.1 AT+CSMS Selectmessage service.............................................................................218
9.2.2 AT+CPMS Preferredmessagestorage........................................................................219
9.2.3 AT+CMGF SelectSMS messageformat......................................................................221
9.2.4 AT+CSCA SMS service centreaddress.......................................................................222
9.2.5 AT+CSCB Selectcell broadcastmessageindication..................................................223
9.2.6 AT+CSMP Settextmodeparameters..........................................................................225
9.2.7 AT+CSDH Showtextmode parameters.......................................................................226
9.2.8 AT+CNMA Newmessage acknowledgementtoME/TA..............................................227
9.2.9 AT+CNMI New messageindications toTE..................................................................229
9.2.10 AT+CGSMS Selectservice forMO SMSmessages...................................................231
9.2.11 AT+CMGL ListSMS messages frompreferred store..................................................233
9.2.12 AT+CMGR Read message...........................................................................................237
9.2.13 AT+CMGS Sendmessage...........................................................................................241
9.2.14 AT+CMSS Sendmessage fromstorage......................................................................242
9.2.15 AT+CMGW Write messagetomemory........................................................................243
9.2.16 AT+CMGD Delete message.........................................................................................245
9.2.17 AT+CMGMT Change messagestatus.........................................................................246
9.2.18 AT+CMVP Setmessagevalidperiod...........................................................................247
9.2.19 AT+CMGRD Read anddelete message......................................................................248
9.2.20 AT+CMGSEX Send message......................................................................................250
9.2.21 AT+CMSSEX Sendmultimessagesfrom storage.......................................................251
9.2.22 AT+CCONCINDEXReportConcatenatedSMS Index...................................................254
ATCommandsforSerialInterface...............................................................................255
10.1 OverviewofATCommands forSerialInterface............................................................................255
10.2 DetailedDescription ofATCommands forSerialInterface..........................................................255
10.2.1 AT&D SetDTRfunction mode......................................................................................255
10.2.2 AT&C SetDCDfunction mode.....................................................................................256
10.2.3 AT+IPR Set localbaud rate temporarily.......................................................................257
10.2.4 AT+IPREX Setlocal baudrate permanently................................................................258


## Página 32

A76XX Series_AT Command Manual_V1.09
www.simcom.com 31/652
10.2.5 AT+ICF Set controlcharacter framing..........................................................................259
10.2.6 AT+IFC Set localdataflowcontrol...............................................................................260
10.2.7 AT+CSCLK ControlUARTSleep..................................................................................262
10.2.8 AT+CMUX Enablethe multiplexer overthe UART......................................................263
10.2.9 AT+CATR ConfigureURCdestination interface..........................................................264
10.2.10 AT+CFGRI Configure RIpin..................................................................................265
10.2.11 AT+CURCD Configure thedelay timeandnumberofURC.................................267
ATCommandsforHardware........................................................................................269
11.1OverviewofATCommandsforHardware.....................................................................................269
11.2Detailed Description ofATCommands forHardware...................................................................269
11.2.1 AT+CVALARM Lowand highvoltageAlarm................................................................269
11.2.2 AT+CVAUXS Setstate ofthe pinnamedVDD_AUX...................................................271
11.2.3 AT+CVAUXV Setvoltagevalue ofthe pinnamedVDD_AUX.....................................272
11.2.4 AT+CADC ReadADCvalue.........................................................................................273
11.2.5 AT+CADC2 ReadADC2 value.....................................................................................274
11.2.6 AT+CMTE Controlthe module criticaltemperature URCalarm..................................275
11.2.7 AT+CPMVT Low and high voltagePowerOff..............................................................276
11.2.8 AT+CRIIC Read values fromregisterof IICdevicenau8810......................................277
11.2.9 AT+CWIIC Write values toregisterof IICdevice nau8810..........................................278
11.2.10 AT+CBC Readthe voltagevalue ofthe powersupply.........................................279
11.2.11 AT+CPMUTEMP Read thetemperature ofthe module.......................................280
11.2.12 AT+CGDRT Setthe direction ofspecifiedGPIO..................................................280
11.2.13 AT+CGSETV Setthe value ofspecified GPIO.....................................................281
11.2.14 AT+CGGETV Getthe valueof specifiedGPIO....................................................283
11.3UnsolicitedResult Codes..............................................................................................................284
ATCommandsforFileSystem....................................................................................285
12.1 OverviewofATCommands forFileSystem.................................................................................285
12.2 DetailedDescription ofATCommands forFileSystem................................................................286
12.2.1 AT+FSCD Selectdirectoryas currentdirectory...........................................................286
12.2.2 AT+FSMKDIR Makenewdirectory incurrentdirectory...............................................287
12.2.3 AT+FSRMDIR Delete directoryin currentdirectory.....................................................288
12.2.4 AT+FSLS Listdirectories/files in currentdirectory.......................................................289
12.2.5 AT+FSDEL Deletefilein currentdirectory...................................................................291
12.2.6 AT+FSRENAME Renamefile incurrentdirectory.......................................................292
12.2.7 AT+FSATTRI Requestfile attributes............................................................................293
12.2.8 AT+FSMEM Checkthe size ofavailable memory........................................................293
12.2.9 AT+FSCOPY Copy anappointed file...........................................................................294
12.2.10 AT+FSPRESET Moves thelocation ofa file.........................................................296
12.2.11 AT+FSOPEN Openafile.......................................................................................297
12.2.12 AT+FSREAD Readafile.......................................................................................298
12.2.13 AT+FSWRITE Writeafile......................................................................................299
12.2.14 AT+FSSEEK SetafilePointertothe Specified Position......................................300
12.2.15 AT+FSPOSITION Getthe Offset ofaFile Pointer................................................301
12.2.16 AT+FSCLOSE Closea file....................................................................................302


## Página 33

A76XX Series_AT Command Manual_V1.09
www.simcom.com 32/652
ATCommandsforFileTransmission....................................................................................304
13.1 OverviewofATCommands forFileTransmission........................................................................304
13.2 DetailedDescription ofATCommands forFileTransmission......................................................304
13.2.1 AT+CFTRANRX Transferafile toEFS........................................................................304
13.2.2 AT+CFTRANTX TransferafilefromEFS tohost........................................................306
13.2.3 AT+CFTRXBUF Setsthe size ofthe buffertotransferfiles toEFS............................307
ATCommandsforInternetService.............................................................................309
14.1 OverviewofATCommands forInternetService...........................................................................309
14.2 DetailedDescription ofATCommands forInternetService.........................................................309
14.2.1 AT+CHTPSERV SetHTPserver information...............................................................309
14.2.2 AT+CHTPUPDATE Updatingdate timeusing HTPprotocol.......................................310
14.2.3 AT+CNTP Updatesystem time.....................................................................................311
14.3 Command ResultCodes...............................................................................................................312
14.3.1 Descriptionof<err>ofHTP............................................................................................312
14.3.2 Descriptionof<err>ofNTP............................................................................................313
ATCommandsforTCP/IP.............................................................................................314
15.1 OverviewofATCommands forTCP/IP.........................................................................................314
15.2 DetailedDescription ofATCommands forTCP/IP.......................................................................314
15.2.1 AT+NETOPEN StartSocketService............................................................................315
15.2.2 AT+NETCLOSE StopSocketService..........................................................................316
15.2.3 AT+CIPOPEN EstablishConnectionin Multi-SocketMode........................................317
15.2.4 AT+CIPSEND Send datathroughTCPorUDPConnection.......................................320
15.2.5 AT+CIPRXGET Setthe Modeto RetrieveData...........................................................324
15.2.6 AT+CIPCLOSE CloseTCPorUDPSocket.................................................................327
15.2.7 AT+IPADDR Inquire SocketPDPaddress...................................................................329
15.2.8 AT+CIPHEAD Addan IPHeader WhenReceiving Data.............................................330
15.2.9 AT+CIPSRIP Show Remote IPAddressandPort.......................................................331
15.2.10 AT+CIPMODE SetTCP/IPApplication Mode.......................................................332
15.2.11 AT+CIPSENDMODE SetSending Mode..............................................................333
15.2.12 AT+CIPTIMEOUT SetTCP/IPTimeoutValue......................................................334
15.2.13 AT+CIPCCFG Configure ParametersofSocket...................................................335
15.2.14 AT+SERVERSTART StartupTCPSever..............................................................337
15.2.15 AT+SERVERSTOP StopTCPSever....................................................................338
15.2.16 AT+CIPACK QueryTCPConnectionDataTransmittingStatus...........................339
15.2.17 AT+CDNSGIP Query theIPAddressof Given Domain Name.............................341
15.2.18 AT+CSOCKSETPN Setactive PDPcontext'sprofile...........................................342
15.2.19 AT+CTCPKA ConfigureTCPheartbeat................................................................343
15.2.20 AT+CDNSCFG ConfigureDomain NameServer....................................................344
15.2.21 AT+CSOC Setsomefeaturesof thedata service................................................345
15.2.22 AT+CIPCFG Configure Parameters ofTcp...........................................................347
15.2.23 AT+CIPSENDSTRSend HEXString Data..............................................................348
15.3 Command ResultCodes...............................................................................................................350
15.3.1 Descriptionof<err_info>.................................................................................................350
15.3.2 Descriptionof<err>.........................................................................................................350
15.4 UnsolicitedResultCodes..............................................................................................................351


## Página 34

A76XX Series_AT Command Manual_V1.09
www.simcom.com 33/652
ATCommandsforHTTP(S)..........................................................................................352
16.1 OverviewofATCommands forHTTP(S)......................................................................................352
16.2 DetailedDescription ofATCommands forHTTP(S)....................................................................352
16.2.1 AT+HTTPINIT StartHTTPService...............................................................................352
16.2.2 AT+HTTPTERM StopHTTPService............................................................................353
16.2.3 AT+HTTPPARA SetHTTPParameters value..............................................................354
16.2.4 AT+HTTPACTION HTTPMethodAction......................................................................356
16.2.5 AT+HTTPHEAD Readthe HTTPHeaderInformationof ServerResponse................357
16.2.6 AT+HTTPREAD Readthe responseinformation ofHTTPServer..............................358
16.2.7 AT+HTTPDATA InputHTTPData.................................................................................360
16.2.8 AT+HTTPPOSTFILE Send HTTPRequest toHTTP(S)serverby File........................361
16.2.9 AT+HTTPREADFILE Receive HTTPResponseContenttoafile...............................362
16.3 Command ResultCodes...............................................................................................................364
16.3.1 Descriptionof<statuscode>............................................................................................364
16.3.2 Descriptionof<errcode>.................................................................................................365
16.4 UnsolicitedResultCodes..............................................................................................................366
ATCommandsforFTP(S).............................................................................................367
17.1 OverviewofATCommands forFTP(S)........................................................................................367
17.2 DetailedDescription ofATCommands forFTP(S).......................................................................367
17.2.1 AT+CFTPSSTART StartFTP(S)service.......................................................................367
17.2.2 AT+CFTPSSTOP StopFTP(S)Service........................................................................368
17.2.3 AT+CFTPSLOGIN Login toaFTP(S)server................................................................369
17.2.4 AT+CFTPSLOGOUT Logout aFTP(S)server..............................................................371
17.2.5 AT+CFTPSLIST Listthe itemsinthe directory on FTP(S)server................................372
17.2.6 AT+CFTPSMKD Create anewdirectory on FTP(S)server.........................................373
17.2.7 AT+CFTPSRMD Deleteadirectory on FTP(S)server.................................................374
17.2.8 AT+CFTPSCWD Change the currentdirectory onFTP(S)server...............................376
17.2.9 AT+CFTPSPWD Getthe currentdirectory onFTP(S)server......................................377
17.2.10 AT+CFTPSDELE Delete afile onFTP(S)server..................................................378
17.2.11 AT+CFTPSGETFILE Download afilefromFTP(S)server tomodule..................379
17.2.12 AT+CFTPSPUTFILE Uploadafilefrom moduletoFTP(S)server.......................380
17.2.13 AT+CFTPSGET Getafile from FTP(S)servertoserial port.................................382
17.2.14 AT+CFTPSPUT PutafiletoFTP(S)server through serialport............................383
17.2.15 AT+CFTPSSINGLEIP Set FTP(S)datasocketaddress type...............................384
17.2.16 AT+CFTPSSIZE Get the file size onFTP(S)server..............................................386
17.2.17 AT+CFTPSTYPE Setthe transfertype onFTP(S)server.....................................387
17.2.18 AT+CFTPSSLCFG Setthe SSLcontextidforFTPSsession..............................388
17.2.19 AT+CFTPSMODE SetActive orPassiveFTPMode............................................389
17.3 Command ResultCodes...............................................................................................................390
17.3.1 Descriptionof<errcode>.................................................................................................390
17.4 UnsolicitedResultcodes...............................................................................................................391
ATCommandsforMQTT(S).........................................................................................392
18.1 OverviewofATCommands forMQTT(S).....................................................................................392
18.2 DetailedDescription ofATCommands forMQTT(S)...................................................................392
18.2.1 AT+CMQTTSTART StartMQTTservice......................................................................392


## Página 35

A76XX Series_AT Command Manual_V1.09
www.simcom.com 34/652
18.2.2 AT+CMQTTSTOP StopMQTTservice........................................................................393
18.2.3 AT+CMQTTACCQ Acquireaclient...............................................................................394
18.2.4 AT+CMQTTREL Releaseaclient.................................................................................396
18.2.5 AT+CMQTTSSLCFG Setthe SSLcontext(only forSSL/TLSMQTT)........................397
18.2.6 AT+CMQTTWILLTOPIC Inputthe topicof will message.............................................398
18.2.1 AT+CMQTTWILLMSG Inputthe willmessage............................................................399
18.2.2 AT+CMQTTCONNECT Connectto MQTTserver.......................................................400
18.2.3 AT+CMQTTDISC Disconnectfrom server...................................................................402
18.2.4 AT+CMQTTTOPIC Inputthe topic ofpublish message...............................................404
18.2.5 AT+CMQTTPAYLOAD Inputthe publish message......................................................405
18.2.6 AT+CMQTTPUB Publishamessage toserver............................................................406
18.2.7 AT+CMQTTSUBTOPIC Inputthe topic ofsubscribe message...................................408
18.2.8 AT+CMQTTSUB Subscribeamessage toserver........................................................409
18.2.9 AT+CMQTTUNSUBTOPIC Inputthe topicofunsubscribemessage..........................411
18.2.10 AT+CMQTTUNSUB Unsubscribe amessageto server.......................................412
18.2.11 AT+CMQTTCFG Configure theMQTTContext....................................................414
18.3 Command ResultCodes...............................................................................................................416
18.3.1 Descriptionof<err>.........................................................................................................416
18.4 UnsolicitedResultCodes..............................................................................................................417
ATCommandsforSSL.................................................................................................420
19.1 OverviewofATCommands forSSL.............................................................................................420
19.2 DetailedDescription ofATCommands forSSL............................................................................420
19.2.1 AT+CSSLCFG Configure the SSLContext..................................................................420
19.2.2 AT+CCERTDOWN Downloadcertificate intothe module...........................................426
19.2.3 AT+CCERTLIST Listcertificates..................................................................................427
19.2.4 AT+CCERTDELE Deletecertificates............................................................................428
19.2.5 AT+CCHSET Configure the reportmodeofsending andreceiving data....................429
19.2.6 AT+CCHMODE Configure the mode ofsending andreceiving data..........................430
19.2.7 AT+CCHSTART StartSSLservice...............................................................................431
19.2.8 AT+CCHSTOP StopSSLservice.................................................................................432
19.2.9 AT+CCHADDR Getthe IPv4address..........................................................................433
19.2.10 AT+CCHSSLCFG Setthe SSLcontext.................................................................433
19.2.11 AT+CCHCFG Configure the ClientContext..........................................................435
19.2.12 AT+CCHOPEN Connecttoserver........................................................................436
19.2.13 AT+CCHCLOSE Disconnectfrom server.............................................................438
19.2.14 AT+CCHSEND Senddata toserver.....................................................................439
19.2.15 AT+CCHRECV Readthe cacheddata thatreceivedfrom the server.................440
19.2.16 AT+CCERTMOVE Move the certfromfile system tocertcontent.......................443
19.3 Command ResultCodes...............................................................................................................444
19.3.1 Descriptionof<err>.........................................................................................................444
19.4 UnsolicitedResultCodes..............................................................................................................444
ATCommandsforTTS..................................................................................................446
20.1 OverviewofATCommands forTTS.............................................................................................446
20.2 DetailedDescription ofATCommands forTTS............................................................................446
20.2.1 AT+CTTS TTSoperation..............................................................................................446


## Página 36

A76XX Series_AT Command Manual_V1.09
www.simcom.com 35/652
20.2.2 AT+CTTSPARAM SetTTSParametersforYOUNGTONETTS.................................448
20.2.3 AT+CTTSPARAM SetTTSParametersforIFLYTTS.................................................450
20.2.4 AT+CDTAM SetLocal orRemoteAudio Play..............................................................451
ATCommandsforAudio..............................................................................................453
21.1 OverviewofATCommands forAudio...........................................................................................453
21.2 DetailedDescription ofATCommands forAudio..........................................................................453
21.2.1 AT+CCMXPLAY Play audiofile....................................................................................453
21.2.2 AT+CCMXSTOP Stop playing audiofile......................................................................456
21.2.3 AT+CREC Recordaudio File........................................................................................456
21.2.4 AT+CRTSWITCH CloseRingTone..............................................................................460
21.2.5 AT+CRINGSET SetRingFile.......................................................................................461
21.2.6 AT+CCODECSWITCH Switchcodec type...................................................................462
21.2.7 AT+CLDTMF LocalDTMFToneGeneration................................................................463
21.2.8 AT+SIMTONE GenerateSpecificallyTone...................................................................464
21.2.9 AT+STTONE Play SIMToolkitTone.............................................................................466
ATCommandsforFOTA...............................................................................................468
22.1 OverviewofATCommand forFOTA.............................................................................................468
22.2 DetailedDescription ofATCommand forFOTA...........................................................................468
22.2.1 AT+CFOTA StartFOTAservice....................................................................................468
22.2.2 AT+LFOTA StartLocal FotaService..........................................................................470
22.3 UnsolicitedResultCodes..............................................................................................................471
ATCommandsforSCFOTA..........................................................................................472
23.1 OverviewofATCommands forSCFOTA......................................................................................472
23.2 DetailedDescription ofATCommands forSCFOTA....................................................................472
23.2.1 AT+CAPFOTA Start/Close FOTAservice...................................................................472
23.2.2 AT+CSCFOTA Configureparameters and download upgradepackage.....................473
23.3 Command ResultCodes...............................................................................................................475
23.3.1 Command ResultReportCodes.....................................................................................475
23.3.2 Descriptionof<err>.........................................................................................................475
ATCommandsforGNSS..............................................................................................477
24.1 OverviewofATCommands forGNSS..........................................................................................477
24.2 DetailedDescription ofATCommands forGNSS........................................................................477
24.2.1 AT+CGNSSPWR GNSS powercontrolandAP-Flash control.....................................477
24.2.2 AT+CGNSSTST Senddata receivedfrom UART3toNMEAport...............................479
24.2.3 AT+CGPSCOLD Coldstart GPS..................................................................................480
24.2.4 AT+CGPSWARM WarmstartGPS..............................................................................481
24.2.5 AT+CGPSHOT Hot startGPS......................................................................................482
24.2.6 AT+CGNSSIPR Configure the baudrate ofUART3and GPSmodule.......................482
24.2.7 AT+CGNSSMODE Configure GNSSsupportmode....................................................483
24.2.8 AT+CGNSSNMEA Configure NMEAsentence type....................................................485
24.2.9 AT+CGPSNMEARATE SetNMEAoutput rate.............................................................487
24.2.10 AT+CGPSFTM StartGPStestmode....................................................................488
24.2.11 AT+CGPSINFO GetGPSfixedposition information............................................489
24.2.12 AT+CGNSSINFO GetGNSS fixedposition information.......................................491


## Página 37

A76XX Series_AT Command Manual_V1.09
www.simcom.com 36/652
24.2.13 AT+CGNSSCMD Send command toGNSS.........................................................493
24.2.14 AT+CGNSSPORTSWITCH Selectthe output portforNMEAsentence..............494
24.2.15 AT+CAGPS GetAGPSdata fromtheAGNSS server forassistedpositioning....495
24.2.16 AT+CGNSSPRODGetthe production ofGNSS.....................................................496
ATCommandsforWIFI.................................................................................................497
25.1 OverviewofATCommands forWIFI.............................................................................................497
25.2 DetailedDescription ofATCommands forWIFI...........................................................................497
25.2.1 AT+CWSTASCAN ScanWIFInetwork.........................................................................497
25.2.2 AT+CWSTASCANEXScan WIFInetwork extensioncommand....................................499
25.2.3 AT+CWSTASCANSYNAsynchronouscontrolcommand ofscanwifinetwork.............500
25.2.4 AT+CWMAPOpen/Close WIFI.......................................................................................502
25.2.5 AT+CWSSIDSSIDsetting..............................................................................................502
25.2.6 AT+CWAUTHAuthentication setting...............................................................................503
25.2.7 AT+CWMOCH80211modeand channelsetting...........................................................505
25.2.8 AT+CWISOClientisolation setting.................................................................................506
25.2.9 AT+CWMACADDRGetMACaddress............................................................................507
25.2.10 AT+CWCLICNTGet clientnumberconnected tothe WIFI.....................................507
ATCommandsforBluetooth........................................................................................509
26.1 OverviewofATCommands forBluetooth.....................................................................................509
26.2 DetailedDescription ofATCommands forBLE............................................................................510
26.2.1 AT+BLEPOWER Poweron/offBle Device...................................................................511
26.2.2 AT+BLESTATUS Inquiry CurrentBLEConnectStatus................................................512
26.2.3 AT+BLEHOST Inquiry andSetHostDevice Name.....................................................513
26.2.4 AT+BLEADDRInquiry and SetDeviceAddress.............................................................513
26.2.5 AT+BLESREG RegisterGATTServer..........................................................................514
26.2.6 AT+BLESDREG DeregisterGATTServer....................................................................515
26.2.7 AT+BLESSAD Add aService.......................................................................................516
26.2.8 AT+BLESSRM RemoveaService...............................................................................518
26.2.9 AT+BLESSCAD AddaCharacteristictoan ExistedService.......................................519
26.2.10 AT+BLESSCRM Remove aCharacteristic...........................................................520
26.2.11 AT+BLESSDAD AddaDescriptortoan ExistedCharacteristic...........................521
26.2.12 AT+BLESSDRM Remove aDescriptor.................................................................523
26.2.13 AT+BLESSSTART StartaServer.........................................................................524
26.2.14 AT+BLESSSTOP Stopa Server...........................................................................525
26.2.15 AT+BLESSETADVDATA SetAdverting Package.................................................527
26.2.16 AT+BLESCLRADVDATA ClearAdverting package..............................................529
26.2.17 AT+BLESSETADVPARAM SetAdverting Paramters...........................................529
26.2.18 AT+BLESLSTART StartAdvertising......................................................................532
26.2.19 AT+BLESLSTOP StopAdvertising........................................................................534
26.2.20 AT+BLEADV SetAdverting Parameters...............................................................536
26.2.21 AT+BLEDISCONN DisconnectBLE Connection..................................................537
26.2.22 AT+BLESIND Sendan Indication toaClient........................................................538
26.2.23 AT+BLESNTY Send an noticeto aClient.............................................................539
26.2.24 AT+BLESRSP Send aResponsetoaClient’SRead orWriteOperation............540
26.2.25 +BLESRREQ Read requestreceived fromremote device..................................540


## Página 38

A76XX Series_AT Command Manual_V1.09
www.simcom.com 37/652
26.2.26 +BLESWREQ Write requestreceived fromremote device..................................541
26.2.27 +BLESCON NotifyWhenaConnection’sStatus Change....................................542
26.2.28 +BLEMTU Exchangemturequest received from remotedevice.........................543
26.2.29 AT+BLECREG RegisterGATTClient...................................................................543
26.2.30 AT+BLECDREG DeregisterGATTClient.............................................................544
26.2.31 AT+BLESCAN Scan SurroundingBLEDevice.....................................................545
26.2.32 +BLESCANRST NotifyWhen FindaBLEDevice................................................546
26.2.33 AT+BLECGDT GetDeviceType...........................................................................547
26.2.34 AT+BLECCON ConnectGATTClienttoRemote LE/Dual-mode Device............547
26.2.35 AT+BLECDISC DisconnectGATTClient toRemoteLE/Dual-mode Device.......548
26.2.36 AT+BLECSS Search Peer’sService.....................................................................549
26.2.37 AT+BLECGC SearchPeer’sCharacteristic..........................................................550
26.2.38 AT+BLECGD SearchPeer’sCharacteristic Descriptor........................................551
26.2.39 AT+BLECRC ReadPeer’s Characteristic.............................................................552
26.2.40 AT+BLECWC WritePeer’sCharacteristic............................................................553
26.2.41 AT+BLECRD ReadPeer’s Characteristic Descriptor...........................................554
26.2.42 AT+BLECWD WritePeer’sCharacteristicDescriptor..........................................555
26.2.43 +BLECNTY NotifyWhen GetaNoticationfromPeer’sDevice...........................555
26.2.44 +BLECIND NotifyWhenGetaIndication fromPeer’sDevice.............................556
26.3 DetailedDescription ofATCommands forBT..............................................................................556
26.3.1 AT+BTPOWER Open/CloseBTDevice.......................................................................557
26.3.2 AT+BTHOST Get/SetBTDeviceName.......................................................................558
26.3.3 AT+BTADDR Get/SetBTDeviceAddress...................................................................559
26.3.4 AT+BTSCAN Scan BTdevice......................................................................................560
26.3.5 AT+BTIOCAP Get/SetBTDeviceIOCapalibity..........................................................561
26.3.6 AT+BTPAIR PairWithOtherBTDevice.......................................................................562
26.3.7 AT+BTUNPAIR UnpairWithThePairedBTDevice.....................................................565
26.3.8 AT+BTPAIRED GetPaired BTDevice.........................................................................566
26.3.9 AT+BTSPPSRV Active/Deactive LocalSPPService...................................................566
26.3.10 AT+BTSPPPROF GetRemote BTDeviceSPPServiceStatus..........................568
26.3.11 AT+BTSPPCONN Establish/ReleaseSPPConnection.......................................569
26.3.12 AT+BTSPPSEND SPPSendData........................................................................570
26.3.13 +BTSPPRECV SPPReceiveData.......................................................................572
26.4 Description of<error_code>..........................................................................................................573
ATCommandsforCTBURST.......................................................................................574
27.1 OverviewofATCommands forCTBURST...................................................................................574
27.2 DetailedDescription ofATCommands forCTBURST(CAT4)......................................................574
27.2.1 AT+CTBURST TheRFTXBurstTest..........................................................................574
27.3 DetailedDescription ofATCommands forCTBURST(CAT1&CAT4)..........................................577
27.3.1 AT+CTBURST TheTX/RXBurstTest..........................................................................577
SummaryofERRORCodes.........................................................................................583
28.1 VerboseCodesand Numeric Codes............................................................................................583
28.2 ResponseString ofAT+CEER......................................................................................................583
28.3 Summaryof CMEERROR Codes................................................................................................588
28.4 Summaryof CMSERROR Codes................................................................................................590


## Página 39

A76XX Series_AT Command Manual_V1.09
www.simcom.com 38/652
ATCommandsforWEBSOCKET.................................................................................592
29.1 OverviewofATCommands forwebsocket...................................................................................592
29.2 DetailedDescription ofATCommands forwebsocket(S)............................................................592
29.2.1 AT+WSSTART Startwebsocketservice.......................................................................592
29.2.2 AT+WSSTOP Stopwebsocketservice........................................................................593
29.2.3 AT+WSCONNECT Connectto websocketserver.......................................................594
29.2.4 AT+WSDISC Disconnectfromserver..........................................................................595
29.2.5 AT+WSSEND Publishamessage toserver................................................................597
29.3 Command ResultCodes...............................................................................................................598
29.3.1 Descriptionof<err>.........................................................................................................598
29.4 UnsolicitedResultCodes..............................................................................................................598
ATCommandsforLWM2M...........................................................................................599
30.1 OverviewofATCommands forLWM2M.......................................................................................599
30.2 DetailedDescription ofATCommands forLWM2M.....................................................................599
30.2.1 AT+LWSTART StartLWM2M service...........................................................................599
30.2.2 AT+LWSTOP StopLWM2M Service.............................................................................600
30.2.3 AT+LWCNFConfig the LWM2M......................................................................................601
30.2.4 AT+LWOPEN RegistertoaLWM2M server.................................................................602
30.2.5 AT+LWCLOSE Deregisterto aLWM2M server...........................................................603
30.2.6 AT+LWADDOBJ Add aLWM2M object........................................................................604
30.2.7 AT+LWDELOBJ Delete aLWM2M object....................................................................605
30.2.8 AT+LWREADRSP Send readresponseto LWM2Mserver.........................................605
30.2.9 AT+LWWRITERSP Send responsetoaLWM2M server.............................................607
30.2.10 AT+LWEXECUTERSP Send responseto LWM2Mserver...................................608
30.2.11 +LWREAD LWM2M client responseofLWM2M serveroperateread..................609
30.2.12 +LWWRITE LWM2M clientresponse ofLWM2Mserver operatewrite................609
30.2.13 +LWEXECUTE LWM2M client responseofLWM2M serveroperateexecute.....610
30.3 Command ResultCodes...............................................................................................................611
ATCommandsforCOAP..............................................................................................612
31.1 OverviewofATCommands forCOAP..........................................................................................612
31.2 DetailedDescription ofATCommands forCOAP........................................................................612
31.2.1 AT+COAPSTART Active PDP......................................................................................612
31.2.2 AT+COAPSTOP Deactive PDP....................................................................................613
31.2.3 AT+COAPOPEN OpenaCOAPserver.......................................................................614
31.2.4 AT+COAPCLOSE Close aCOAPserver.....................................................................615
31.2.5 AT+COAPHEAD Configthe head ofCOAP.................................................................615
31.2.6 AT+COAPOPTION Configthe optionof COAP...........................................................616
31.2.7 AT+COAPSEND Send COAPmessage tothe server.................................................617
31.2.8 AT+COAPSENDTX Send COAP message to the server by transparent transmission
618
31.2.9 +COAPRECV Receiveresponsemessage fromserver................................................619
ATCommandsforSMTPS............................................................................................620
32.1 OverviewofATCommands forSMTPS........................................................................................620
32.2 DetailedDescription ofATCommands forSMTPS......................................................................620


## Página 40

A76XX Series_AT Command Manual_V1.09
www.simcom.com 39/652
32.2.1 AT+CSMTPSCFG Config the SMTPcontext...............................................................620
32.2.2 AT+CSMTPSSRV SetSMTPserver address and portnumber..................................622
32.2.3 AT+CSMTPSAUTH SMTPserverauthentication........................................................623
32.2.4 AT+CSMTPSFROM Senderaddressand name.........................................................624
32.2.5 AT+CSMTPSRCPT Recipientaddress andname(TO/CC/BCC)...............................626
32.2.6 AT+CSMTPSSUB E-mailsubject.................................................................................627
32.2.7 AT+CSMTPSBODY E-mailbody..................................................................................628
32.2.8 AT+CSMTPSBCH E-mail bodycharacter set..............................................................629
32.2.9 AT+CSMTPSFILE Selectattachment..........................................................................630
32.2.10 AT+CSMTPSSEND Initiate session andsende-mail..........................................632
32.2.11 AT+CSMTPSSTOP Force tostop sendinge-mail................................................632
32.2.12 AT+CSMTPSCLEAN Clean mail contentand setting..........................................633
32.3 Summaryof resultcodes forSMTPS...........................................................................................633
ATCommandsforTelecomself-registration..............................................................635
33.1 OverviewofATCommands forTelecom self-registration.............................................................635
33.2 DetailedDescription ofATCommands forTelecom self-registration...........................................635
33.2.1 AT+HWVER Hardwareversion numberquery.............................................................635
33.2.2 AT+AUTOREGCFG Datadomainself-registration statusquery.................................636
ATCommandsforPSM.................................................................................................638
34.1 OverviewofATCommands forPSM............................................................................................638
34.2 DetailedDescription ofATCommands forpsm............................................................................638
34.2.1 AT*COMCFG Set cat1_1bis.........................................................................................638
34.2.2 AT+CPSMS Powersaving modesetting......................................................................639
34.2.3 AT+MEDCR Set/GetMEDATACOMM RESERVER...................................................641
ATCommandsforUSB.................................................................................................644
35.1 OverviewofATCommands forUSB.............................................................................................644
35.2 DetailedDescription ofATCommands forusb.............................................................................644
35.2.1 AT+DIALMODE ConfigUSBNETnetwork...................................................................644
35.2.2 AT$MYCONFIG SetRNDIS/ECM Mode......................................................................645
35.2.3 AT+USBNETIPChange RNDIS/ECM PrivateIPtoPublic IP........................................646
35.2.4 AT+USBNETMACSetUSBNETMACAddress..............................................................647
ATCommandsforJammingDetect..............................................................................649
36.1 OverviewofATCommands forJammingDetect...........................................................................649
36.2 DetailedDescription ofATCommands forJammingDetect........................................................649
36.2.1 AT+SJDR Enable Jamming Detect..............................................................................649
36.2.2 AT+SJDCFG ConfigJamming Detect..........................................................................650
THISDOCUMENTISAREFERENCEGUIDETOALLTHEATCOMMANDS.


## Página 41

A76XX Series_AT Command Manual_V1.09
www.simcom.com 40/652
Introduction
1.1 Scopeofthedocument
This documentpresents theATCommand SetforSIMComA76XXSeries.
More information aboutthe SIMCom Modulewhichincludesthe Software Versioninformationcan be
retrievedby the commandATI.Inthisdocument,ashortdescription,thesyntax, thepossiblesettingvalues
andresponses, andsomeExamples ofATcommandsare presented.
Priorto usingthe Module, pleasereadthis documentandthe VersionHistory toknowthe differencefrom
the previous document.
Inordertoimplementcommunication successfully between CustomerApplicationand theModule, itis
recommended touse theATcommands inthis document,butnotto usesomecommands which arenot
includedin thisdocument.
1.2 Relateddocuments
[1] A76XXSeries_TCPIP_Application_Note
[2] A76XXSeries_HTTP(S)_Application_Note
[3] A76XXSeries_FTP(S)_Application_Note
[4] A76XXSeries_MQTT(S)_Application_Note
[5] A76XXSeries_SSL_Application_Note
[6] A76XXSeries_AUDIO_Application_Note
Youcanvisitthe SIMCom Website formore informationby the following link:
http://www.simcom.com


## Página 42

A76XX Series_AT Command Manual_V1.09
www.simcom.com 41/652
1.3 TermsandAbbreviations
Forthe purposesof the presentdocument,the following abbreviationsapply:
Abbreviation Description
AT ATtention; the two-character abbreviation is used to start acommand
linetobe sentfromTE/DTEtoTA/DCE
DCE DataCommunication Equipment
DCS DigitalCellularNetwork
DTE DataTerminalEquipment
DTMF DualTone Multi–Frequency
EDGE EnhancedDataGSM Environment
EGPRS EnhancedGeneral PacketRadio Service
GPIO General–Purpose Input/Output
GPRS GeneralPacketRadio Service
GSM GlobalSystem forMobile communications
HSDPA HighSpeedDownlink PacketAccess
HSUPA HighSpeedUplink PacketAccess
I2C Inter–Integrated Circuit
IMEI InternationalMobile station EquipmentIdentity
IMSI InternationalMobile SubscriberIdentity
ME MobileEquipment
MO Mobile–Originated
MS MobileStation
MT Mobile–Terminated;MobileTermination
PCS PersonalCommunication System
PDU ProtocolDataUnit
PIN PersonalIdentification Number
PUK PersonalUnlock Key
SIM SubscriberIdentity Module
SMS ShortMessageService
SMS-SC ShortMessageService ServiceCenter
TA TerminalAdaptor;e.g.adata card(equal toDCE)
TE TerminalEquipment; e.g.acomputer(equal toDTE)
UE UserEquipment
UMTS UniversalMobileTelecommunications System
USIM UniversalSubscriberIdentity Module
WCDMA WidebandCode DivisionMultipleAccess
FTP FileTransferProtocol


## Página 43

A76XX Series_AT Command Manual_V1.09
www.simcom.com 42/652
1.4 DefinitionsandConventions
1. Definitions
Forthe purposes ofthe presentdocument,the following syntacticaldefinitions apply:
 <CR> Carriage return character.
<LF> Linefeedcharacter.
<…> Nameenclosed inangle brackets isasyntacticalelement.Brackets themselvesdo not
appearinthe commandline.
[…] OptionalsubparameterofATcommand oran optionalpartofTAinformationresponseis
enclosedin square brackets.Bracketsthemselves do notappearinthe command line.If
subparameteris notgiven,its valueequalsto its previous valueorthe recommended
defaultvalue.
underline Underlinedanddefined subparametervalue isthe recommended defaultsetting or
factory setting.
 ParameterSaving Mode
NO_SAVE: Theparameterof the currentATcommandwill belostifmoduleisrebooted or currentAT
command doesn'thave parameter.
AUTO_SAVE:Theparameterof thecurrentATcommand will bekeptinNVRAM automatically and
take ineffectimmediately,anditwon'tbe lostifmodule isrebooted.
AUTO_SAVE_REBOOT:The parameterofthe currentATcommand willbe keptinNVRAM
automatically andtake ineffectafter reboot,and itwon'tbelostifmoduleis rebooted.
AT&W_SAVE:Theparameterofthe currentATcommandwillbe keptinusersetting_save.nvm by
sending the commandof "AT&W".
 Max ResponseTime
Max responsetimeisestimated maximumtimetogetresponse,the unitis seconds.
2. DocumentConventions
 Generally,the characters<CR>and <LF>are intentionally omitted throughoutthis document.
HTTP HyperTextTransferProtocol
RTC RealTimeClock
URC UnsolicitedResultCode


## Página 44

A76XX Series_AT Command Manual_V1.09
www.simcom.com 43/652
 Ifcommand responseisERROR, notlistthe ERRORresponse inside commandsyntax.
ATcommandsand responses infigures may benotfollowing above conventions.
1.5 ATInterfaceSynopsis
1.5.1 InterfaceSettings
Between CustomerApplicationandthe Module, standardized RS–232 interface isusedforthe
communication,anddefaultvalues forthe interfacesettings as following:
115200bps,8bitdata,no parity, 1bitstop,no datastream control.
1.5.2 ATCommandsSyntax
The"AT"or "at"or"aT"or"At"prefixmustbe includedatthe beginning ofeachcommand line(exceptA/
and+++),and the character<CR>is usedto finishacommand linesoasto issuethe command lineto the
module. Itisrecommended thatacommand lineonly includesacommand.
WhenCustomerApplication issuesaseries ofATcommandson separate commandlines,leaveapause
betweentheprecedingandthefollowingcommanduntilinformationresponsesorresultcodesareretrieved
by CustomerApplication,forExamples,"OK"is appeared.Thisadviceavoids too manyATcommandsare
issuedatatimewithout waitingforaresponseforeachcommand.
TheATCommandsetimplementedbyA7600Seriesisacombinationof3GPPTS27.005,3GPPTS27.007
andITU-Trecommendation V.25terand theATcommands developedby SIMCom.
Inthe presentdocument,ATcommands are dividedinto threecategories: Basic Command,SParameter
Command,and Extended Command.
1. Basic Command
Theformatof BasicCommand is "AT<x><n>"or"AT&<x><n>",where "<x>"is the commandname,and
NOTE


## Página 45

A76XX Series_AT Command Manual_V1.09
www.simcom.com 44/652
"<n>"is/are the parameter(s)forthe basiccommand whichisoptional.AnExamplesof BasicCommand is
"ATE<n>",whichinformstheTA/DCEwhetherreceivedcharacters shouldbe echoedbackto theTE/DTE
according tothe value of"<n>";"<n>" isoptionalandadefaultvalue willbe usedifomitted.
2. SParametersyntax
Theformatof SParameterCommand is "ATS<n>=<m>","<n>"is the index oftheS–registertoset,and
"<m>"isthe value toassigntoit. "<m>"is optional;inthis case,the format is"ATS<n>",andthen adefault
value isassigned.
3. ExtendedSyntax
TheExtendedCommand hasseveral formats,as following table list:
Table1:TypesofATcommandsandresponses
TestCommand
AT+<x>=?
Themobile equipmentreturnsthe list ofparameters andvalue
rangessetwith the corresponding WriteCommand orby internal
processes.
Read Command
AT+<x>?
Thiscommand returnsthe currently setvalue ofthe parameteror
parameters.
WriteCommand
AT+<x>=<…> Thiscommand setsthe user-definableparametervalues.
Execution Command
AT+<x>
Theexecution commandreadsnon-variable parameters affected
byinternal processes inthe GSM engine.
Thecharacter"+"betweentheprefix"AT"andcommandnamemaybereplacedbyothercharacter.For
Examples,using "#"or"$"instead of"+".
4. CombiningAT commandsonthe sameCommandline
YoucanenterseveralATcommandsonthe sameline.Inthiscase,you do notneed totype the "AT"or"at"
prefixbefore every command.Instead,you only need type "AT"or"at"the beginningof the commandline.
Pleasenotetouseasemicolonasthecommanddelimiterafteranextendedcommand;inbasicsyntaxorS
parametersyntax, the semicolonneed notenter,forExamples:
ATE1Q0S0=1S3=13V1X4;+IFC=0,0;+IPR=115200.
TheCommand line buffercanaccept amaximumof 3071characters (counted fromthe firstcommand
without "AT"or"at"prefix).Ifthe charactersenteredexceeded this numberthen none ofthe Commandwill
executedandTAwillreturn "ERROR".
5. EnteringsuccessiveATcommandsonseparatelines
Whenyou need toenteraseries ofATcommands on separatelines,pleaseNotethatyou need towaitthe
finalresponse(forExamplesOK,CME error,CMSerror)oflastATCommand you entered beforeyou enter
NOTE


## Página 46

A76XX Series_AT Command Manual_V1.09
www.simcom.com 45/652
the nextATCommand.
1.5.3 Supportedcharactersets
TheA7600 SeriesATCommand interfacedefaults tothe IRAcharacterset.TheA7600Seriessupports the
following charactersets:
GSM format
UCS2
IRA
Thecharactersetcan besetand interrogated usingthe "AT+CSCS"Command (3GPPTS27.007).The
charactersetis definedin GSMspecification 3GPPTS27.005.
Thecharactersetaffects transmissionand reception ofSMS andSMS CellBroadcastmessages,the entry
anddisplay ofphone book entries textfield and SIMApplicationToolkit alphastrings.


## Página 47

A76XX Series_AT Command Manual_V1.09
www.simcom.com 46/652
AT Commands According to
V.25TER
2.1 OverviewofATCommandsAccordingtoV.25TER
Command Description
ATD Mobileoriginated call todialanumber
ATA Callanswer
ATH Disconnect existingcall
ATS0 Automatic answerincoming call
+++ Switchfrom datamodeto commandmode
ATO Switchfrom commandmodetodata mode
ATI Displayproduct identificationinformation
ATE Enable commandecho
AT&V Displaycurrentconfiguration
ATV Setresultcodeformatmode
AT&F Setallcurrentparameters tomanufacturerdefaults
ATQ SetResultCode PresentationMode
ATX SetCONNECTResultCode Forma
AT&W Save the usersetting toME
ATZ Restore the usersettingfrom ME
AT+CGMI Requestmanufactureridentification
AT+CGMM Requestmodel identification
AT+CGMR Requestrevision identification
AT+CGSN Requestproduct serialnumberidentification
AT+CSCS SelectTEcharacterset
AT+GCAP Requestoverall capabilities
2.2 DetailedDescriptionofATCommandsforV.25TER


## Página 48

A76XX Series_AT Command Manual_V1.09
www.simcom.com 47/652
2.2.1 ATDMobile originatedcalltodialanumber
This commandis used tolistcharacters thatmay beused inadialling string formakingacallorcontrolling
supplementaryservices.
ATDMobile originatedcalltodialanumber
Execution Command
ATD<n>[<mgsm>][;]
Response
Originate avoicecall successfully:
OK
VOICE CALL:BEGIN
Originate adatacall successfully:
CONNECT[<text>]
Originate acallunsuccessfullyduring command execution:
ERROR
Originate acallunsuccessfullyforfailedconnection recovery:
NOCARRIER
Originate acallunsuccessfullyforerrorrelated tothe MT:
+CMEERROR: <err>
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<n> String ofdialing digitsand optionallyV.25ter modifiersdialingdigits:
0-9,*, #,+,A,B,C
Following V.25ter modifiers are ignored:
,(comma),T,P,!,W,@
Emergencycall:
<n> Standardized emergency number112(no SIM needed)
<mgsm> String ofGSM modifiers:
I ActivesCLIR(Disables presentation of own numberto calledparty)
i Deactivates CLIR(Enable presentation ofown numberto called
party)
G ActivatesClosed UserGroupinvocation forthis callonly
g Deactivates Closed UserGroupinvocation forthiscall only
<;> Theterminationcharacter";"ismandatorytosetupvoicecalls.Itmust
notbe usedfordata andfaxcalls.
<text> CONNECTresult code string;the string formats pleasereferATX


## Página 49

A76XX Series_AT Command Manual_V1.09
www.simcom.com 48/652
command.
<err> Servicefailureresultcodestring;thestringformatspleaserefer+CME
ERRORresult code andAT+CMEE command.
Examples
ATD10086;
OK
VOICE CALL:BEGIN
1.Supportseveral"P"or"p"intheDTMFstringbutthevalidauto-sendingDTMFaftercharacters"P"or
"p"shouldnotbe more than29.
2.Auto-sending DTMF aftercharacter"P"or"p"shouldbeASCIIcharacterin theset0-9, *,#.
2.2.2 ATA Callanswer
Thiscommandisusedtomakeremotestationtogooff-hook,e.g.answeranincomingcall.Ifthereisnoan
incomingcall andentering this commandtoTA,it will bereturn "NOCARRIER" toTA.
ATA Callanswer
Execution Command
ATA
Response
1)Forvoice call:
OK
VOICECALL:BEGIN
2)Fordata call,andTAswitches todata mode:
CONNECT
3)No connectionorno incoming call:
NOCARRIER
ParameterSaving Mode -
Max ResponseTime -
Reference -
Examples
NOTE


## Página 50

A76XX Series_AT Command Manual_V1.09
www.simcom.com 49/652
ATA
OK
VOICE CALL:BEGIN
Ifthere isan incomingcall andnotconnected,unsolicited result codeRING willreporteverysix
seconds.
2.2.3 ATH Disconnectexistingcall
This commandis used todisconnectexisting call.Before usingATHcommand tohang up avoicecall,it
mustsetAT+CVHU=0.Otherwise,ATHcommandwillbeignored and"OK"responseis givenonly.
This commandis also usedto disconnectPS datacall,andin thiscaseitdoesn’tdependon the valueof
AT+CVHU.
ATH Disconnectexistingcall
Execution Command
ATH
Response
IfAT+CVHU=0:
OK
VOICECALL:END:<time>
ParameterSaving Mode -
Max ResponseTime -
Reference -
Examples
AT+CVHU=0
OK
ATH
OK
VOICE CALL:END: 000017
NOTE


## Página 51

A76XX Series_AT Command Manual_V1.09
www.simcom.com 50/652
2.2.4 ATS0 Automaticanswerincomingcall
TheS-parametercommandcontrolstheautomaticansweringfeatureoftheModule.Ifsetto000,automatic
answeringis disabled,otherwise itcauses theModule toanswerwhen the incoming callindication
(RING)hasoccurred the numberof timesindicatedby the specified value;and the setting will notbe stored
upon power-off,i.e.the defaultvaluewillbe restoredafterrestart.
ATS0 Automaticanswerincomingcall
Read Command
ATS0?
Response
1)
<n>
OK
2)
ERROR
WriteCommand
ATS0=<n>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AT&W_SAVE
Max ResponseTime -
Reference -
DefinedValues
<n> 000 Automaticansweringmode isdisable.
001–255 Enable automatic answeringon the ring numberspecified.
Examples
ATS0=003
OK
ATS0?
000
OK
NOTE


## Página 52

A76XX Series_AT Command Manual_V1.09
www.simcom.com 51/652
TheS-parametercommandis effectiveon voice calland datacall.
If<n>is settoo high,the remoteparty may hang upbefore the callcan beanswered automatically.
2.2.5 +++ Switchfromdatamodetocommandmode
This commandis only availableduring aconnectingPSdata call.The+++charactersequencecausesthe
TAtocancelthe data flowovertheATinterface and switchto Command Mode.This allows to enterAT
commandswhile maintaining the dataconnection tothe remote device.
+++ Switchfromdatamodetocommandmode
Execution Command
+++
Response
OK
ParameterSaving Mode -
Max ResponseTime -
Reference -
Examples
+++
OK
Topreventthe +++escapesequence frombeingmisinterpreted as data,itmustbepreceded and
followed by apause ofatleast 1000 milliseconds,andthe intervalbetween two ‘+’charactercan’t
exceed 900 milliseconds.
2.2.6 ATO Switchfromcommandmodetodatamode
ATOis thecorresponding commandtothe +++escapesequence.Whenthere is aPS datacall connected
andtheTAis inCommand Mode,ATOcauses theTAtoresumethe data andtakesback toDataMode.
ATO Switchfromcommandmodetodatamode
NOTE


## Página 53

A76XX Series_AT Command Manual_V1.09
www.simcom.com 52/652
Execution Command
ATO
Response
1)TA/DCEswitches toDataModefrom CommandMode:
CONNECT[<baud rate>]
2)Ifconnectionis notsuccessfully resumed:
NOCARRIER
3)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
Examples
ATO
CONNECT115200
2.2.7 ATI Displayproductidentificationinformation
This commandis used torequestthe productinformation, whichconsistsofmanufactureridentification,
modelidentification,revisionidentification,InternationalMobilestationEquipmentIdentity(IMEI)andoverall
capabilitiesofthe product.
ATI Displayproductidentificationinformation
Execution Command
ATI
Response
Manufacturer:<manufacturer>
Model:<model>
Revision:<revision>
IMEI:<sn>
+GCAP:listof <name>s
OK
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<manufacturer> Theidentification ofmanufacturer.
<model> Theidentification ofmodel.


## Página 54

A76XX Series_AT Command Manual_V1.09
www.simcom.com 53/652
<revision> Therevision identification offirmware.
<sn> Serialnumberidentification,whichconsists ofasingle linecontaining
IMEI (International Mobile station EquipmentIdentity)number.
<name> Listofadditionalcapabilities:
+CGSM GSM function issupported
+FCLASS FAX function issupported
+DS Datacompressionis supported
+ES Synchronous data modeis supported.
+CIS707-A CDMA data servicecommand set
+CIS-856 EVDOdata servicecommand set
+MS MobileSpecific commandset
Examples
ATI
Manufacturer:INCORPORATED
Model:A7600C
Revision:A7600C_V1.0
IMEI:351602000330570
+GCAP:+CGSM,+FCLASS,+DS
OK
2.2.8 ATE Enablecommandecho
This commandsets whether ornottheTAechoescharacters.
ATE Enablecommandecho
Execution Command
ATE[<value>]
Response
1)if formatisright
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 120000ms
Reference -
DefinedValues
<value> 0 Echo modeoff


## Página 55

A76XX Series_AT Command Manual_V1.09
www.simcom.com 54/652
1 Echo modeon
Examples
ATE1
OK
ATE0
OK
2.2.9 AT&V Displaycurrentconfiguration
This commandreturns someofthe baseconfiguration parameterssettings.
AT&V Displaycurrentconfiguration
Execution Command
AT&V
Response
1)
<TEXT>
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<TEXT> All relativeconfigurationinformation.
Examples
AT&V
&C:1;&D:0;&F:0;&W:0;E:1;Q:0;V:1;X:0;Z:0;S0:0;S2:43;S3:13;S4:10;S5:8;S6:2;
S7:1;S8:2;S9:6;S10:7;S11:63;S30:10;+FCLASS:0;+IPR:115200;+IPREX:115200;
+CSCS:IRA; +CREG:0;+CGREG: 0;+CEREG:0; +CGDCONT:
(1,"IP","ctnet.mnc011.mcc460.gprs","10.13.204.244",0,0,,,,),(2,"IP","CMNET");+CGDSCONT:;
+CGEQMIN:(1,0,256000,256000,256000,256000,2,1520,"0E0,6E8,",3,150,0,0,0);
+CGQMIN:(1,3,4,5,1,1),(2,3,4,5,1,1); +CGEREP:(2,0); +CGCLASS:"A"; +CGACT:(1,1),(2,0);


## Página 56

A76XX Series_AT Command Manual_V1.09
www.simcom.com 55/652
+CGAUTH:(1,0),(2,0);+CPBS:"SM";+CMEE:2;+CFUN:1;+CMGF:0;+CSCA:
("+316540942000",145);+CSMP:33,167,0,0;+CSDH:0;+CPMS:
"SM",0,50,"SM",0,50,"SM",0,50;
OK
2.2.10 ATV Setresultcodeformatmode
This parametersetting determinesthe contentsofthe headerandtrailer transmitted with resultcodesand
informationresponses.
ATV Setresultcodeformatmode
WriteCommand
ATV[<value>]
Response
1)if <value>=0
0
2)If<value>=1
OK
ParameterSaving Mode AT&W_SAVE
Max ResponseTime -
Reference -
DefinedValues
<value> 0 Information response: <text><CR><LF>
Shortresult codeformat: <numeric code><CR>
1 Information response: <CR><LF><text><CR><LF>
Long resultcodeformat:<CR><LF><verbose
code><CR><LF>
Examples
ATV1
OK
Incase ofusingThiscommand withoutparameter <value>willbe setto1.
NOTE


## Página 57

A76XX Series_AT Command Manual_V1.09
www.simcom.com 56/652
2.2.11 AT&F Setallcurrentparameterstomanufacturerdefaults
This commandis used tosetallcurrent parametersto the manufacturer defined profile.
AT&F Setallcurrentparameterstomanufacturerdefaults
Execution Command
AT&F[<value>]
Response
OK
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<value> 0 SetsometemporaryTAparameters tomanufacturerdefaults.
Thesetting afterpower onorresetis sameas value0.
defaultvalues
TAparameters
AT+CATR
AT+CNMP
AT+CTZU
AT+CVAUXV
VALUE
0
2
0
2850
Examples
AT&F
OK
ListofparametersresettomanufacturerdefaultcanbefoundinDefinedValues,factorydefaultsettings
restorable withAT&F[<value>].
2.2.12 ATQ SetResultCodePresentationMode
SpecifywhethertheTAtransmitsany resultcodetotheTEornot.Text information transmitted inresponse
NOTE


## Página 58

A76XX Series_AT Command Manual_V1.09
www.simcom.com 57/652
is notaffected bythis setting
ATQ SetResultCodePresentationMode
WriteCommand
ATQ<n>
Response
1)If<n>=0:
OK
2)If<n>=1:
NoResponses
Execution Command
ATQ
Response
1)Setdefaultvalue:0
OK
2)
NoResponses
ParameterSaving Mode AT&W_SAVE
Max ResponseTime -
Reference -
DefinedValues
<n> 0 DCEtransmits resultcode
1 DCEnottransmitsresult code
Examples
ATQ0
OK
ATQ
OK
2.2.13 ATX SetCONNECTResultCodeFormat
This parametersetting determineswhethertheTAtransmitsunsolicited resultcodes ornot.Theunsolicited
resultcodes are <CONNECT><SPEED><COMMUNICATIONPROTOCOL>[<TEXT>]
ATX SetCONNECTResultCodeFormat
WriteCommand
ATX<VALUE>
Response
1)
OK
2)


## Página 59

A76XX Series_AT Command Manual_V1.09
www.simcom.com 58/652
ERROR
Execution Command
ATX
Response
1)Setdefaultvalue:1
OK
2)
ERROR
ParameterSaving Mode AT&W_SAVE
Max ResponseTime -
Reference -
DefinedValues
<value> 0 CONNECTresultcodereturned
1,2,3,4 May be transmitsexternresult codes.
Examples
ATX1
OK
ATX
OK
2.2.14 AT&W Save theusersettingtoME
This commandwillsavethe usersettingsto MEwhichset byATE,ATQ,ATV,ATX,AT&C,AT&D,AT+IFC
andATS0.Afterrestarted,the valuesaved byAT&Wmustberestored byATZ.
AT&W Save theusersettingtoME
WriteCommand
AT&W<value>
Response
1)
OK
2)
ERROR
Execution Command
AT&W
Response
1)Setdefaultvalue:0
OK
2)
ERROR
ParameterSaving Mode -


## Página 60

A76XX Series_AT Command Manual_V1.09
www.simcom.com 59/652
Max ResponseTime -
Reference -
DefinedValues
<value> 0 Save
Examples
AT&W0
OK
AT&W
OK
2.2.15 ATZ RestoretheusersettingfromME
This commandwillrestore the usersettingfromME whichsetbyATE,ATQ,ATV,ATX,AT&C,AT&Dand
ATS0.
ATZ RestoretheusersettingfromME
WriteCommand
ATZ<value>
Response
1)
OK
2)
ERROR
Execution Command
ATZ
Response
1)Setdefaultvalue:0
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<value> 0 Restore


## Página 61

A76XX Series_AT Command Manual_V1.09
www.simcom.com 60/652
Examples
ATZ0
OK
ATZ
OK
2.2.16 AT+CGMI Requestmanufactureridentification
Thiscommandisusedtorequestthemanufactureridentificationtext,whichisintendedtopermittheuserof
the Moduletoidentifythe manufacturer.
AT+CGMI Requestmanufactureridentification
TestCommand
AT+CGMI=?
Response
OK
Execution Command
AT+CGMI
Response
<manufacturer>
OK
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<manufacturer> Theidentification ofmanufacturer.
Examples
AT+CGMI
SIMCOM INCORPORATED
OK
AT+CGMI=?
OK


## Página 62

A76XX Series_AT Command Manual_V1.09
www.simcom.com 61/652
2.2.17 AT+CGMM Requestmodelidentification
This commandis used torequests modelidentification text,whichisintended topermitthe userofthe
Module toidentifythe specificmodel.
AT+CGMM Requestmodelidentification
TestCommand
AT+CGMM=?
Response
OK
Execution Command
AT+CGMM
Response
<model>
OK
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<model> Theidentification ofmodel.
Examples
AT+CGMM
A7600E
OK
AT+CGMM=?
OK
2.2.18 AT+CGMR Requestrevisionidentification
This commandis used torequestproductfirmware revision identification text,whichis intended topermit
the userofthe Module toidentifythe version.
AT+CGMR Requestrevisionidentification
TestCommand
AT+CGMR=?
Response
OK
Execution Command Response


## Página 63

A76XX Series_AT Command Manual_V1.09
www.simcom.com 62/652
AT+CGMR +CGMR:<revision>
OK
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<revision> Therevision identification offirmware.
Examples
AT+CGMR
+CGMR:A35B01A7600C
OK
AT+CGMR=?
OK
2.2.19 AT+CGSN Requestproductserialnumberidentification
Thiscommandrequestsproductserialnumberidentificationtext,whichisintendedtopermitthe userofthe
Module toidentifythe individualME towhichitis connected to.
AT+CGSN Requestproductserialnumberidentification
TestCommand
AT+CGSN=?
Response
OK
Execution Command
AT+CGSN
Response
<sn>
OK
Ifthereis anyerror, response
ERROR
or
+CMEERROR :<err>
ParameterSaving Mode -


## Página 64

A76XX Series_AT Command Manual_V1.09
www.simcom.com 63/652
Max ResponseTime -
Reference -
DefinedValues
<sn> Serialnumberidentification,whichconsists ofasingle linecontaining
the IMEI(InternationalMobile station EquipmentIdentity)number of
the MT.
Examples
AT+CGSN
351602000330570
OK
AT+CGSN=?
OK
2.2.20 AT+CSCS SelectTEcharacterset
WritecommandinformsTAwhichcharacterset<chest>isusedby theTE.TAisthen able toconvert
characterstrings correctly betweenTEandMTcharactersets.
Readcommand shows currentsetting andtestcommanddisplays conversion schemesimplementedinthe
TA.
AT+CSCS SelectTEcharacterset
TestCommand
AT+CSCS=?
Response
+CSCS:(list ofsupported<chset>s)
OK
Read Command
AT+CSCS?
Response
+CSCS:<chset>
OK
WriteCommand
AT+CSCS=<chset>
Response
OK
or
ERROR
Execution Command
AT+CSCS
Response
Setsubparameters as defaultvalue(IRA):


## Página 65

A76XX Series_AT Command Manual_V1.09
www.simcom.com 64/652
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime -
Reference -
DefinedValues
<chset> Characterset,the definition asfollowing:
"IRA" Internationalreferencealphabet.
"GSM" GSM defaultalphabet;this setting causeseasily
softwareflowcontrol(XON/XOFF)problems.
"UCS2" 16-bituniversal multiple-octetcoded character set;
UCS2 characterstrings are convertedto
hexadecimalnumbersfrom 0000 toFFFF.
“HEX” The bytesare expressed in8bits,dividedinto two 4bits
andfilled 0before,and the resulting 8bitsare
converted tohexadecimalisHEXcode.
Examples
AT+CSCS="IRA"
OK
AT+CSCS?
+CSCS:"IRA"
OK
AT+CSCS=?
+CSCS:("IRA","UCS2","GSM")
OK
AT+CSCS
OK
2.2.21 AT+GCAP Requestoverallcapabilities
Execution commandcauses theTAreportsalist ofadditionalcapabilities.
AT+GCAP Requestoverallcapabilities
TestCommand Response


## Página 66

A76XX Series_AT Command Manual_V1.09
www.simcom.com 65/652
AT+GCAP=? 1)
OK
2)
ERROR
Execution Command
AT+GCAP
Response
1)
+GCAP:(listof <name>s)
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<name> Listofadditionalcapabilities.
+CGSM GSM function issupported
+FCLASS FAXfunction issupported
+DS Data compressionis supported
+ES Synchronous data modeis supported.
+CIS707-A CDMAdata service commandset
+CIS-856 EVDOdata servicecommand set
+MS MobileSpecific commandset
Examples
AT+GCAP
+GCAP:+CGSM,+FCLASS,+DS
OK
AT+GCAP=?
OK


## Página 67

A76XX Series_AT Command Manual_V1.09
www.simcom.com 66/652
ATCommandsforStatusControl
3.1 OverviewofATCommandsforStatusControl
Command Description
AT+CFUN Setphonefunctionality
AT+CSQ Query signalquality
AT+AUTOCSQ SetCSQreport
AT+CSQDELTA SetRSSI deltachange threshold
AT+CPOF Power down the module
AT+CRESET Resetthe module
AT+CACM Accumulatedcall meter
AT+CAMM Accumulatedcall metermaximum
AT+CPUC Price perunitandcurrencytable
AT+CCLK Real timeclock management
AT+CMEE Reportmobileequipmenterror
AT+CPAS Phone activitystatus
AT+SIMEI SetIMEIforthe module
3.2 DetailedDescriptionofATCommandsforStatusControl
3.2.1 AT+CFUN Setphonefunctionality
This commandis used toselectthe leveloffunctionality <fun>inthe ME.Level "fullfunctionality" is where
the highestlevel ofpower isdrawn. "Minimum functionality" iswhere minimumpower isdrawn. Level of
functionality between thesemayalsobe specified bymanufacturers.Whensupported bymanufacturers,
ME resetting with<rst>parametermay be utilized.
AT+CFUN Setphonefunctionality


## Página 68

A76XX Series_AT Command Manual_V1.09
www.simcom.com 67/652
TestCommand
AT+CFUN=?
Response
+CFUN:(rangeof supported<fun>s),(range ofsupported<rst>s)
OK
Read Command
AT+CFUN?
Response
1)
+CFUN:<fun>
OK
2)
ERROR
3)
+CMEERROR:<err>
WriteCommand
AT+CFUN=<fun>[,<rst>]
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<fun> 0 minimumfunctionality
1 full functionality,online mode
4 disable phoneboth transmitandreceive RFcircuits
5 FactoryTestMode
6 Reset
7 Offline Mode
8 Disable SIM
<rst> 0 do notresetthe MEbefore setting itto<fun> powerlevel
1 resetthe ME beforesettingitto <fun>power level.This value only
takes effectwhen <fun>equals1.
NOTE:<fun>=5 only display<Operation Mode>asFactory TestMode,butnothas anyfunction.
Examples
AT+CFUN=?
+CFUN:(0-1,4-8),(0-1)
OK


## Página 69

A76XX Series_AT Command Manual_V1.09
www.simcom.com 68/652
AT+CFUN?
+CFUN:1
OK
AT+CFUN=1
OK
AT+CFUN=6 mustbe usedafter settingAT+CFUN=7.Ifmoduleinoffline mode,mustexecute
AT+CFUN=6 orrestartmoduleto onlinemode.
3.2.2 AT+CSQ Querysignalquality
This commandis used toreturn received signalstrengthindication<rssi>andchannelbiterrorrate<ber>
fromthe ME.Testcommand returns values supportedby theTAas compound values.
AT+CSQ Querysignalquality
TestCommand
AT+CSQ=?
Response
+CSQ:(range ofsupported <rssi>s),(rangeofsupported <ber>s)
OK
Execution Command
AT+CSQ
Response
1)
+CSQ:<rssi>,<ber>
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<rssi> 0 -113dBmor less
1 -111dBm
2…30 -109…-53 dBm
31 -51 dBm orgreater
NOTE


## Página 70

A76XX Series_AT Command Manual_V1.09
www.simcom.com 69/652
99 notknown ornotdetectable
<ber> (in percent)
0 <0.01%
1 0.01% ---0.1%
2 0.1% ---0.5%
3 0.5% ---1.0%
4 1.0% ---2.0%
5 2.0% ---4.0%
6 4.0% ---8.0%
7 >=8.0%
99 notknown ornotdetectable
Examples
AT+CSQ=?
+CSQ:(0-31,99),(0-7,99)
OK
AT+CSQ
+CSQ:31,99
OK
3.2.3 AT+AUTOCSQ SetCSQreport
This commandis used toenableordisable automatic reportCSQinformation, whenautomatic report
enabled,the modulereports CSQinformationeveryfive seconds oronly after<rssi>or<ber>is changed,
the formatof automaticreportis"+CSQ: <rssi>,<ber>".
AT+AUTOCSQ SetCSQreport
TestCommand
AT+AUTOCSQ=?
Response
+AUTOCSQ:(range ofsupported<auto>s),(rangeof
supported<mode>s)
OK
Read Command
AT+AUTOCSQ?
Response
+AUTOCSQ:<auto>,<mode>
OK
WriteCommand
AT+AUTOCSQ=<auto>[,<mo
Response
1)


## Página 71

A76XX Series_AT Command Manual_V1.09
www.simcom.com 70/652
de>] OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference Vendor
DefinedValues
<auto> 0 disable automatic report
1 enableautomatic report
<mode> 0 CSQautomatic reportevery fiveseconds
1 CSQautomatic reportonly after<rssi>or<ber>ischanged.
NOTE:Ifthe parameterof<mode> isomittedwhen executing write
command,<mode>willbe settodefaultvalue.
Examples
AT+AUTOCSQ=?
+AUTOCSQ:(0-1),(0-1)
OK
AT+AUTOCSQ?
+AUTOCSQ:0,0
OK
AT+AUTOCSQ=1
OK
3.2.4 AT+CSQDELTA SetRSSIdeltachangethreshold
This commandis used tosetRSSI delta thresholdforsignalstrength reporting.
AT+CSQDELTA SetRSSIdeltachangethreshold
TestCommand
AT+CSQDELTA=?
Response
+CSQDELTA:(list ofsupported<delta>s)
OK
Read Command Response


## Página 72

A76XX Series_AT Command Manual_V1.09
www.simcom.com 71/652
AT+CSQDELTA? 1)
+CSQDELTA:<delta>
OK
2)
ERROR
WriteCommand
AT+CSQDELTA=<delta>
Response
1)
OK
2)
ERROR
Execution Command
AT+CSQDELTA
Response
Setdefaultvalue(<delta>=5)
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference Vendor
DefinedValues
<delta> Range: from0to5.
Examples
AT+CSQDELTA=?
+CSQDELTA: (0-5)
OK
AT+CSQDELTA?
+CSQDELTA: 5
OK
AT+CSQDELTA
OK
3.2.5 AT+CPOF Powerdownthemodule
Thiscommandisusedtopoweroffthemodule.OncetheAT+CPOFcommandisexecuted,Themodulewill
store userdataanddeactivate from network,andthen shutdown.


## Página 73

A76XX Series_AT Command Manual_V1.09
www.simcom.com 72/652
AT+CPOF Powerdownthemodule
TestCommand
AT+CPOF=?
Response
OK
Execution Command
AT+CPOF
Response
OK
ParameterSaving Mode -
Max ResponseTime 9000ms
Reference Vendor
Examples
AT+CPOF=?
OK
AT+CPOF
OK
3.2.6 AT+CRESET Resetthemodule
This commandis used toresetthe module.
AT+CRESET Resetthemodule
Execution Command
AT+CRESET
Response
OK
TestCommand
AT+CRESET=?
Response
OK
ParameterSaving Mode -
Max ResponseTime 9000ms
Reference Vendor
Examples
AT+CRESET=?
OK
AT+CRESET
OK


## Página 74

A76XX Series_AT Command Manual_V1.09
www.simcom.com 73/652
3.2.7 AT+CACM Accumulatedcallmeter
This commandis used toresettheAdviceofChargerelated accumulated callmetervalue inSIM file
EFACM.
AT+CACM Accumulatedcallmeter
TestCommand
AT+CACM=?
Response
1)
OK
2)
ERROR
Read Command
AT+CACM?
Response
1)
+CACM:<acm>
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CACM=<passwd>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+CACM
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<passwd> String type,SIM PIN2.
<acm> String type,accumulated callmetervalue similarly coded as<ccm>
under+CAOC.


## Página 75

A76XX Series_AT Command Manual_V1.09
www.simcom.com 74/652
Examples
AT+CACM=?
OK
AT+CACM?
+CACM:"000000"
OK
AT+CACM="000000"
+CMEERROR:SIM PUK2 required
AT+CACM
+CMEERROR:SIM PINrequired
3.2.8 AT+CAMM Accumulatedcallmetermaximum
This commandis used tosettheAdviceofCharge related accumulatedcallmeter maximumvaluein SIM
fileEFACMmax.
AT+CAMM Accumulatedcallmetermaximum
TestCommand
AT+CAMM=?
Response
1)
OK
2)
ERROR
Read Command
AT+CAMM?
1)
+CAMM:<acmmax>
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CAMM=<acmmax>[,<pas
swd>]
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>


## Página 76

A76XX Series_AT Command Manual_V1.09
www.simcom.com 75/652
Execution Command
AT+CAMM
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<acmmax> Stringtype,accumulatedcallmetermaximumvaluesimilarlycodedas
<ccm>underAT+CAOC,value zerodisablesACMmax feature.
<passwd> String type,SIM PIN2.
Examples
AT+CAMM=?
OK
AT+CAMM?
+CAMM:"000000"
OK
AT+CAMM="000000"
+CMEERROR:SIM PINrequired
AT+CAMM
+CMEERROR:SIM PINrequired
3.2.9 AT+CPUC Priceperunitandcurrencytable
ThiscommandisusedtosettheparametersofAdviceofChargerelatedpriceperunitandcurrencytablein
SIM fileEFPUCT.
AT+CPUC Priceperunitandcurrencytable
TestCommand
AT+CPUC=?
Response
1)
OK
2)
ERROR


## Página 77

A76XX Series_AT Command Manual_V1.09
www.simcom.com 76/652
Read Command
AT+CPUC?
Response
1)
+CPUC:[<currency>,<ppu>]
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CPUC=<currency>,<ppu
>[,<passwd>]
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<currency> String type,three-charactercurrency code (e.g."GBP","DEM"),
charactersetas specified bycommand SelectTECharacter Set
AT+CSCS.
<ppu> String type,priceperunit, dotisusedas adecimalseparator.(e.g.
"2.66").
<passwd> String type,SIM PIN2
Examples
AT+CPUC=?
OK
AT+CPUC?
+CPUC:"","0.000000"
OK
AT+CPUC="1","0.000000"
+CMEERROR:SIM PINrequired


## Página 78

A76XX Series_AT Command Manual_V1.09
www.simcom.com 77/652
3.2.10 AT+CCLK Realtimeclockmanagement
This commandis used tomanageRealTimeClockofthe module.
AT+CCLK Realtimeclockmanagement
TestCommand
AT+CCLK=?
Response
OK
Read Command
AT+CCLK?
Response
+CCLK:<time>
OK
Write Command
AT+CCLK=<time>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
NOTE:timezone notsave
Maximum ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<time> String type value; format is "yy/MM/dd,hh:mm:ss±zz", where
characters indicate year (two last digits), month, day, hour, minutes,
seconds and time zone (indicates the difference, expressed in
quarters of an hour,between the local time and GMT; three last digits
are mandatory, range (-96 to 96). E.g. 6th of May 2008, 14:28:10
GMT+8 equals to"08/05/06,14:28:10+32".
NOTE:
1.<time>formatmustbe"yy/MM/dd,hh:mm:ss±zz",Otherwise,itwill
lead tounpredictable results.
2.Timezoneis nonvolatile,andthe factory value isinvalidtimezone.
3. Command+CCLK? willreturn timezonewhen timezone isvalid,
andif timezoneis 00, command+CCLK? will return"+00", butnot
"-00".
Examples
AT+CCLK=?
OK
AT+CCLK?


## Página 79

A76XX Series_AT Command Manual_V1.09
www.simcom.com 78/652
+CCLK:"14/01/01,02:14:36+08"
OK
AT+CCLK="14/01/01,02:14:36+08"
OK
3.2.11 AT+CMEE Reportmobileequipmenterror
This commandis used todisable orenable the useof resultcode "+CMEERROR: <err>"or"+CMS
ERROR: <err>"as anindication ofanerrorrelating tothe functionalityof ME;whenenabled,the formatof
<err>canbe settonumericor verbosestring.
AT+CMEE Reportmobileequipmenterror
TestCommand
AT+CMEE=?
Response
+CMEE:(listof supported<n>s)
OK
Read Command
AT+CMEE?
Response
+CMEE:<n>
OK
WriteCommand
AT+CMEE=<n>
Response
1)
OK
2)
ERROR
Execution Command
AT+CMEE
Response
OK
Note:Setdefaultvalue
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 0 Disable resultcode,i.e.only "ERROR"willbedisplayed.
1 Enable errorresultcode with numericvalues.
2 Enable errorresultcode with string values.
Examples


## Página 80

A76XX Series_AT Command Manual_V1.09
www.simcom.com 79/652
AT+CMEE=?
+CMEE:(0-2)
OK
AT+CMEE?
+CMEE:2
OK
AT+CMEE=2
OK
3.2.12 AT+CPAS Phoneactivitystatus
This commandis used toreturn the activitystatus <pas>ofthe ME.Itcanbe usedtointerrogate the ME
beforerequesting action fromthe phone.
AT+CPAS Phoneactivitystatus
TestCommand
AT+CPAS=?
Response
+CPAS:(list ofsupported<pas>s)
OK
Execution Command
AT+CPAS
Response
+CPAS:<pas>
OK
ParameterSaving Mode -
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<pas> 0 ready (MEallows commands fromTA/TE)
3 ringing(ME isready forcommands fromTA/TE,butthe ringeris
active)
4 call in progress(ME isready forcommandsfromTA/TE,butacall
is inprogress)
Examples
AT+CPAS=?


## Página 81

A76XX Series_AT Command Manual_V1.09
www.simcom.com 80/652
+CPAS:(0,3,4)
OK
AT+CPAS
+CPAS:0
OK
This commandis sameasAT+CLCC,butAT+CLCCis morecommonly used. SoAT+CLCCis
recommended touse.
3.2.13 AT+SIMEI SettheIMEIforthemodule
This commandis used tosetthe module’sIMEIvalue.
AT+SIMEI SettheIMEIforthemodule
TestCommand
AT+SIMEI=?
Response
OK
Read Command
AT+SIMEI?
Response
1)
+SIMEI:<imei>
OK
2)
ERROR
WriteCommand
AT+SIMEI=<imei>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference Vendor
DefinedValues
<imei> The15-digit IMEIvalue.
NOTE


## Página 82

A76XX Series_AT Command Manual_V1.09
www.simcom.com 81/652
Examples
AT+SIMEI=?
OK
AT+SIMEI?
+SIMEI:357396012183175
OK
AT+SIMEI=357396012183175
OK
Writecommandsmustbe executedin factory mode.
NOTE


## Página 83

A76XX Series_AT Command Manual_V1.09
www.simcom.com 82/652
ATCommandsforNetwork
4.1 OverviewofATCommandsforNetwork
Command Description
AT+CREG Networkregistration
AT+COPS Operatorselection
AT+CUSD Unstructuredsupplementary servicedata
AT+CSSN Supplementary service notifications
AT+CPOL Preferredoperator list
AT+COPN Read operatornames
AT+CNMP Preferredmode selection
AT+CNBP Preferredband selection
AT+CPSI Inquiring UEsystem information
AT+CNSMOD Shownetwork systemmode
AT+CTZU Automatic timeandtimezoneupdate
AT+CTZR Timeandtimezone reporting
4.2 DetailedDescriptionofATCommandsforNetwork
4.2.1 AT+CREG Networkregistration
Thiscommandisusedtocontrolthepresentationofanunsolicitedresultcode+CREG:<stat>when<n>=1
andthere is achange inthe ME network registration status,orcode+CREG: <stat>[,<lac>,<ci>]when
<n>=2 andthere is achange ofthe network cell.
Read commandreturns thestatus of resultcode presentation andan integer<stat>whichshows whether
thenetworkhascurrentlyindicatedtheregistrationoftheME.Locationinformationelements<lac>and<ci>
are returned only when<n>=2 andME is registeredin thenetwork.


## Página 84

A76XX Series_AT Command Manual_V1.09
www.simcom.com 83/652
AT+CREG Networkregistration
TestCommand
AT+CREG=?
Response
+CREG:(rangeofsupported <n>s)
OK
Read Command
AT+CREG?
Response
1)
+CREG:<n>,<stat>[,<lac>,<ci>]
OK
2)
ERROR
3)
+CMEERROR:<err>
WriteCommand
AT+CREG=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
Execution Command
AT+CREG
Response
Setdefaultvalue(<n>=0):
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 0 disable network registrationunsolicited resultcode.
1 enablenetworkregistrationunsolicitedresultcode+CREG:<stat>.
2 enablenetwork registrationand location information unsolicited
resultcode +CREG:<stat>[,<lac>,<ci>].
<stat> 0 not registered,MEis notcurrently searching anewoperatorto
registerto.
1 registered,homenetwork.
2 not registered,butME iscurrently searching anewoperatorto
registerto.
3 registrationdenied.
4 unknown.
5 registered,roaming.
6 registered for"SMS only",homenetwork(applicable only when
E-UTRAN)


## Página 85

A76XX Series_AT Command Manual_V1.09
www.simcom.com 84/652
7 registered for"SMS only",roaming (applicableonly when <AcT>
indicates E-UTRAN)
11attachedforemergencybearerservicesonly
<lac> Two byte locationarea codein hexadecimalformat(e.g."00C3"equals
193in decimal).
<ci> CellIdentifyinhexadecimalformat.
GSM: Maximum istwo byte.
WCDMA:Maximum isfourbyte.
Examples
AT+CREG=?
+CREG:(0-2)
OK
AT+CREG?
+CREG:0,1
OK
AT+CREG=1
OK
AT+CREG
OK
4.2.2 AT+COPS Operatorselection
Writecommandforcesan attempttoselectand registerthe GSM/UMTSnetworkoperator.<mode>isused
toselectwhetherthe selection isdone automatically bythe MEoris forcedby thiscommand tooperator
<oper>(itshallbegiveninformat<format>).Iftheselectedoperatorisnotavailable,nootheroperatorshall
be selected (except<mode>=4).Theselectedoperator nameformatshallapply tofurther readcommands
(AT+COPS?)also. <mode>=2 forcesan attempttoderegister from the network.Theselected mode affects
toallfurther networkregistration(e.g. after<mode>=2,ME shallbe unregistered until<mode>=0 or1is
selected).
Read commandreturns thecurrentmodeand the currently selected operator.Ifno operatoris
selected,<format>and<oper>areomitted.
Testcommandreturns alistof quadruplets,each representingan operatorpresentin the network.
Quadrupletconsists ofan integer indicatingthe availability oftheoperator <stat>,long and short
alphanumericformat ofthe nameoftheoperator,and numericformatrepresentationofthe operator.Anyof
the formats may beunavailableand should thenbe an emptyfield.Thelistofoperators shall bein order:
homenetwork,networks referencedin SIM, andothernetworks.


## Página 86

A76XX Series_AT Command Manual_V1.09
www.simcom.com 85/652
Itis recommended(althoughoptional)thatafter theoperator listTAreturns lists ofsupported <mode>s and
<format>s.These lists shallbedelimited fromthe operatorlist bytwo commas.
AT+COPS Operatorselection
TestCommand
AT+COPS=?
Response
1)
[+COPS:[listofsupported (<stat>,longalphanumeric <oper>,short
alphanumeric<oper>,numeric <oper>[,<AcT>])s]
[,,(list ofsupported<mode>s),(listofsupported <format>s)]]
OK
2)
ERROR
3)
+CMEERROR: <err>
Read Command
AT+COPS?
Response
1)
+COPS:<mode>[,<format>,<oper>[,<AcT>]]
OK
2)
ERROR
3)
+CMEERROR:<err>
WriteCommand
AT+COPS=<mode>[,<format
>[,<oper>[,<AcT>]]]
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 60S
Reference 3GPPTS27.007
DefinedValues
<mode> 0 automatic
1 manual
2 forcederegister
3 set only<format>
4 manual/automatic
NOTE:if<mode>is setto1,4in write command,the <oper>is


## Página 87

A76XX Series_AT Command Manual_V1.09
www.simcom.com 86/652
needed.Nomally,LTEisdefault search mode,So,ifnot inLTE,the <
AcT>isneeded.
<format> 0 long formatalphanumeric <oper>
1 shortformat alphanumeric<oper>
2 numeric <oper>
<oper> stringtype,<format> indicates ifthe format isalphanumeric or
numeric.
<stat> 0 unknown
1 available
2 current
3 forbidden
<AcT> Access technologyselected
0 GSM
1 GSM Compact
2 UTRAN
3 GSM w/EGPRS
4 UTRANw/HSDPA
5 UTRANw/HSUPA
6 UTRANw/HSDPAandHSUPA
7 EUTRAN
8 UTRANHSPA+
Examples
AT+COPS=?
+COPS:(2,"CHN-UNICOM","UNICOM","46001",7),(1,"CHN-UNICOM","UNICOM",
"46001",2),(1,"CHN-UNICOM","UNICOM","46001",0),(3,"CHINA MOBILE","CMCC",
"46000",7),(3,"CHN-CT","CT","46011",7),(3,"CHINAMOBILE","CMCC","46000",
0),,(0,1,2,3,4),(0,1,2)
OK
AT+COPS?
+COPS:0,2,"46001",7
OK
AT+COPS=0,2,"46001",7
OK
4.2.3 AT+CUSD Unstructuredsupplementaryservicedata
This commandallows controlof the Unstructured Supplementary ServiceData(USSD).Bothnetwork and
mobileinitiated operations are supported.Parameter<n>is used to disable/enablethe presentation ofan


## Página 88

A76XX Series_AT Command Manual_V1.09
www.simcom.com 87/652
unsolicited resultcode(USSDresponsefromthe network,ornetwork initiated operation)+CUSD:
<m>[,<str>,<dcs>]to theTE.Inaddition,value<n>=2 is used tocancel anongoing USSDsession.
AT+CUSD Unstructuredsupplementaryservicedata
TestCommand
AT+CUSD=?
Response
+CUSD:(rangeofsupported <n>s)
OK
Read Command
AT+CUSD?
Response
+CUSD:<n>
OK
WriteCommand
AT+CUSD=<n>[,<str>[,<dcs>
]]
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
Execution Command
AT+CUSD
Response
Setdefaultvalue(<n>=0):
OK
NOTE:Afterexecute，you musttest modulewhethertosupportIMS:
at*imsrcfg="switch"
1) ifspported,the module return:
*IMSRCFG:"switch","on"
OK
Inthe case,you must executeat*imsrcfg="switch",offtoswitch
offIMS,otherwise,AT+CUSDwillreturn ERROR.
2) ifnotsupported,the modulereturn:
ERROR
Inthe case,you can executeAT+CUSDdirectly.
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 0 disable the resultcodepresentation in theTA
1 enablethe resultcode presentation intheTA
2 cancel session (notapplicabletoread commandresponse)
<str> String type USSD-string.


## Página 89

A76XX Series_AT Command Manual_V1.09
www.simcom.com 88/652
<dcs> CellBroadcastData CodingSchemeinintegerformat(default17).
<m> 0 no furtheruseraction required (network initiated USSD-Notify,or
no furtherinformationneeded aftermobileinitiated operation)
1 further useraction required(network initiated USSD-Request,or
further information needed aftermobile initiatedoperation)
2 USSD terminated bynetwork
4 operationnot supported
5 network timeout
Examples
AT+CUSD=?
+CUSD:(0-2)
OK
AT+CUSD?
+CUSD:1
OK
AT+CUSD=0
OK
AT+CUSD
OK
4.2.4 AT+CSSN Supplementaryservicenotifications
This commandrefers to supplementary servicerelated network initiated notifications.Thesetcommand
enables/disablesthe presentation ofnotification resultcodes fromTAtoTE.
When<n>=1 andasupplementaryservice notification isreceivedafteramobileoriginated call setup,
intermediate resultcode+CSSI:<code1>[,<index>] issenttoTEbefore anyotherMOcall setupresult
codespresentedinthepresentdocument.Whenseveraldifferent<code1>sarereceivedfromthenetwork,
eachof themshallhave itsown +CSSIresultcode.
When<m>=1andasupplementary servicenotification isreceivedduring amobile terminated callsetupor
during acall,orwhenaforwardchecksupplementaryservice notificationis received,unsolicited result
code+CSSU:<code2>[,<index>[,<number>,<type>[,<subaddr>,<satype>]]] issenttoTE.IncaseofMT
call setup,result code issentafterevery +CLIPresultcode(refercommand "Calling line identification
presentation +CLIP")andwhen severaldifferent<code2>sare receivedfromthe network,eachof them
shallhaveits own +CSSU resultcode.
AT+CSSN Supplementaryservicenotifications


## Página 90

A76XX Series_AT Command Manual_V1.09
www.simcom.com 89/652
TestCommand
AT+CSSN=?
Response
1)
+CSSN:(list ofsupported<n>s),(listof supported<m>s)
OK
2)
ERROR
Read Command
AT+CSSN?
Response
+CSSN:<n>,<m>
OK
WriteCommand
AT+CSSN=<n>[,<m>]
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> Parametersets/shows the +CSSIresultcodepresentation status in
theTA:
0 disable
1 enable
<m> Parametersets/shows the +CSSUresultcodepresentation statusin
theTA:
0 disable
1 enable
<code1> 0 unconditionalcall forwarding is active
1 someofthe conditionalcall forwarding are active
2 call has beenforwarded
3 call is waiting
5 outgoingcalls are barred
<index> Refer"Closed usergroup+CCUG".
<code2> 0 this isa forwardedcall(MTcallsetup)
2 call has beenputon hold (duringavoice call)
3 call has beenretrieved (during avoicecall)
5 call on holdhas been released (thisis not aSSnotification)(during
avoice call)
<number> String type phonenumberof formatspecified by <type>.


## Página 91

A76XX Series_AT Command Manual_V1.09
www.simcom.com 90/652
<type> Typeofaddressoctetinintegerformat;default145whendialingstring
includesinternational accesscodecharacter"+",otherwise 129.
<subaddr> String type subaddress offormatspecified by <satype>.
<satype> Typeof subaddress octet inintegerformat, default 128.
Examples
AT+CSSN=?
+CSSN:(0-1),(0-1)
OK
AT+CSSN?
+CSSN:1,1
OK
AT+CSSN=1,1
OK
4.2.5 AT+CPOL Preferredoperatorlist
This commandis used toeditthe SIM preferredlist ofnetworks.
AT+CPOL Preferredoperatorlist
TestCommand
AT+CPOL=?
Response
1)
+CPOL:(rangeof supported<index>s),(rangeof supported
<format>s)
OK
2)
ERROR
Read Command
AT+CPOL?
Response
1)
[+CPOL:
<index1>,<format>,<oper1>[<GSM_AcT1>,<GSM_Compact_AcT
1>,<UTRAN_AcT1>,<LTE_AcT1>][<CR><LF><CR><LF>
+CPOL:
<index2>,<format>,<oper2>[,<GSM_AcT1>,<GSM_Compact_AcT
1>,<UTRAN_AcT1>,<LTE_AcT1>]
[..]]]


## Página 92

A76XX Series_AT Command Manual_V1.09
www.simcom.com 91/652
OK
2)
ERROR
WriteCommand
AT+CPOL=<index>[,<format
>[,<oper>][,<GSM_AcT1>,<G
SM_Compact_AcT1>,<UTRA
N_AcT1>,<LTE_AcT1>]]
NOTE: If using USIM card, the
lastfourparametersmust set.
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<index> Integertype,the ordernumberof operatorinthe SIMpreferred
operatorlist.
Ifonly input<index>,commandwilldelete the valueindicate by
<index>.
<format> 0 long formatalphanumeric <oper>
1 shortformat alphanumeric<oper>
2 numeric <oper>
<operX> String type.
<GSM_AcTn> GSM accesstechnology:
0 access technologynot selected
1 access technologyselected
<GSM_Compact_AcTn> GSM compactaccesstechnology:
0 access technologynot selected
1 access technologyselected
<UTRA_AcTn> UTRAaccess technology:
0 access technologynot selected
1 access technologyselected
<LTE_AcTn> LTEaccess technology:
0 access technologynot selected
1 access technologyselected
Examples
AT+CPOL=?
+CPOL:(1-80),(0-2)


## Página 93

A76XX Series_AT Command Manual_V1.09
www.simcom.com 92/652
OK
AT+CPOL?
+CPOL:1,2,"46001"
+CPOL:2,2,"46001"
+CPOL:3,2,"46001",0,0,0,1
+CPOL:4,2,"46009",0,0,0,1
+CPOL:5,2,"46001",0,0,1,0
+CPOL:6,2,"46009",0,0,1,0
OK
AT+CPOL=1,2,"46001"
OK
4.2.6 AT+COPN Readoperatornames
This commandis used toreturn the listofoperatornames from the ME.Eachoperator code <numericX>
thathas analphanumeric equivalent<alphaX>in theME memoryshall bereturned.
AT+COPN Readoperatornames
TestCommand
AT+COPN=?
Response
1)
OK
2)
ERROR
Execution Command
AT+COPN
Response
1)
+COPN:<numeric1>,<alpha1>[<CR><LF><CR><LF>
+COPN:<numeric2>,<alpha2>
[..]]
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms


## Página 94

A76XX Series_AT Command Manual_V1.09
www.simcom.com 93/652
Reference 3GPPTS27.007
DefinedValues
<numericX> String type,operator innumericformat (seeAT+COPS).
<alphaX> String type,operator inlong alphanumericformat(seeAT+COPS).
Examples
AT+COPN=?
OK
AT+COPN
+COPN:"46000","CMCC"
+COPN:"46001","UNICOM"
……
OK
4.2.7 AT+CNMP Preferredmodeselection
This commandis used toselectorsetthe stateofthe modepreference.
AT+CNMP Preferredmodeselection
TestCommand
AT+CNMP=?
Response
+CNMP:(listofsupported <mode>s)
OK
Read Command
AT+CNMP?
Response
+CNMP:<mode>
OK
WriteCommand
AT+CNMP=<mode>
Response
1)
OK
2)If<mode> notsupported by module,this commandwillreturn
ERROR.
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms


## Página 95

A76XX Series_AT Command Manual_V1.09
www.simcom.com 94/652
Reference 3GPPTS27.007
DefinedValues
<mode> 2 Automatic
13 GSM Only
14 WCDMAOnly
38 LTEOnly
Examples
AT+CNMP=?
+CNMP:(2,13,14,38)
OK
AT+CNMP?
+CNMP:2
OK
AT+CNMP=2
OK
1 Theresponsewillbe returnedimmediatelyforTestCommand andReadCommand.TheMax
ResponseTimeforWrite Command is10 seconds.
2 Thesetvalue inWriteCommand willtakeefficient immediately;
4.2.8 AT+CNBP Preferredbandselection
This commandis used toselectorsetthe stateofthe band preference.
AT+CNBP Preferredbandselection
Read Command
AT+CNBP?
Response
+CNBP:<mode>[,<lte_mode>][,<lte_modeExt>]
OK
WriteCommand Response
NOTE


## Página 96

A76XX Series_AT Command Manual_V1.09
www.simcom.com 95/652
AT+CNBP=<mode>[,<lte_mo
de>][,<lte_modeExt>]
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 64bitnumber,the valueis "1"<<"<pos>",then orby bit.
<pos> Value:
0xFFFFFFFF7FFFFFFF Any (any value)
7 GSM_DCS_1800
8 GSM_EGSM_900
9 GSM_PGSM_900
16 GSM_450
17 GSM_480
18 GSM_750
19 GSM_850
20 GSM_RGSM_900
21 GSM_PCS_1900
22 WCDMA_IMT_2000
23 WCDMA_PCS_1900
24 WCDMA_III_1700
25 WCDMA_IV_1700
26 WCDMA_850
27 WCDMA_800
48 WCDMA_VII_2600
49 WCDMA_VIII_900
50 WCDMA_IX_1700
<lte_mode> 64 bit number,the valueis "1"<<"<lte_pos>",thenor bybit.
NOTE:FDD(band1~band32),TDD(band33 ~band42)
<lte_pos> Value:
0x000007FF3FDF3FFF Any (any value)
0 EUTRAN_BAND1(UL:1920-1980; DL:2110-2170)
1 EUTRAN_BAND2(UL:1850-1910; DL:1930-1990)
2 EUTRAN_BAND3(UL:1710-1785; DL:1805-1880)
3 EUTRAN_BAND4(UL:1710-1755; DL:2110-2155)
4 EUTRAN_BAND5(UL:824-849;DL:869-894)
5 EUTRAN_BAND6(UL:830-840;DL:875-885)
6 EUTRAN_BAND7(UL:2500-2570; DL:2620-2690)


## Página 97

A76XX Series_AT Command Manual_V1.09
www.simcom.com 96/652
7 EUTRAN_BAND8(UL:880-915;DL:925-960)
8 EUTRAN_BAND9(UL:1749.9-1784.9;
DL:1844.9-1879.9)
9 EUTRAN_BAND10(UL:1710-1770; DL:2110-2170)
10 EUTRAN_BAND11(UL:1427.9-1452.9; DL:1475.9-1500.9)
11 EUTRAN_BAND12(UL:698-716;DL:728-746)
12 EUTRAN_BAND13(UL:777-787;DL: 746-756)
13 EUTRAN_BAND14(UL:788-798;DL: 758-768)
16 EUTRAN_BAND17(UL:704-716;DL: 734-746)
17 EUTRAN_BAND18(UL:815-830;DL: 860-875)
18 EUTRAN_BAND19(UL:830-845;DL: 875-890)
19 EUTRAN_BAND20(UL:832-862;DL: 791-821)
20 EUTRAN_BAND21(UL:1447.9-1462.9;DL:1495.9-1510.9)
22 EUTRAN_BAND23(UL:2000-2020;DL: 2180-2200)
23 EUTRAN_BAND24(UL:1626.5-1660.5; DL:1525 -1559)
24 EUTRAN_BAND25(UL:1850-1915;DL: 1930 -1995)
25 EUTRAN_BAND26(UL:814-849;DL: 859-894)
26 EUTRAN_BAND27(UL:807.5-824;DL: 852-869)
27 EUTRAN_BAND28(703-748;DL:758-803)
28 EUTRAN_BAND29(UL:1850-1910 or1710-1755;
DL:716-728)
29 EUTRAN_BAND30(UL:2305-2315 ;DL:2350 -2360)
30 EUTRAN_BAND31(UL:452.5-457.4;DL:462.5–467.4)
32 EUTRAN_BAND33(UL:1900-1920;DL: 1900-1920)
33 EUTRAN_BAND34(UL:2010-2025;DL: 2010-2025)
34 EUTRAN_BAND35(UL:1850-1910;DL: 1850-1910)
35 EUTRAN_BAND36(UL:1930-1990;DL: 1930-1990)
36 EUTRAN_BAND37(UL:1910-1930;DL: 1910-1930)
37 EUTRAN_BAND38(UL:2570-2620;DL: 2570-2620)
38 EUTRAN_BAND39(UL:1880-1920;DL: 1880-1920)
39 EUTRAN_BAND40(UL:2300-2400;DL: 2300-2400)
40 EUTRAN_BAND41(UL:2496-2690;DL: 2496-2690)
41 EUTRAN_BAND42(UL:3400-3600;DL: 3400-3600)
42 EUTRAN_BAND43(UL:3600-3800;DL: 3600-3800)
<lte_modeExt> 16 bit number,the valueis "1"<<"<lte_posExt>",then orby bit.
NOTE:band65~band76
<lte_posExt> 7 EUTRAN_BAND72(UL: 451-455.9;DL:461-465.9)
Examples
AT+CNBP?
+CNBP:0X0000000000000180,0X0000000040080085,0X0080
OK


## Página 98

A76XX Series_AT Command Manual_V1.09
www.simcom.com 97/652
AT+CNBP=0X0000000000000180,0X0000000040080085,0X0080
OK
4.2.9 AT+CPSI InquiringUEsysteminformation
This commandis used toreturn the UEsystem information.
AT+CPSI InquiringUEsysteminformation
TestCommand
AT+CPSI=?
Response
1)
OK
2)
ERROR
Read Command
AT+CPSI?
Response
1)Ifcampingon agsmcell:
+CPSI:<System Mode>,<Operation
Mode>,<MCC>-<MNC>,<LAC>,<CellID>,<Absolute RFCh
Num>,<RxLev>,<TrackLOAdjust>,<C1-C2>
OK
2)Ifcampingon awcdmacell:
+CPSI:<System Mode>,<Operation
Mode>,<MCC>-<MNC>,<LAC>,<CellID>,<Frequency
Band>,<PSC>,<Freq>,<SSC>,<EC/IO>,<RSCP>,<Qual>,<RxLev>,
<TXPWR>
OK
3)Ifcampingon alte cell:
+CPSI:<System Mode>,<Operation Mode>[,<MCC>-<MNC>,<TA
C>,<SCellID>,<PCellID>,<Frequency Band>,<earfcn>,<dlbw>,<
ulbw>,<RSRQ>,<RSRP>,<RSSI>,<RSSNR>]
OK
4)Ifno service:
+CPSI:NOSERVICE,LowPowerMode
OK
5)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007


## Página 99

A76XX Series_AT Command Manual_V1.09
www.simcom.com 98/652
DefinedValues
<System Mode> System mode, values:"NOSERVICE","GSM","WCDMA","LTE"
<Operation Mode> UEoperationmode, values:"Unknown", "Online","Offline","Factory
TestMode", "Reset","LowPowerMode", "FlightMode".
<MCC> MobileCountry Code(firstpartofthe PLMN code)
<MNC> MobileNetwork Code(second partofthe PLMN code)
<LAC> LocationArea Code(hexadecimaldigits)
<CellID> Service-cell Identify.
<Absolute RFChNum> AFRCNforservice-cell.
<TrackLOAdjust> TrackLOAdjust
<C1> Coefficientforbasestation selection
<C2> CoefficientforCellre-selection
<Frequency Band> Frequency Band ofactive set
<PSC> Primary synchronizationcodeof active set.
<Freq> Downlink frequencyofactive set.
<SSC> Secondary synchronizationcodeofactive set
<EC/IO> Ec/Io value
<RSCP> ReceivedSignal CodePower
<Qual> Quality valueforbasestationselection
<RxLev> RXlevelvalue forbasestationselection
<TXPWR> UETXpower in dBm.IfnoTX,the valueis 500.
<Cpid> CellParameterID
<TAC> TracingArea Code
<PCellID> PhysicalCellID
<earfcn> E-UTRAabsoluteradio frequencychannelnumberforsearching LTE
cells
<dlbw> Transmission bandwidthconfigurationof theserving cellon the
downlink
<ulbw> Transmission bandwidthconfigurationof theserving cellon the uplink
<RSRP> Currentreferencesignalreceivedpowerin（RSRPreportvalue-140）
dBm.Available forLTE
<RSRQ> Thesignalreception quality is:(RSRQreportvalue-40)/2 dBm.
<RSSNR> Average referencesignalsignal-to-noiseratio ofthe servingcell
<SCellID> String type.cell IDindecimalformatforserving cell
<RSSI> Receivedsignalstrength indicatorvalue:(RSSIreportvalue -110)
dBm.
Examples


## Página 100

A76XX Series_AT Command Manual_V1.09
www.simcom.com 99/652
AT+CPSI?
+CPSI:
LTE,Online,460-01,0x230A,175499523,318,EUTRAN-BAND3,1650,5,0,21,67,255,19
OK
4.2.10 AT+CPSITD InquiringUELTEsysteminformation
This commandis used toreturn the UELTEsystem information.Supportsthe 1803S platform.
AT+CPSITD InquiringUELTEsysteminformation
TestCommand
AT+CPSITD=?
Response
1)
OK
2)
ERROR
Read Command
AT+CPSITD?
Response(Thiscommand is onlyusedunder LTEnetwork)
1)
+CPSITD:<System Mode>,<OperationMode>[,< Frequency
Band>,<SA>,<SSP>,<RSRP>,<RSSNR>,<FREQ>,<
PCellID>,<MCC>-<MNC>]
OK
2)Ifno service:
+CPSITD:NOSERVICE,LowPowerMode
OK
3)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<System Mode> System mode, values:"NOSERVICE","LTE"
<Operation Mode> UEoperationmode, values:"Unknown", "Online","Offline","Factory
TestMode", "Reset","LowPowerMode", "FlightMode".
<Frequency Band> Frequency Band ofactive set
<SA> Subframe Assignment
<SSP> Special Subframe Patterns


## Página 101

A76XX Series_AT Command Manual_V1.09
www.simcom.com 100 /652
<RSRP> Currentreferencesignalreceivedpower in-1/10 dBm.Availablefor
LTE
<RSSNR> Average referencesignalsignal-to-noiseratio ofthe servingcell
<FREQ> Cellcenterfrequency
<PCellID> PhysicalCellID
<MCC> MobileCountry Code(firstpartofthe PLMN code)
<MNC> MobileNetwork Code(second partofthe PLMN code)
Examples
AT+CPSITD?
+CPSITD:LTE,Online,EUTRAN-BAND40,91,5336,17,3735928559,180,460-00
OK
4.2.11 AT+CNSMOD Shownetworksystemmode
This commandis used toreturn the currentnetwork systemmode.
NOTE:Generally,it’sappliedto the networksystem modechanged platformto indicate currentaccess
network system type.
AT+CNSMOD Shownetworksystemmode
TestCommand
AT+CNSMOD=?
Response
+CNSMOD:(listof supported<n>s)
OK
Read Command
AT+CNSMOD?
Response
1)
+CNSMOD:<n>,<stat>
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CNSMOD=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>


## Página 102

A76XX Series_AT Command Manual_V1.09
www.simcom.com 101 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 0 disable auto reportthe network systemmode information
1 auto reportthe networksystem modeinformation, command:
+CNSMOD: <stat>
<stat> 0 no service
1 GSM
2 GPRS
3 EGPRS (EDGE)
4 WCDMA
5 HSDPAonly(WCDMA)
6 HSUPAonly(WCDMA)
7 HSPA(HSDPAand HSUPA,WCDMA)
8 LTE
NOTE:When AT+CNSMOD?,MTmaybe return GSM but indication is
GSM/GPRS/EDGE inGSM,orMTmaybe return WCDMAbut
indicationis HSDPA/HSUPA/HSPAin WCDMA.
Examples
AT+CNSMOD=?
+CNSMOD: (0,1)
OK
AT+CNSMOD?
+CNSMOD: 0,8
OK
AT+CNSMOD=0
OK
4.2.12 AT+CTZU Automatictimeandtimezoneupdate
This commandis used toenableanddisable automatictimeandtimezoneupdate viaNITZ
AT+CTZU Automatictimeandtimezoneupdate


## Página 103

A76XX Series_AT Command Manual_V1.09
www.simcom.com 102 /652
TestCommand
AT+CTZU=?
Response
+CTZU:(range ofsupported <on/off>s)
OK
Read Command
AT+CTZU?
Response
+CTZU:<on/off>
OK
WriteCommand
AT+CTZU=<on/off>
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<on/off> Integertype valueindicating:
0 Disable automatic timezone updateviaNITZ.
1 Enable automatictimezone update viaNITZ.
NOTE:1.Thevalueof<on/off>is nonvolatile,andfactory value is0.
2.Forautomatic timeand timezone updateis enabled
(+CTZU=1):
Iftimezoneis only receivedfromnetwork anditisn’tequal
tolocaltimezone(AT+CCLK),timezone isupdated
automatically,andrealtimeclockis updated basedon local
timeandthe differencebetween timezonefrom network
andlocaltimezone(Local timezonemust bevalid).
IfUniversalTimeandtimezone are received from network,
both timezoneand realtimeclockisupdated automatically,
andreal timeclock isbased on UniversalTimeand time
zonefrom network.
Examples
AT+CTZU=?
+CTZU:(0-1)
OK


## Página 104

A76XX Series_AT Command Manual_V1.09
www.simcom.com 103 /652
AT+CTZU?
+CTZU:0
OK
AT+CTZU=0
OK
4.2.13 AT+CTZR Timeandtimezonereporting
This commandis used toenableanddisable the timezone changeeventreporting.Ifthe reporting is
enabled the MTreturnsthe unsolicited resultcode +CTZV:<tz>[,<time>][,<dst>]wheneverthe timezoneis
changed.
AT+CTZR Timeandtimezonereporting
TestCommand
AT+CTZR=?
Response
+CTZR:(range ofsupported <on/off>s)
OK
Read Command
AT+CTZR?
Response
+CTZR:<on/off>
OK
WriteCommand
AT+CTZR=<on/off>
Response
1)
OK
2)
ERROR
Execution Command
AT+CTZR
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<on/off> Integertype valueindicating:
0 Disable timezonechange eventreportin.
1 Enable timezonechangeevent reporting.
+CTZV:
<tz>[,<time>][,<dst>]
Unsolicited resultcode whentimezonereceivedfromnetwork isn’t
equal tolocaltimezone,andif the informationsfrom networkdon’t
include dateand time,timezonewillbe only reported,andif network


## Página 105

A76XX Series_AT Command Manual_V1.09
www.simcom.com 104 /652
daylightsavingtimeispresent, itis alsoreported. ForExamples:
+CTZV:+32 (Only report timezone)
+CTZV:+32,1 (Reporttimezone andnetwork daylight saving
time)
+CTZV:+32,08/12/09,17:00:00 (Reporttimeandtimezone)
+CTZV:+32,08/12/09,17:00:00,1 (Reporttime,timezone and
daylightsavingtime)
Formore detailedinformations about timeandtimezone, pleaserefer
3GPPTS24.008.
<tz> Localtimezonereceivedfrom network.
<time> Universaltimereceived from network,andthe format is
"yy/MM/dd,hh:mm:ss",wherecharacters indicate year
(two lastdigits), month,day,hour,minutes andseconds.
<dst> Network daylight savingtime,and ifitis received from
network,itindicates the value thathas been usedto
adjustthe localtimezone.Thevalues as following:
0 Noadjustment forDaylightSavingTime.
1 +1hour adjustmentforDaylight SavingTime.
2 +2hoursadjustmentforDaylight SavingTime.
NOTE:Herein,<time>isUniversalTimeorNITZtime,butnotlocal
time.
Examples
AT+CTZR=?
+CTZR:(0-1)
OK
AT+CTZR?
+CTZR:0
OK
AT+CTZR=0
OK
AT+CTZR
OK
Thetimezone reporting isnotaffectedby theAutomaticTimeandTimeZone commandAT+CTZU.
NOTE


## Página 106

A76XX Series_AT Command Manual_V1.09
www.simcom.com 105 /652
ATCommandsforPacketDomain
5.1 OverviewofATCommandsforPacketDomain
Command Description
AT+CGERG Networkregistration status
AT+CEREG EPS networkregistration status
AT+CGATT Packetdomainattach ordetach
AT+CGACT PDPcontextactivate ordeactivate
AT+CGDCONT Define PDPcontext
AT+CGDSCONT Define SecondaryPDPContext
AT+CGTFT TrafficFlowTemplate
AT+CGQREQ Quality ofserviceprofile (requested)
AT+CGEQREQ 3Gqualityof serviceprofile (requested)
AT+CGQMIN Quality ofserviceprofile (minimum acceptable)
AT+CGEQMIN 3Gqualityof serviceprofile (minimumacceptable)
AT+CGDATA Enter datastate
AT+CGPADDR ShowPDPaddress
AT+CGCLASS GPRS mobile station class
AT+CGEREP GPRS eventreporting
AT+CGAUTH Settype of authenticationforPDP-IPconnectionsof GPRS
AT+CPING Ping destinationaddress
5.2 DetailedDescriptionofATCommandsforPacketDomain
5.2.1 AT+CGREG Networkregistrationstatus
This commandcontrols the presentation ofanunsolicited result code "+CGREG: <stat>"when <n>=1and
there isachange inthe MT'sGPRSnetwork registrationstatus.


## Página 107

A76XX Series_AT Command Manual_V1.09
www.simcom.com 106 /652
Thereadcommand returns the status ofresultcodepresentation andan integer<stat>whichshows
Whetherthe network has currently indicated theregistrationof the MT.
AT+CGREG Networkregistrationstatus
TestCommand
AT+CGREG=?
Response
+CGREG:(listof supported<n>s)
OK
Read Command
AT+CGREG?
Response
+CGREG:<n>,<stat>[,<lac>,<ci>]
OK
WriteCommand
AT+CGREG=<n>
Response
OK
Execution Command
AT+CGREG
Response
Setdefaultvalue:0
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 0 disable network registrationunsolicited resultcode
1 enablenetwork registrationunsolicited result code+CGREG:
<stat>
2 thereisachangeintheMEnetworkregistrationstatusorachange
of thenetwork cell:
+CGREG:<stat>[,<lac>,<ci>]
<stat> 0 notregistered,MEisnotcurrentlysearchinganoperatortoregister
to
1 registered,homenetwork
2 not registered,butME iscurrently tryingto attachorsearching an
operatorto registerto
3 registrationdenied
4 unknown
5 registered,roaming
6 registered for"SMS only",homenetwork
11 attachedforemergencybearerservices only
<lac> Two byte locationarea codein hexadecimalformat(e.g."00C3"equals
193in decimal).
<ci> CellIDin hexadecimalformat.
GSM: Maximum istwo byte.
WCDMA:Maximum isfourbyte.


## Página 108

A76XX Series_AT Command Manual_V1.09
www.simcom.com 107 /652
Examples
AT+CGREG=?
+CGREG:(0-2)
OK
AT+CGREG?
+CGREG:0,1
OK
AT+CGREG=1
OK
AT+CGREG
OK
5.2.2 AT+CEREG EPSnetworkregistrationstatus
Thesetcommandcontrolsthepresentationofanunsolicitedresultcode+CEREG:<stat>when<n>=1and
there isachange inthe MT'sEPSnetwork registrationstatus inE-UTRAN, orunsolicited resultcode
+CEREG:<stat>[,<tac>,<ci>[,<AcT>]]when<n>=2 andthere is achangeofthe network cellin E-UTRAN;
inthis latest case<AcT>,<tac>and<ci>are sentonly ifavailable.
Thereadcommand returns the status ofresultcodepresentation andan integer<stat>whichshows
whetherthe networkhas currentlyindicated the registrationofthe MT.Location information elements
<tac>,<ci>and<AcT>,ifavailable,arereturned onlywhen <n>=2 andMTis registeredin thenetwork.
AT+CEREG EPSnetworkregistrationstatus
TestCommand
AT+CEREG=?
Response
1)
+CEREG:(range ofsupported<n>s)
OK
2)
ERROR
Read Command
AT+CEREG?
Response
1)
+CEREG:<n>,<stat>[,<tac>,<ci>]
OK
2)


## Página 109

A76XX Series_AT Command Manual_V1.09
www.simcom.com 108 /652
ERROR
WriteCommand
AT+CEREG=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
Execution Command
AT+CEREG
Response
1)
Setdefaultvalue(<n>=0):
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS24.008 [8]
DefinedValues
<n> 0 disable network registrationunsolicited resultcode
1 enablenetwork registrationunsolicited result code+CEREG:
<stat>
2 enablenetwork registrationand location information unsolicited
resultcode +CEREG:<stat>[,<tac>,<ci>[,<AcT>]]
<stat> 0 notregistered,MTisnotcurrentlysearchinganoperatortoregister
to
1 registered,homenetwork
2 not registered,butMTis currently trying toattachor searchingan
operatorto registerto
3 registrationdenied
4 unknown (e.g.outof E-UTRANcoverage)
5 registered,roaming
6 registered for"SMS only",homenetwork(notapplicable)
7 registered for"SMS only",roaming (notapplicable)
11 attachedforemergencybearerservices only
<tac> stringtype; two byte trackingarea codeinhexadecimalformat(e.g.
"00C3"equals195in decimal)
<ci> stringtype; fourbyte E-UTRANcell identifyinhexadecimalformat
<AcT> Anumbericparameterthatindicates the access technologyof serving
cell
0 GSM (notapplicable)
1 GSM Compact(notapplicable)
2 UTRAN(notapplicable)


## Página 110

A76XX Series_AT Command Manual_V1.09
www.simcom.com 109 /652
3 GSM w/EGPRS(see NOTE3)(notapplicable)
4 UTRANw/HSDPA(seeNOTE4)(notapplicable)
5 UTRANw/HSUPA(seeNOTE4)(notapplicable)
6 UTRANw/HSDPAandHSUPA(seeNOTE4)(notapplicable)
7 E-UTRAN
Examples
AT+CEREG=?
+CEREG:(0-2)
OK
AT+CEREG?
+CEREG:0,1
OK
AT+CEREG=1
OK
AT+CEREG
OK
Ifthe EPSMTin GERAN/UTRAN/E-UTRANalsosupports circuitmode servicesand/orGPRS
services,the +CREGcommandand+CREG: resultcodes and/or the+CGREG commandand
+CGREG: resultcodes applytothe registration statusand locationinformation forthose services.
5.2.3 AT+CGATT Packetdomainattachordetach
Thewritecommand isusedto attachthe MTto, ordetachthe MTfrom,thePacket Domainservice.
Thereadcommand returns the currentPacketDomain service state.
AT+CGATT Packetdomainattachordetach
TestCommand
AT+CGATT=?
Response
1)
+CGATT:(list ofsupported <state>s)
OK
2)
NOTE


## Página 111

A76XX Series_AT Command Manual_V1.09
www.simcom.com 110 /652
ERROR
Read Command
AT+CGATT?
Response
1)
+CGATT: <state>
OK
2)
ERROR
WriteCommand
AT+CGATT=<state>
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<state> Indicates the state ofPacketDomain attachment:
0 detached
1 attached
Examples
AT+CGATT=?
+CGATT:(0-1)
OK
AT+CGATT?
+CGATT:1
OK
AT+CGATT=1
OK
5.2.4 AT+CGACT PDPcontextactivateordeactivate


## Página 112

A76XX Series_AT Command Manual_V1.09
www.simcom.com 111 /652
Thewritecommand isusedto activateordeactivate the specifiedPDPcontext(s).
AT+CGACT PDPcontextactivateordeactivate
TestCommand
AT+CGACT=?
Response
+CGACT:(list ofsupported<state>s)
OK
Read Command
AT+CGACT?
Response
+CGACT:[<cid>,<state>[<CR><LF>
+CGACT:<cid>,<state>[<CR><LF>
[..]]]
OK
WriteCommand
AT+CGACT=<state>[,<cid>]
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<state> Indicates the state ofPDPcontextactivation:
0 deactivated
1 activated
<cid> Anumeric parameterwhichspecifies aparticularPDPcontext
definition (seeAT+CGDCONTcommand).
1…15
NOTE:InLTEmode, it’sprohibited toactive ordeactive defaultbearer(cid1)anddedicated beaeer(cid 8).
Examples
AT+CGACT=?
+CGACT:(0,1)
OK
AT+CGACT?
+CGACT:1,1
OK


## Página 113

A76XX Series_AT Command Manual_V1.09
www.simcom.com 112 /652
AT+CGACT=1,1
OK
5.2.5 AT+CGDCONT DefinePDPcontext
ThesetcommandspecifiesPDPcontextparametervaluesforaPDPcontextidentifiedbythe(local)context
identificationparameter<cid>.ThenumberofPDPcontextsthatmaybeinadefinedstateatthesametime
is givenby the range returnedby the testcommand.Aspecialform ofthe writecommand
(AT+CGDCONT=<cid>)causes the valuesforcontext <cid>tobecomeundefined.
AT+CGDCONT DefinePDPcontext
TestCommand
AT+CGDCONT=?
Response
1)
+CGDCONT:(rangeof supported<cid>s),<PDP_type>,,,(list of
supported<d_comp>s),(listof supported<h_comp>s),(listof
<ipv4_ctrl>s),(listof<request_type>s)
OK
2)
ERROR
Read Command
AT+CGDCONT?
Response
1)
+CGDCONT:
<cid>,<PDP_type>,<APN>[[,<PDP_addr>],<d_comp>,<h_comp>,
<ipv4_ctrl>,<request_type>,<P-CSCF_discovery>,<IM_CN_Signa
lling_Flag_Ind>]<CR><LF>
+CGDCONT:
<cid>,<PDP_type>,<APN>[[,<PDP_addr>],<d_comp>,<h_comp>,
<ipv4_ctrl>,<request_type>,<P-CSCF_discovery>,<IM_CN_Signa
lling_Flag_Ind>]
OK
2)
ERROR
WriteCommand
AT+CGDCONT=<cid>[,<PDP
_type>[,<APN>[,<PDP_addr>
[,<d_comp>[,<h_comp>][,<ip
v4_ctrl>[,<request_type>]]]]]
]
Response
1)
OK
2)
ERROR
Execution Command Response


## Página 114

A76XX Series_AT Command Manual_V1.09
www.simcom.com 113 /652
AT+CGDCONT 1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> (PDPContextIdentifier)anumeric parameterwhichspecifies a
particular PDPcontextdefinition.Theparameterislocal totheTE-MT
interfaceandis usedin otherPDPcontext-relatedcommands.The
rangeofpermitted values (minimumvalue=1)is returnedby the test
form ofthe command.
1…15
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol
IPV6 InternetProtocol Version6
IPV4V6 DualPDNStack
<APN> (AccessPointName)astringparameterwhichisalogicalnamethatis
usedto selecttheGGSN ortheexternal packetdata network.
<PDP_addr> Astringparameter thatidentifiesthe MTin the addressspace
applicable tothe PDP.Thisparameterwillbe omitted when PDP_type
is PPPtype.
Read commandwillcontinuetoreturn the null string evenifan
address hasbeen allocated duringthe PDPstartup procedure.The
allocated addressmay be readusing commandAT+CGPADDR.
<d_comp> Anumeric parameterthatcontrols PDPdata compression,thisvalue
may dependon platform:
0 off(default ifvalue is omitted)
1 on
2 V.42bis
<h_comp> Anumeric parameterthatcontrols PDPheadercompression,this
value maydepend onplatform:
0 off(default ifvalue is omitted)
1 RFC1144
<ipv4_ctrl> Parameterthatcontrols howthe MT/TArequests to getthe IPv4
address information:
0 AddressAllocation throughNASSignaling
1 on
<request_type> integertype;indicates the type of PDPcontext activation requestfor
the PDPcontext, see3GPPTS24.301 [83](subclause6.5.1.2)and


## Página 115

A76XX Series_AT Command Manual_V1.09
www.simcom.com 114 /652
3GPPTS24.008 [8] (subclause10.5.6.17).Iftheinitial PDPcontextis
supported (seesubclause 10.1.0)itis notallowed toassign<cid>=0
foremergencybearerservices.Accordingto3GPPTS24.008 [8]
(subclause 4.2.4.2.2 and subclause4.2.5.1.4)and3GPPTS24.301
[83](subclause 5.2.2.3.3 andsubclause5.2.3.2.2), aseparate PDP
context mustbe establishedforemergency bearerservices.
NOTE4:Ifthe PDPcontextforemergencybearerservicesis the only
activated context,only emergency calls are allowed, see3GPPTS
23.401 [82] subclause4.3.12.9.
0 PDPcontextis fornewPDPcontextestablishment orfor
handoverfromanon-3GPPaccess network (howthe MT
decides whetherthe PDPcontextis fornewPDPcontext
establishmentorforhandover isimplementation specific)
1 PDPcontextis foremergency bearerservices
2 PDPcontextis fornewPDPcontextestablishment
<P-CSCF_discovery> integertype;influences howthe MT/TArequests to getthe P-CSCF
address,see3GPPTS24.229 [89]annex Bandannex L.
0 Preference ofP-CSCFaddress discoverynotinfluenced by
+CGDCONT
1 Preference ofP-CSCFaddress discoverythrough NAS
signalling
2 Preference ofP-CSCFaddress discoverythrough DHCP
<IM_CN_Signalling_Flag_In
d>
integertype;indicates tothe network whetherthe PDPcontextis for
IM CNsubsystem-related signalling onlyor not.
0 UEindicates thatthe PDPcontextis notforIMCN
subsystem-relatedsignallingonly
1 UEindicates thatthe PDPcontextis forIM CN
subsystem-relatedsignallingonly
Examples
AT+CGDCONT=?
+CGDCONT:(1-15),"IP",,,(0-2),(0-1),(0-1),(0-2)
+CGDCONT:(1-15),"IPV6",,,(0-2),(0-1),(0-1),(0-2)
+CGDCONT:(1-15),"IPV4V6",,,(0-2),(0-1),(0-1),(0-2)
OK
AT+CGDCONT?
+CGDCONT:1,"IP",""
OK
AT+CGDCONT=1,"IP","cnnet"
OK
AT+CGDCONT


## Página 116

A76XX Series_AT Command Manual_V1.09
www.simcom.com 115 /652
OK
5.2.6 AT+CGDSCONT DefineSecondaryPDPContext
Thesetcommand specifiesPDPcontext parametervalues foraSecondary PDPcontextidentifiedby the
(local)contextidentificationparameter,<cid>.ThenumberofPDPcontexts thatmay bein adefinedstateat
the sametimeis givenby therange returned by the testcommand.Aspecial form ofthe setcommand,
AT+CGDSCONT=<cid>causesthe values forcontextnumber<cid> tobecomeundefined.
AT+CGDSCONT DefineSecondaryPDPContext
TestCommand
AT+CGDSCONT=?
Response
1)
+CGDSCONT:(rangeofsupported <cid>s),(listof<p_cid>sfor
activeprimarycontexts),<PDP_type>,(listof supported
<d_comp>s),(list ofsupported <h_comp>s)
OK
2)
ERROR
Read Command
AT+CGDSCONT?
Response
1)
+CGDSCONT:[<cid>,<p_cid>,<d_comp>,<h_comp>
[<CR><LF>+CGDSCONT:<cid>,<p_cid>,<d_comp>,<h_comp>
[..]]]
OK
2)
ERROR
WriteCommand
AT+CGDSCONT=<cid>[,<p_
cid>[,<d_comp>[,<h_comp>]
]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> anumeric parameterwhichspecifies aparticularPDPcontext
definition.TheparameterislocaltotheTE-MTinterfaceandisusedin


## Página 117

A76XX Series_AT Command Manual_V1.09
www.simcom.com 116 /652
otherPDPcontext-related commands.Therange ofpermittedvalues
(minimumvalue=1)is returnedby the test form ofthe command.
NOTE: The<cid>s fornetwork-initiatedPDPcontextshavevalues
outside the ranges activatedby the +CGACT.
<p_cid> anumeric parameterwhichspecifies aparticularPDPcontext
definition which hasbeen specified byuse ofthe +CGDCONT
commandandactivatedbythe+CGACT.Theparameterislocaltothe
TE-MTinterface.Thelistof permittedvalues isreturnedby the test
form ofthe command.
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol
IPV6 InternetProtocol Version6
IPV4V6 DualPDNStack
<d_comp> anumeric parameterthatcontrols PDPdata compression(applicable
forSNDCPonly)(refer3GPPTS44.065 [61])
0 off
1 on (manufacturerpreferredcompression)
2 V.42bis
Othervaluesare reserved.
<h_comp> anumeric parameterthatcontrols PDPheadercompression (refer
3GPPTS44.065 [61]and3GPPTS25.323 [62])
0 off
1 RFC1144
Othervaluesare reserved.
Examples
AT+CGDSCONT=?
+CGDSCONT:(2,3,4,5,6,7,8,9,10,11,12,13,14,15),(1),"IP",(0-2),(0-1)
+CGDSCONT:(2,3,4,5,6,7,8,9,10,11,12,13,14,15),(1),"IPV6",(0-2),(0-1)
+CGDSCONT:(2,3,4,5,6,7,8,9,10,11,12,13,14,15),(1),"IPV4V6",(0-2),(0-1)
OK
AT+CGDSCONT?
+CGDSCONT:
OK
AT+CGDSCONT=4,2
+CMEERROR:operationnot supported


## Página 118

A76XX Series_AT Command Manual_V1.09
www.simcom.com 117 /652
5.2.7 AT+CGTFT TrafficFlowTemplate
Thiscommand allows theTEtospecifyaPacketFilter-PFforaTrafficFlowTemplate-TFTthat isusedin
the GGSNinUMTS/GPRS andPacketGWinEPS forrouting ofpackets onto differentQoSflows towards
theTE.Theconceptis furtherdescribedin the 3GPPTS23.060 [47].ATFTconsistsoffromone andup to
15 Packet Filters,eachidentified by aunique <packetfilteridentifier>.APacketFilteralsohas an
<evaluationprecedence index>thatisuniquewithin allTFTsassociated with allPDPcontextsthat are
associated with the samePDPaddress.
AT+CGTFT TrafficFlowTemplate
TestCommand
AT+CGTFT=?
Response
1)
+CGTFT: <PDP_type>,(listofsupported <packetfilter
identifier>s),(list ofsupported<evaluationprecedence
index>s),(listofsupported<source address and subnet
mask>s),(listofsupported <protocolnumber(ipv4)/nextheader
(ipv6)>s),(listof supported<destinationport range>s),(list of
supported<source portrange>s),(listofsupported<ipsecsecurity
parameterindex (spi)>s),(listof supported<typeofservice
(tos)(ipv4)andmask /trafficclass (ipv6)andmask>s),(listof
supported<flowlabel(ipv6)>s),(listofsupported <direction>s),(list
ofsupported<localaddress andsubnetmask>s),(range of
supported<QRI>s),(listofsupported <traffic_segregation>s)
[<CR><LF>+CGTFT: <PDP_type>,(listofsupported<packetfilter
identifier>s),(list ofsupported<evaluationprecedence
index>s),(listofsupported<source address and subnet
mask>s),(listofsupported <protocolnumber(ipv4)/nextheader
(ipv6)>s),(listof supported<destinationport range>s),(list of
supported<source portrange>s),(listofsupported<ipsecsecurity
parameterindex (spi)>s),(listof supported<typeofservice
(tos)(ipv4)andmask /trafficclass (ipv6)andmask>s),(listof
supported<flowlabel(ipv6)>s),(listofsupported <direction>s),(list
ofsupported<localaddress andsubnetmask>s),(range of
supported<QRI>s),(listofsupported <traffic_segregation>s)
[..]]
OK
2)
ERROR
Read Command
AT+CGTFT?
Response
1)
+CGTFT: [<cid>,<packet filteridentifier>,<evaluationprecedence
index>,<sourceaddress and subnetmask>,<protocolnumber


## Página 119

A76XX Series_AT Command Manual_V1.09
www.simcom.com 118 /652
(ipv4)/next header(ipv6)>,<source portrange>,<destinationport
range>,<ipsecsecurity parameterindex (spi)>,<typeofservice
(tos)(ipv4)andmask /trafficclass (ipv6)andmask>,<direction>
[<CR><LF>+CGTFT: <cid>,<packetfilteridentifier>,<evaluation
precedenceindex>,<sourceaddress andsubnet
mask>,<protocolnumber(ipv4)/next header(ipv6)>,<source
portrange>,<destination portrange>,<ipsec security parameter
index(spi)>,<type ofservice (tos)(ipv4)andmask/traffic class
(ipv6)andmask>,<direction>
[..]]]
OK
2)
ERROR
WriteCommand
AT+CGTFT=<cid>[,[<packet
filter identifier>,<evaluation
precedence index>[,<source
address and subnet
mask>[,<protocol number
(ipv4)/ next header
(ipv6)>[,<destination port
range>[,<source port
range>[,<ipsec security
parameter index
(spi)>[,<type of service
(tos)(ipv4)and mask / traffic
class (ipv6)and
mask>[,<flow label
(ipv6)>[,<direction>]]]]]]]]]]
Response
1)
OK
2)
ERROR
Execution Command
AT+CGTFT
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> anumeric parameterwhichspecifies aparticularPDPcontext
definition (seetheAT+CGDCONTandAT+CGDSCONTcommands).
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol


## Página 120

A76XX Series_AT Command Manual_V1.09
www.simcom.com 119 /652
IPV6 InternetProtocol Version6
IPV4V6 DualPDNStack
<packetfilteridentifier> anumeric parameter,value range from1to 15.
<evaluationprecedence
index>
anumeric parameter.Thevaluerangeis from0to255.
<sourceaddress and
subnetmask>
stringtypeThestringis givenas dot-separated numeric
(0-255)parameters on the form:
"a1.a2.a3.a4.m1.m2.m3.m4"forIPv4or
"a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.m1.m2.m
3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16",forIPv6.
NOTE:subnetmaskcan’tbe 0.0.0.0
<protocolnumber (ipv4)/
nextheader (ipv6)>
anumeric parameter,value range from0to 255.
<destinationport range> stringtype.Thestringis givenas dot-separatednumeric
(0-65535)parameterson the form"f.t".
<sourceport range> stringtype.Thestringis givenas dot-separatednumeric
(0-65535)parameterson the form"f.t".
<ipsecsecurity parameter
index (spi)>
numeric valuein hexadecimalformat.Thevalue rangeisfrom
00000000 toFFFFFFFF.
<typeofservice
(tos)(ipv4)and mask /traffic
class(ipv6)and mask>
stringtype.Thestringis givenas dot-separatednumeric
(0-255)parameters on the form"t.m".
<flowlabel(ipv6)> numeric valuein hexadecimalformat.Thevalue rangeisfrom 00000
to FFFFF.ValidforIPv6only.
<direction> integertype.Specifiesthe transmission direction inwhichthe packet
filtershallbe applied.
0 Pre-Release 7TFTfilter
1 Uplink
2 Downlink
3 Up&Downlink
Examples
AT+CGTFT=?
+CGTFT:
"IP",(1-15),(0-255),,(0-255),(0-65535.0-65535),(0-65535.0-65535),(0-FFFFFFFF),(0-255.0-255),(0-FFF
FF)
+CGTFT:
"IPV6",(1-15),(0-255),,(0-255),(0-65535.0-65535),(0-65535.0-65535),(0-FFFFFFFF),(0-255.0-255),(0-F
FFFF)
+CGTFT:
"IPV4V6",(1-15),(0-255),,(0-255),(0-65535.0-65535),(0-65535.0-65535),(0-FFFFFFFF),(0-255.0-255),(0
-FFFFF)


## Página 121

A76XX Series_AT Command Manual_V1.09
www.simcom.com 120 /652
OK
AT+CGTFT?
+CGTFT:
OK
AT+CGTFT=1,1,0,"74.125.71.100.255.255.255.255"
OK
AT+CGTFT
OK
Ifaspecified PDPcontext isdeactivate, the correspondingPacketFilterTFTneed tobespecified
again.
5.2.8 AT+CGQREQ Qualityofserviceprofile(requested)
This commandallows theTEtospecifyaQualityof ServiceProfilethat isusedwhen the MTsends an
Activate PDPContextRequestmessage tothe network.Aspecial form ofthe setcommand
(AT+CGQREQ=<cid>)causesthe requestedprofileforcontextnumber<cid>to becomeundefined.
AT+CGQREQ Qualityofserviceprofile(requested)
TestCommand
AT+CGQREQ=?
Response
1)
+CGQREQ:<PDP_type>,(listof supported<precedence>s),(listof
supported<delay>s),(listof supported<reliability>s),(listof
supported<peak>s),(listofsupported <mean>s)
OK
2)
ERROR
Read Command
AT+CGQREQ?
Response
1)
+CGQREQ:
[<cid>,<precedence>,<delay>,<reliability>,<peak>,<mean>[<CR>
<LF><LF>
+CGQREQ:
<cid>,<precedence>,<delay>,<reliability>,<peak>,<mean>[…]]]
OK
NOTE


## Página 122

A76XX Series_AT Command Manual_V1.09
www.simcom.com 121 /652
2)
ERROR
WriteCommand
AT+CGQREQ=<cid>[,<prece
dence>[,<delay>[,<reliability
>[,<peak>[,<mean>]]]]]
Response
1)
OK
2)
ERROR
Execution Command
AT+CGQREQ
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> Anumeric parameterwhichspecifies aparticularPDPcontext
definition (seeAT+CGDCONTcommand).Therange isfrom1to 15
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol
<precedence> Anumeric parameterwhichspecifies theprecedence class:
0 network subscribed value
1 high priority
2 normalpriority
3 lowpriority
<delay> Anumeric parameterwhichspecifies thedelay class:
0 network subscribed value
1 delay class1
2 delay class2
3 delay class3
4 delay class4
<reliability> Anumeric parameterwhichspecifies thereliability class:
0 network subscribed value
1 Non real-timetraffic,error-sensitiveapplication thatcannot
copewith data loss
2 Non real-timetraffic,error-sensitiveapplication thatcan cope
with infrequent dataloss
3 Non real-timetraffic,error-sensitiveapplication thatcan cope
with dataloss,GMM/-
SM, and SMS
4 Real-timetraffic,error-sensitiveapplication thatcan cope with


## Página 123

A76XX Series_AT Command Manual_V1.09
www.simcom.com 122 /652
data loss
5 Real-timetrafficerrornon-sensitive applicationthat cancope
with data loss
<peak> Anumeric parameterwhichspecifies thepeak throughputclass:
0 network subscribed value
1 Up to1000 (8kbit/s)
2 Up to2000 (16 kbit/s)
3 Up to4000 (32 kbit/s)
4 Up to8000 (64 kbit/s)
5 Up to16000 (128kbit/s)
6 Up to32000 (256kbit/s)
7 Up to64000 (512kbit/s)
8 Up to128000(1024kbit/s)
9 Up to256000(2048kbit/s)
<mean> Anumeric parameterwhichspecifies themean throughputclass:
0 network subscribedvalue
1 100(~0.22 bit/s)
2 200(~0.44 bit/s)
3 500(~1.11bit/s)
4 1000 (~2.2bit/s)
5 2000 (~4.4bit/s)
6 5000 (~11.1bit/s)
7 10000 (~22 bit/s)
8 20000 (~44 bit/s)
9 50000 (~111bit/s)
10 100000(~0.22 kbit/s)
11 200000(~0.44 kbit/s)
12 500000(~1.11kbit/s)
13 1000000 (~2.2kbit/s)
14 2000000 (~4.4kbit/s)
15 5000000 (~11.1kbit/s)
16 10000000 (~22kbit/s)
17 20000000 (~44kbit/s)
18 50000000 (~111kbit/s)
31 optimization
Examples
AT+CGQREQ=?
+CGQREQ:"IP",(0-3),(0-4),(0-5),(0-9),(0-18,31)
OK
AT+CGQREQ?
+CGQREQ:1,3,4,3,9,31


## Página 124

A76XX Series_AT Command Manual_V1.09
www.simcom.com 123 /652
OK
AT+CGQREQ=1,3,4,3,9,31
OK
AT+CGQREQ
OK
5.2.9 AT+CGEQREQ 3Gqualityofserviceprofile(requested)
Thetest commandreturns values supported asacompound value.
Thereadcommand returns the currentsettings foreach definedcontext forwhichaQOSwasexplicitly
specified.
Thewritecommand allows theTEtospecifyaQuality of ServiceProfile forthe context identifiedby the
context identificationparameter<cid> which isusedwhen the MTsendsanActivatePDPContextRequest
messageto the network.
Aspecialform ofthe writecommand,AT+CGEQREQ=<cid>causes the requestedprofile forcontext
number<cid>to becomeundefined.
AT+CGEQREQ 3Gqualityofserviceprofile(requested)
TestCommand
AT+CGEQREQ=?
Response
1)
+CGEQREQ:<PDP_type>,(listofsupported <Trafficclass>s),(list
ofsupported<Maximum bitrate UL>s),(listofsupported <Maximum
bitrateDL>s),(listof supported<Guaranteed bitrate UL>s,(listof
supported<Guaranteed bitrateDL>s),(listofsupported<Deliv ery
order>s),(listofsupported<MaximumSDUsize>s),(listofsupported
<SDUerrorratio>s),(listof supported<Residualbiterror
Ratio>s),(listofsupported <Deliveryof erroneous SDUs>s),(listof
Supported<Transferdelay>s),(listofsupported <Traffic handling
priority>s),(list ofsupported<Sourcestatistics descriptor>s),(list
ofsupported<Signalingindication flag>s)
OK
2)
ERROR
Read Command
AT+CGEQREQ?
Response
1)
+CGEQREQ:[<cid>,<Traffic class>,<Maximum bitrate
UL>,<Maximum bitrateDL>,<Guaranteed bitrate
UL>,<Guaranteedbitrate DL>,<Delivery order>,<Maximum SDU


## Página 125

A76XX Series_AT Command Manual_V1.09
www.simcom.com 124 /652
size>,<SDUerrorratio>,<Residualbit errorratio>,<Delivery of
erroneous SDUs>,<TransferDelay>,<Traffic handling
priority>,<Sourcestatistics descriptor>,<Signalingindication
flag>][<CR><LF><LF>
+CGEQREQ:<cid>,<Traffic class>,<Maximum bitrate
UL>,<Maximum bitrateDL>,<Guaranteed bitrate
UL>,<Guaranteedbitrate DL>,<Delivery order>,<Maximum SDU
size>,<SDUerrorratio>,<Residualbit errorratio>,<Delivery of
erroneous SDUs>,<TransferDelay>,<Traffic handling
priority>,<Sourcestatistics descriptor>,<Signalingindication
flag>[…]]
OK
2)
ERROR
WriteCommand
AT+CGEQREQ=<cid>[,<Traff
icclass>[,<Maximum bitrate
UL>[,<Maximumbitrate
DL>[,<Guaranteedbitrate
UL>[,<Guaranteedbitrate
DL>[,<Delivery
order>[,<MaximumSDU
size>[,<SDUerror
ratio>[,<Residualbit error
ratio>[,<Deliveryof
erroneousSDUs>[,<Transfer
delay>[,<Traffic handling
priority>[,<Sourcestatistics
descriptor>[,<Signaling
indication flag>]]]]]]]]]]]]]]
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+CGEQREQ
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> Parameterspecifies aparticularPDPcontextdefinition.The
parameteris alsoused inotherPDPcontext-related commands.The


## Página 126

A76XX Series_AT Command Manual_V1.09
www.simcom.com 125 /652
rangeis from1to15
<Trafficclass> 0 conversational
1 streaming
2 interactive
3 background
4 subscribed value
<Maximum bitrate UL> This parameterindicates themaximum numberofkbits/s deliveredto
UMTS(up-linktraffic)ataSAP.Asan Examplesabitrateof 32kbit/s
would be specified as32(e.g.AT+CGEQREQ=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and 8640
(except8640), itshouldbe an integermultiple of 64;between 8641
and16000,itshouldbeanintegermultipleof100;between16000and
128000, itshouldbe anintegermultiple of1000;between 128000and
256000, itshouldbe anintegermultiple of2000.Thedefaultvalue is
0. Ifthe parameter issetto'0' the subscribedvalue will berequested.
<Maximum bitrate DL> This parameterindicates themaximum numberofkbits/s deliveredto
UMTS(down-linktraffic)ataSAP.AsanExamplesabitrateof32kbit/s
would be specified as32(e.g.AT+CGEQREQ=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and 8640
(except8640), itshouldbe an integermultiple of 64;between 8641
and16000,itshouldbeanintegermultipleof100;between16000and
128000, itshouldbe anintegermultiple of1000;between 128000and
256000, itshouldbe anintegermultiple of2000.Thedefaultvalue is
0. Ifthe parameter issetto'0' the subscribedvalue will be
requested.3600-3800)
<Guaranteed bitrate UL> This parameterindicates theguaranteednumber ofkbit/s deliveredto
UMTS(up-linktraffic)ataSAP(providedthatthereisdatatodeliver).As
an Examplesabitrate of32kbit/s would bespecified as
32(e.g.AT+CGEQREQ=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and
8640(except8640),it should bean integermultiple of64;between
8641 and 16000,it should bean integermultiple of100; between
16000 and128000, itshould bean integermultiple of1000;between
128000 and256000, itshould bean integermultiple of2000.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Guaranteed bitrate DL> This parameterindicates theguaranteednumber ofkbit/s deliveredto
UMTS(down-link traffic)ataSAP(providedthat thereis datato
deliver).As an Examplesabitrateof 32kbit/s would bespecified as
32(e.g.AT+CGEQREQ=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and


## Página 127

A76XX Series_AT Command Manual_V1.09
www.simcom.com 126 /652
8640(except8640),it should bean integermultiple of64;between
8641 and 16000,it should bean integermultiple of100; between
16000 and128000, itshould bean integermultiple of1000;between
128000 and256000, itshould bean integermultiple of2000.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Delivery order> This parameterindicates whetherthe UMTSbearer shallprovide
in-sequenceSDUdelivery ornot.
0 no
1 yes
2 subscribed value
<Maximum SDUsize> This parameterindicates themaximum allowed SDUsize in octets.
Therangeis 0,10 to1500,1510, 1520.Whenthe parameteris
between 10 and1510,it shouldbe anintegermultiple of10.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<SDUerrorratio> This parameterindicates thetarget valueforthe fraction ofSDUs lost
or detected aserroneous.SDUerrorratiois definedonly for
conformingtraffic.AsanExamplesatargetSDUerrorratio of5*10-3
would be specified as"5E3"(e.g.AT+CGEQREQ=.,"5E3",…).
"0E0" subscribed value
"1E2"
"7E3"
"1E3"
"1E4"
"1E5"
"1E6"
"1E1"
<Residualbit errorratio> This parameterindicates thetarget valueforthe undetected bit error
ratio inthe delivered SDUs.Ifnoerrordetection isrequested,
Residualbiterrorratioindicates the biterrorratioin the delivered
SDUs.AsanExamples atargetresidual biterrorratioof5*10-3would
be specified as"5E3"(e.g.
AT+CGEQREQ=…,"5E3",.).
"0E0" subscribedvalue
"5E2"
"1E2"
"5E3"
"4E3"
"1E3"
"1E4"
"1E5"
"1E6"
"6E8"
<Delivery oferroneous This parameterindicates whetherSDUs detected as erroneous shall


## Página 128

A76XX Series_AT Command Manual_V1.09
www.simcom.com 127 /652
SDUs> be deliveredornot.
0 no
1 yes
2 no detect
3 subscribed value
<Transferdelay> Thisparameterindicatesthetargetedtimebetweenrequesttotransfer
an SDUatoneSAPto itsdelivery atthe otherSAP,in milliseconds.
Therangeis 0to950. Whenthe parameterisbetween 10 and150, it
should bean integermultipleof10. When theparameteris between
150and 950,itshould bean integermultiple of50.Thedefaultvalue
is 0.Ifthe parameteris setto '0'the subscribed valuewillbe
requested.
<Traffichandlingpriority> This parameterspecifies the relativeimportanceforhandlingofall
SDUs belonging tothe UMTS
Bearercomparedto the SDUs ofthe otherbearers.
Therangeis from0to3.Thedefaultvalue is0. Iftheparameteris set
to '0'the subscribedvalue will berequested.
<Sourcestatistics
descriptor>
This parameterindicates profileparameterthat Sourcestatistics
descriptorforrequested UMTSQoSTherange isfrom 0to1.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Signalingindication flag> This parameterindicates Signaling flag.
Therangeis from0to1Thedefaultvalue is0.Ifthe parameteris set
to '0'the subscribedvalue will berequested.
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol
Examples
AT+CGEQREQ=?
+CGEQREQ:
"IP",(0-4),(0-256000),(0-256000),(0-256000),(0-256000),(0-2),(0-1520),("0E0","1E1","1E2","7E3","1E
3","1E4","1E5","1E6"),("0E0","5E2","1E2","5E3","4E3","1E3","1E4","1E5","1E6","6E8"),(0-3),(0-95
0),(0-3),(0-1),(0-1)
OK
AT+CGEQREQ?
+CGEQREQ:1,4,0,0,0,0,2,0,"0E0","0E0",3,0,0,0,0
OK
AT+CGEQREQ=1,4,0,0,0,0,2,0,"0E0","0E0",3,0,0,0,0
OK
AT+CGEQREQ


## Página 129

A76XX Series_AT Command Manual_V1.09
www.simcom.com 128 /652
OK
5.2.10 AT+CGQMIN Qualityofserviceprofile(minimumacceptable)
This commandallows theTEtospecifyaminimumacceptable profilewhichis checkedby the MTagainst
the negotiatedprofile returned intheActivate PDPContextAccept message.Aspecial formof theset
command,AT+CGQMIN=<cid>causesthe minimumacceptable profileforcontextnumber<cid> to
becomeundefined.
AT+CGQMIN Qualityofserviceprofile(minimumacceptable)
TestCommand
AT+CGQMIN=?
Response
1)
+CGQMIN:<PDP_type>,(listofsupported <precedence>s),(listof
supported<delay>s),(listof supported<reliability>s),(listof
supported<peak>s),(listofsupported <mean>s)[<CR><LF>
+CGQMIN:<PDP_type>,(listofsupported <precedence>s),(listof
supported<delay>s),(listof supported<reliability>s),(listof
supported<peak>s),(listofsupported <mean>s)[…]]
OK
2)
ERROR
Read Command
AT+CGQMIN?
Response
1)
+CGQMIN:
[<cid>,<precedence>,<delay>,<reliability>,<peak>,<mean>[<CR>
<LF><LF>
+CGQMIN:
<cid>,<precedence>,<delay>,<reliability>,<peak>,<mean>[…]]]
OK
2)
ERROR
WriteCommand
AT+CGQMIN=<cid>[,<preced
ence>[,<delay>[,<reliability>
[,<peak>[,<mean>]]]]]
Response
1)
OK
2)
ERROR
Execution Command
AT+CGQMIN
Response
1)
OK
2)


## Página 130

A76XX Series_AT Command Manual_V1.09
www.simcom.com 129 /652
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> Anumeric parameterwhichspecifies aparticularPDPcontext
definition (seeAT+CGDCONTcommand).Therange isfrom1to 15
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol
<precedence> Anumeric parameterwhichspecifies theprecedence class:
0 network subscribed value
1 high priority
2 normalpriority
3 lowpriority
<delay> Anumeric parameterwhichspecifies thedelay class:
0 network subscribed value
1 delay class1
2 delay class2
3 delay class3
4 delay class4
<reliability> Anumeric parameterwhichspecifies thereliability class:
0 network subscribed value
1 Non real-timetraffic,error-sensitiveapplication thatcannot
copewith data loss
2 Non real-timetraffic,error-sensitiveapplication thatcan cope
with infrequent dataloss
3 Non real-timetraffic,error-sensitiveapplication thatcan cope
with dataloss,GMM/-
SM, and SMS
4 Real-timetraffic,error-sensitive application thatcancopewith
data loss
5 Real-timetrafficerrornon-sensitive applicationthat cancope
with data loss
<peak> Anumeric parameterwhichspecifies thepeak throughputclass:
0 network subscribed value
1 Up to1000 (8kbit/s)
2 Up to2000 (16 kbit/s)
3 Up to4000 (32 kbit/s)
4 Up to8000 (64 kbit/s)
5 Up to16000 (128kbit/s)
6 Up to32000 (256kbit/s)


## Página 131

A76XX Series_AT Command Manual_V1.09
www.simcom.com 130 /652
7 Up to64000 (512kbit/s)
8 Up to128000(1024kbit/s)
9 Up to256000(2048kbit/s)
<mean> Anumeric parameterwhichspecifies themean throughputclass:
0 network subscribedvalue
1 100(~0.22 bit/s)
2 200(~0.44 bit/s)
3 500(~1.11bit/s)
4 1000 (~2.2bit/s)
5 2000 (~4.4bit/s)
6 5000 (~11.1bit/s)
7 10000 (~22 bit/s)
8 20000 (~44 bit/s)
9 50000 (~111bit/s)
10 100000(~0.22 kbit/s)
11 200000(~0.44 kbit/s)
12 500000(~1.11kbit/s)
13 1000000 (~2.2kbit/s)
14 2000000 (~4.4kbit/s)
15 5000000 (~11.1kbit/s)
16 10000000 (~22kbit/s)
17 20000000 (~44kbit/s)
18 50000000 (~111kbit/s)
31 optimization
Examples
AT+CGQMIN=?
+CGQMIN:"IP",(0-3),(0-4),(0-5),(0-9),(0-18,31)
OK
AT+CGQMIN?
+CGQMIN:1,3,4,5,1,1
OK
AT+CGQMIN=1,3,4,5,1,1
OK
AT+CGQMIN
OK
5.2.11 AT+CGEQMIN 3Gqualityofserviceprofile(minimumacceptable)


## Página 132

A76XX Series_AT Command Manual_V1.09
www.simcom.com 131 /652
Thetest commandreturns values supported asacompound value.
Thereadcommand returns the currentsettings foreach definedcontext forwhichaQOSwasexplicitly
specified.
Thewritecommand allowtheTEtospecifyaQuallity ofServiceProfile forthe contextidentified by the
context identificationparameter<cid>whichis checkedby the MTagainstthe negotiatedprofile returnedin
theActivate/ModifyPDPContextAcceptmessage.
Aspecialformofthe writecommand,AT+CGEQMIN=<cid>causestherequestedforcontextnumber<cid>
tobecomeundefined.
AT+CGEQMIN 3Gqualityofserviceprofile(minimumacceptable)
TestCommand
AT+CGEQMIN=?
Response
1)
+CGEQMIN:<PDP_type>,(list ofsupported<Traffic class>s),(listof
supported<Maximum bitrate UL>s),(listofsupported <Maximum
bitrateDL>s),(listof supported<Guaranteed bitrate UL>s,(listof
supported<Guaranteed bitrateDL>s),(listof supported<Delivery
order>s),(listofsupported<MaximumSDUsize>s),(listofsupported
<SDUerrorratio>s),(listof supported<Residualbiterror
Ratio>s),(listofsupported <Deliveryof erroneous SDUs>s),(list of
Supported<Transferdelay>s),(listofsupported <Traffic
handlingpriority>s),(listofsupported <Source statistics
descriptor>s),(listofsupported <Signalingindicationflag>s)
OK
2)
ERROR
Read Command
AT+CGEQMIN?
Response
1)
+CGEQMIN:[<cid>,<Traffic class>,<Maximum bitrate
UL>,<Maximum bitrateDL>,<Guaranteed bitrate
UL>,<GuaranteedbitrateDL>,<Deliveryorder>,<Maximum SDU
size>,<SDUerrorratio>,<Residualbit errorratio>,<Delivery of
erroneous SDUs>,<TransferDelay>,<Traffic handling
priority>,<Sourcestatistics descriptor>,<Signalingindication
flag>][<CR><LF><LF>+CGEQMIN:<cid>,<Traffic
class>,<Maximum bitrate UL>,<Maximum bitrate
DL>,<Guaranteedbitrate UL>,<GuaranteedbitrateDL>,<Delivery
order>,<Maximum SDUsize>,<SDUerrorratio>,<Residualbit
errorratio>,<Delivery oferroneousSDUs>,<Transfer
Delay>,<Traffichandlingpriority>,<Source statistics
descriptor>,<Signalingindicationflag>[…]]


## Página 133

A76XX Series_AT Command Manual_V1.09
www.simcom.com 132 /652
OK
2)
ERROR
WriteCommand
AT+CGEQMIN=<cid>[,<Traffi
c class>[,<Maximum bitrate
UL>[,<Maximum bitrate
DL>[,<Guaranteed bitrate
UL>[,<Guaranteed
bitrateDL>[,<Delivery
order>[,<Maximum SDU
size>[,<SDU error
ratio>[,<Residual biterror
ratio>[,<Delivery of
erroneous SDUs>[,<Transfer
delay>[,<Traffic
handlingpriority>[,<Source
statistics
descriptor>[,<Signaling
indication flag>]]]]]]]]]]]]]]
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+CGEQMIN
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> Parameterspecifies aparticularPDPcontextdefinition.The
parameteris alsoused inotherPDPcontext-related commands.The
rangeis from1to15.
<Trafficclass> 0 conversational
1 streaming
2 interactive
3 background
4 subscribed value
<Maximum bitrate UL> This parameterindicates themaximum numberofkbits/s deliveredto
UMTS(up-linktraffic)ataSAP.Asan Examples abitrateof32kbit/s
would be specified as32(e.g.AT+CGEQMIN=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween
64 and568, itshouldbe anintegermultiple of8; between 568 and


## Página 134

A76XX Series_AT Command Manual_V1.09
www.simcom.com 133 /652
8640(except8640),it should bean integermultiple of64;between
8641 and 16000,it should bean integermultiple of100; between
16000 and128000, itshould bean integermultiple of1000;between
128000 and256000, itshould bean integermultiple of2000.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Maximum bitrate DL> This parameterindicates themaximum numberofkbits/s deliveredto
UMTS(down-link traffic)ataSAP.AsanExamples abitrate of32kbit/s
would be specified as32(e.g.AT+CGEQMIN=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and
8640(except8640),it should bean integermultiple of64;between
8640 and 16000,it should bean integermultiple of100; between
16000 and128000, itshould bean integermultiple of1000;between
128000 and256000, itshould bean integermultiple of2000.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Guaranteed bitrate UL> This parameterindicates theguaranteednumber ofkbit/s deliveredto
UMTS(up-linktraffic)ataSAP(providedthatthereisdatatodeliver).As
an Examplesabitrate of32kbit/s would bespecified as
32(e.g.AT+CGEQMIN=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and
8640(except8640),it should bean integermultiple of64;between
8640 and 16000,it should bean integermultiple of100; between
16000 and128000, itshould bean integermultiple of1000;between
128000 and256000, itshould bean integermultiple of2000.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Guaranteed bitrate DL> This parameterindicates theguaranteednumber ofkbit/s deliveredto
UMTS(down-link traffic)ataSAP(providedthat thereis datato
deliver).As an Examplesabitrateof 32kbit/s would bespecified as
32(e.g.AT+CGEQMIN=…,32,…).
Therangeis from0to256000. Whenthe parameterisbetween 64
and568, itshouldbe an integermultiple of 8;between 568and
8640(except8640),it should bean integermultiple of64;between
8641 and 16000,it should bean integermultiple of100; between
16000 and128000, itshould bean integermultiple of1000;between
128000 and256000, itshould bean integermultiple of2000.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<Delivery order> This parameterindicates whetherthe UMTSbearer shallprovide
in-sequenceSDUdelivery ornot.
0 no
1 yes


## Página 135

A76XX Series_AT Command Manual_V1.09
www.simcom.com 134 /652
2 subscribed value
<Maximum SDUsize> This parameterindicates themaximum allowed SDUsize inoctets.
Therangeis 0,10 to1500,1510, 1520.Whenthe parameteris
between 10 and1510,it shouldbe anintegermultiple of10.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.
<SDUerrorratio> This parameterindicates thetarget valueforthe fraction ofSDUs lost
or detected aserroneous.SDUerrorratiois definedonly for
conforming traffic.Asan Examplesatarget SDUerrorratioof 5*10-3
would be specified as"5E3"(e.g.AT+CGEQMIN=.,"5E3",…).
"0E0" subscribed value
"1E2"
"7E3"
"1E3"
"1E4"
"1E5"
"1E6"
"1E1"
<Residualbit errorratio> This parameterindicates thetarget valueforthe undetected bit error
ratio inthe delivered SDUs.Ifnoerrordetection isrequested,
Residualbiterrorratioindicates the biterrorratioin the delivered
SDUs.AsanExamples atargetresidual biterrorratioof5*10-3would
be specified as"5E3"(e.g.
AT+CGEQMIN=…,"5E3",.).
"0E0" subscribed value
"5E2"
"1E2"
"5E3"
"4E3"
"1E3"
"1E4"
"1E5"
"1E6"
"6E8"
<Delivery oferroneous
SDUs>
This parameterindicates whetherSDUs detected as erroneous shall
be deliveredornot.
0 no
1 yes
2 no detect
3 subscribed value
<Transferdelay> Thisparameterindicatesthetargetedtimebetweenrequesttotransfer
an SDUatoneSAPto itsdelivery atthe otherSAP,in milliseconds.
Therangeis from0to950,andthe parameter isan integerof 10.The
defaultvalueis0.Iftheparameterissetto'0'thesubscribedvaluewill
be requested.


## Página 136

A76XX Series_AT Command Manual_V1.09
www.simcom.com 135 /652
<Traffichandlingpriority> This parameterspecifies the relativeimportanceforhandlingofall
SDUs belonging tothe UMTS.
Bearercomparedto the SDUs ofthe otherbearers.
Therangeis 0to3.Thedefaultvalue is0.Ifthe parameteris setto'0'
the subscribed valuewillbe requested.
<Sourcestatistics
descriptor>
This parameterindicates profileparameterthat Sourcestatistics
descriptorforrequested UMTSQoS
Therangeis from0to1.Thedefaultvalue is0. Iftheparameteris set
to '0'the subscribedvalue will berequested.
<Signalingindication flag> This parameterindicates Signaling flag.
Therangeis from0to1Thedefaultvalue is0.Ifthe parameteris set
to '0'the subscribedvalue will berequested.
<PDP_type> (PacketDataProtocoltype)astringparameterwhichspecifiesthetype
of packetdataprotocol.
IP InternetProtocol
Examples
AT+CGEQMIN=?
+CGEQMIN:
"IP",(0-4),(0-256000),(0-256000),(0-256000),(0-256000),(0-2),(0-1520),("0E0","1E1","1E2","7E3","1E
3","1E4","1E5","1E6"),("0E0","5E2","1E2","5E3","4E3","1E3","1E4","1E5","1E6","6E8"),(0-3),(0-95
0),(0-3),(0-1),(0-1)
OK
AT+CGEQMIN?
+CGEQMIN:1,4,0,0,0,0,2,0,"0E0","0E0",3,0,0,0,0
OK
AT+CGEQMIN=1,4,0,0,0,0,2,0,"0E0","0E0",3,0,0,0,0
OK
AT+CGEQMIN
OK
5.2.12 AT+CGDATA Enterdatastate
Thecommand causesthe MTtoperform whatever actionsare necessary toestablish communication
between theTEandthe network usingone ormorePacketDomain PDPtypes.Thismay include
performing aPSattachandone ormore PDPcontext activations.
AT+CGDATA Enterdatastate


## Página 137

A76XX Series_AT Command Manual_V1.09
www.simcom.com 136 /652
TestCommand
AT+CGDATA=?
Response
1)
+CGDATA:(listofsupported<L2P>s)
OK
2)
ERROR
WriteCommand
AT+CGDATA=[<L2P>,[<cid>]
]
Response
1)
CONNECT[<text>]
2)
NOCARRIER
3)
OK
4)
ERROR
5)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<L2P> Astringparameter thatindicatesthe layer 2protocol tobe used
between theTEandMT.
NULL
<text> CONNECTresult code string;the string formats pleasereferATX
command.
<cid> Anumeric parameterwhichspecifies aparticularPDPcontext
definition (seeAT+CGDCONTcommand).
1…15
Examples
AT+CGDATA=?
+CGDATA: （""）
OK
AT+CGDATA="",1
CONNECT


## Página 138

A76XX Series_AT Command Manual_V1.09
www.simcom.com 137 /652
5.2.13 AT+CGPADDR ShowPDPaddress
Thewritecommand returnsalist ofPDPaddresses forthe specified contextidentifiers.
AT+CGPADDR ShowPDPaddress
TestCommand
AT+CGPADDR=?
Response
1)
[+CGPADDR:(listofdefined<cid>s)]
OK
2)
ERROR
WriteCommand
AT+CGPADDR=<cid>[,<cid>[
,…]]
Response
1)
[+CGPADDR:<cid>,<PDP_addr>[<CR><LF>
+CGPADDR:<cid>,<PDP_addr>[..]]]
OK
2)
SIMcard supports IPV4V6 type andthe PDP_type ofthe command
"at+cgdcont"defined is ipv4v6:
[+CGPADDR:<cid>,<PDP_addr_IPV4>,<PDP_addr_IPV6>]
+CGPADDR:<cid>,<PDP_addr_IPV4>,<PDP_addr_IPV6>[..]
OK
3)
ERROR
Execution Command
AT+CGPADDR
Response
1)
[+CGPADDR:<cid>,<PDP_addr>]
+CGPADDR:<cid>,<PDP_addr>[..]
OK
2)
SIMcard supports IPV4V6 type andthe PDP_type ofthe command
"at+cgdcont"defined is ipv4v6:
[+CGPADDR:<cid>,<PDP_addr_IPV4>,<PDP_addr_IPV6>]
+CGPADDR:<cid>,<PDP_addr_IPV4>,<PDP_addr_IPV6>[..]
OK
3)
ERROR
4)
+CMEERROR: <err>


## Página 139

A76XX Series_AT Command Manual_V1.09
www.simcom.com 138 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> Anumeric parameterwhichspecifies aparticularPDPcontext
definition (seeAT+CGDCONTcommand).Ifno <cid>isspecified,the
addresses forall definedcontextsare returned.
1…15
<PDP_addr> Astringthat identifiesthe MTin the addressspaceapplicableto the
PDP.Theaddress maybe staticordynamic.Forastatic address,it
willbe the onesetby theAT+CGDCONTcommand whenthe context
was defined.Foradynamic addressitwillbethe oneassignedduring
thelastPDPcontextactivationthatusedthecontextdefinitionreferred
to by<cid>.<PDP_addr>is omitted ifnone isavailable.
<PDP_addr_IPV4> Astringparameter thatidentifiesthe MTin the addressspace
applicable tothe PDP.
<PDP_addr_IPV6> Astringparameter thatidentifiesthe MTin the addressspace
applicable tothe PDPwhenthe sim_cardsupports ipv6.The pdptype
mustbe setto"ipv6"or"ipv4v6" bytheAT+CGDCONTcommand.
Examples
AT+CGPADDR=?
+CGPADDR: (1)
OK
AT+CGPADDR=1
+CGPADDR: 1,10.83.214.110
OK
AT+CGPADDR
+CGPADDR: 1,10.83.214.110
OK
5.2.14 AT+CGCLASS GPRSmobilestationclass
This commandis used tosetthe MTtooperate accordingtothe specified GPRSmobileclass.


## Página 140

A76XX Series_AT Command Manual_V1.09
www.simcom.com 139 /652
AT+CGCLASS GPRSmobilestationclass
TestCommand
AT+CGCLASS=?
Response
1)
+CGCLASS:(listof supported<class>s)
OK
2)
ERROR
Read Command
AT+CGCLASS?
Response
1)
+CGCLASS:<class>
OK
2)
ERROR
WriteCommand
AT+CGCLASS=<class>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+CGCLASS
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<class> Astringparameter which indicatesthe GPRSmobile class(in
descending orderoffunctionality)
A classA(highest)
Examples
AT+CGCLASS=?
+CGCLASS:("A")


## Página 141

A76XX Series_AT Command Manual_V1.09
www.simcom.com 140 /652
OK
AT+CGCLASS?
+CGCLASS:"A"
OK
AT+CGCLASS="A"
OK
AT+CGCLASS
OK
5.2.15 AT+CGEREP GPRSeventreporting
Thewrite commandenables ordisablessending ofunsolicitedresult codes,"+CGEV"fromMTtoTEin the
caseof certain events occurring inthe PacketDomain MTorthe network.<mode>controls the processing
ofunsolicited resultcodes specified within thiscommand.<bfr>controls theeffecton bufferedcodes when
<mode>1 or2isentered. Ifasettingis notsupported bythe MT,ERRORor +CME ERROR:is returned.
Read commandreturns thecurrent<mode> and buffersettings.
Testcommandreturns the modes andbuffersettings supportedby the MTascompound values.
AT+CGEREP GPRSeventreporting
TestCommand
AT+CGEREP=?
Response
1)
+CGEREP:(listofsupported <mode>s),(list ofsupported<bfr>s)
OK
2)
ERROR
Read Command
AT+CGEREP?
Response
1)
+CGEREP:<mode>,<bfr>
OK
2)
ERROR
WriteCommand
AT+CGEREP=<mode>[,<bfr>
]
Response
1)
OK
2)
ERROR
3)


## Página 142

A76XX Series_AT Command Manual_V1.09
www.simcom.com 141 /652
+CMEERROR: <err>
Execution Command
AT+CGEREP
Response
1)Setdefaultvalue(<mode>=2,<bfr>=0):
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 0 bufferunsolicitedresult codes inthe MT;ifMTresultcode bufferis
full, the oldestonescan bediscarded.No codes areforwarded tothe
TE.
1 discard unsolicitedresult codes whenMT-TElink isreserved (e.g.
in on-line datamode);otherwise forward them directlytotheTE.
2 bufferunsolicitedresult codes inthe MTwhen MT-TElink is
reserved (e.g.in on-line datamode)and flushthem totheTEwhen
MT-TElink becomesavailable;otherwise forward them directlytothe
TE.
<bfr> 0 MTbufferofunsolicited resultcodes defined within this command
is clearedwhen <mode>1or2isentered.
1 MTbufferofunsolicited resultcodes defined within this command
is flushedtotheTEwhen<mode> 1or2isentered (OKresponse
shall begiven beforeflushing the codes).
Theeventsare validforGPRS/UMTSand LTEunlessexplicitly mentioned.
Fornetworkattachment, the following unsolicitedresultcodes and the correspondingeventsare defined:
+CGEV:NWDETACH Thenetwork hasforced aPSdetach.Thisimplies thatallactive
contextshave beendeactivated.Theseare notreported separately.
+CGEV:MEDETACH Themobile termination has forced aPSdetach.This implies thatall
active contextshave beendeactivated.Theseare notreported
separately.
ForMTclass,the followingunsolicited resultcodes andthe correspondingeventsare defined:
+CGEV:NWCLASS <class> Thenetwork hasforced achangeofMTclass.Thehighestavailable
class isreported (see+CGCLASS).Theformatofthe parameter
<class>is foundincommand +CGCLASS.
+CGEV:MECLASS <class> Themobile termination has forced achange ofMTclass.Thehighest


## Página 143

A76XX Series_AT Command Manual_V1.09
www.simcom.com 142 /652
available class isreported(see +CGCLASS).Theformat ofthe
parameter<class>is foundin command+CGCLASS.
ForPDPcontextactivation,thefollowingunsolicitedresultcodesandthecorrespondingeventsaredefined:
+CGEV:NWPDNACT
<cid>[,<WLAN_Offload>]
Thenetwork hasactivated acontext.Thecontext representsa
Primary PDPcontext in GSM/UMTS.The<cid>forthiscontext is
provided to theTE.Theformatofthe parameter<cid> isfoundin
command +CGDCONT.
<WLAN_Offload>:integertype.Anintegerthat indicateswhether
trafficcanbe offloaded usingthe specified PDNconnection viaa
WLANornot.This refers tobit1(E-UTRANoffloadacceptability
value)and bit2(UTRANoffload acceptability value)inthe WLAN
offload acceptability IEas specified in3GPPTS24.008 [8] subclause
10.5.6.20.
0 offloading thetrafficof thePDN connectionviaaWLANwhen in
S1 mode orwhenin Iumodeis notacceptable.
1 offloading thetrafficof thePDN connectionviaaWLANwhen in
S1 mode isacceptable,butnotacceptablein Iumode.
2 offloading thetrafficof thePDN connectionviaaWLANwhen in
Iu mode isacceptable,butnotacceptable inS1mode.
3 offloading thetrafficof thePDN connectionviaaWLANwhen in
S1 mode orwhenin Iumodeis acceptable.
This eventis notapplicable forEPS.
+CGEV:MEPDNACT
<cid>[,<reason>[,<cid_other
>]][,<WLAN_Offload>]
Themobile termination has activated acontext.Thecontext
represents aPDNconnection inLTEoraPrimary PDPcontextin
GSM/UMTS.The<cid>forthiscontextis provided totheTE.This
event issenteitherinresult ofexplicitcontextactivation request
(+CGACT),orinresultofimplicitcontextactivationrequestassociated
to attachrequest(+CGATT=1).Theformatofthe parameters <cid>
and<cid_other>are foundincommand +CGDCONT.Theformatof
the parameter<WLAN_Offload>isdefined above.
<reason>: integertype; indicatesthe reason why the context
activation requestforPDPtype IPv4v6 was notgranted.This
parameteris only includedifthe requestedPDPtype associatedwith
NOTE


## Página 144

A76XX Series_AT Command Manual_V1.09
www.simcom.com 143 /652
<cid> isIPv4v6,and thePDPtype assignedby the network for<cid>
is eitherIPv4or IPv6.
0 IPv4 onlyallowed
1 IPv6 onlyallowed
2 single address bearers only allowed.
3 single address bearers only allowed andMTinitiated
contextactivation forasecondaddress type
bearerwas notsuccessful.
4 CI_PS_PDP_INVALID_REASON
<cid_other>:integertype;indicates the contextidentifierallocatedby
MTforanMTinitiated contextofasecondaddresstype.MTshallonly
include thisparameterif<reason> parameterindicates single address
bearers only allowed,and MTsupports MTinitiatedcontext activation
of asecond address type without additionalcommands fromTE,and
MThas activatedthe PDNconnection orPDPcontextassociatedwith
<cid_other>.
ForlegacyTEssupporting MTinitiated contextactivation withoutTErequests,there isalsoa
subsequentevent+CGEV:ME PDNACT<cid_other>returned toTE.
+CGEV:NWACT
<p_cid>,<cid>,<event_type>
[,<WLAN_Offload>]
Thenetwork hasactivated acontext.The<cid> forthiscontext is
provided to theTEinaddition tothe associatedprimary<p_cid>.The
format ofthe parameters <p_cid>and <cid>are found incommand
+CGDSCONT.The formatofthe parameter<WLAN_Offload>is
defined above.
<event_type>:integertype; indicates whetherthis isan informational
event orwhethertheTEhasto acknowledge it.
0 Informationalevent
1 Informationrequest:Acknowledgementrequired.The
acknowledgement canbe acceptorreject, see+CGANS.
+CGEV:MEACT
<p_cid>,<cid>,<event_type>
[,<WLAN_Offload>]
Thenetwork hasresponded to anME initiated contextactivation.The
<cid> forthiscontext is providedto theTEin additionto the
associated primary<p_cid>.The formatofthe parameters <p_cid>
and<cid> are found incommand +CGDSCONT.Theformatof the
parameters <event_type> and<WLAN_Offload>are defined above.
NOTE


## Página 145

A76XX Series_AT Command Manual_V1.09
www.simcom.com 144 /652
ForPDPcontext deactivation,the following unsolicited resultcodes andthe corresponding events are
defined:
+CGEV:NWDEACT
<PDP_type>,<PDP_addr>[,<
cid>]
Thenetwork hasforced acontextdeactivation.The <cid>thatwas
usedtoactivate the contextisprovidedifknown to theMT.Theformat
of theparameters <PDP_type>,<PDP_addr>and<cid> arefound in
command +CGDCONT.
+CGEV:MEDEACT
<PDP_type>,<PDP_addr>[,<
cid>]
Themobile termination has forced acontext deactivation.The<cid>
thatwas used toactivate thecontext is provided ifknown tothe MT.
Theformat ofthe parameters <PDP_type>,<PDP_addr>and <cid>
are foundin command+CGDCONT.
+CGEV:NWPDNDEACT
<cid>[,<WLAN_Offload>]
Thenetworkhasdeactivatedacontext.ThecontextrepresentsaPDN
connection inLTEoraPrimary PDPcontextin GSM/UMTS.The
associated <cid>forthis context isprovided totheTE.Theformatof
the parameter<cid> isfoundin command+CGDCONT.Theformat of
the parameter<WLAN_Offload>isdefined above.
Occurrence ofthisevent replaces usage ofthe event+CGEV:NWDEACT
<PDP_type>,<PDP_addr>[,<cid>].
+CGEV:MEPDNDEACT
<cid>
Themobile termination has deactivated acontext.Thecontext
represents aPDNconnection inLTEoraPrimary PDPcontextin
GSM/UMTS.The<cid>forthiscontextis provided totheTE.The
format ofthe parameter<cid>is found incommand +CGDCONT.
Occurrence ofthisevent replaces usage ofthe event+CGEV:MEDEACT
<PDP_type>,<PDP_addr>[,<cid>].
+CGEV:NWDEACT
<p_cid>,<cid>,<event_type>
[,<WLAN_Offload>]
Thenetwork hasdeactivated acontext.The<cid> forthis contextis
provided to theTEinaddition tothe associatedprimary<p_cid>.The
format ofthe parameters <p_cid>and <cid>are found incommand
+CGDSCONT.The formatofthe parameters <event_type>and
<WLAN_Offload>are definedabove.
NOTE
NOTE
NOTE


## Página 146

A76XX Series_AT Command Manual_V1.09
www.simcom.com 145 /652
Occurrence ofthisevent replaces usage ofthe event+CGEV:NWDEACT
<PDP_type>,<PDP_addr>[,<cid>].
+CGEV:MEDEACT
<p_cid>,<cid>,<event_type>
Thenetwork hasresponded to anME initiated contextdeactivation
request.Theassociated <cid>is provided totheTEin addition tothe
associated primary<p_cid>.The formatofthe parameters <p_cid>
and<cid> are found incommand +CGDSCONT.Theformatof the
parameter<event_type> isdefined above.
Occurrence ofthisevent replaces usage ofthe event+CGEV:MEDEACT
<PDP_type>,<PDP_addr>[,<cid>].
ForPDPcontext modification,the following unsolicited resultcodes andthe corresponding events are
defined:
+CGEV:NWMODIFY
<cid>,<change_reason>,<ev
ent_type>[,<WLAN_Offload
>]
Thenetwork hasmodifieda context.The associated<cid> isprovided
to theTEinaddition tothe <change_reason>and <event_type>.The
format ofthe parameter<cid>is found incommand +CGDCONTor
+CGDSCONT.The formatofthe parameters
<change_reason>,<event_type>,and<WLAN_Offload>aredefined
above.
<change_reason>:integertype;abitmapthatindicates whatkind of
change occurred.The<change_reason>value isdeterminedby
summing allthe applicable bits.ForExamples ifboththe valuesof
QoS changed (Bit2)and<WLAN_Offload>changed (Bit 3)have
changed, thenthe <change_reason>valueis 6.
TheWLANoffload valuewillchangewhen bit1orbit2or bothofthe indicators inthe WLANoffload
acceptability IEchange, seethe parameter<WLAN_Offload>defined above.
Bit 1 TFTchanged
Bit 2 Qoschanged
Bit 3 WLANOffloadchanged
NOTE
NOTE


## Página 147

A76XX Series_AT Command Manual_V1.09
www.simcom.com 146 /652
+CGEV:MEMODIFY
<cid>,<change_reason>,<ev
ent_type>[,<WLAN_Offload
>]
Themobile termination has modifiedacontext.Theassociated <cid>
is provided totheTEin addition tothe <change_reason>and
<event_type>.Theformatofthe parameter<cid> isfoundin
command +CGDCONTor+CGDSCONT.Theformatof the
parameters <change_reason>,<event_type>and <WLAN_Offload>
are defined above.
ForotherPDPcontext handling,the following unsolicited resultcodesand the correspondingevents are
defined:
+CGEV:REJECT
<PDP_type>,<PDP_addr>
Anetwork requestforcontext activation occurredwhen the MTwas
unabletoreportittotheTEwitha+CRINGunsolicitedresultcodeand
wasautomaticallyrejected.Theformatoftheparameters<PDP_type>
and<PDP_addr> are foundincommand +CGDCONT.
This eventis notapplicable forEPS.
+CGEV:NWREACT
<PDP_type>,<PDP_addr>[,<
cid>]
Thenetwork hasrequested acontextreactivation.The<cid>that was
usedto reactivatethe context isprovided ifknown to the MT.The
format ofthe parameters <PDP_type>,<PDP_addr>and <cid>are
foundin command +CGDCONT.
This eventis notapplicable forEPS.
Examples
AT+CGEREP=?
+CGEREP:(0-2),(0-1)
OK
AT+CGEREP?
NOTE
NOTE


## Página 148

A76XX Series_AT Command Manual_V1.09
www.simcom.com 147 /652
+CGEREP:2,0
OK
AT+CGEREP=2,0
OK
AT+CGEREP
OK
5.2.16 AT+CGAUTH Settype ofauthenticationforPDP-IPconnectionsofGPRS
This commandis used tosettype ofauthenticationforPDP-IPconnections ofGPRS.
AT+CGAUTH Settype ofauthenticationforPDP-IPconnectionsofGPRS
TestCommand
AT+CGAUTH=?
Response
1)
+CGAUTH:(range ofsupported<cid>s),(list ofsupported
<auth_type>s),50,50
OK
2)
ERROR
3)
+CMEERROR: <err>
Read Command
AT+CGAUTH?
Response
1)
+CGAUTH:[<cid>,<auth_type>[,<user>,<passwd>]]
…
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CGAUTH=<cid>[,<auth_t
ype>[,<passwd>[,<user>]]]
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+CGAUTH
Response
1)


## Página 149

A76XX Series_AT Command Manual_V1.09
www.simcom.com 148 /652
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<cid> Parameterspecifies aparticularPDPcontextdefinition.This isalso
usedin otherPDPcontext-relatedcommands.
1…15
<auth_type> Indicatethe type ofauthenticationtobeused forthe specifiedcontext.
IfCHAPis selected anotherparameter<passwd>needs tobe
specified.IfPAPis selectedtwo additionalparameters <passwd>and
<user>need tospecified.
0 none
1 PAP
2 CHAP
3 PAPorCHAP //1803Splatform
<passwd> Parameterspecifies the password used forauthentication.
<user> Parameterspecifies the usernameusedforauthentication.
Examples
AT+CGAUTH=?
+CGAUTH:(1-15),(0-2),50,50
OK
AT+CGAUTH=?
+CGAUTH:(1-15),(0-3),50,50
OK
//1803Splatform
AT+CGAUTH?
+CGAUTH:1,0
OK
AT+CGAUTH=1,0
OK
AT+CGAUTH
OK


## Página 150

A76XX Series_AT Command Manual_V1.09
www.simcom.com 149 /652
5.2.17 AT+CPING Pingdestinationaddress
This commandis used toping destinationaddress.
AT+CPING Pingdestinationaddress
TestCommand
AT+CPING=?
Response
1)
+CPING:IPaddress,(listof supported
<dest_addr_type>s),(1-5),(4-188),(1000-10000),(10000-100000),(1
6-255)
OK
2)
ERROR
WriteCommand
AT+CPING=<dest_addr>,<de
st_addr_type>[,<num_pings
>[,<data_packet_size>[,<inte
rval_time>[,<wait_time>[,<T
TL>]]]]]
Response
1)
OK
Ifping’s result_type=1
+CPING:
<result_type>,<resolved_ip_addr>,<data_packet_size>,<rtt>,<TT
L>
Ifping’s result_type=2
+CPING:<result_type>
Ifping’s result_type=3>
+CPING:
<result_type>,<num_pkts_sent>,<num_pkts_recvd>,<num_pkts
_lost>,<min_rtt>,<max_rtt>,<avg_rtt>
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<dest_addr> Thedestination isto bepinged; itcanbe an IPaddressoradomain
name.


## Página 151

A76XX Series_AT Command Manual_V1.09
www.simcom.com 150 /652
<dest_addr_type> Integertype.Addressfamily type ofthe destination address
1 IPv4.
2 IPv6(reserved)
<num_pings> Integertype.The num_pings specifies the numberof timesthe ping
request(1-5)is tobe sent.Thedefaultvalueis 4.
<data_packet_size> Integertype.Databyte size ofthe ping packet(4-188).Thedefault
value is64 bytes.
<interval_time> Integertype.Intervalbetween eachping.Value is specifiedin
milliseconds (1000ms-10000ms).Thedefaultvalue is2000ms.
<wait_time> Integertype.Waittimeforping response.Anping responsereceived
after thetimeout shallnotbe processed.Value specifiedin
milliseconds (10000ms-100000ms).Thedefaultvalue is10000ms.
<TTL> Integertype.TTL(Time-To-Live)value forthe IPpacketoverwhich the
ping(ICMPECHORequest message)issent(16-255),the default
value is255.
<result_type> 1 Ping success
2 Ping timeout
3 Ping result
<num_pkts_sent> Indicates the numberofping requests thatwere sentout.
<num_pkts_recvd> Indicates the numberofping responses thatwere received.
<num_pkts_lost> Indicates the numberofping requests forwhichno responsewas
received.
<min_rtt> Indicates the minimumRoundTripTime(RTT).
<max_rtt> Indicates the maximumRTT.
<avg_rtt> Indicates the averageRTT.
<resolved_ip_addr> Indicates the resolved ipaddress.
<rtt> RoundTripTime.
Examples
AT+CPING=?
+CPING:IP
address,(1,2),(1-5),(4-188),(1000-10000),(10000-100000),(16-255)
OK
AT+CPING="www.baidu.com",1,4,64,1000,10000,255
OK
+CPING:2
+CPING:2
+CPING:2


## Página 152

A76XX Series_AT Command Manual_V1.09
www.simcom.com 151 /652
+CPING:2
+CPING:3,4,0,4,0,0,0


## Página 153

A76XX Series_AT Command Manual_V1.09
www.simcom.com 152 /652
ATCommandsforSIMCard
6.1 OverviewofATCommandsforSIMCard
Command Description
AT+CICCID Read ICCIDfromSIM card
AT+CPIN Enter PIN
AT+CLCK Facility lock
AT+CPWD Change password
AT+CIMI Requestinternationalmobilesubscriberidentity
AT+CSIM Generic SIM access
AT+CRSM RestrictedSIM access
AT+SPIC Times remaintoinput SIM PIN/PUK
AT+CSPN Getservice provider namefromSIM
AT+UIMHOTSWAPON SetUIMhotswap function on
AT+UIMHOTSWAPLEVEL SetUIMcard detection level
AT+SWITCHSIM SwitchmasterSIM
AT+DUALSIM Setdual-sim mode
AT+BINDSIM BindATPtoSIM1 orSIM2
AT+DUALSIMURC Dual cardreporting control
6.2 DetailedDescriptionofATCommandsforSIMCard
6.2.1 AT+CICCID ReadICCIDfromSIMcard
This commandis used toRead the ICCIDfromSIM card.
AT+CICCID ReadICCIDfromSIMcard
TestCommand Response


## Página 154

A76XX Series_AT Command Manual_V1.09
www.simcom.com 153 /652
AT+CICCID=? OK
Execution Command
AT+CICCID
Response
1)
+ICCID:<ICCID>
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference Vendor
DefinedValues
<ICCID> Integrate circuitcardidentity,astandard ICCIDis a20-digitserial
numberofthe SIMcard,it presents the publishstate, networkcode,
publish area,publish date,publish manufactureand pressserial
numberofthe SIMcard.
Examples
AT+CICCID
+ICCID:89860318760238610932
OK
AT+CICCID=?
OK
6.2.2 AT+CPIN EnterPIN
This commandis used tosendthe MEa passwordwhichis necessary beforeit canbeoperated (SIM PIN,
SIM PUK, PH-SIMPIN, etc.).Ifthe PIN istobe entered twice,theTAshallautomatically repeat the PIN.If
no PINrequestis pending,noaction istaken towards MTandan errormessage,+CME ERROR, is
returnedtoTE.
IfthePINrequiredisSIMPUKorSIMPUK2,thesecondpinisrequired.Thissecondpin,<newpin>,isused
toreplace theold pin inthe SIM.


## Página 155

A76XX Series_AT Command Manual_V1.09
www.simcom.com 154 /652
AT+CPIN EnterPIN
TestCommand
AT+CPIN=?
Response
OK
Read Command
AT+CPIN?
Response
1)
+CPIN:<code>
OK
2)
ERROR
3)
+CMEERROR:<err>
WriteCommand
AT+CPIN=<pin>[,<newpin>]
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE_REBOOT
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<pin> String type values.
<newpin> String type values.
<code> Valuesreserved by the presentdocument:
READY MEis notpending foranypassword
SIM PIN ME is waitingSIM PINtobe given
SIM PUK MEiswaiting SIM PUKto begiven
PH-SIM PIN ME iswaiting phone-to-SIMcard password tobegiven
SIM PIN2 ME iswaiting SIM PIN2to be given
SIM PUK2 ME iswaiting SIM PUK2 tobe given
PH-NETPIN ME iswaiting network personalizationpassword tobe
given
Examples
AT+CPIN=?
OK
AT+CPIN?
+CPIN:READY


## Página 156

A76XX Series_AT Command Manual_V1.09
www.simcom.com 155 /652
OK
AT+CPIN=1234
OK
6.2.3 AT+CLCK Facilitylock
This commandis used tolock,unlock orinterrogateaME oranetworkfacility <fac>.Password is normally
neededtodosuchactions.Whenqueryingthestatusofanetworkservice(<mode>=2)theresponselinefor
'notactive'case (<status>=0)shouldbe returnedonly ifservice isnotactive forany <class>.
AT+CLCK Facilitylock
TestCommand
AT+CLCK=?
Response
+CLCK:(list ofsupported<fac>s)
OK
WriteCommand
AT+CLCK=<fac>,<mode>
[,<passwd>[,<class>]]
Response
1)
OK
2)
When<mode>=2 and commandsuccessful:
+CLCK:<status>[,<class1>[
+CLCK:<status>,<class2>
[..]]
OK
3)
ERROR
4)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE_REBOOT
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<fac> "SC" lockSIM card orUSIM card
"AO" BarrAll OutgoingCalls
"OI" BarrOutgoingInternationalCalls
"OX" BarrOutgoingInternationalCalls exceptto HomeCountry
"AI" BarrAll IncomingCalls


## Página 157

A76XX Series_AT Command Manual_V1.09
www.simcom.com 156 /652
"IR" BarrIncomingCalls when roamingoutsidethe homecountry
"AB" AllBarring services (only for<mode>=0)
"AG" All outGoing barringservices(only for<mode>=0)
"AC" AllinComing barring services (only for<mode>=0)
"FD" SIMfixeddialing memoryfeature
"PN" Network Personalization
"PU" network subsetPersonalization
"PP" serviceProviderPersonalization
"PC" Corporate Personalization
<mode> 0 unlock
1 lock
2 query status
<status> 0 not active
1 active
<passwd> Password.
stringtype; shallbethe sameaspassword specified forthe facility
from theME userinterfaceor with commandChange Password
+CPWD
<class> Itisasumofintegerseachrepresentingaclassofinformation(default
7):
1 voice (telephony)
2 data (refers toallbearer services)
4 fax (facsimileservices)
8 shortmessage service
16 data circuit sync
32 data circuit sync
64 dedicated packetaccess
128 dedicated PADaccess
255 Thevalue 255covers allclasses
Examples
AT+CLCK="SC",2
+CLCK:0
OK
AT+CLCK=?
+CLCK:
("SC","AO","OI","OX","AI","IR","AB","AG","AC","FD","PN","PU","PP","PC")
OK


## Página 158

A76XX Series_AT Command Manual_V1.09
www.simcom.com 157 /652
6.2.4 AT+CPWD Changepassword
Writecommandsets anewpassword forthe facility lock functiondefined bycommand FacilityLock
AT+CLCK.
Testcommandreturns alistof pairswhich presentthe availablefacilities andthe maximum lengthof their
password.
AT+CPWD Changepassword
TestCommand
AT+CPWD=?
Response
1)
+CPWD:(listofsupported (<fac>,<pwdlength>)s)
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CPWD=<fac>,<oldpwd>,
<newpwd>
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode AUTO_SAVE_REBOOT
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<fac> ReferFacility Lock+CLCKforothervalues:
"SC" SIM orUSIM PIN1
"P2" SIM orUSIM PIN2
"AB" AllBarring services
"AC" AllinComing barring services (only for<mode>=0)
"AG" All outGoing barringservices(only for<mode>=0)
"AI" BarrAll IncomingCalls
"AO" BarrAll OutgoingCalls
"IR" BarrIncomingCalls when roamingoutsidethe homecountry
"OI" BarrOutgoingInternationalCalls
"OX" BarrOutgoingInternationalCalls exceptto HomeCountry
<oldpwd> String type,itshall bethe sameas password specifiedforthe facility


## Página 159

A76XX Series_AT Command Manual_V1.09
www.simcom.com 158 /652
from theME userinterfaceor with commandChange Password
AT+CPWD.
<newpwd> String type,itis the newpassword; maximumlength ofpassword can
be determined with <pwdlength>.
<pwdlength> Integertype,maxlength ofpassword.
Examples
AT+CPWD=?
+CPWD:
("AB",4),("AC",4),("AG",4),("AI",4),("AO",4),("IR",4),("OI",4),("OX",4),(
"SC",8),("P2",8)
OK
AT+CPWD="SC","1234","4321"
OK
6.2.5 AT+CIMI Requestinternationalmobilesubscriberidentity
Execution commandcauses theTAtoreturn <IMSI>,which isintendedto permittheTEto identifythe
individualSIM card which isattachedto MT.
AT+CIMI Requestinternationalmobilesubscriberidentity
TestCommand
AT+CIMI=?
Response
1)
OK
2)
ERROR
Execution Command
AT+CIMI
Response
1)
<IMSI>
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues


## Página 160

A76XX Series_AT Command Manual_V1.09
www.simcom.com 159 /652
<IMSI> InternationalMobileSubscriberIdentity(string,withoutdouble
quotes).
Examples
AT+CIMI=?
OK
AT+CIMI
460010222028133
OK
IfUSIM card contains two apps,like ChinaTelecom 4Gcard,one RUIM/CSIM app,and anotherUSIM
app; sothereare two IMSIinit;AT+CIMIwillreturn the RUIM/CSIM IMSI.
6.2.6 AT+CSIM GenericSIMaccess
This commandis used tocontrolthe SIM carddirectly.
Compared torestrictedSIM access commandAT+CRSM,AT+CSIM allows the ME totake more control
overthe SIMinterface.
ForSIM–MEinterfacepleaserefer 3GPPTS11.11.
AT+CSIM GenericSIMaccess
TestCommand
AT+CSIM=?
Response
OK
WriteCommand
AT+CSIM=<length>,<comma
nd>
Response
1)
+CSIM:<length>,<response>
OK
2)
ERROR
3)
+CMEERROR:<err>
NOTE


## Página 161

A76XX Series_AT Command Manual_V1.09
www.simcom.com 160 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<length> Intergertype;length ofcharacters thatare sent toTEin <command>
or <response>
<command> Command passed from MTtoSIM card.
<response> Responseto the commandpassed fromSIM cardto MT.
Examples
AT+CSIM=?
OK
AT+CSIM=10,"A0F2000016"
+CSIM:4,"6E00"
OK
The SIM Application Toolkit functionality is not supported by AT+CSIM. Therefore the following SIM
commandscan notbeused:TERMINALPROFILE,ENVELOPE,FETCHandTEMINALRESPONSE.
6.2.7 AT+CRSM RestrictedSIMaccess
By usingAT+CRSM insteadofGeneric SIMAccessAT+CSIM,TEapplication haseasier butmore limited
access tothe SIM database.
Writecommandtransmitsto the MTthe SIM <command>and its requiredparameters.MThandles
internallyall SIM-MTinterfacelocking andfileselection routines.Asresponse tothe command,MTsends
the actualSIM information parameters andresponsedata.MTerrorresult code +CME ERRORmay be
returnedwhen the commandcannotbe passedto the SIM, butfailurein the execution ofthe commandin
the SIM isreported in<sw1>and <sw2>parameters.
AT+CRSM RestrictedSIMaccess
NOTE


## Página 162

A76XX Series_AT Command Manual_V1.09
www.simcom.com 161 /652
TestCommand
AT+CRSM=?
Response
OK
WriteCommand
AT+CRSM=<command>[,<fil
eID>[,<p1>,<p2>,<p3>[,<data
>]]]
Response
1)
+CRSM:<sw1>,<sw2>[,<response>]
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime -
Reference 3GPPTS27.007
DefinedValues
<command> Command passed onby the MTtothe SIM:
176 READBINARY
178 READRECORD
192 GETRESPONSE
214 UPDATEBINARY
220 UPDATERECORD
242 STATUS
203 RETRIEVEDATA
219 SETDATA
<fileID> Identifierforanelementarydata fileon SIM, ifusedby <command>.
Thefollowing listthe fileIDhexvalue,userneedsto convetthem to
decimal.
EFs underMF
0x2FE2 ICCID
0x2F05 ExtendedLanguage Preferences
0x2F00 EFDIR
0x2F06 Access RuleReferenceEFs underUSIMADF
0x6F05 Language Indication
0x6F07 IMSI
0x6F08 Ciphering andIntegrity keys
0x6F09 Cand Ikeys forpktswitched domain
0x6F60 Usercontrolled PLMNselectorw/AccTech
0x6F30 Usercontrolled PLMNselector
0x6F31 HPLMN search period
0x6F37 ACM maximum value
0x6F38 USIM Servicetable
0x6F39 Accumulated Call meter


## Página 163

A76XX Series_AT Command Manual_V1.09
www.simcom.com 162 /652
0x6F3E GroupIdentifierLevel
0x6F3F GroupIdentifierLevel 2
0x6F46 ServiceProviderName
0x6F41 Price PerUnitandCurrency table
0x6F45 CellBcastMsg identifierselection
0x6F78 Access controlclass
0x6F7B ForbiddenPLMNs
0x6F7E Location information
0x6FAD Administrativedata
0x6F48 CellBcastmsgidfordatadownload
0x6FB7 Emergency callcodes
0x6F50 Cellbcastmsgid range selection
0x6F73 Packetswitched location information
0x6F3B Fixeddialling numbers
0x6F3C Shortmessages
0x6F40 MSISDN
0x6F42 SMS parameters
0x6F43 SMS Status
0x6F49 Servicedialling numbers
0x6F4B Extension2
0x6F4C Extension3
0x6F47 SMS reports
0x6F80 Incomingcall information
0x6F81 Outgoing callinformation
0x6F82 Incomingcall timer
0x6F83 Outgoing calltimer
0x6F4E Extension5
0x6F4F CapabilityConfig Parameters2
0x6FB5 Enh MultiLevelPrecedenceand Pri
0x6FB6 Automaticanswer foreMLPPservice
0x6FC2 Groupidentity
0x6FC3 Key forhiddenphonebook entries
0x6F4D Barreddialling numbers
0x6F55 Extension 4
0x6F58 ComparisonMethodinformation
0x6F56 Enabled services table
0x6F57 Access Point NameControlList
0x6F2C De-personalizationControl Keys
0x6F32 Co-operative networklist
0x6F5B Hyperframenumber
0x6F5C Maximumvalue ofHyperframenumber
0x6F61 OPLMN selectorwith access tech
0x6F5D OPLMN selector
0x6F62 HPLMN selectorwith access technology
0x6F06 Access Rulereference
0x6F65 RPLMN lastusedaccess tech


## Página 164

A76XX Series_AT Command Manual_V1.09
www.simcom.com 163 /652
0x6FC4 Network Parameters
0x6F11 CPHS:VoiceMailWaiting Indicator
0x6F12 CPHS:ServiceStringTable
0x6F13 CPHS:CallForwarding Flag
0x6F14 CPHS:OperatorNameString
0x6F15 CPHS:CustomerServiceProfile
0x6F16 CPHS:CPHSInformation
0x6F17 CPHS:Mailbox Number
0x6FC5 PLMN NetworkName
0x6FC6 OperatorPLMN List
0x6F9F Dynamic Flags Status
0x6F92 Dynamic2Flag Setting
0x6F98 CustomerServiceProfileLine2
0x6F9B EFPARAMS-WelcomeMessage
0x4F30 Phone bookreference file
0x4F22 Phone booksynchronization center
0x4F23 Change counter
0x4F24 Previous UniqueIdentifier
0x4F20 GSM cipheringkeyKc
0x4F52 GPRSciphering key
0x4F63 CPBCCHinformation
0x4F64 Investigation scan
0x4F40 MExE Servicetable
0x4F41 OperatorRootPublicKey
0x4F42 AdministratorRootPublic Key
0x4F43 Third party Rootpublic key
0x6FC7 MailBoxDialing Number
0x6FC8 Extension6
0x6FC9 Mailbox Identifier
0x6FCA Message Waiting Indication Status
0x6FCD ServiceProvider DisplayInformation
0x6FD2 UIM_USIM_SPT_TABLE
0x6FD9 EquivalentHPLMN
0x6FCB CallForwardingIndicatorStatus
0x6FD6 GBABootstrappingparameters
0x6FDA GBANAFList
0x6FD7 MBMS Service Key
0x6FD8 MBMS User Key
0x6FCE MMS Notification
0x6FD0 MMS Issuerconnectivity parameters
0x6FD1 MMS UserPreferences
0x6FD2 MMS Userconnectivityparameters
0x6FCF Extension 8
0x5031 Object Directory File
0x5032 TokenInformationFile
0x5033 Unused spaceInformation FileEFs underTelecom DF


## Página 165

A76XX Series_AT Command Manual_V1.09
www.simcom.com 164 /652
0x6F3A AbbreviatedDialing Numbers
0x6F3B Fixeddialling numbers
0x6F3C Shortmessages
0x6F3D CapabilityConfiguration Parameters
0x6F4F Extended CCP
0x6F40 MSISDN
0x6F42 SMS parameters
0x6F43 SMS Status
0x6F44 Lastnumberdialled
0x6F49 ServiceDialling numbers
0x6F4A Extension1
0x6F4B Extension2
0x6F4C Extension3
0x6F4D BarredDialing Numbers
0x6F4E Extension4
0x6F47 SMS reports
0x6F58 ComparisonMethodInformation
0x6F54 Setup Menuelements
0x6F06 Access Rulereference
0x4F20 Image
0x4F30 Phone bookreference file
0x4F22 Phone booksynchronization center
0x4F23 Change counter
0x4F24 Previous UniqueIdentifier
<p1><p2><p3> Integertype;parameters tobe passedon by the Module tothe SIM.
<data> Informationwhichshall bewrittentothe SIM (hexadecimalcharacter
format, referAT+CSCS).
<sw1><sw2> Status information from the SIM aboutthe execution ofthe actual
command.Itisreturned inboth cases,on successfulorfailed
execution ofthe command.
<response> Responsedata incase ofasuccessfulcompletionof thepreviously
issued command.
"STATUS"and"GETRESPONSE"commands return data,which
gives informationaboutthe currently selectedelementary datafield.
This informationincludes thetype offileand itssize.
After"READ BINARY"or"READ RECORD"commandsthe requested
data willbereturned.
<response>isempty after"UPDATEBINARY" or"UPDATE
RECORD"commands.
Examples
AT+CRSM=?
OK


## Página 166

A76XX Series_AT Command Manual_V1.09
www.simcom.com 165 /652
AT+CRSM=242
+CRSM:
144,0,"000000003F00040000FFBB01020000"
OK
6.2.8 AT+SPIC TimesremaintoinputSIMPIN/PUK
This commandis used toinquiretimesremain to inputSIM PIN/PUK.
AT+SPIC TimesremaintoinputSIMPIN/PUK
TestCommand
AT+SPIC=?
Response
OK
Execution Command
AT+SPIC
Response
1)
+SPIC:<pin1>,<puk1>,<pin2>,<puk2>
OK
2)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime -
Reference Vendor
DefinedValues
<pin1> Times remaintoinput PIN1code.
<puk1> Times remaintoinput PUK1code.
<pin2> Times remaintoinput PIN2code.
<puk2> Times remaintoinput PUK2code.
Examples
AT+SPIC=?
OK
AT+SPIC
+SPIC:3,10,0,10
OK


## Página 167

A76XX Series_AT Command Manual_V1.09
www.simcom.com 166 /652
6.2.9 AT+CSPN GetserviceprovidernamefromSIM
This commandis used togetserviceprovider namefromSIM card.
AT+CSPN GetserviceprovidernamefromSIM
TestCommand
AT+CSPN=?
Response
1)
OK
2)
ERROR
Read Command
AT+CSPN?
Response
1)
+CSPN:<spn>,<displaymode>
OK
2)
OK
3)
ERROR
4)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime -
Reference Vendor
DefinedValues
<spn> String type;service providernameon SIM
<display mode> 0 doesn’tdisplay PLMN.Already registeredon PLMN.
1 display PLMN
Examples
AT+CSPN=?
OK
AT+CSPN?
+CSPN:"China Telecom",1
OK


## Página 168

A76XX Series_AT Command Manual_V1.09
www.simcom.com 167 /652
6.2.10 AT+UIMHOTSWAPON SetUIMHotswapFunctionOn
AT+UIMHOTSWAPON SetUIMhotswapfunctionon
TestCommand
AT+UIMHOTSWAPON=?
Response
1)
+UIMHOTSWAPON:(0-2)
OK
2)
ERROR
Read Command
AT+UIMHOTSWAPON?
Response
1)
+UIMHOTSWAPON:<onoff>
OK
2)
ERROR
WriteCommand
AT+UIMHOTSWAPON=<onof
f>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference Vendor
DefinedValues
<onoff> 0 TheUIM hotswap functionis disabled
1 TheUIM hotswap function isenabled(Inserting the SIM cardwill
be recognized immediately,removing itwillnot)
2 TheUIM hotswap function isenabled(Inserting andpulling outthe
SIM card willbe recognized immediately)
Examples
AT+UIMHOTSWAPON=?
+UIMHOTSWAPON:(0-2)
OK


## Página 169

A76XX Series_AT Command Manual_V1.09
www.simcom.com 168 /652
AT+UIMHOTSWAPON?
+UIMHOTSWAPON:0
OK
AT+UIMHOTSWAPON=1
OK
6.2.11 AT+UIMHOTSWAPLEVEL SetUIMCardDetectionLevel
AT+UIMHOTSWAPLEVEL SetUIMCardDetectionLevel
TestCommand
AT+UIMHOTSWAPLEVEL=?
Response
1)
+UIMHOTSWAPLEVEL:(0-1)
OK
2)
ERROR
Read Command
AT+UIMHOTSWAPLEVEL?
Response
1)
+UIMHOTSWAPLEVEL:<level>
OK
2)
ERROR
WriteCommand
AT+UIMHOTSWAPLEVEL=<l
evel>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference Vendor
DefinedValues
<level> 0 ACTIVELOW
1 ACTIVEHIGH


## Página 170

A76XX Series_AT Command Manual_V1.09
www.simcom.com 169 /652
Examples
AT+UIMHOTSWAPLEVEL=?
+UIMHOTSWAPLEVEL:(0-1)
OK
AT+UIMHOTSWAPLEVEL?
+UIMHOTSWAPLEVEL:0
OK
AT+UIMHOTSWAPLEVEL=1
OK
6.2.12 AT+SWITCHSIM SwitchmasterSIM
This commandtoset materSIM.Only appliedfordual-sim project.
AT+SWITCHSIM SwitchmasterSIM
TestCommand
AT+SWITCHSIM=?
Response
+SWITCHSIM:(0:SIM1,1:SIM2)
OK
Read Command
AT+SWITCHSIM?
Response
+SWITCHSIM:<simID>
OK
WriteCommand
AT+SWITCHSIM=<simID>
Response
1)Ifthe parameteris correct,and the corresponding cardhas been
inserted,response:
OK
2)Others:
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<simID> Integertype
0 SIM1
1 SIM2


## Página 171

A76XX Series_AT Command Manual_V1.09
www.simcom.com 170 /652
Examples
AT+SWITCHSIM=?
+SWITCHSIM:(0: SIM1,1: SIM2)
OK
AT+SWITCHSIM?
+SWITCHSIM:0
OK
AT+SWITCHSIM=1
OK
This command is only supported by specific hardware PN which support DS, for detailed information
pleasecontactwith SIMComFAE
6.2.13 AT+DUALSIM Setdual-simmode
This commandtoset thedual-sim mode isdual standby orsingle standby.TheSIM2 willregister IMSwhen
<dsmode>=3,ifitsupport.
AT+DUALSIM Setdual-simmode
TestCommand
AT+DUALSIM=?
Response
+DUALSIM:(0:DUALSIMDUALSTANDBY,1:DUALSIM
SINGLESTANDBY,3:DUALSIM DUALSTANDBYFP)
OK
Read Command
AT+DUALSIM?
Response
+DUALSIM:<dsmode>
OK
WriteCommand
AT+DUALSIM=<dsmode>
Response
1)Ifthe parameteris correct,response:
OK
2)Others:
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
NOTE


## Página 172

A76XX Series_AT Command Manual_V1.09
www.simcom.com 171 /652
DefinedValues
<dsmode> Integertype
0 Dual SIMdual standby
1 Dual SIMsingle standby
3 Dual SIMdual standby with extras
Examples
AT+DUALSIM=?
+DUALSIM:(0:DUALSIM DUALSTANDBY,1:
DUALSIM SINGLESTANDBY,3:DUALSIM
DUALSTANDBYFP)
OK
AT+DUALSIM?
+DUALSIM:0
OK
AT+DUALSIM=0
OK
This command is only supported by specific hardware PN which support DS, for detailed information
pleasecontactwith SIMComFAE
6.2.14 AT+BINDSIM BindATPtoSIM1orSIM2
This setcommandto bindATchanneltoSIM card 1orSIM card 2.Only forATinterface.
AT+BINDSIM BindATPtoSIM1orSIM2
TestCommand
AT+BINDSIM=?
Response
+BINDSIM:(0:SIM1,1:SIM2)
OK
Read Command
AT+BINDSIM?
Response
+BINDSIM:<simID>
NOTE


## Página 173

A76XX Series_AT Command Manual_V1.09
www.simcom.com 172 /652
OK
WriteCommand
AT+BINDSIM=<simID>
Response
1)Ifthe parameteris correct,response:
OK
2)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<simID> Integertype
0 SIM1
1 SIM2
Examples
AT+BINDSIM=?
+BINDSIM:(0:SIM1,1:SIM2)
OK
AT+BINDSIM?
+BINDSIM:0
OK
AT+BINDSIM=0
OK
AT+CIMI
460003064859756
OK
AT+BINDSIM=1
OK
AT+CIMI
460012360528428
OK
This command is only supported by specific hardware PN which support DS, for detailed information
NOTE


## Página 174

A76XX Series_AT Command Manual_V1.09
www.simcom.com 173 /652
pleasecontactwith SIMComFAE
6.2.15 AT+DUALSIMURCDualcardreportingcontrol
This setcommandto choosewhichSIM cardto report,ifenable the SIM2 URC,all URCstrings aresuffixed
with "DS".
AT+DUALSIMURC Dualcardreportingcontrol
TestCommand
AT+DUALSIMURC=?
Response
+DUALSIMURC:(0:SIM1,1:SIM2, 2:SIM1&SIM2)
OK
Read Command
AT+DUALSIMURC?
Response
+DUALSIMURC:<opt>
OK
WriteCommand
AT+DUALSIMURC=<opt>
Response
1)Ifthe parameteris correct,response:
OK
2)Others:
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<opt> Integertype
0 SIM1
1 SIM2
2 SIM1 &SIM2
Examples
AT+DUALSIMURC=?
+DUALSIMURC:(0:SIM1,1:SIM2,2:SIM1&SIM2)
OK
AT+DUALSIMURC?
+DUALSIMURC:0
OK


## Página 175

A76XX Series_AT Command Manual_V1.09
www.simcom.com 174 /652
AT+DUALSIMURC=1
OK
This command is only supported by specific hardware PN which support DS, for detailed information
pleasecontactwith SIMComFAE
NOTE


## Página 176

A76XX Series_AT Command Manual_V1.09
www.simcom.com 175 /652
ATCommandsforCallControl
7.1 OverviewofATCommandsforCallControl
Command Description
AT+CVHU Voicehang upcontrol
AT+CHUP Hang upcall
AT+CBST Selectbearerservicetype
AT+CRLP Radio link protocol
AT+CRC Cellularresult codes
AT+CLCC Listcurrent calls
AT+CEER Extendederrorreport
AT+CCWA Callwaiting
AT+CCFC Callforwarding numberand conditions
AT+CLIP Calling line identificationpresentation
AT+CLIR Calling line identificationrestriction
AT+COLP Connected lineidentification presentation
AT+VTS DTMF and tone generation
AT+VTD Toneduration
AT+CSTA Selecttype ofaddress
AT+CMOD Callmode
AT+VMUTE Speakermute contro
AT+CMUT Microphone mutecontrol
AT+CSDVC Switchvoice channel device
AT+CMICGAIN Adjustmicgain
AT+COUTGAIN Adjustout gain
7.2 DetailedDescriptionofATCommandsforCallControl


## Página 177

A76XX Series_AT Command Manual_V1.09
www.simcom.com 176 /652
7.2.1 AT+CVHU Voicehangupcontrol
Writecommandselects whetherATHor"dropDTR"shallcause avoiceconnectionto be disconnected or
not.By voiceconnection isalsomeant alternating mode calls that arecurrently in voicemode.
AT+CVHU Voicehangupcontrol
TestCommand
AT+CVHU=?
Response
+CVHU:(rangeofsupported <mode>s)
OK
Read Command
AT+CVHU?
Response
+CVHU:<mode>
OK
WriteCommand
AT+CVHU=<mode>
Response
1)
OK
2)
ERROR
Execution Command
AT+CVHU
Setdefaultvalue
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 0 "DropDTR"ignored butOKresponsegiven.ATHdisconnects.
1 "DropDTR"andATHignoredbutOKresponsegiven.
Examples
AT+CVHU=?
+CVHU:(0-1)
OK
AT+CVHU?
+CVHU:1
OK


## Página 178

A76XX Series_AT Command Manual_V1.09
www.simcom.com 177 /652
AT+CVHU=0
OK
AT+CVHU
OK
7.2.2 AT+CHUP Hangupcall
This commandis used tocancelvoice calls.Ifthere is nocall,itwilldo nothingbutOKresponseisgiven.
AfterrunningAT+CHUP,multiple "VOICE CALLEND:"may be reported whichrelieson howmany calls
existbefore calling thiscommand.
AT+CHUP Hangupcall
TestCommand
AT+CHUP=?
Response
OK
Execution Command
AT+CHUP
Response
1)
OK
VOICECALL:END:<time>
2)NoCall
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<time> Voicecallconnection time.
Format HHMMSS(HH:hour,MM: minute, SS:second)
Examples
AT+CHUP=?
OK
AT+CHUP
OK
VOICE CALL:END: 000033


## Página 179

A76XX Series_AT Command Manual_V1.09
www.simcom.com 178 /652
7.2.3 AT+CBST Selectbearerservicetype
Writecommandselects the bearerservice <name>with data rate <speed>,andthe connection element
<ce>tobeusedwhendatacallsareoriginated.Valuesmayalsobeusedduringmobileterminateddatacall
setup,especiallyin caseofsingle numbering schemecalls.
AT+CBST Selectbearerservicetype
TestCommand
AT+CBST=?
Response
+CBST:(list ofsupported<speed>s),(listofsupported
<name>s),(list ofsupported<ce>s)
OK
Read Command
AT+CBST?
Response
+CBST:<speed>,<name>,<ce>
OK
WriteCommand
AT+CBST=<speed>[,<name>
[,<ce>]]
Response
1)
+CBST:<speed>,<name>,<ce>
OK
2)
ERROR
Execution Command
AT+CBST
Setdefaultvalue
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<speed> 0 autobauding(automatic selectionof the speed;thissettingis
possible incaseof 3.1 kHzmodemand
non-transparentservice)
1 300 bps(V.21)
2 1200bps (V.22)
3 1200/75 bps (V.23)
4 2400bps (V.22bis)
5 2400bps (V.26ter)
6 4800bps (V.32)
7 9600 bps (V.32)
12 9600 bps (V.34)


## Página 180

A76XX Series_AT Command Manual_V1.09
www.simcom.com 179 /652
14 14400 bps (V.34)
15 19200 bps (V.34)
16 28800 bps (V.34)
17 33600 bps (V.34)
34 1200bps (V.120)
36 2400bps (V.120)
38 4800bps (V.120)
39 9600 bps (V.120)
43 14400 bps (V.120)
47 19200 bps (V.120)
48 28800 bps (V.120)
49 38400 bps (V.120)
50 48000 bps (V.120)
51 56000 bps (V.120)
65 300bps (V.110)
66 1200bps (V.110)
68 2400bps (V.110orX.31flag stuffing)
70 4800bps (V.110orX.31flag stuffing)
71 9600 bps (V.110orX.31flag stuffing)
75 14400 bps (V.110orX.31flag stuffing)
79 19200 bps (V.110orX.31flag stuffing)
80 28800 bps (V.110orX.31flag stuffing)
81 38400 bps (V.110orX.31flag stuffing)
82 48000 bps (V.110orX.31flag stuffing)
83 56000 bps (V.110orX.31flag stuffing)
84 64000 bps (X.31 flagstuffing)
115 56000 bps(bittransparent)
116 64000 bps(bittransparent)
120 32000bps (PIAFS32K)
121 64000bps (PIAFS64K)
130 28800bps (multimedia)
131 32000bps (multimedia)
132 33600bps (multimedia)
133 56000bps (multimedia)
134 64000bps (multimedia)
<name> 0 Asynchronousmodem
1 Synchronous modem–PADAccess(asynchronous)(UDI)
2 PacketAccess (synchronous)(UDI)
3 data circuitasynchronous (RDI)
4 data circuitsynchronous(RDI)
5 PADAccess(asynchronous)(RDI)
6 PacketAccess (synchronous)(RDI)
7 PacketAccess (synchronous)(RDI)
<ce> 0 transparent
1 non-transparent


## Página 181

A76XX Series_AT Command Manual_V1.09
www.simcom.com 180 /652
2 both,transparentpreferred
3 both,non-transparent preferred
Examples
AT+CBST=?
+CBST:
(0,1,2,3,4,5,6,7,12,14,15,16,17,34,36,38,39,43,47
,48,49,50,51,65,66,68,70,71,75,79,80,81,82,83,8
4,115,116,120,121,130,131,132,133,134),(0-7),(0-
3)
OK
AT+CBST?
+CBST:0,0,1
OK
AT+CBST=0,2,1
OK
AT+CBST
OK
Not allcombinations ofthese subparameters are supported.
7.2.4 AT+CRLP Radiolinkprotocol
Radio Link Protocol(RLP)parametersusedwhen non-transparentdatacalls are originatedmay bealtered
with write command.
AT+CRLP Radiolinkprotocol
TestCommand
AT+CRLP=?
Response
+CRLP:(rangeof supported<iws>s),(range ofsupported
<mws>s),(range ofsupported<T1>s),(range ofsupported
<N2>s)[,<ver>[,(rangeofsupported <T4>s)]]
OK
NOTE


## Página 182

A76XX Series_AT Command Manual_V1.09
www.simcom.com 181 /652
Read Command
AT+CRLP?
Response
+CRLP:<iws>,<mws>,<T1>,<N2>[,<ver>[,<T4>]]
OK
WriteCommand
AT+CRLP=<iws>[,<mws>[,<
T1>[,<N2>[,<ver>[,<T4>]]]]]
Response
1)
OK
2)
ERROR
Execution Command
AT+CRLP
Setdefaultvalue
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<ver> RLPversion numberin integerformat,and itcanbe 0or1;when
version indicationis notpresentitshall equal1.
<iws> IWFtoMS windowsize.
<mws> MS toIWFwindowsize.
<T1> Acknowledgement timer.
<N2> Retransmission attempts.
<T4> Re-sequencing period inintegerformat.
Examples
AT+CRLP=?
+CRLP:(0-61),(0-61),(39-255),(1-255),(0-1),(3-255)
OK
AT+CRLP?
+CRLP:61,61,128,255,1,3
OK
AT+CRLP=61,61,128,255,1,3
OK
AT+CRLP
OK
NOTE


## Página 183

A76XX Series_AT Command Manual_V1.09
www.simcom.com 182 /652
<T1>and <T4>arein units of10 ms.
7.2.5 AT+CRC Cellularresultcodes
Writecommandcontrols whetherornotthe extendedformatofincoming call indication orGPRSnetwork
requestforPDPcontext activation isused. Whenenabled,an incoming callis indicatedto theTEwith
unsolicited resultcode"+CRING:<type>"insteadofthe normalRING.
Testcommandreturns valuessupported bytheTAasacompound value.
AT+CRC Cellularresultcodes
TestCommand
AT+CRC=?
Response
+CRC:(listofsupported <mode>s)
OK
Read Command
AT+CRC?
Response
+CRC:<mode>
OK
WriteCommand
AT+CRC=<mode>
Response
1)
OK
2)
+CMEERROR: <err>
Execution Command
AT+CRC
Setdefaultvalue
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 0 disables reporting
1 enables reporting
<type> ASYNC asynchronoustransparent
SYNC synchronoustransparent
RELASYNC asynchronous non-transparent


## Página 184

A76XX Series_AT Command Manual_V1.09
www.simcom.com 183 /652
RELSYNC synchronous non-transparent
FAX facsimile
VOICE normalvoice
VOICE/XXX voicefollowed by data(XXXisASYNC,SYNC,REL
ASYNCorRELSYNC)
ALTVOICE/XXX alternating voice/data, voicefirst
ALTXXX/VOICE alternating voice/data, datafirst
ALTFAX/VOICE alternating voice/fax,faxfirst
Examples
AT+CRC=?
+CRC:(0,1)
OK
AT+CRC?
+CRC:0
OK
AT+CRC=1
OK
AT+CRC
OK
7.2.6 AT+CLCC Listcurrentcalls
This commandis used toreturn listof currentcallsof ME. Ifcommand succeeds butnocalls are available,
no information responseis senttoTE.
AT+CLCC Listcurrentcalls
TestCommand
AT+CLCC=?
Response
+CLCC:(rangeof supported<n>s)
OK
Read Command
AT+CLCC?
Response
+CLCC:<n>
OK
WriteCommand
AT+CLCC=<n>
Response
1)


## Página 185

A76XX Series_AT Command Manual_V1.09
www.simcom.com 184 /652
OK
2)
ERROR
Execution Command
AT+CLCC
Response
1)
+CLCC:
<id1>,<dir>,<stat>,<mode>,<mpty>[,<number>,<type>[,<alph
a>][,<priority>][,<CLIvalidity>]]
OK
2)
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
URC Description
+CLCC:
<id1>,<dir>,<stat>,<mode>,<mpty>[,<number>,<type>[
,<alpha>][,<priority>][,<CLIvalidity>]][
+CLCC:
<id2>,<dir>,<stat>,<mode>,<mpty>[,<number>,<type>[
,<alpha>][,<priority>][,<CLIvalidity>]]
Note:Thiscan bean indicationtolist the
currentcallinformationwhen<n>setto1.
DefinedValues
<n> 0 Don’treportalistofcurrentcallsofMEautomaticallywhenthe
currentcall statuschanges.
1 Reportalist ofcurrentcalls ofME automatically whenthe
currentcall statuschanges.
<idX> Integertype,call identificationnumber.
<dir> 0 mobileoriginated (MO)call
1 mobileterminated (MT)call
<stat> State ofthe call:
0 active
1 held
2 dialing (MOcall)
3 alerting (MOcall)
4 incoming (MTcall)
5 waiting (MTcall)
6 disconnect
<mode> bearer/teleservice:
0 voice


## Página 186

A76XX Series_AT Command Manual_V1.09
www.simcom.com 185 /652
1 data
2 fax
9 unknown
<mpty> 0 call is notone ofmultiparty (conference)callparties
1 call is one ofmultiparty (conference)call parties
<number> String type phonenumberin formatspecified by <type>.
<type> Typeof addressoctetinintegerformat;
128 Restrictednumbertype includesunknown type and format
145 Internationalnumbertype
161 national number.Thenetworksupport forthis type is
optional
177 network specificnumber,ISDNformat
129 Otherwise
<alpha> String type alphanumeric representationof<number> corresponding
totheentryfoundinphonebook;usedcharactersetshouldbetheone
selectedwith command SelectTECharacterSetAT+CSCS.
<priority> integertype parameterindicatingthe eMLPPprioritylevelof the call,
values specifiedin 3GPPTS22.067 [54].
<CLI validity> integertype.Thisparametercan provide detailswhy <number>does
notcontain acalling party BCDnumber(refer3GPPTS24.008 [8]
subclause10.5.4.30).Theparameterisnotpresent forMOcall types.
0 CLI valid
1 CLI hasbeen withheld bythe originator(refer 3GPP TS24.008 [8]
table10.5.135a/3GPPTS24.008 code "Rejectby user")
2 CLI isnotavailable dueto interworking problemsorlimitationsof
originatingnetwork (refer 3GPPTS24.008 [8]
table10.5.135a/3GPPTS24.008 code "Interaction with other
service")
3 CLI isnotavailable dueto calling party beingof type payphone
(refer 3GPPTS24.008 [8]table 10.5.135a/3GPPTS24.008 code
"Coinline/payphone")
4 CLIisnotavailableduetootherreasons(refer3GPPTS24.008 [8]
table10.5.135a/3GPPTS24.008 code "Unavailable")
WhenCLI isnotavailable (<CLI validity>=2,<CLI validity>=3 or
<CLI validity>=4),<number>shallbe an emptystring ("")and <type>
value will notbe significant.Nevertheless,TAmayreturn the
recommended value 128 for<type>(TON/NPIunknown in
accordance with 3GPP TS24.008 [8]subclause 10.5.4.7).
WhenCLI hasbeen withheldby the originator,(<CLIvalidity>=1)and
the CLIPis provisioned with the "overridecategory" option (refer
3GPPTS22.081 [3] and 3GPPTS23.081 [40]), <number>and
<type>is provided.Otherwise,TAshallreturn the samesettingfor
<number>and <type>as ifthe CLI was notavailable.


## Página 187

A76XX Series_AT Command Manual_V1.09
www.simcom.com 186 /652
Examples
AT+CLCC=?
+CLCC:(0-1)
OK
AT+CLCC?
+CLCC:1
OK
AT+CLCC=1
OK
AT+CLCC
OK
AT+CLCC
+CLCC:1,0,0,0, 0,"13883113271",129, "",,0
OK
AT+CLCC
+CLCC:1,0,0,0, 0,"13883113271",129, ""
OK
2Gcall
4Gcall
Parameters afteralpha:priority,CLI validity are currentlyonly implemented in2Gcalls
7.2.7 AT+CEER Extendederrorreport
ExecutioncommandcausestheTAtoreturntheinformationtext<report>,whichshouldoffertheuserofthe
TAan extended reportofthe reason for:
1. Thefailure inthe lastunsuccessfulcallsetup(originating oranswering)orin-call modification.
2. Thelastcallrelease.
3. ThelastunsuccessfulGPRSattach orunsuccessfulPDPcontextactivation.
4. ThelastGPRSdetachorPDPcontextdeactivation.
AT+CEER Extendederrorreport
NOTE


## Página 188

A76XX Series_AT Command Manual_V1.09
www.simcom.com 187 /652
TestCommand
AT+CEER=?
Response
OK
Execution Command
AT+CEER
Response
+CEER:<report>
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<report> Wrong informationwhichispossibly occurred.
Examples
AT+CEER=?
OK
AT+CEER
+CEER:"31 Normal unspecified"
OK
7.2.8 AT+CCWA Callwaiting
This commandallows controlof the CallWaiting supplementary service.Activation,deactivationandstatus
queryare supported.Whenquerying the statusof anetworkservice (<mode>=2)the responseline for'not
active' case(<status>=0)shouldbe returned onlyifservice isnotactive forany <class>.Parameter<n>is
usedtodisable/enablethepresentation ofanunsolicited resultcode+CCWA:<number>,<type>,<class>to
theTEwhencallwaiting serviceisenabled.Command shouldbe abortablewhen networkis interrogated.
AT+CCWA Callwaiting
TestCommand
AT+CCWA=?
Response
+CCWA:(rangeof supported<n>s),(rangeofsupported
<mode>s),(rangeof supported<class>s)
OK
Read Command
AT+CCWA?
Response
+CCWA:<n>


## Página 189

A76XX Series_AT Command Manual_V1.09
www.simcom.com 188 /652
OK
WriteCommand
AT+CCWA=<n>[,<mode>[,<c
lass>]]
Response
1)When<mode>=2 and commandsuccessful:
+CCWA:<status>,<class>[
+CCWA:<status>,<class>[..]]
OK
2)
OK
3)
+CMEERROR: <err>
Execution Command
AT+CCWA
Setdefaultvalue
Response
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> Sets/shows the resultcodepresentation status intheTA
0 disable
1 enable
<mode> When<mode>parameter isnotgiven,networkis notinterrogated:
0 disable
1 enable
2 query status
<class> Itisasumofintegerseachrepresentingaclassofinformation(default
7)
1 voice (telephony)
2 data (refers toallbearer services)
4 fax (facsimileservices)
7 voice,data and fax(1+2+4)
8 shortmessage service
16 data circuit sync
32 data circuit async
64 dedicated packetaccess
128 dedicated PADaccess
255 Thevalue 255covers allclasses
<status> 0 not active
1 active
<number> String type phonenumberof callingaddress informatspecified by


## Página 190

A76XX Series_AT Command Manual_V1.09
www.simcom.com 189 /652
<type>.
<type> Typeof addressoctetinintegerformat;
128 Restrictednumbertype includesunknown type and format
145 Internationalnumbertype
129 Otherwise
Examples
AT+CCWA=?
+CCWA: (0-1),(0-2),(1-255)
OK
AT+CCWA?
+CCWA: 1
OK
AT+CCWA=1
OK
AT+CCWA=1,2,7
+CCWA: 1,1
+CCWA: 0,2
+CCWA: 0,4
OK
AT+CCWA
OK
7.2.9 AT+CCFC Callforwardingnumberandconditions
Thiscommandallowscontrolofthecallforwardingsupplementaryservice.Registration,erasure,activation,
deactivation, andstatus query are supported.
AT+CCFC Callforwardingnumberandconditions
TestCommand
AT+CCFC=?
Response
+CCFC:(list ofsupported<reason>s)
OK
WriteCommand
AT+CCFC=<reason>,<mode
>[,<number>[,<type>[,<class
Response
1)When<mode>=2 and commandsuccessful:
+CCFC:


## Página 191

A76XX Series_AT Command Manual_V1.09
www.simcom.com 190 /652
>[,<subaddr>[,<satype>[,<ti
me>]]]]]]
<status>,<class1>[,<number>,<type>[,<subaddr>,<satype>[,<tim
e>]]][
+CCFC:
<status>,<class2>[,<number>,<type>[,<subaddr>,<satype>[,<tim
e>]]][..]]
OK
2)
OK
3)
ERROR
4)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<reason> 0 unconditional
1 mobilebusy
2 no reply
3 not reachable
4 allcall forwarding
5 allconditional call forwarding
<mode> 0 disable
1 enable
2 query status
3 registration
4 erasure
<number> Stringtypephonenumberofforwardingaddressinformatspecifiedby
<type>.
<type> Typeof addressoctetinintegerformat:
145 dialingstring<number> includes internationalaccesscode
character‘+’
129 otherwise
<subaddr> String type subaddress offormatspecified by <satype>.
<satype> Typeof subaddress octet inintegerformat, default 128.
<classX> Itisasumofintegerseachrepresentingaclassofinformation(default
7):
1 voice (telephony)
2 data (refers toallbearer services)
4 fax (facsimileservices)


## Página 192

A76XX Series_AT Command Manual_V1.09
www.simcom.com 191 /652
16 data circuit sync
32 data circuit async
64 dedicated packetaccess
128 dedicated PADaccess
255 Thevalue 255covers allclasses
<time> 1..30 when"noreply" is enabled orqueried, thisgivesthe timein
secondsto wait beforecall isforwarded,defaultvalue20.
<status> 0 not active
1 active
Examples
AT+CCFC=?
+CCFC:(0,1,2,3,4,5)
OK
AT+CCFC=0,2
+CCFC:0,7
OK
7.2.10 AT+CLIP Callinglineidentificationpresentation
This commandrefers to theGSM/UMTSsupplementary serviceCLIP(Calling Line Identification
Presentation)thatenables acalled subscriberto getthecalling lineidentity (CLI)ofthe calling party when
receivingamobileterminated call.
Writecommandenables ordisables thepresentation ofthe CLIat theTE.Ithasno effecton the execution
ofthe supplementary serviceCLIPin the network.
Whenthe presentation ofthe CLI attheTEis enabled(and calling subscriberallows),+CLIP:
<number>,<type>,,[,[<alpha>][,<CLIvalidity>]] responseis returned aftereveryRING(or +CRING:<type>;
refersub clause"Cellularresultcodes +CRC")resultcodesentfromTAtoTE.Itis manufacturerspecificif
this responseisused whennormalvoicecall isanswered.
AT+CLIP Callinglineidentificationpresentation
TestCommand
AT+CLIP=?
Response
+CLIP:(range ofsupported<n>s)
OK
Read Command
AT+CLIP?
Response
1)
+CLIP:<n>,<m>


## Página 193

A76XX Series_AT Command Manual_V1.09
www.simcom.com 192 /652
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+CLIP=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+CLIP
Setdefaultvalue
Response
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> Parametersets/shows the resultcodepresentation status in theTA:
0 disable
1 enable
<m> 0 CLIPnotprovisioned
1 CLIPprovisioned
2 unknown (e.g.no network,etc.)
<number> String type phonenumberof callingaddress informatspecified by
<type>.
<type> Typeof addressoctetinintegerformat;
128 Restrictednumbertype includesunknown type and format
145 Internationalnumbertype
161 national number.Thenetworksupport forthis type is
optional
177 network specificnumber,ISDNformat
129 Otherwise
<alpha> String type alphanumeric representationof<number> corresponding
to theentry foundin phonebook.
<CLI validity> 0 CLIvalid
1 CLI hasbeen withheld bythe originator
2 CLIisnotavailableduetointerworkingproblemsorlimitations
of originating network


## Página 194

A76XX Series_AT Command Manual_V1.09
www.simcom.com 193 /652
Examples
AT+CLIP=?
+CLIP:(0-1)
OK
AT+CLIP?
+CLIP:1,1
OK
AT+CLIP=0
OK
AT+CLIP
OK
7.2.11 AT+CLIR Callinglineidentificationrestriction
This commandrefers to CLIR-servicethat allows acalling subscriberto enable ordisablethe presentation
ofthe CLI tothe called party whenoriginatingacall.
Writecommandoverridesthe CLIRsubscription(default isrestrictedorallowed)when temporarymodeis
provisioned as adefault adjustmentforallfollowing outgoingcalls.This adjustmentcan berevoked by
using theoppositecommand.Ifthis commandis usedby asubscriberwithoutprovisionof CLIRin
permanentmodethe networkwillact.
Read commandgives the default adjustmentforalloutgoingcalls (givenin <n>),andalsotriggers an
interrogation ofthe provision statusof theCLIRservice(given in <m>).
Testcommandreturns valuessupported asacompound value.
AT+CLIR Callinglineidentificationrestriction
TestCommand
AT+CLIR=?
Response
+CLIR:(range ofsupported<n>s)
OK
Read Command
AT+CLIR?
Response
1)
+CLIR:<n>,<m>
OK
2)
ERROR


## Página 195

A76XX Series_AT Command Manual_V1.09
www.simcom.com 194 /652
3)
+CMEERROR: <err>
WriteCommand
AT+CLIR=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 0 presentation indicator isusedaccording tothe subscriptionof
the CLIRservice
1 CLIRinvocation
2 CLIRsuppression
<m> 0 CLIRnotprovisioned
1 CLIRprovisioned inpermanentmode
2 unknown (e.g.no network,etc.)
3 CLIRtemporarymodepresentation restricted
4 CLIRtemporarymodepresentation allowed
Examples
AT+CLIR=?
+CLIR:(0-2)
OK
AT+CLIR?
+CLIR:0,0
OK
AT+CLIR=1
OK


## Página 196

A76XX Series_AT Command Manual_V1.09
www.simcom.com 195 /652
7.2.12 AT+COLP Connectedlineidentificationpresentation
This commandrefers to theGSM/UMTSsupplementary serviceCOLP(Connected LineIdentification
Presentation)thatenables acalling subscribertogetthe connectedline identity(COL)of the called party
aftersetting upamobileoriginated call.Thecommand enablesor disables the presentation ofthe COLat
theTE.Ithasno effectonthe execution ofthe supplementaryservice COLRin the network.
Whenenabled (andcalled subscriberallows),+COLP:<number>,<type>[,<subaddr>,<satype>[,<alpha>]]
intermediate resultcodeis returned fromTAtoTEbefore any+CRresponses.Itis manufacturerspecific if
this responseisused whennormalvoicecall isestablished.
AT+COLP Connectedlineidentificationpresentation
TestCommand
AT+COLP=?
Response
+COLP:(list ofsupported<n>s)
OK
Read Command
AT+COLP?
Response
1)
+COLP:<n>,<m>
OK
2)
ERROR
3)
+CMEERROR: <err>
WriteCommand
AT+COLP=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+COLP
Setdefaultvalue
Response
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 20S
Reference 3GPPTS27.007
DefinedValues
<n> Parametersets/shows the resultcodepresentation status in theTA:
0 disable


## Página 197

A76XX Series_AT Command Manual_V1.09
www.simcom.com 196 /652
1 enable
<m> 0 COLPnotprovisioned
1 COLPprovisioned
2 unknown (e.g.no network,etc.)
Examples
AT+COLP=?
+COLP:(0-1)
OK
AT+COLP?
+COLP:1,0
OK
AT+COLP=1
OK
AT+COLP
OK
7.2.13 AT+CHLD Callrelatedsupplementaryservices
This commandallows the control ofthe following callrelated services:
- acallcan betemporarilydisconnected fromthe MTbutthe connectionis retainedby thenetwork;
- multipartyconversation (conferencecalls);
- theservedsubscriberwhohastwo calls(oneheldandtheothereitheractiveoralerting)canconnect
the otherparties andrelease the served subscriber'sown connection.
Calls can be put on hold, recovered, released, added to conversation, and transferred similarly as defined
in3GPPTS22.030[19].
This isbased on thesupplementary services HOLD(CallHold; refer3GPPTS22.083 [5] clause2and
3GPPTS24.610 [135]),MPTY/CONF (MultiParty; refer3GPPTS22.084 [22] and Conference;refer
3GPPTS24.605 [133])andECT(Explicit CallTransfer;refer 3GPPTS22.091 [30]and
3GPPTS24.629 [139]).
AT+CHLD Callrelatedsupplementaryservices
TestCommand
AT+CHLD=?
Response
+CHLD:(listofsupported <n>s)
OK
WriteCommand Response


## Página 198

A76XX Series_AT Command Manual_V1.09
www.simcom.com 197 /652
AT+CHLD=<n> 1)
OK
2)
ERROR
3)
+CMEERROR:<err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 20S
Reference 3GPPTS27.007
DefinedValues
<n> integertype; equalstonumbers enteredbeforeSEND buttonin
3GPPTS22.030[19] clause6.5.5.1.
0 Releaseallheld callsorsetuserdetermineduserbusy
(UDUB)forawaiting call.
1 Releaseallactive calls(ifany exist)andaccept theother
(held orwaiting)call.
1x Releases aspecificactive callX.
2 Places all active calls(ifanyexist) onhold andaccepts the
other(held orwaiting)call.
2x Places allactive calls on holdexceptcall Xwith which
communication shallbesupported.
3 Addaheld calltothe conversation (multiparty).
4 Connects the two calls and disconnectsthe subscriberfrom
both calls (ECT).
Examples
AT+CHLD=?
+CHLD:(0,1,1x,2,2x,3,4)
OK
AT+CHLD=1
OK
7.2.14 AT+VTS DTMFandtonegeneration
This commandallows the transmissionofDTMF tones andarbitrarytones which causethe Mobile
Switching Center(MSC)to transmittonesto aremote subscriber.Thecommand can onlybe used invoice


## Página 199

A76XX Series_AT Command Manual_V1.09
www.simcom.com 198 /652
modeof operation (active voicecall).
AT+VTS DTMFandtonegeneration
TestCommand
AT+VTS=?
Response
+VTS:(list ofsupported<dtmf>s)
OK
WriteCommand
AT+VTS=<dtmf>[,<duration>
]
or
AT+VTS=<dtmf-string>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<dtmf> AsingleASCIIcharacterinthe set 0-9,*,#,A,B,C,D.
<duration> Toneduration in1/10 seconds,from 300to600.This isinterpreted as
aDTMFtone of differentduration fromthatmandated by theAT+VTD
command,otherwise,the duration whichbe settheAT+VTD
command will beusedforthe tone(<duration>is omitted).
<dtmf-string> AsequenceofASCIIcharacterin the set0-9, *,#,A,B,C,D,and
maximallength ofthe string is32.Thestring mustbeenclosed in
doublequotes ("").Eachofthe tones with adurationwhichis setby
theAT+VTDcommand.
Examples
AT+VTS=?
+VTS:(0-9,*,#,A,B,C,D)
OK
AT+VTS=1,600
OK
AT+VTS="135"
OK
NOTE


## Página 200

A76XX Series_AT Command Manual_V1.09
www.simcom.com 199 /652
TheENDeventof voice callwillterminate the transmissionoftones,and asan operatoroption,the
tone maybe ceasedafter apre-determinedtimewhetherornottoneduration hasbeen reached.
7.2.15 AT+VTD Toneduration
Thisreferstoaninteger<n>thatdefinesthelengthoftonesemittedasaresultoftheAT+VTScommand.A
value differentthan zero causesatone ofduration <n>/10seconds.
AT+VTD Toneduration
TestCommand
AT+VTD=?
Response
+VTD:(rangeofsupported <n>s)
OK
Read Command
AT+VTD?
Response
+VTD:<n>
OK
WriteCommand
AT+VTD=<n>
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<n> 300-600Toneduration ofeverysingle tone in1/10 seconds.
Examples
AT+VTD=?
+VTD:(300-600)


## Página 201

A76XX Series_AT Command Manual_V1.09
www.simcom.com 200 /652
OK
AT+VTD?
+VTD:300
OK
AT+VTD=400
OK
7.2.16 AT+CSTA Selecttypeofaddress
Writecommandis used toselectthe type ofnumberforfurther dialingcommands(ATD)according to
GSM/UMTSspecifications.
Read commandreturns thecurrenttype ofnumber.
Testcommandreturns valuessupported bythe Module as acompound value.
AT+CSTA Selecttypeofaddress
TestCommand
AT+CSTA=?
Response
+CSTA:(list ofsupported<type>s)
OK
Read Command
AT+CSTA?
Response
+CSTA:<type>
OK
WriteCommand
AT+CSTA=<type>
Response
1)
OK
2)
ERROR
Execution Command
AT+CSTA
Setdefaultvalue
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<type> Typeof addressoctetinintegerformat:


## Página 202

A76XX Series_AT Command Manual_V1.09
www.simcom.com 201 /652
145 whendialling stringincludes international accesscode
character"+"
161 national number.Thenetworksupport forthis type is
optional
177 network specificnumber,ISDNformat
129 otherwise
Examples
AT+CSTA=?
+CSTA: (129,145,161,177)
OK
AT+CSTA?
+CSTA: 129
OK
AT+CSTA=145
OK
AT+CSTA
OK
Becausethe type of addressis automatically detected onthe dial string ofdialing command,command
AT+CSTAhasreally noeffect.
7.2.17 AT+CMOD Callmode
Writecommandselects the call mode offurtherdialing commands (ATD)orfornextanswering command
(ATA).Modecan beeithersingle or alternating.
Testcommandreturns valuessupported bytheTAasacompound value.
AT+CMOD Callmode
TestCommand
AT+CMOD=?
Response
+CMOD:(listofsupported <mode>s)
OK
NOTE


## Página 203

A76XX Series_AT Command Manual_V1.09
www.simcom.com 202 /652
Read Command
AT+CMOD?
Response
+CMOD:<mode>
OK
WriteCommand
AT+CMOD=<mode>
Response
1)
OK
2)
ERROR
Execution Command
AT+CMOD
Setdefaultvalue:
Response
OK
ParameterSaving Mode -
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 0 single mode(only supported)
Examples
AT+CMOD=?
+CMOD:(0)
OK
AT+CMOD?
+CMOD:0
OK
AT+CMOD=0
OK
AT+CMOD
OK
Thevalue of<mode>shall besetto zeroafterasuccessfully completed alternatingmodecall.Itshall
be settozeroalsoafter afailedanswering.Thepower-on,factory and userresetsshallalsosetthe
value tozero.This reducesthe possibility thatalternating modecalls areoriginated oranswered
accidentally.
NOTE


## Página 204

A76XX Series_AT Command Manual_V1.09
www.simcom.com 203 /652
7.2.18 AT+VMUTE Speakermutecontrol
This commandis used tocontrolthe loudspeakertomute andunmute duringavoice calloravideo call
whichis connected.Ifthere isnotaconnectedcall,writecommand can’tbeused. When all callsare
disconnected,the Module sets thesubparameter as0automatically.
AT+VMUTE Speakermutecontrol
TestCommand
AT+VMUTE=?
Response
+VMUTE:(list ofsupported<mode>s)
OK
Read Command
AT+VMUTE?
Response
+VMUTE:<mode>
OK
WriteCommand
AT+VMUTE=<mode>
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 0 mute off
1 mute on
Examples
AT+VMUTE=?
+VMUTE:(0-1)
OK
AT+VMUTE?
+VMUTE:0
OK
AT+VMUTE=1


## Página 205

A76XX Series_AT Command Manual_V1.09
www.simcom.com 204 /652
OK
7.2.19 AT+CMUT Microphonemutecontrol
Thiscommandisusedtoenableanddisabletheuplinkvoicemutingduringavoicecalloravideocallwhich
isconnected.Ifthereisnotaconnectedcall,writecommandcan’tbeused.Whenallcallsaredisconnected,
the Modulesets the subparameteras 0automatically.
AT+CMUT Microphonemutecontrol
TestCommand
AT+CMUT=?
Response
+CMUT:(listofsupported <mode>s)
OK
Read Command
AT+CMUT?
Response
+CMUT:<mode>
OK
WriteCommand
AT+CMUT=<mode>
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> 0 mute off
1 mute on
Examples
AT+CMUT=?
+CMUT:(0-1)
OK
AT+CMUT?
+CMUT:0
OK
AT+CMUT=1


## Página 206

A76XX Series_AT Command Manual_V1.09
www.simcom.com 205 /652
OK
7.2.20 AT+CSDVC Switchvoicechanneldevice
This commandis used toswitchvoice channeldevice.Afterchanging currentvoicechanneldeviceand if
there isaconnecting voicecall,itwillusethe settings ofprevious device(loudspeakervolumelevel, mute
stateof loudspeakerand microphone,refertoAT+VMUTE, andAT+CMUT).
AT+CSDVC Switchvoicechanneldevice
TestCommand
AT+CSDVC=?
Response
+CSDVC:(list ofsupported<dev>s)
OK
Read Command
AT+CSDVC?
Response
+CSDVC:<dev>
OK
WriteCommand
AT+CSDVC=<dev>
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<dev> 1 handset
3 speakerphone
Examples
AT+CSDVC=?
+CSDVC:(1,3)
OK
AT+CSDVC?
+CSDVC:1
OK
AT+CSDVC=3


## Página 207

A76XX Series_AT Command Manual_V1.09
www.simcom.com 206 /652
OK
7.2.21 AT+CMICGAIN Adjustmicgain
Thiscommandisusedtoadjustmicgain.Ifthiscommandwasusedduringcall,itwilltakeimmediateeffect.
Otherwise,it will takeeffectinnextcall.
AT+CMICGAIN Adjustmicgain
TestCommand
AT+CMICGAIN=?
Response
+CMICGAIN:(rangeof supported<value>s)
OK
Read Command
AT+CMICGAIN?
Response
+CMICGAIN:<value>
OK
WriteCommand
AT+CMICGAIN=<value>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<value> Gainvalue from0-7,7is the max.4is the defaultvalue.
Examples
AT+CMICGAIN=?
+CMICGAIN:(0-7)
OK
AT+CMICGAIN?
+CMICGAIN:4
OK


## Página 208

A76XX Series_AT Command Manual_V1.09
www.simcom.com 207 /652
AT+CMICGAIN=7
OK
7.2.22 AT+COUTGAIN Adjustoutgain
Thiscommandisusedtoadjustout(speaker/handset)gain.Ifthiscommandwasusedduringcall,itwilltake
immediate effect.Otherwise, itwilltakeeffectin nextcall.
AT+COUTGAIN Adjustoutgain
TestCommand
AT+COUTGAIN=?
Response
+COUTGAIN:(range ofsupported <value>s)
OK
Read Command
AT+COUTGAIN?
Response
+COUTGAIN:<value>
OK
WriteCommand
AT+COUTGAIN=<value>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<mode> Gainvalue from0-7,7is the max.4is the defaultvalue.
Examples
AT+COUTGAIN=?
+COUTGAIN:(0-7)
OK
AT+COUTGAIN?
+COUTGAIN:4


## Página 209

A76XX Series_AT Command Manual_V1.09
www.simcom.com 208 /652
OK
AT+COUTGAIN=7
OK


## Página 210

A76XX Series_AT Command Manual_V1.09
www.simcom.com 209 /652
ATCommandsforPhonebook
8.1 OverviewofATCommandsforPhonebook
Command Description
AT+CPBS Selectphonebook memorystorage
AT+CPBR Readphonebook entries
AT+CPBF Findphonebook entries
AT+CPBW Writephonebookentry
AT+CNUM Subscribernumber
8.2 DetailedDescriptionofATCommandsforPhonebook
8.2.1 AT+CPBS Selectphonebookmemorystorage
This commandselects the active phonebook storage, i.e.the phonebook storage thatallsubsequent
phonebookcommands will beoperatingon.
AT+CPBS Selectphonebookmemorystorage
TestCommand
AT+CPBS=?
Response
+CPBS:(list ofsupported <storage>s)
OK
Read Command
AT+CPBS?
Response
1)
+CPBS:<storage>[,<used>,<total>]
OK
2)
+CMEERROR: <err>


## Página 211

A76XX Series_AT Command Manual_V1.09
www.simcom.com 210 /652
WriteCommand
AT+CPBS=<storage>
Response
1)
OK
2)
ERROR
3)
+CMEERROR:<err>
Execution Command
AT+CPBS
Setdefaultvalue "SM"
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<storage> Valuesreserved bythe present document:
"FD" SIM/USIM fixdialing phonebook.Ifa SIMcard is
presentorifaUICCwith an activeGSM application ispresent,
the information inEFFDNunderDFTelecom is selected.Ifa
UICCwith anactive USIM application is present,the information
inEFFDNunderADFUSIM is selected.
"ON" SIM(orMT)own numbers(MSISDNs)list(reading of
thisstoragemay beavailable through+CNUM also).When
storinginformation inthe SIM/UICC,ifa SIMcard ispresent orif
aUICCwith anactive GSM applicationis present,the
informationinEFMSISDN underDFTelecom isselected.Ifa
UICCwith anactive USIM application is present,the information
inEFMSISDN underADFUSIM isselected.
"SM" SIM/UICCphonebook.IfaSIM card ispresentorifa
UICCwith anactive GSM application ispresent,the EFADN
underDFTelecomis selected.IfaUICC with anactive USIM
applicationis present,the globalphonebook,DFPHONEBOOK
underDFTelecomis selected.
"AP" Selected application phonebook.IfaUICCwith an
activeUSIM applicationis present,the application phonebook,
DFPHONEBOOKunderADFUSIM is selected.
<used> Integertype valueindicating thenumber ofused locationsin
selectedmemory.
<total> Integertype valueindicating thetotal numberoflocations in
selectedmemory.
Examples


## Página 212

A76XX Series_AT Command Manual_V1.09
www.simcom.com 211 /652
AT+CPBS=?
+CPBS:("SM","FD","ON","AP")
OK
AT+CPBS?
+CPBS:"SM",8,500
OK
AT+CPBS="SM"
OK
AT+CPBS
OK
8.2.2 AT+CPBR Readphonebookentries
This commandgets the recordinformationfrom the selected memorystoragein phonebook.Ifthe storage
is selectedas"SM" then the commandwillreturn the recordin SIM phonebook,the sametoothers.
AT+CPBR Readphonebookentries
TestCommand
AT+CPBR=?
Response
1)
+CPBR:(<minIndex>-<maxIndex>),[<nlength>],[<tlength>]
OK
2)
+CMEERROR: <err>
WriteCommand
AT+CPBR=<index1>[,<index2>]
Response
1)
[+CPBR:<index>,<number>,<type>,<text>[
+CPBR:<index>,<number>,<type>,<text>[…]]]
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007


## Página 213

A76XX Series_AT Command Manual_V1.09
www.simcom.com 212 /652
DefinedValues
<index1> Integertype valuein the range oflocation numbers ofphonebook
memory.
<index2> Integertype valuein the range oflocation numbers ofphonebook
memory.
<index> Integertype.the current positionnumber ofthe Phonebook index.
<minIndex> Integertype the minimum<index>number.
<maxIndex> Integertype the maximum <index>number
<number> Stringtype,phone numberofformat <type>,the maximumlength is
<nlength>.
<type> Type ofphone numberoctet inintegerformat,default145 when
dialingstringincludes internationalaccess codecharacter"+",
otherwise 129.
<text> Stringtype field ofmaximum length <tlength>;often this valueisset
asname.
<nlength> Integertype valueindicatingthe maximumlengthoffield <number>.
<tlength> Integertype valueindicating themaximum length offield <text>.
Examples
AT+CPBR=?
+CPBR:(1-500),40,14
OK
AT+CPBR=3
+CPBR:3,"1234567890123456789012345678901234567890",129,""
OK
8.2.3 AT+CPBF Findphonebookentries
This commandfindsthe record inphonebook(from the current phonebook memorystorageselectedwith
AT+CPBS)whichalphanumeric field has substring<findtext>.If <findtext>is null, itwilllists allthe entries.
AT+CPBF Findphonebookentries
TestCommand
AT+CPBF=?
Response
1)
+CPBF:[<nlength>],[<tlength>]


## Página 214

A76XX Series_AT Command Manual_V1.09
www.simcom.com 213 /652
OK
2)
+CMEERROR: <err>
WriteCommand
AT+CPBF=[<findtext>]
Response
1)
[+CPBF:<index1>,<number>,<type>,<text>[
+CPBF:<indexN>,<number>,<type>,<text>[…]]]
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<findtext> Stringtype,this value isusedtofind the record.Characterset
shouldbe theone selectedwith commandAT+CSCS.
<index> Integertype valuesin therange oflocation numbers of
phonebookmemory.
<number> Stringtype,phonenumberofformat<type>,themaximumlength
is<nlength>.
<type> Type ofphone numberoctet inintegerformat,default145 when
dialingstringincludes internationalaccess codecharacter"+",
otherwise 129.
<text> Stringtype field ofmaximum length <tlength>;often this valueis
setasname.
<nlength> Integertype valueindicating themaximum length offield
<number>.
<tlength> Integertype valueindicating themaximum length offield <text>.
Examples
AT+CPBF=?
+CPBF:40,14
OK


## Página 215

A76XX Series_AT Command Manual_V1.09
www.simcom.com 214 /652
AT+CPBF="lly"
+CPBF:500,"1234567890123456789012345678901234567890",129,"lly"
OK
8.2.4 AT+CPBW Writephonebookentry
This commandwrites phonebookentry inlocation number<index>inthe currentphonebookmemory
storageselected withAT+CPBS.
AT+CPBW Writephonebookentry
TestCommand
AT+CPBW=?
Response
1)
+CPBW:(listofsupported <index>s),[<nlength>],(listof supported
<type>s),[<tlength>]
OK
2)
+CMEERROR: <err>
WriteCommand
AT+CPBW=[<index>][,<number
>[,<type>[,<text>]]]
Response
1)
OK
2)
ERROR
3)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<index> Integertype valuesin therange oflocation numbers ofphonebook
memory.If<index>is notgiven,the firstfreeentry will beused. If
<index>is givenas the only parameter,the phonebook entry
specifiedby <index>is deleted.Ifrecordnumber<index>already
exists,itwill beoverwritten.
<number> Stringtype,phone numberofformat <type>,the maximumlength is
<nlength>.Itmustbean non-empty string.
<type> Type ofaddressoctet inintegerformat,Therange ofvalue isfrom
129to255. If<number>contains aleading "+"<type>=145


## Página 216

A76XX Series_AT Command Manual_V1.09
www.simcom.com 215 /652
(international)isused. Supported valueare:
145 whendialling stringincludes internationalaccesscode
character"+"
161 nationalnumber.Thenetwork supportforthistype is
optional
177 network specificnumber,ISDN format
129 otherwise
NOTE:Othervalue referTS24.008 [8]subclause 10.5.4.7.
<text> Stringtype field ofmaximum length <tlength>;charactersetas
specifiedby commandSelectTECharacterSetAT+CSCS.
<nlength> Integertype valueindicatingthe maximumlengthoffield <number>.
<tlength> Integertype valueindicating themaximum length offield <text>.
<text>has amaximumof14 bytes.PBiseventually storedinthe
UnicodeUCS2 encoding,whichis double-byte encoding,soonly
sevencharacters can bewritten
NOTE:Ifthe parameters of<type>and<text>are omittedandthe
firstcharacterof<number>is ‘+’,itwillspecify<type>as 145(129if
the firstcharacterisn’t‘+’)and<text>as NULL.
Examples
AT+CPBW=?
+CPBW:(1-500),40,(129,145,161,177),14
OK
AT+CPBW=493,"12345678901234567890",129,"lly1"
OK
8.2.5 AT+CNUM Subscribernumber
Execution commandreturns the MSISDNsrelated tothe subscriber(this information canbe storedin the
SIM orinthe ME).Ifsubscriberhas differentMSISDNfordifferentservices,eachMSISDNisreturnedin a
separate line.
AT+CNUM Subscribernumber
TestCommand
AT+CNUM=?
Response
1)
OK
WriteCommand
AT+CNUM=<index>[,<number>[
Response
1)


## Página 217

A76XX Series_AT Command Manual_V1.09
www.simcom.com 216 /652
,<type>[,<text>]]] OK
2)
+CMEERROR: <err>
Execution Command
AT+CNUM
Response
1)
[+CNUM:<text>,<number>,<type>
+CNUM:<text>,<number>,<type>]
OK
2)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.007
DefinedValues
<index> Integertype valuesin therange (1,2). If<index>is givenas the
onlyparameterandis1or2,theMSISDNspecifiedby<index>is
deleted.Ifrecord number<index>already exists,itwillbe
overwritten.
<number> Stringtype phone numberofformatspecified by <type>.
<type> Type ofaddressoctet inintegerformat.Refer tothe CPBW
<type>.
<text> Stringtype field ofmaximum length <tlength>;charactersetas
specifiedby commandSelectTECharacterSetAT+CSCS.
Examples
AT+CNUM=?
OK
AT+CNUM
OK
CAT1,CAT4modules do notsupportWriteCommand.
NOTE


## Página 218

A76XX Series_AT Command Manual_V1.09
www.simcom.com 217 /652
ATCommandsforSMS
9.1 OverviewofATCommandsforSMS
Command Description
AT+CSMS Selectmessageservice
AT+CPMS Preferred message storage
AT+CMGF SelectSMS messageformat
AT+CSCA SMS servicecentreaddress
AT+CSCB Selectcell broadcastmessage indication
AT+CSMP Settext mode parameters
AT+CSDH Showtextmodeparameters
AT+CNMA Newmessageacknowledgement toME/TA
AT+CNMI Newmessageindications toTE
AT+CGSMS Selectservice forMOSMS messages
AT+CMGL ListSMS messagesfrompreferred store
AT+CMGR Readmessage
AT+CMGS Sendmessage
AT+CMSS Sendmessage fromstorages
AT+CMGW Writemessage tomemory
AT+CMGD Deletemessage
AT+CMGMT Changemessage status
AT+CMVP Setmessagevalid period
AT+CMGRD Readand deletemessage
AT+CMGSEX Sendmessage
AT+CMSSEX Sendmultimessagesfrom storage
AT+CCONCINDEX ReportConcatenated SMS Index
Command Description SupportedModules
AT+CSCB=<mode>,<mi
ds>,<dcss>
Parameters are not allowed to be
omitted
OnlyCAT1Modules
AT+CMGS
AT+CMGW
Allow deleting input SMS data in data
mode
OnlyCAT1Modules
AT+CMGSEX NoSMSsendURCreport,justreporta OnlyCAT1Modules


## Página 219

A76XX Series_AT Command Manual_V1.09
www.simcom.com 218 /652
OKbefore lastSMS input.
9.2 DetailedDescriptionofATCommandsforSMS
9.2.1 AT+CSMS Selectmessageservice
This commandis used toselectmessagingservice <service>.
AT+CSMS Selectmessageservice
TestCommand
AT+CSMS=?
Response
+CSMS:(rangeofsupported <service>s)
OK
Read Command
AT+CSMS?
Response
+CSMS:<service>,<mt>,<mo>,<bm>
OK
WriteCommand
AT+CSMS=<service>
Response
1)
+CSMS:<mt>,<mo>,<bm>
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<service> 0 SMS atcommand iscompatiblewith GSM phase2.
1 SMS atcommand iscompatiblewith GSM phase2+.
<mt> 0 type notsupported.
1 type supported.
<mo> 0 type notsupported.
1 type supported.


## Página 220

A76XX Series_AT Command Manual_V1.09
www.simcom.com 219 /652
<bm> 0 type notsupported.
1 type supported.
Examples
AT+CSMS=0
+CSMS:1,1,1
OK
AT+CSMS?
+CSMS:0,1,1,1
OK
AT+CSMS=?
+CSMS:(0-1)
OK
9.2.2 AT+CPMS Preferredmessagestorage
This commandis used toselectmemorystorages <mem1>,<mem2>and<mem3>to beusedforreading,
writing, etc.
AT+CPMS Preferredmessagestorage
TestCommand
AT+CPMS=?
Response
+CPMS:(listof supported<mem1>s),(list ofsupported
<mem2>s),(listofsupported <mem3>s)
OK
Read Command
AT+CPMS?
Response
+CPMS:
<mem1>,<used1>,<total1>,<mem2>,<used2>,<total2>,<mem3>,
<used3>,<total3>
OK
WriteCommand
AT+CPMS=<mem1>[,<mem2>[,
<mem3>]]
Response
1)
+CPMS:<used1>,<total1>,<used2>,<total2>,<used3>,<total3>
OK


## Página 221

A76XX Series_AT Command Manual_V1.09
www.simcom.com 220 /652
2)
ERROR
3)
+CMSERROR: <err>
Execution Command
AT+CPMS
Response
1)Setdefaultvalue
(<mem1>="SM",<mem2>="SM",<mem3>="SM"):
+CPMS:<used1>,<total1>,<used2>,<total2>,<used3>,<total3>
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<mem1> Stringtype,memory fromwhichmessagesare readand deleted
(commandsListMessagesAT+CMGL,ReadMessageAT+CMGR
andDeleteMessageAT+CMGD).
"ME" FLASHmessagestorage
"SM" SIMmessage storage
<mem2> Stringtype,memory towhichwritingandsending operations are
made(commandsSend MessagefromStorageAT+CMSS and
WriteMessage toMemoryAT+CMGW).
"ME" FLASHmessagestorage
"SM" SIMmessage storage
<mem3> Stringtype,memorytowhichreceivedSMSispreferredtobestored
(unlessforwardeddirectly toTE;refercommand NewMessage
IndicationsAT+CNMI).
"ME" FLASHmessagestorage
"SM" SIMmessage storage
<bm> Integertype,numberof messages currentlyin <memX>.
<totalX> Integertype,total numberofmessage locationsin <memX>.
Examples
AT+CPMS=?
+CPMS:("ME","SM"),("ME","SM"),("ME","SM")


## Página 222

A76XX Series_AT Command Manual_V1.09
www.simcom.com 221 /652
OK
AT+CPMS?
+CPMS:"ME",0,180,"ME", 0,180,"ME",0,180
OK
AT+CPMS="SM","SM","SM"
+CPMS:3,50,3,50,3,50
OK
AT+CPMS
+CPMS:3,50,3,50,3,50
OK
9.2.3 AT+CMGF SelectSMSmessageformat
This commandis used tospecifytheinput and output formatofthe shortmessages.
AT+CMGF SelectSMSmessageformat
TestCommand
AT+CMGF=?
Response
1)
+CMGF:(rangeofsupported<mode>s)
OK
2)
ERROR
Read Command
AT+CMGF?
Response
1)
+CMGF:<mode>
OK
2)
ERROR
WriteCommand
AT+CMGF=<mode>
Response
1)
OK
2)
ERROR
Execution Command
AT+CMGF
Response
1)
Setdefaultvalue(<mode>=0):
OK


## Página 223

A76XX Series_AT Command Manual_V1.09
www.simcom.com 222 /652
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<mode> 0 PDUmode
1 Textmode
Examples
AT+CMGF?
+CMGF:0
OK
AT+CMGF=?
+CMGF:(0-1)
OK
AT+CMGF=1
OK
AT+CMGF
OK
9.2.4 AT+CSCA SMSservicecentreaddress
ThiscommandisusedtoupdatetheSMSCaddress,throughwhichmobileoriginatedSMSaretransmitted.
AT+CSCA SMSservicecentreaddress
TestCommand
AT+CSCA=?
Response
OK
Read Command
AT+CSCA?
Response
1)
+CSCA: <sca>,<tosca>
OK
2)
ERROR


## Página 224

A76XX Series_AT Command Manual_V1.09
www.simcom.com 223 /652
WriteCommand
AT+CSCA=<sca>[,<tosca>]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<sca> ServiceCentreAddress,value field instringformat, BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset (referto
commandAT+CSCS), type of addressgivenby <tosca>.
<tosca> SCaddressType-of-Address octetinintegerformat, whenfirst
characterof<sca>is+(IRA43)defaultis 145,otherwise default
is129.
Examples
AT+CSCA=?
OK
AT+CSCA="+8613012345678"
OK
AT+CSCA?
+CSCA:"+8613010314500", 145
OK
9.2.5 AT+CSCB Selectcellbroadcastmessageindication
Thetest commandreturns the supported<mode>s as acompound value.
Thereadcommand displays theaccepted messagetypes.
Depending onthe <mode> parameter,the writecommand adds ordeletesthe messagetypes accepted.
AT+CSCB Selectcellbroadcastmessageindication
TestCommand
AT+CSCB=?
Response
1)
+CSCB:(rangeofsupported <mode>s)


## Página 225

A76XX Series_AT Command Manual_V1.09
www.simcom.com 224 /652
OK
2)
ERROR
Read Command
AT+CSCB?
Response
1)
+CSCB:<mode>,<mids>,<dcss>
OK
2)
ERROR
WriteCommand
AT+CSCB=<mode>[,<mids>[,<d
css>]]
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<mode> 0 messagetypesspecifiedin<mids>and<dcss>areaccepted.
1 messagetypes specifiedin <mids>and<dcss>arenot
accepted.
<mids> Stringtype;alldifferentpossible combinationsof CBM message
identifiers.
<dcss> Stringtype;alldifferentpossible combinationsof CBM data
codingschemes.
The Read command for A7600 series return a list of available parameters <mids> and <dcss> with
<mode>0. Ifnoparameters are available,return <mode>1.
Examples
AT+CSCB=?
+CSCB:(0-1)
NOTE


## Página 226

A76XX Series_AT Command Manual_V1.09
www.simcom.com 225 /652
OK
AT+CSCB?
+CSCB:1,"",""
OK
AT+CSCB=0,"15-17,50,86",""
OK
9.2.6 AT+CSMP Settextmodeparameters
ThiscommandisusedtoselectvaluesforadditionalparametersneededwhenSMissenttothenetworkor
placedinstorage whentextformat messagemodeis selected.
AT+CSMP Settextmodeparameters
TestCommand
AT+CSMP=?
Response
OK
Read Command
AT+CSMP?
Response
1)
+CSMP:<fo>,<vp>,<pid>,<dcs>
OK
WriteCommand
AT+CSMP=<fo>[,<vp>[,<pid>[,<
dcs>]]]
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<fo> Dependingon the Command orresultcode:firstoctetofGSM
03.40SMS-DELIVER, SMS-SUBMIT(default17),
SMS-STATUS-REPORT,or SMS-COMMAND (default2)in
integerformat.SMS statusreportissupportedundertextmodeif
<fo>isset to49.


## Página 227

A76XX Series_AT Command Manual_V1.09
www.simcom.com 226 /652
<vp> Dependingon SMS-SUBMIT<fo>setting: GSM
03.40,TP-Validity-Periodeitherinintegerformat (default167),in
time-stringformat, orifissupported,in enhanced format
(hexadecimalcoded stringwith quotes),(<vp>is inrange0..
255).
<pid> GSM03.40TP-Protocol-Identifierinintegerformat(default 0).
<dcs> GSM03.38SMS DataCoding Scheme(default 0),orCell
BroadcastData Coding Schemeinintegerformat depending on
the commandorresultcode.
Examples
AT+CSMP=17,23,0,244
OK
AT+CSMP?
+CSMP:17,23,0,244
OK
AT+CSMP=?
OK
9.2.7 AT+CSDH Showtextmodeparameters
This commandis used tocontrolwhetherdetailed headerinformationis shown in textmoderesultcodes.
AT+CSDH Showtextmodeparameters
TestCommand
AT+CSDH=?
Response
+CSDH:(rangeofsupported <show>s)
OK
Read Command
AT+CSDH?
Response
+CSDH:<show>
OK
WriteCommand
AT+CSDH=<show>
Response
1)
OK
2)
ERROR
Execution Command
AT+CSDH
Setdefaultvalue (<show>=0):
1)


## Página 228

A76XX Series_AT Command Manual_V1.09
www.simcom.com 227 /652
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<show> 0 donotshowheadervalues definedin commandsAT+CSCA
andAT+CSMP(<sca>,<tosca>,<fo>,<vp>,<pid>and<dcs>)nor
<length>,<toda>or <tooa>in +CMT,AT+CMGL,AT+CMGR
resultcodes forSMS-DELIVERsand SMS-SUBMITsin text
mode;forSMS-COMMANDsinAT+CMGRresultcode,do not
show<pid>,<mn>,<da>,<toda>,<length>or<data>
1 showthe values inresultcodes
Examples
AT+CSDH=?
+CSDH:(0-1)
OK
AT+CSDH?
+CSDH:0
OK
AT+CSDH=1
OK
AT+CSDH
OK
9.2.8 AT+CNMA NewmessageacknowledgementtoME/TA
This commandis used toconfirmsuccessfulreceiptofanewmessage(SMS-DELIVERor
SMS-STATUSREPORT)routed directlytotheTE.IfME does not receiveacknowledgement within required
time(networktimeout),itwillsendRP-ERRORtothe network.
AT+CNMA NewmessageacknowledgementtoME/TA
TestCommand
AT+CNMA=?
Response
iftextmode(AT+CMGF=1):


## Página 229

A76XX Series_AT Command Manual_V1.09
www.simcom.com 228 /652
OK
ifPDUmode(AT+CMGF=0):
+CNMA:(range ofsupported <n>s)
OK
WriteCommand
AT+CNMA=<n>
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
Execution Command
AT+CNMA
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<n> Parameterrequired only forPDUmode.
0 Command operates similarly as executioncommand intext
mode.
1 Sendpositive(RP-ACK)acknowledgement tothe network.
Accepted only inPDUmode.
2 Sendnegative(RP-ERROR)acknowledgement tothe
network.Accepted only inPDUmode.
Examples
AT+CNMI=1,2,0,0,0
OK
+CMT:"1380022xxxx","","02/04/03,11:06:38+32"
Testing
AT+CNMA
OK
AT+CNMA
+CMSERROR:340
//receivenewshortmessage
//sendACKtothe network
//the second timereturn error,itneedsACK
only once


## Página 230

A76XX Series_AT Command Manual_V1.09
www.simcom.com 229 /652
The execute /write command shall only be used whenAT+CSMS parameter <service> equals 1 (=phase
2+)andappropriateURChas beenissued by themodule, i.e.:
<+CMT>for<mt>=2incoming message classes0,1, 3and none;
<+CMTI>for<mt>=3 incomingmessageclasses0;
<+CDS>for<ds>=1.
9.2.9 AT+CNMI NewmessageindicationstoTE
This commandis used toselectthe procedurehowreceiving ofnewmessages from the networkis
indicatedtotheTEwhenTEisactive,e.g.DTRsignalisON.IfTEisinactive(e.g.DTRsignalisOFF).Ifset
<mt>＝3or<ds>＝1, makesure<mode>＝1,Ifset<mt>=2,makesure<mode>=1or 2,otherwise itwill
return error.
AT+CNMI NewmessageindicationstoTE
TestCommand
AT+CNMI=?
Response
+CNMI:(listofsupported <mode>s),(listofsupported <mt>s),(list
ofsupported<bm>s),(listof supported<ds>s),(listofsupported
<bfr>s)
OK
Read Command
AT+CNMI?
Response
+CNMI:<mode>,<mt>,<bm>,<ds>,<bfr>
OK
WriteCommand
AT+CNMI=<mode>[,<mt>[,<bm>
[,<ds>[,<bfr>]]]]
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
Execution Command
AT+CNMI
Setdefaultvalue:
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
NOTE


## Página 231

A76XX Series_AT Command Manual_V1.09
www.simcom.com 230 /652
DefinedValues
<mode> 0 Bufferunsolicited resultcodes intheTA.IfTAresultcode
bufferis full,indications canbe buffered insomeotherplace or
the oldestindications may bediscarded andreplaced with the
newreceivedindications.
1 Discardindication and rejectnewreceivedmessage
unsolicitedresult codes whenTA-TElinkis reserved (e.g.in
on-linedata mode).Otherwise forwardthem directlyto theTE.
2 Bufferunsolicited resultcodes intheTAwhenTA-TElink is
reserved(e.g.inon-linedatamode)andflushthemtotheTEafter
reservation.Otherwiseforwardthem directly totheTE.
<mt> Therules forstoring received SMS dependon its datacoding
scheme,preferred memorystorage(AT+CPMS)setting andthis
value:
0 NoSMS-DELIVERindications are routedto theTE.
1 IfSMS-DELIVERis storedintoME/TA,indicationofthe
memorylocationisroutedtotheTEusingunsolicitedresultcode:
+CMTI:<mem3>,<index>.
2 SMS-DELIVERs (exceptclass 2messages andmessagesin
themessagewaitingindicationgroup(storemessage))arerouted
directlytotheTEusingunsolicited result code:
+CMT:[<alpha>],<length><CR><LF><pdu>(PDU mode
enabled);or
+CMT:
<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>
,<length>]<CR><LF><data>
(textmodeenabled,about parametersin italics,refercommand
ShowTextModeParametersAT+CSDH).
3 Class 3SMS-DELIVERsare routeddirectly toTEusing
unsolicitedresult codes defined in<mt>=2.Messages ofother
datacodingschemes resultinindication asdefined in<mt>=1.
<bm> Therules forstoring received CBMs depend onits data coding
scheme,the setting ofSelectCBMTypes(AT+CSCB)and this
value:
0 NoCBM indications areroutedto theTE.
2 NewCBMs are routeddirectlytotheTEusing unsolicited
resultcode:
+CBM: <length><CR><LF><pdu>(PDUmode enabled);or
+CBM: <sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data>
(textmodeenabled)
<ds> 0 NoSMS-STATUS-REPORTsareroutedto theTE.
1 SMS-STATUS-REPORTsare routedto theTEusing
unsolicitedresult code:


## Página 232

A76XX Series_AT Command Manual_V1.09
www.simcom.com 231 /652
+CDS:<length><CR><LF><pdu>(PDUmodeenabled); or
+CDS:<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>(textmode
enabled)
2 IfSMS-STATUS-REPORTis storedinto ME/TA,indication of
the memorylocation isroutedto theTEusing unsolicited result
code:+CDSI:<mem3>,<index>.
<bfr> 0 TAbufferofunsolicited resultcodesdefined withinthis
commandis flushedtotheTEwhen<mode> 1to 2isentered
(OKresponse shallbe givenbefore flushingthe codes).
1 TAbufferofunsolicited resultcodesdefined withinthis
commandis cleared when<mode> 1to2isentered.
Examples
AT+CNMI?
+CNMI:2,1,0,0,0
OK
AT+CNMI=?
+CNMI:(0,1,2),(0,1,2,3),(0,2),(0,1,2),(0,1)
OK
AT+CNMI=2,1(unsolicited resultcodesafter
receivedmessages.)
OK
AT+CNMI
OK
9.2.10 AT+CGSMS SelectserviceforMOSMSmessages
Thewritecommand isusedto specifythe service orservicepreference thatthe MTwilluseto sendMO
SMS messages.
Thetestcommandisusedforrequesting informationonwhichservicesandservicepreferencescanbeset
by usingtheAT+CGSMS writecommand
Thereadcommand returns the currently selected serviceorservice preference.
AT+CGSMS SelectserviceforMOSMSmessages
TestCommand
AT+CGSMS=?
Response
+CGSMS:(rangeofsupported <service>s)
OK


## Página 233

A76XX Series_AT Command Manual_V1.09
www.simcom.com 232 /652
Read Command
AT+CGSMS?
Response
+CGSMS:<service>
OK
WriteCommand
AT+CGSMS=<service>
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<service> Anumeric parameterwhichindicates the serviceorservice
preferencetobe used
0 GPRS(valueis notreallysupported andisinternally mapped
to2)
1 circuit switched(valueis notreally supportedandis internally
mappedto3)
2 GPRSpreferred(use circuitswitchedif GPRSnotavailable)
3 circuit switchedpreferred (useGPRSif circuitswitched not
available)
Examples
AT+CGSMS?
+CGSMS:3
OK
AT+CGSMS=?
+CGSMS:(0-3)
OK
AT+CGSMS=3
OK


## Página 234

A76XX Series_AT Command Manual_V1.09
www.simcom.com 233 /652
9.2.11 AT+CMGL ListSMSmessagesfrompreferredstore
This commandis used toreturn messages with status value<stat>from messagestorage <mem1>to the
TE.
Ifthe status ofthe messageis 'received unread',the status inthe storagechanges to'received read'.
AT+CMGL ListSMSmessagesfrompreferredstore
TestCommand
AT+CMGL=?
Response
+CMGL:(listofsupported <stat>s)
OK
WriteCommand
AT+CMGL=<stat>
Response
1)Iftext mode (AT+CMGF=1),command successfuland
SMS-SUBMITsand/or SMS-DELIVERs:
+CMGL:
<index>,<stat>,<oa>/<da>,[<alpha>],[<scts>][,<tooa>/<toda>,<f
o>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>[
+CMGL:
<index>,<stat>,<oa>/<da>,[<alpha>],[<scts>][,<tooa>/<toda>,<f
o>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>[..]]
OK
2)Iftext mode (AT+CMGF=1),command successfuland SMS-
STATUS-REPORTs:
+CMGL:
<index>,<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>[
+CMGL:
<index>,<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>[..]]
OK
3)Iftext mode (AT+CMGF=1),command successfuland SMS-
COMMANDs:
+CMGL:<index>,<stat>,<fo>,<ct>[
+CMGL:<index>,<stat>,<fo>,<ct>[..]]
OK
4)Iftext mode (AT+CMGF=1),command successfuland CBM
storage:
+CMGL:<index>,<stat>,<sn>,<mid>,<page>,<pages>
<data>[
+CMGL:<index>,<stat>,<sn>,<mid>,<page>,<pages>
<data>[..]]


## Página 235

A76XX Series_AT Command Manual_V1.09
www.simcom.com 234 /652
OK
5)IfPDUmode (AT+CMGF=0)andCommand successful:
+CMGL:<index>,<stat>,[<alpha>],<length>
<pdu>[
+CMGL:<index>,<stat>,[<alpha>],<length>
<pdu>
[…]]
OK
6)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<stat> 1.TextMode:
"RECUNREAD" received unreadmessage (i.e.newmessage)
"RECREAD" received readmessage
"STOUNSENT" storedunsentmessage
"STOSENT" storedsent message
"ALL" allmessages
2.PDUMode:
0 received unread message(i.e.newmessage)
1 received readmessage
2 storedunsentmessage
3 storedsentmessage
4 all messages
<index> Integertype;valueintherangeoflocationnumberssupportedby
the associatedmemory and startwith one.
<oa> Originating-Address,Address-Value fieldin string format; BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<tooa>.
<da> Destination-Address,Address-Value field instring format;BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<toda>.
<alpha> Stringtype alphanumeric representation of<da>or <oa>
corresponding totheentry found inMTphonebook;
implementationofthis feature ismanufacturerspecific;used
charactersetshouldbe theone selectedwith commandSelect
TECharacterSetAT+CSCS.


## Página 236

A76XX Series_AT Command Manual_V1.09
www.simcom.com 235 /652
<scts> TP-Service-Centre-Time-Stampintime-string format(refer<dt>).
<tooa> TP-Originating-Address,Type-of-Addressoctetin integerformat.
(defaultrefer<toda>).
<toda> TP-Destination-Address,Type-of-Addressoctetinintegerformat.
(whenfirstcharacterof <da>is+(IRA43)default is145,
otherwise defaultis 129).Therangeof valueisfrom 128to 255.
<length> Integertype valueindicating inthe textmode(AT+CMGF=1)the
lengthofthe messagebody <data>incharacters;orin PDU
mode(AT+CMGF=0), thelength ofthe actualTPdata unitin
octets.(i.e.theRPlayerSMSCaddressoctetsarenotcountedin
the length)
<data> Inthe caseofSMS:TP-User-Data intext mode responses;
format:
1.If<dcs>indicates thatGSM 7bitdefaultalphabetisusedand
<fo>indicates thatTP-User-Data-Header-Indication is not set:
a.IfTEcharacter setotherthan "HEX": ME/TAconverts GSM
alphabetintocurrentTEcharacterset.
b.IfTEcharacter setis "HEX": ME/TAconverts each7-bit
characterofGSM7 bitdefaultalphabetinto two IRAcharacter
longhexadecimalnumbers.(e.g.character(GSM 7bitdefault
alphabet23)is presentedas 17 (IRA49and 55))
2.If<dcs>indicates that8-bit orUCS2 datacoding schemeis
used,or<fo>indicates thatTP-User-Data-Header-Indicationis
set:ME/TAconverts each8-bit octetintotwo IRAcharacterlong
hexadecimalnumbers.(e.g. octet with integervalue 42is
presentedtoTEastwo characters 2A(IRA50 and65))
3.If<dcs>indicates thatGSM 7bitdefaultalphabetisused:
a.IfTEcharacter setotherthan "HEX": ME/TAconverts GSM
alphabetintocurrentTEcharacterset.
b.IfTEcharacter setis "HEX": ME/TAconverts each7-bit
characterofthe GSM 7bitdefaultalphabetinto two IRA
characterlong hexadecimalnumbers.
4.If<dcs>indicates that8-bit orUCS2 datacoding schemeis
used:ME/TAconverts each8-bit octetintotwo IRAcharacter
longhexadecimalnumbers.
<fo> Dependingon the commandorresult code:firstoctetof GSM
03.40SMS-DELIVER, SMS-SUBMIT(default17),
SMS-STATUS-REPORT,or SMS-COMMAND (default2)in
integerformat.SMS statusreportissupportedundertextmodeif
<fo>isset to49.
<mr> MessageReference
GSM03.40TP-Message-Referencein integerformat.
<ra> RecipientAddress
GSM03.40TP-Recipient-AddressAddress-Value field instring
format;BCDnumbers (orGSM defaultalphabetcharacters)are


## Página 237

A76XX Series_AT Command Manual_V1.09
www.simcom.com 236 /652
convertedto charactersof thecurrently selectedTEcharacter
set(refertocommandAT+CSCS);typeofaddressgivenby<tora>
<tora> Type ofRecipientAddress
GSM04.11TP-Recipient-AddressType-of-Addressoctetin
integerformat (defaultrefer<toda>)
<dt> DischargeTime
GSM03.40TP-Discharge-Timein time-string
format:"yy/MM/dd,hh:mm:ss+zz",wherecharacters indicate year
(two lastdigits),month,day,hour,minutes,secondsand timezone.
<st> Status
GSM03.40TP-Statusin integerformat
0…255
<ct> Status
GSM03.40TP-Statusin integerformat
0…255
<ct> CommandType
GSM03.40TP-Command-Type inintegerformat
0…255
<sn> SerialNumber
GSM03.41CBM SerialNumberinintegerformat
<mid> MessageIdentifier
GSM03.41CBM MessageIdentifierin integerformat
<page> PageParameter
GSM03.41CBM Page Parameterbits4-7 inintegerformat
<pages> PageParameter
GSM03.41CBM Page Parameterbits0-3 inintegerformat
<pdu> Inthe caseofSMS:SCaddress followed byTPDUin
hexadecimalformat: ME/TAconverts eachoctetofTPdata unit
intotwo IRAcharacterlonghexadecimalnumbers.(e.g. octet
withintegervalue 42is presentedtoTEas two characters 2A
(IRA50 and 65)).
Examples
AT+CMGL=?
+CMGL:("RECUNREAD","RECREAD","STOUNSENT","STOSENT","ALL")
OK
AT+CMGL="ALL"
+CMGL:1,"STOUNSENT","+10011",,,145,4
HelloWorld
OK


## Página 238

A76XX Series_AT Command Manual_V1.09
www.simcom.com 237 /652
9.2.12 AT+CMGR Readmessage
This commandis used toreturn messagewith location value <index>from messagestorage<mem1>to
theTE.
AT+CMGR Readmessage
TestCommand
AT+CMGR=?
Response
OK
WriteCommand
AT+CMGR=<index>
Response
1)Iftext mode (AT+CMGF=1),command successfuland SMS-
DELIVER:
+CMGR:
<stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,
<tosca>,<length>]
<data>
OK
2)Iftext mode (AT+CMGF=1),command successfuland SMS-
SUBMIT:
+CMGR:
<stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>],<sca>,<
tosca>,<length>]
<data>
OK
3)Iftext mode (AT+CMGF=1),command successfuland SMS-
STATUS-REPORT:
+CMGR:<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>
OK
Iftextmode (AT+CMGF=1),commandsuccessfulandSMS-
COMMAND:
+CMGR:
<stat>,<fo>,<ct>[,<pid>,[<mn>],[<da>],[<toda>],<length>]<CR>
<LF><data>
OK
4)Iftext mode (AT+CMGF=1),command successfuland CBM
storage:
+CMGR:
<stat>,<sn>,<mid>,<dcs>,<page>,<pages><CR><LF><data>


## Página 239

A76XX Series_AT Command Manual_V1.09
www.simcom.com 238 /652
OK
5)IfPDUmode (AT+CMGF=0)andCommand successful:
+CMGR:<stat>,[<alpha>],<length><CR><LF><pdu>
OK
6)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<index> Integertype;value inthe rangeof location numberssupported by
the associatedmemory and startwith one.
<stat> 1.TextMode:
"RECUNREAD" received unreadmessage (i.e.newmessage)
"RECREAD" received readmessage
"STOUNSENT" storedunsentmessage
"STOSENT" storedsent message
2.PDUMode:
0 received unread message(i.e.newmessage)
1 received readmessage
2 storedunsentmessage
3 storedsentmessage
<oa> Originating-Address,Address-Value fieldin string format; BCD
numbers(or GSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<tooa>.
<alpha> Stringtype alphanumeric representation of<da>or <oa>
corresponding totheentry found inMTphonebook;
implementationofthis feature ismanufacturerspecific;used
charactersetshouldbe theone selectedwith commandSelectTE
CharacterSetAT+CSCS.
<scts> TP-Service-Centre-Time-Stampintime-string format(refer<dt>).
<tooa> TP-Originating-Address,Type-of-Addressoctetin integerformat.
(defaultrefer<toda>).
<fo> Dependingon the commandorresult code:firstoctetof GSM
03.40SMS-DELIVER, SMS-SUBMIT(default17),
SMS-STATUS-REPORT,or SMS-COMMAND (default2)ininteger
format.SMS status reportissupported undertextmode if<fo>is
setto49.
<pid> ProtocolIdentifier
GSM03.40TP-Protocol-Identifierinintegerformat


## Página 240

A76XX Series_AT Command Manual_V1.09
www.simcom.com 239 /652
0…255
<dcs> Dependingon the commandorresult code:SMS DataCoding
Scheme(default0),orCell BroadcastData Coding Schemein
integerformat.
<sca> RPSCaddressAddress-Valuefield instringformat;BCDnumbers
(orGSM7bitdefaultalphabetcharacters)are convertedto
charactersof the currently selectedTEcharacterset, type of
addressgiven by<tosca>.
<tosca> RPSCaddressType-of-Addressoctetinintegerformat (default
refer<toda>).
<length> Integertype valueindicating inthe textmode(AT+CMGF=1)the
lengthofthe messagebody <data>incharacters;orin PDUmode
(AT+CMGF=0),the length ofthe actualTPdata unitinoctets.(i.e.
the RPlayer SMSCaddressoctets are notcountedin the length)
<data> InthecaseofSMS:TP-User-Dataintextmoderesponses;format:
1.If<dcs>indicates thatGSM 7bitdefaultalphabetisusedand
<fo>indicates thatTP-User-Data-Header-Indication is not set:
a.IfTEcharacter setotherthan "HEX": ME/TAconverts GSM
alphabetintocurrentTEcharacterset.
b.IfTEcharacter setis "HEX": ME/TAconverts each7-bit
characterofGSM7bitdefaultalphabetintotwoIRAcharacterlong
hexadecimalnumbers.(e.g. character(GSM7 bitdefaultalphabet
23)is presentedas 17(IRA49and 55))
2.If<dcs>indicates that8-bit orUCS2 datacoding schemeis
used,or<fo>indicates thatTP-User-Data-Header-Indicationis
set:ME/TAconverts each8-bit octetintotwo IRAcharacterlong
hexadecimalnumbers.(e.g. octet with integervalue 42is
presentedtoTEastwo characters 2A(IRA50 and65))
3.If<dcs>indicates thatGSM 7bitdefaultalphabetisused:
a.IfTEcharacter setotherthan "HEX": ME/TAconverts GSM
alphabetintocurrentTEcharacterset.
b.IfTEcharacter setis "HEX": ME/TAconverts each7-bit
characterofthe GSM 7bitdefaultalphabetinto two IRAcharacter
longhexadecimalnumbers.
4.If<dcs>indicates that8-bit orUCS2 datacoding schemeis
used:ME/TAconverts each8-bit octetintotwo IRAcharacterlong
hexadecimalnumbers.
<da> Destination-Address,Address-Value field instring format;BCD
numbers(or GSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<toda>.
<toda> TP-Destination-Address,Type-of-Addressoctetinintegerformat.
(whenfirstcharacterof <da>is+(IRA43)default is145, otherwise
defaultis 129).Therangeof valueis from128to 255.
<vp> Dependingon SMS-SUBMIT<fo>setting:TP-Validity-Period


## Página 241

A76XX Series_AT Command Manual_V1.09
www.simcom.com 240 /652
eitherinintegerformat(default 167)orintime-string format(refer
<dt>).
<mr> MessageReference
GSM03.40TP-Message-Referencein integerformat.
<ra> RecipientAddress
GSM03.40TP-Recipient-AddressAddress-Value field instring
format;BCDnumbers (orGSM defaultalphabetcharacters)are
convertedto charactersof thecurrently selectedTEcharacter
set(refertocommandAT+CSCS);type of addressgiven by<tora>
<tora> Type ofRecipientAddress
GSM04.11TP-Recipient-AddressType-of-Addressoctetininteger
format(defaultrefer<toda>)
<dt> DischargeTime
GSM03.40TP-Discharge-Timein time-string
format:"yy/MM/dd,hh:mm:ss+zz",wherecharacters indicate year
(two lastdigits),month,day,hour,minutes,secondsand timezone.
<st> Status
GSM03.40TP-Statusin integerformat
0…255
<ct> CommandType
GSM03.40TP-Command-Type inintegerformat
0…255
<mn> MessageNumber
GSM03.40TP-Message-Numberinintegerformat
<sn> SerialNumber
GSM03.41CBM SerialNumberinintegerformat
<mid> MessageIdentifier
GSM03.41CBM MessageIdentifierin integerformat
<page> PageParameter
GSM03.41CBM Page Parameterbits4-7 inintegerformat
<pages> Pageparameter
GSM03.41CBM Page Parameterbits0-3 inintegerformat
<pdu> InthecaseofSMS:SCaddressfollowed byTPDUinhexadecimal
format:ME/TAconverts each octetofTPdataunit intotwo IRA
characterlong hexadecimalnumbers.(eg.octetwith integervalue
42is presentedtoTEas two characters 2A(IRA50and 65)).
Examples
AT+CMGR=?
OK
AT+CMGR=1
+CMGR:"STOUNSENT","+10011",,145,17,0,0,167,"+8613800100500",145,11
HelloWorld


## Página 242

A76XX Series_AT Command Manual_V1.09
www.simcom.com 241 /652
OK
9.2.13 AT+CMGS Sendmessage
This commandis used tosendmessagefrom aTEtothe network (SMS-SUBMIT).
AT+CMGS Sendmessage
TestCommand
AT+CMGS=?
Response
OK
WriteCommand
Iftextmode(AT+CMGF=1)
AT+CMGS=<da>[,<toda>]
Textisentered.
<CTRL-Z/ESC>
IfPDUmode(AT+CMGF=0)
AT+CMGS=<length>
PDUis entered
<CTRL-Z/ESC>
Response
1)Ifsending successfully:
+CMGS:<mr>
OK
2)Ifcancel sending:
OK
3)Ifsending fails
ERROR
4)Ifsending fails:
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 40s
Reference 3GPPTS27.005
DefinedValues
<da> Destination-Address,Address-Value field instring format;BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<toda>.
<toda> TP-Destination-Address,Type-of-Addressoctetinintegerformat.
(whenfirstcharacterof <da>is+(IRA43)default is145,
otherwise defaultis 129).Therangeof valueisfrom 128to 255.
<length> integertype value indicating inthe textmode (AT+CMGF=1)the
lengthofthe messagebody <data>>(or<cdata>)in characters;
orinPDUmode(AT+CMGF=0),the length ofthe actualTPdata
unitinoctets.(i.e. the RPlayer SMSCaddressoctets are not
countedin the length)
<mr> MessageReference
GSM03.40TP-Message-Referencein integerformat.


## Página 243

A76XX Series_AT Command Manual_V1.09
www.simcom.com 242 /652
Examples
AT+CMGS=?
OK
AT+CMGS="13012832788"
>ABCD<ctrl-Z/ESC>
+CMGS:46
OK
//TEXTMODE
Intextmode,the maximumlengthofanSMSdepends ontheusedcodingscheme:Itis160charactersif
the 7bitGSMcoding schemeis used.
9.2.14 AT+CMSS Sendmessagefromstorage
This commandis used tosendmessagewith location value <index>from preferredmessage storage
<mem2>tothe network (SMS-SUBMITorSMS-COMMAND).
AT+CMSS Sendmessagefromstorage
TestCommand
AT+CMSS=?
Response
OK
WriteCommand
AT+CMSS=<index>[,<da>[,<tod
a>]]
Response
1)
+CMSS:<mr>
OK
2)
ERROR
3)Ifsending fails:
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
NOTE


## Página 244

A76XX Series_AT Command Manual_V1.09
www.simcom.com 243 /652
<index> Integertype;valueintherangeoflocationnumberssupportedby
the associatedmemory and startwith one.
<da> Destination-Address,Address-Value field instring format;BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<toda>.
<mr> MessageReference
GSM03.40TP-Message-Referencein integerformat.
<toda> TP-Destination-Address,Type-of-Addressoctetinintegerformat.
(whenfirstcharacterof <da>is+(IRA43)default is145,
otherwise defaultis 129).Therangeof valueisfrom 128to 255.
Examples
AT+CMSS=?
OK
AT+CMSS=3
+CMSS:0
OK
AT+CMSS=3,"13012345678"
+CMSS:55
OK
Intextmode,the maximumlengthofanSMSdepends ontheusedcodingscheme:Itis160charactersif
the 7bitGSMcoding schemeis used.
9.2.15 AT+CMGW Writemessagetomemory
This commandis used tostore message(eitherSMS-DELIVER orSMS-SUBMIT)to memorystorage
<mem2>.
AT+CMGW Writemessagetomemory
TestCommand
AT+CMGW=?
Response
OK
NOTENOTE


## Página 245

A76XX Series_AT Command Manual_V1.09
www.simcom.com 244 /652
WriteCommand
Iftextmode(AT+CMGF=1)
AT+CMGW=<oa>/<da>[,<tooa>/
<toda>[,<stat>]]
Textisentered.
<CTRL-Z/ESC>
IfPDUmode(AT+CMGF=0):
AT+CMGW=<length>[,<stat>]
PDUis entered.
<CTRL-Z/ESC>
Response
1)Ifwritesuccessfully:
+CMGW:<index>
OK
2)Ifwritefails:
ERROR
3)Ifwritefails:
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 40s
Reference 3GPPTS27.005
DefinedValues
<index> Integertype;valueintherangeoflocationnumberssupportedby
the associatedmemory and startwith one.
<oa> Originating-Address,Address-Value fieldin string format; BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<tooa>.
<tooa> TP-Originating-Address,Type-of-Addressoctetin integerformat.
(defaultrefer<toda>).
<da> Destination-Address,Address-Value field instring format;BCD
numbers(orGSM7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgiven by<toda>.
<toda> TP-Destination-Address,Type-of-Addressoctetinintegerformat.
(whenfirstcharacterof <da>is+(IRA43)default is145,
otherwise defaultis 129).Therangeof valueisfrom 128to 255.
<length> Integertype valueindicating inthe textmode(AT+CMGF=1)the
lengthofthe messagebody <data>>(or<cdata>)in characters;
orinPDUmode(AT+CMGF=0),the length ofthe actualTPdata
unitinoctets.(i.e. the RPlayer SMSCaddressoctets are not
countedin the length).
<stat> 1 TextMode:
"STOUNSENT" storedunsentmessage
"STOSENT" storedsent message
2 PDUMode:
2 storedunsentmessage
3 storedsentmessage


## Página 246

A76XX Series_AT Command Manual_V1.09
www.simcom.com 245 /652
Examples
AT+CMGW=?
OK
AT+CMGW="13012832788"
>ABCD<ctrl-Z/ESC>
+CMGW:1
OK
//TEXTMODE
Intextmode,the maximumlengthofanSMSdepends ontheusedcodingscheme:Itis160charactersif
the 7bitGSMcoding schemeis used.
9.2.16 AT+CMGD Deletemessage
This commandis used todelete messagefrompreferred messagestorage<mem1>location <index>.If
<delflag>is presentandnotsetto 0thenthe ME shallignore <index>andfollowthe rulesfor<delflag>
shown below.
AT+CMGD Deletemessage
TestCommand
AT+CMGD=?
Response
+CMGD:(listofsupported <index>s)[,(listofsupported <delflag>s
)]
OK
WriteCommand
AT+CMGD=<index>[,<delflag>]
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
NOTE


## Página 247

A76XX Series_AT Command Manual_V1.09
www.simcom.com 246 /652
<index> Integertype;valueintherangeoflocationnumberssupportedby
the associatedmemory and startwith one.
<delflag> 0 (oromitted)Deletethe message specifiedin <index>.
1 Delete allreadmessagesfrom preferredmessage
storage, leavingunread messagesandstored mobile
originated messages (whethersentornot)untouched.
2 Delete allreadmessagesfrom preferredmessage
storageand sentmobileoriginated messages,
leaving unread messages and unsentmobile
originated messages untouched.
3 Delete allreadmessagesfrom preferredmessage
storage,sentandunsentmobileoriginatedmessages
leaving unread messages untouched.
4 Delete allmessages frompreferredmessage storage
includingunread messages.
Examples
AT+CMGD=?
+CMGD:(1),(0-4)
OK
AT+CMGD=1
OK
Ifset<delflag>=1,2, 3or4,<index>isomitted, suchasAT+CMGD=,1.
9.2.17 AT+CMGMT Changemessagestatus
Thiscommand isused tochangethemessage status.Ifthe statusis unread,itwillbechangedread.Other
statusesdon’tchange.
AT+CMGMT Changemessagestatus
TestCommand
AT+CMGMT=?
Response
OK
NOTE


## Página 248

A76XX Series_AT Command Manual_V1.09
www.simcom.com 247 /652
WriteCommand
AT+CMGMT=<index>
Response
1)
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<index> Integertype;value inthe rangeoflocation numbers supported bythe
associated memoryandstart with one.
Examples
AT+CMGMT=?
OK
AT+CMGMT=1
OK
9.2.18 AT+CMVP Setmessagevalidperiod
This commandis used tosetvalidperiod forsending shortmessage.
AT+CMVP Setmessagevalidperiod
TestCommand
AT+CMVP=?
Response
+CMVP:(listof supported<vp>s)
OK
Read Command
AT+CMVP?
Response
+CMVP:<vp>
OK
WriteCommand
AT+CMVP=<vp>
Response
1)


## Página 249

A76XX Series_AT Command Manual_V1.09
www.simcom.com 248 /652
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference 3GPPTS27.005
DefinedValues
<vp> Validityperiod value:
0to143 (<vp>+1)x 5minutes (up to12hours)
144to167 12hours +(<vp>-143)x30 minutes
168to196 (<vp>-166)x 1day
197to255 (<vp>-192)x 1week
Examples
AT+CMVP=?
+CMVP:(0-255)
OK
AT+CMVP=167
OK
AT+CMVP?
+CMVP:167
OK
9.2.19 AT+CMGRD Readanddeletemessage
This commandis used toread message,anddelete the message atthe sametime.ItintegrateAT+CMGR
andAT+CMGD,butitdoesn’tchange the messagestatus.
AT+CMGRD Readanddeletemessage
TestCommand
AT+CMGRD=?
Response
OK


## Página 250

A76XX Series_AT Command Manual_V1.09
www.simcom.com 249 /652
WriteCommand
AT+CMGRD=<index>
Response
1)Iftext mode(AT+CMGF=1),commandsuccessfuland
SMS-DE-LIVER:
+CMGRD:
<stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,
<tosca>,<length>]
<data>
OK
2)Iftext mode(AT+CMGF=1),commandsuccessfulandSMS-SU-
BMIT:
+CMGRD:
<stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>],<sca>,<
tosca>,<length>]
<data>
OK
3)Iftext mode(AT+CMGF=1),commandsuccessfulandSMS-STA-
TUS-REPORT:
+CMGRD:<stat>,<fo>,<mr>,[<ra>],[<tora>],<scts>,<dt>,<st>
OK
4)Iftext mode(AT+CMGF=1),commandsuccessfuland
SMS-CO-MMAND:
+CMGRD:
<stat>,<fo>,<ct>[,<pid>,[<mn>],[<da>],[<toda>],<length>
<data>]
OK
5)Iftext mode(AT+CMGF=1),commandsuccessfulandCBM sto-
rage:
+CMGRD:<stat>,<sn>,<mid>,<dcs>,<page>,<pages>
<data>
OK
6)IfPDUmode(AT+CMGF=0)and commandsuccessful:
+CMGRD:<stat>,[<alpha>],<length>
<pdu>
OK
7)
ERROR
8)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE


## Página 251

A76XX Series_AT Command Manual_V1.09
www.simcom.com 250 /652
Max ResponseTime 40s
Reference 3GPPTS27.005
DefinedValues
Referto commandAT+CMGR.
Examples
AT+CMGRD=?
OK
AT+CMGRD=6
+CMGRD:"REC
READ","+8613917787249",,"06/07/10,12:09:
38+32",145,4,0,0,"+8613800210500",145,4
Howdoyoudo
OK
9.2.20 AT+CMGSEX Sendmessage
This commandis used tosendmessagefrom aTEtothe network (SMS-SUBMIT).
AT+CMGSEX Sendmessage
TestCommand
AT+CMGSEX=?
Response
OK
WriteCommand
Iftextmode(AT+CMGF=1):
AT+CMGSEX=<da>[,<toda>][,<
mr>,<msg_seg>,<msg_total>]
Textisentered.
<CTRL-Z/ESC>
Response
1)
+CMGSEX:<mr>
OK
2)
ERROR
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 40s


## Página 252

A76XX Series_AT Command Manual_V1.09
www.simcom.com 251 /652
Reference 3GPPTS27.005
DefinedValues
<da> Destination-Address,Address-Value field instringformat; BCD
numbers(orGSM 7bitdefaultalphabetcharacters)areconverted
tocharacters ofthe currently selectedTEcharacterset, type of
addressgivenby <toda>.
<toda> TP-Destination-Address,Type-of-Address octetin integerformat.
(Whenfirstcharacterof<da>is+(IRA43)default is145,
otherwisedefaultis 129).Therangeofvalue isfrom 128to255.
<mr> MessageReferenceGSM03.40TP-Message-Referencein
integerformat.Therange ofvalueis from 1to255.
<msg_seg> Thesegmentnumberforlong sms
<msg_total> Thesegmentnumberforlong sms,max value is15.
Examples
AT+CMGSEX=?
OK
AT+CMGSEX="13012832788",190,1,2
//TEXTMODE
>ABCD<ctrl-Z/ESC>
OK
AT+CMGSEX="13012832788",190,2,2
>EFGH<ctrl-Z/ESC>
+CMGSEX:190
OK
//TEXTMODE
Intext mode,the maximumlength ofan SMS depends on the used codingscheme:Forsingle SMS, itis
160charactersifthe7bitGSMcodingschemeisused;Formultiplelongsms,itis153charactersifthe7
bitGSMcodingschemeis used.ForUCS2shortmessages,themaximum length ofasingle messageis
268characters.Themaximumlength ofashortorlong messageis 808characters.
9.2.21 AT+CMSSEX Sendmultimessagesfromstorage
NOTE


## Página 253

A76XX Series_AT Command Manual_V1.09
www.simcom.com 252 /652
This commandis used tosendmessageswith location value <index1>,<index2>,<index3>…from
preferredmessage storage <mem2>to the network (SMS-SUBMITor SMS-COMMAND).Themaxcount of
index is13 onetime.
AT+CMSSEX Sendmultimessagesfromstorage
TestCommand
AT+CMSSEX=?
Response
OK
WriteCommand
AT+CMSSEX=<index>[,<index>[
,…]]
Response
1)
[+CMSSEX:<mr>[,<mr>[,…]]]
OK
2)
ERROR
3)Ifsending fails:
[+CMSSEX:<mr>[,<mr>[,…]]]
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 40s
Reference 3GPPTS27.005
DefinedValues
<index> Integertype;valueintherangeoflocationnumberssupportedby
the associatedmemory and startwith one.
<mr> MessageReference
Examples
AT+CMSSEX=?
OK
AT+CMSSEX=1,2
+CMSSEX:239,240
OK
AT+CMSSEX=1,2
+CMSSEX:241
+CMSERROR:Invalid memory index
NOTE


## Página 254

A76XX Series_AT Command Manual_V1.09
www.simcom.com 253 /652
Intext mode,the maximumlength ofan SMS depends on the used codingscheme:Forsingle SMS, itis
160characters ifthe 7bitGSM coding schemeis used.


## Página 255

A76XX Series_AT Command Manual_V1.09
www.simcom.com 254 /652
9.2.22 AT+CCONCINDEXReportConcatenatedSMSIndex
This commandis used tolistthe SMS index numbers ofreceived shortand longmessages.
AT+CCONCINDEXReportConcatenatedSMSIndex
TestCommand
AT+CCONCINDEX=?
Response
+CCONCINDEX:(0-10),(1-50),...
OK
Execution Command
AT+CCONCINDEX
Response
1)
+CCONCINDEX:N,i,j,k,...
OK
2)
OK
3)
+CMSERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime 12s
Reference 3GPPTS27.005
DefinedValues
<N> Nis the numberof segmentsthat formthe whole concatenated
SMS.
<i,j,k> i,j,kare the SMS indexes ofeachSMS segment
Examples
AT+CCONCINDEX=?
+CCONCINDEX:(0-10),(1-50),...
OK
AT+CCONCINDEX
+CCONCINDEX:3,1,2,3
+CCONCINDEX:2,45
OK
Ifnosegmentis received, Nis 0.
NOTE


## Página 256

A76XX Series_AT Command Manual_V1.09
www.simcom.com 255 /652
There isno long messageon the SIM cardorME,only the OKresultcode is returned.
ATCommandsforSerialInterface
10.1 OverviewofATCommandsforSerialInterface
Command Description
AT&D SetDTRfunction mode
AT&C SetDCDfunctionmode
AT+IPR Setlocalbaud ratetemporarily
AT+IPREX Setlocalbaud ratepermanently
AT+ICF Setcontrolcharacterframing
AT+IFC Setlocaldata flowcontrol
AT+CSCLK Control UARTSleep
AT+CMUX Enable the multiplexeroverthe UART
AT+CATR Configure URCdestinationinterface
AT+CFGRI Configure RIpin
AT+CURCD Configure the delaytimeandnumberof URC
10.2 DetailedDescriptionofATCommandsforSerialInterface
10.2.1 AT&D SetDTRfunctionmode
This commanddetermineshowtheTAresponds whenDTRPIN ischangedfromthe ONtothe OFF
conditionduring data mode.
AT&D SetDTRfunctionmode
Execution Command
AT&D[<value>]
Response
1)
OK
2)


## Página 257

A76XX Series_AT Command Manual_V1.09
www.simcom.com 256 /652
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<value> 0 TAignores status onDTR.
1 ON->OFF on DTR:Change toCommand modewith remaining the
connected call.
2 ON->OFF on DTR:Disconnectcall,change toCommand
mode.Duringstate DTR=OFFisauto-answer off.
Examples
AT&D1
OK
10.2.2 AT&C SetDCDfunctionmode
ThiscommanddetermineshowthestateofDCDPINrelatestothedetectionofreceivedlinesignalfromthe
distantend.
AT&C SetDCDfunctionmode
Execution Command
AT&C[<value>]
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<value> 0 DCDline shallalways beon.
1 DCDline shallbe ononly when datacarriersignalis present.
2 Setting the DCDlinebe on just1second afterthe data callsend.


## Página 258

A76XX Series_AT Command Manual_V1.09
www.simcom.com 257 /652
Examples
AT&C1
OK
10.2.3 AT+IPR Setlocalbaudratetemporarily
Thiscommandsetsthebaudrateofmodule’sserialinterfacetemporarily,afterrebootthebaudrateissetto
value ofIPREX.
AT+IPR Setlocalbaudratetemporarily
TestCommand
AT+IPR=?
Response
+IPR:(listof supported<speed>s)
OK
Read Command
AT+IPR?
Response
+IPR:<speed>
OK
WriteCommand
AT+IPR=<speed>
Response
1)
OK
2)
ERROR
Execution Command
AT+IPR
Response
Setthe value tobootvalue:
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<speed> Baud rate persecond:
300, 600, 1200,2400,4800,9600, 19200, 38400,57600,115200,
230400, 460800,921600, 1842000,3686400.
Examples


## Página 259

A76XX Series_AT Command Manual_V1.09
www.simcom.com 258 /652
AT+IPR?
+IPR:115200
OK
AT+IPR=?
+IPR:
(300,600,1200,2400,4800,9600,19200,38400,57600,115200,230400,460800,921600,1842000,3686400
)
OK
AT+IPR=115200
OK
10.2.4 AT+IPREX Setlocalbaudratepermanently
Thiscommandsetsthebaudrateofmodule’sserialinterfacepermanently,afterrebootthebaudrateisalso
valid.
AT+IPREX Setlocalbaudratepermanently
TestCommand
AT+IPREX=?
Response
+IPREX:(listofsupported <speed>s)
OK
Read Command
AT+IPREX?
Response
+IPREX:<speed>
OK
WriteCommand
AT+IPREX=<speed>
Response
1)
OK
2)
ERROR
Execution Command
AT+IPREX
Response
Setdefaultvalue 115200:
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues


## Página 260

A76XX Series_AT Command Manual_V1.09
www.simcom.com 259 /652
<speed> Baud rate persecond:
300, 600, 1200,2400,4800,9600, 19200, 38400,57600,115200,
230400, 460800,921600, 1842000,3686400.
Examples
AT+IPREX?
+IPREX:115200
OK
AT+IPREX=?
+IPREX:(300,600,1200,2400,4800,9600,19200,38400,57600,115200,230400,460800,921600,
1842000,3686400)
OK
AT+IPREX=115200
OK
10.2.5 AT+ICF Setcontrolcharacterframing
This commandsets characterframingwhichcontainsdata bit,stop bitand parity bit.
AT+ICF Setcontrolcharacterframing
TestCommand
AT+ICF=?
Response
+ICF:(list ofsupported<format>s),(listofsupported<parity>s)
OK
Read Command
AT+ICF?
Response
+ICF:<format>,<parity>
OK
WriteCommand
AT+ICF=<format>,<parity>
Response
1)
OK
2)
ERROR
Execution Command
AT+ICF
Response
Setdefaultvalue:
OK
ParameterSaving Mode NO_SAVE


## Página 261

A76XX Series_AT Command Manual_V1.09
www.simcom.com 260 /652
Max ResponseTime 9000ms
Reference -
DefinedValues
<format> 1 data bit8,paritybit1,stop bit 1.
2 data bit8,stop bit1.
3 data bit7,paritybit1,stop bit 1.
4 data bit7,stop bit1.
<parity> 0 Odd
1 Even
2 none
Examples
AT+ICF?
+ICF:2,2
OK
AT+ICF=?
+ICF:(1-4),(0-2)
OK
AT+ICF=2,2
OK
AT+ICF
OK
10.2.6 AT+IFC Setlocaldataflowcontrol
Thecommand sets theflowcontrolmodeof the module.
AT+IFC Setlocaldataflowcontrol
TestCommand
AT+IFC=?
Response
+IFC:(list ofsupported<DCE>s),(listofsupported<DTE>s)
OK
Read Command
AT+IFC?
Response
+IFC:<DCE>,<DTE>


## Página 262

A76XX Series_AT Command Manual_V1.09
www.simcom.com 261 /652
OK
WriteCommand
AT+IFC=<DCE>[,<DTE>]
Response
1)
OK
2)
ERROR
Execution Command
AT+IFC
Response
Setdefaultvalue:
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<DCE> 0 none
1 softwareRTSXON/XOFF
2 RTShardwareflowcontrol
<DTE> 0 none
1 softwareCTSXON/XOFF
2 CTShardwareflowcontrol
Thehardwareflowcontrolis notsupported byA7600X(X)-MNSEandA7600E-MASE.
Examples
AT+IFC?
+ICF:0,0
OK
AT+IFC=?
+IFC:(0-2),(0-2)
OK
AT+IFC=2,2
OK
AT+IFC
OK
NOTE


## Página 263

A76XX Series_AT Command Manual_V1.09
www.simcom.com 262 /652
10.2.7 AT+CSCLK ControlUARTSleep
This commandis used toenableUARTSleep oralways work. Ifsetto0,UARTalways work.Ifsetto1,
ensure thatDTRis pulled high and the modulecango toDTRsleep.Ifsetto 2,the modulewillenterRX
sleep.RXwakeupdirectly sends data throughthe serialport(forexample:AT)towake up.
AT+CSCLK ControlUARTSleep
TestCommand
AT+CSCLK=?
Response
+CSCLK:(range ofsupported<status>s)
OK
Read Command
AT+CSCLK?
Response
+CSCLK:<status>
OK
WriteCommand
AT+CSCLK=<status>
Response
1)
OK
2)
ERROR
Execution Command
AT+CSCLK
Response
Set<status>=0:
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<status> 0 off
1 DTRsleep
2 RXsleep
TheRXsleepis notsupported by1802S.
NOTE


## Página 264

A76XX Series_AT Command Manual_V1.09
www.simcom.com 263 /652
Examples
AT+CSCLK?
+CSCLK:0
OK
AT+CSCLK=?
+CSCLK:(0-2)
OK
AT+CSCLK=1
OK
AT+CSCLK=2
OK
AT+CSCLK
OK
10.2.8 AT+CMUX EnablethemultiplexerovertheUART
ThiscommandisusedtoenablethemultiplexerovertheUART,afterenabledfourvirtualportscanbeused
asATcommand portorMODEMport, the physicalUARTcannolongertransferdata directlyunderthis
case. Bydefaultallofthe fourvirtual ports areused asATcommandport. Secondserial portis notsupport
this command.
AT+CMUX EnablethemultiplexerovertheUART
TestCommand
AT+CMUX=?
Response
+CMUX:(0),(0),(1-8),(1-1500),(0),(0),(2-1000)
OK
Read Command
AT+CMUX?
Response
+CMUX:<value>,<subset>,<port_speed>,<N1>,<T1>,<N2>,<T2>
OK
WriteCommand
AT+CMUX=<value>[,<subset
>[,<port_speed>[,<N1>[,<T1
>[,<N2>[,<T2>]]]]]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -


## Página 265

A76XX Series_AT Command Manual_V1.09
www.simcom.com 264 /652
DefinedValues
<value> 0 currently only 0is supported(basic operation mode).
<subset> Currently omitted
<port_speed> Currently omitted,you can setspeed beforeenablemultiplexer
<N1> 1-1500
<T1> Currently omitted
<N2> Currently omitted
<T2> 2-1000
Examples
AT+CMUX?
+CMUX:0,0,5,1500,0,0,600
OK
AT+CMUX=?
+CMUX:(0),(0),(1-8),(1-1500),(0),(0),(2-1000)
OK
AT+CMUX=0
OK
10.2.9 AT+CATR ConfigureURCdestinationinterface
This commandis used toconfigurethe serial portwhichwillbe usedtooutput URCs.Werecommend
configureadestination portforreceivingURCinthe systeminitialization phase,in particular,in the case
thattransmitting large amounts ofdata,e.g.useTCP/UDPandMTSMS relatedATcommand.
AT+CATR ConfigureURCdestinationinterface
TestCommand
AT+CATR=?
Response
+CATR:(list ofsupported<port>s)
OK
Read Command
AT+CATR?
Response
+CATR:<port>
OK
WriteCommand Response


## Página 266

A76XX Series_AT Command Manual_V1.09
www.simcom.com 265 /652
AT+CATR=<port> 1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<port> 0 allports
1 use UARTporttooutput URCs
2 use MODEMport tooutputURCs
3 useATCOMporttooutput URCs
4 use cmuxvirtual port1 tooutputURCs
5 use cmuxvirtual port2 tooutputURCs
6 use cmuxvirtual port3 tooutputURCs
7 use cmuxvirtual port4 tooutputURCs
Examples
AT+CATR?
+CATR:0
OK
AT+CATR=?
+CATR:(0-7)
OK
AT+CATR=1
OK
10.2.10AT+CFGRI ConfigureRIpin
This commandconfiguresthe timeofpullingRIdown.These placesare goingto useit,forExamples:
SMS,FTP,NETWORK,PB,CM, OSand soon.
AT+CFGRI ConfigureRIpin
TestCommand
AT+CFGRI=?
Response
+CFGRI:(listofsupported<status>),(listof


## Página 267

A76XX Series_AT Command Manual_V1.09
www.simcom.com 266 /652
supported<URC_time>ms),(listofsupported<SMS_time>ms)
OK
Read Command
AT+CFGRI?
Response
+CFGRI:<status>,<URC_time>,<SMS_time>
OK
WriteCommand
AT+CFGRI=<status>[,<
URC_time>[,<SMS_time >]]
Response
1)
OK
2)
ERROR
Execution Command
AT+CFGRI
Response
Setdefaultvalue:
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<status> 0 off.
1 on.
<URC_time> anumeric parameterwhichis numberofmilliseconds to assertRI
delay toresetRI.The range is10to6000,defaultvalue is60ms.
<SMS_time> anumeric parameterwhichis numberofmilliseconds to assertRI
delay toresetRI.The range is20 to6000,defaultvalue is120ms.
Examples
AT+CFGRI?
+CFGRI:0,60,120
OK
AT+CFGRI=?
+CFGRI:(0-1),(10-6000),(20-6000)
OK
AT+CFGRI=0,60,120
OK
AT+CFGRI
OK


## Página 268

A76XX Series_AT Command Manual_V1.09
www.simcom.com 267 /652
10.2.11AT+CURCD ConfigurethedelaytimeandnumberofURC
ThiscommandisusedtoconfiguredelaytimewhenoutputURCandthenumberofcachedURCs.Youcan
controldelay timeifsomeURCsupports delay output.Youcan alsosetsize tostore URCs,they will output
togetherwhen the delaytimeends. ForExamples,if you setdelay timeto10msandsetthe numberof
cached URCsto 1,thereis only oneURCoutput after10ms.
AT+CURCD ConfigurethedelaytimeandnumberofURC
TestCommand
AT+CURCD=?
Response
+CURCD:(rangeof supported<delay_time>ms),(1)
OK
Read Command
AT+CURCD?
Response
+CURCD:<delay_time>,1
OK
WriteCommand
AT+CURCD=<delay_time>,<
cache_size>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<delay_time> 0-10000 the unitis ms
<cache_size> 1 currently only 1is supported
Examples
AT+CURCD?
+CURCD:0,1
OK
AT+CURCD=?
+CURCD:(0-10000),(1)
OK


## Página 269

A76XX Series_AT Command Manual_V1.09
www.simcom.com 268 /652
AT+CURCD=100,1
OK
Currentlyonly supportdelaytimesetting, thedefaultcache size forURCis one.Thiscommandapplies
toplatform 1601 related projects,suchasA7600E,A7600C-C1SEetc.
NOTE


## Página 270

A76XX Series_AT Command Manual_V1.09
www.simcom.com 269 /652
ATCommandsforHardware
11.1 OverviewofATCommandsforHardware
Command Description
AT+CVALARM Lowandhigh voltageAlarm
AT+CVAUXS Setstate ofthe pinnamedVDD_AUX
AT+CVAUXV Setvoltage valueofthe pinnamedVDD_AUX
AT+CADC ReadADCvalue
AT+CADC2 ReadADC2 value
AT+CMTE Control the module criticaltemperatureURCalarm
AT+CPMVT Lowandhigh voltage Power Off
AT+CRIIC Read valuesfromregisterof IICdevicenau8810
AT+CWIIC Writevaluesto registerofIICdevicenau8810
AT+CBC Read the voltagevalue ofthe powersupply
AT+CPMUTEMP Read the temperature ofthe module
AT+CGDRT Setthe direction ofspecified GPIO
AT+CGSETV Setthe valueof specifiedGPIO
AT+CGGETV Getthe valueofspecified GPIO
11.2 DetailedDescriptionofATCommandsforHardware
11.2.1 AT+CVALARM LowandhighvoltageAlarm
This commandis used toopen orclose the lowvoltage alarm function.
AT+CVALARM LowandhighvoltageAlarm
TestCommand
AT+CVALARM=?
Response
+CVALARM:(listofsupported <enable>s),(list ofsupported<low


## Página 271

A76XX Series_AT Command Manual_V1.09
www.simcom.com 270 /652
voltage>s),(listof supported<highvoltage>s)
OK
Read Command
AT+CVALARM?
Response
+CVALARM:<enable>,<lowvoltage>,<highvoltage>
OK
WriteCommand
AT+CVALARM=<enable>[,<l
owvoltage>],[<high
voltage>]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime -
Reference -
DefinedValues
<enable> 0 Close
1 Open.Ifvoltagelessthan <lowvoltage>, itwillreport
"UNDER-VOLTAGEWARNNING"every 10s. Ifvoltage greaterthan
<high voltage>,itwill report"OVER-VOLTAGEWARNNING"every
10s.
<lowvoltage> Between 3300mVand4000mV.Default valueis 3300.
<highvoltage> Between 4001mVand4300mV.Default valueis 4300.
Examples
AT+CVALARM=1,3400,4300
OK
AT+CVALARM?
+CVALARM:1,3400,4300
OK
AT+CVALARM=?
+CVALARM:(0,1),(3300-4000),(4001-4300)
OK


## Página 272

A76XX Series_AT Command Manual_V1.09
www.simcom.com 271 /652
11.2.2 AT+CVAUXS SetstateofthepinnamedVDD_AUX
This commandis used tosetstateofthe pin whichis namedVDD_AUX.
AT+CVAUXS SetstateofthepinnamedVDD_AUX
TestCommand
AT+CVAUXS=?
Response
1)
+CVAUXS:(listofsupported <state>s)
OK
Read Command
AT+CVAUXS?
Response
+CVAUXS:<state>
OK
WriteCommand
AT+CVAUXS=<state>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime -
Reference -
DefinedValues
<state> 0 output ofthe pindisabled.
1 output ofthe pinenabled.
Examples
AT+CVAUXS=?
+CVAUXS:(0,1)
OK
AT+CVAUXS=1
OK
AT+CVAUXS?
+CVAUXS:1
OK
NOTE


## Página 273

A76XX Series_AT Command Manual_V1.09
www.simcom.com 272 /652
Forthe 1603 platform,ThisATcommandisonly applicable toA7600C1_V401MODULE.
11.2.3 AT+CVAUXV SetvoltagevalueofthepinnamedVDD_AUX
This commandis used tosetthe voltagevalue ofthe pinwhichis named VDD_AUX.
AT+CVAUXV SetvoltagevalueofthepinnamedVDD_AUX
TestCommand
AT+CVAUXV=?
Response
+CVAUXV:(listofsupported <voltage>s)
OK
Read Command
AT+CVAUXV?
Response
+CVAUXV:<voltage>
OK
WriteCommand
AT+CVAUXV=<voltage>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime -
Reference -
DefinedValues
<voltage> Voltagevalue ofthe pinwhichis named VDD_AUX.Theunit isin mv.
Examples
AT+CVAUXV=?
+CVAUXV:
(1200,1250,1700,1800,1850,1900,2500,2600,2700,2750,2800,2850,2900,3000,3100,3300)
OK
AT+CVAUXV=3000
OK
AT+CVAUXV?


## Página 274

A76XX Series_AT Command Manual_V1.09
www.simcom.com 273 /652
+CVAUXV:3000
OK
Forthe 1603 platform,onlyA7600C1series canexecute this command. ForA7600C1_V301series
models,the valid parameters are (2500,2600,2700,2800, 2850,2900,3000, 3100,3300)
11.2.4 AT+CADC ReadADCvalue
This commandis used toread theADC valuefrommodem.MEsupports 2types ofADC,which areraw
type andvoltage type.
AT+CADC ReadADCvalue
TestCommand
AT+CADC=?
Response
+CADC:(rangeofsupported <adc>s)
OK
WriteCommand
AT+CADC=<adc>
Response
1)
+CADC:<value>
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<adc> ADCtype:
0 raw type.
2 voltage type(mv).
<value> Integertype valueof theADC.
Examples
NOTE


## Página 275

A76XX Series_AT Command Manual_V1.09
www.simcom.com 274 /652
AT+CADC=?
+CADC:(0,2)
OK
AT+CADC=2
+CADC:908
OK
11.2.5 AT+CADC2 ReadADC2value
This commandis used toread theADC2 value from modem.MEsupports 2typesofADC,whichare raw
type andvoltage type.
AT+CADC2 ReadADC2value
TestCommand
AT+CADC2=?
Response
+CADC2:(range ofsupported<adc>s)
OK
Write Command
AT+CADC2=<adc>
Response
1)
+CADC2:<value>
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<adc> ADC2 type:
0 raw type.
2 voltage type(mv)
<value> Integertype valueof theADC2.
Examples


## Página 276

A76XX Series_AT Command Manual_V1.09
www.simcom.com 275 /652
AT+CADC2=?
+CADC2:(0,2)
OK
AT+CADC2=2
+CADC2:904
OK
11.2.6 AT+CMTE ControlthemodulecriticaltemperatureURCalarm
This commandis used tocontrolthe modulewhetherURCalarm whenthe module’stemperatureupon the
criticaltemperature.
AT+CMTE ControlthemodulecriticaltemperatureURCalarm
TestCommand
AT+CMTE=?
Response
+CMTE:(listof supported<on/off>s)
OK
Read Command
AT+CMTE?
Response
+CMTE:<on/off>
OK
WriteCommand
AT+CMTE=<on/off>
Response
1)
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<on/off> 0 Disable temperature detection
1 Enable temperaturedetection
Examples
AT+CMTE=?


## Página 277

A76XX Series_AT Command Manual_V1.09
www.simcom.com 276 /652
+CMTE:(0,1)
OK
AT+CMTE=1
OK
AT+CMTE?
+CMTE:1
OK
11.2.7 AT+CPMVT LowandhighvoltagePowerOff
Thiscommandisusedtoopen orclosethe lowandhighvoltagepower offfunction andsetthethresholdof
power offvoltage.
AT+CPMVT LowandhighvoltagePowerOff
TestCommand
AT+CPMVT=?
Response
+CPMVT:(list ofsupported<enable>s),(listof supported<low
voltage>s),(listof supported<highvoltage>s)
OK
Read Command
AT+CPMVT?
Response
+CPMVT:<enable>,<lowvoltage>,<highvoltage>
OK
WriteCommand
AT+CPMVT=<enable>[,<low
voltage>],[<highvoltage>]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime -
Reference -
DefinedValues
<enable> 0 Close.0isthe defaultvalue
1 Open.Ifvoltagelessthan <lowvoltage>, itwillreport
"UNDER-VOLTAGEWARNNINGPOWERDOWN"and poweroffthe
module. Ifvoltagegreaterthan<high voltage>,itwillreport


## Página 278

A76XX Series_AT Command Manual_V1.09
www.simcom.com 277 /652
"OVER-VOLTAGEWARNNINGPOWERDOWN"and poweroffthe
module
<lowvoltage> Between 3200mVand4000mV.Default valueis 3200.
<highvoltage> Between 4001mVand4300mV.Default valueis 4300.
Examples
AT+CPMVT=1,3400,4300
OK
AT+CPMVT?
+CPMVT: 1,3400,4300
OK
AT+CPMVT=?
+CPMVT: (0,1),(3200-4000),(4001-4300)
OK
Forthe 160x platform,lowvoltagerange is3300mv-4000mv,for180x platform,lowvoltagerange is
3200mv-4000mv.
11.2.8 AT+CRIIC ReadvaluesfromregisterofIICdevicenau8810
This commandis used toread values from registerof IICdevice nau8810.
AT+CRIIC ReadvaluesfromregisterofIICdevicenau8810
TestCommand
AT+CRIIC=?
Response
OK
WriteCommand
AT+CRIIC=<addr>,<reg>,<le
n>
Response
1)
+CRIIC:<data>
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
NOTE


## Página 279

A76XX Series_AT Command Manual_V1.09
www.simcom.com 278 /652
DefinedValues
<addr> Deviceaddress.Inputformatmustbe hex,such asFF(do notinput
"0x").
<reg> Registeraddress.Inputformatmustbe hex,suchas FF(do notinput
"0x").
<len> Read length.Range:2;unit:byte.
<data> Dataread.Inputformatmustbe hex,suchas0xFFFF.
Examples
AT+CRIIC=34,f,2
+CRIIC:0xff
OK
AT+CRIIC=34,6,2
+CRIIC:0x140
OK
11.2.9 AT+CWIIC WritevaluestoregisterofIICdevicenau8810
This commandis used towritevaluesto registerofIICdevicenau8810.
AT+CWIIC WritevaluestoregisterofIICdevicenau8810
TestCommand
AT+CWIIC=?
Response
OK
WriteCommand
AT+CWIIC=<addr>,<reg>,<d
ata>,<len>
1)
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<addr> Deviceaddress.Inputformatmustbe hex,such asFF(do notinput


## Página 280

A76XX Series_AT Command Manual_V1.09
www.simcom.com 279 /652
"0x").
<reg> Registeraddress.Inputformatmustbe hex,suchas FF(donotinput
"0x").
<len> Read length.Range:2; unit:byte.
<data> Datawritten. Inputformatmustbe hex,suchas 0xFFFF
Examples
AT+CWIIC=34,6,141,2
OK
11.2.10AT+CBC Readthevoltagevalueofthepowersupply
This commandis used toread thevoltage valueofthe powersupply.
AT+CBC Readthevoltagevalueofthepowersupply
Execution Command
AT+CBC
Response
1)
+CBC:<vol>
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<vol> Thevoltage value, suchas3.8.
Examples
AT+CBC
+CBC:3.749V
OK


## Página 281

A76XX Series_AT Command Manual_V1.09
www.simcom.com 280 /652
11.2.11AT+CPMUTEMP Readthetemperatureofthemodule
This commandis used toread thetemperature ofthe module.
AT+CPMUTEMP Readthetemperatureofthemodule
Execution Command
AT+CPMUTEMP
Response
+CPMUTEMP:<temp>
OK
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<temp> TheTemperaturevalue,suchas 29.
Examples
AT+CPMUTEMP
+CPMUTEMP:15
OK
11.2.12AT+CGDRT SetthedirectionofspecifiedGPIO
This commandis used tosetthe specified GPIOtoinputoroutput state.Ifsettingto inputstate, thenthis
GPIOcan notbe settohigh orlowvalue.
AT+CGDRT SetthedirectionofspecifiedGPIO
TestCommand
AT+CGDRT=?
Response
+CGDRT:(list ofsupported<GPIO>s),(list ofsupported <gpio_io>s)
OK
WriteCommand
AT+CGDRT=<GPIO>
Response
1)
+CGDRT:<GPIO>,<gpio_io>


## Página 282

A76XX Series_AT Command Manual_V1.09
www.simcom.com 281 /652
OK
2)
ERROR
WriteCommand
AT+CGDRT=<GPIO>,<gpio_i
o>
Response
1)
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<GPIO> Thevalue isGPIO ID,different hardwareversions have different
values.
<gpio_io> 0 in
1 out
Examples
AT+CGDRT=?
+CGDRT: (1,2,3,6,12,14,16,18,22,41,43,63,77),(0-1)
OK
AT+CGDRT=3,0
OK
AT+CGDRT=3
+CGDRT: 3,0
OK
Thereturn value of‘AT+CGDRT=?’applies only toplatform 1603.Platform 1601 hasdifferentreturn
values.
11.2.13AT+CGSETV SetthevalueofspecifiedGPIO
NOTE


## Página 283

A76XX Series_AT Command Manual_V1.09
www.simcom.com 282 /652
This commandis used tosetthe valueof the specifiedGPIO tohigh orlow.
ThedirectionofspecifiedGPIOmustbesetasOUTdirectionbyusingAT+CGDRTbeforethisATcommand,
otherwise an errorwillbe returned.
AT+CGSETV SetthevalueofspecifiedGPIO
TestCommand
AT+CGSETV=?
Response
+CGSETV:(listofsupported <GPIO>s),(listof supported
<gpio_hl>s)
OK
WriteCommand
AT+CGSETV=<GPIO>,<gpio
_hl>
Response
1)
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<GPIO> Thevalue isGPIO ID,different hardwareversions have different
values.
<gpio_hl> 0 low
1 high
Examples
AT+CGSETV=?
+CGSETV:(1,2,3,6,12,14,16,18,22,41,43,63,77),(0-1)
OK
AT+CGSETV=6,0
OK
Thereturn value of‘AT+CGSETV=?’appliesonly to platform 1603.Platform 1601 has differentreturn
values.
NOTE


## Página 284

A76XX Series_AT Command Manual_V1.09
www.simcom.com 283 /652
11.2.14AT+CGGETV GetthevalueofspecifiedGPIO
This commandis used togetthe value(high orlow)ofthe specified GPIO.
Thedirectionofspecified GPIOmustbe setas INdirectionby usingAT+CGDRTbefore thisATcommand,
otherwise an errorwillbe returned.
AT+CGSETV GetthevalueofspecifiedGPIO
TestCommand
AT+CGGETV=?
Response
+CGGETV:(listofsupported <GPIO>s)
OK
WriteCommand
AT+CGGETV=<GPIO>
Response
1)
+CGGETV:<GPIO>,<gpio_hl>
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<GPIO> Thevalue isGPIO ID,different hardwareversions have different
values.
<gpio_hl> 0 low
1 high
Examples
AT+CGGETV=?
+CGGETV:(1,2,3,6,12,14,16,18,22,41,43,63,77)
OK
AT+CGGETV=3
+CGGETV:3,0
OK
NOTE


## Página 285

A76XX Series_AT Command Manual_V1.09
www.simcom.com 284 /652
Thereturn value of‘AT+CGGETV=?’applies only toplatform 1603.Platform 1601 hasdifferentreturn
values.
11.3 UnsolicitedResultCodes
URC Description ATCommand
CMTE: <temp_level> Whilemodule’stemperatureover
thehigh thresholdand belowthe
lowthreshold, the URCwilloccur.
AT+CMTE
UNDER-VOLTAGEWARNING
Thisis aURCALARM when
Currentvoltage is UNDERthe
valuewhichyou set.
AT+CVALARM
OVER-VOLTAGE WARNING
Thisis aURCALARM when
CurrentvoltageisOVERthevalue
whichyou set.
AT+CVALARM
UNDER-VOLTAGEWARNING POWER
DOWN
Thisis aURCALARM when
Currentvoltage is UNDERthe
valuewhichyou set.
AT+CPMVT
OVER-VOLTAGEWARNINGPOWERDOWN
Thisis aURCALARM when
CurrentvoltageisOVERthevalue
whichyou set.
AT+CPMVT
DefinedValues
<temp_level> -2 below-45 celsius degree.
-1 (-45,-30]celsius degree.
1 (80,85]celsius degree.
2 over85celsius degree.


## Página 286

A76XX Series_AT Command Manual_V1.09
www.simcom.com 285 /652
ATCommandsforFileSystem
12.1 OverviewofATCommandsforFileSystem
Command Description
AT+FSCD Selectdirectory as currentdirectory
AT+FSMKDIR Makenewdirectory incurrentdirectory
AT+FSRMDIR Deletedirectory incurrentdirectory
AT+FSLS Listdirectories/files incurrentdirectory
AT+FSDEL Deletefileincurrentdirectory
AT+FSRENAME Renamefileincurrentdirectory
AT+FSATTRI Requestfileattributes
AT+FSMEM Checkthe size ofavailable memory
AT+FSCOPY Copyan appointed file
AT+FSPRESET move the location ofafile
AT+FSOPEN open afile
AT+FSCLOSE closeafile
AT+FSREAD readafile
AT+FSWRITE writeafile
AT+FSSEEK setafilepointertothe specified position
AT+FSPOSITION getthe offsetof afilepointer
Command Description SupportedModules
AT+FSRENAME D:/directory filerename OnlyCat1modules
AT+FSOPEN Openafile OnlyASR1603 standard branch
AT+FSCLOSE OnlyASR1603 standard branch
AT+FSREAD OnlyASR1603 standard branch
AT+FSWRITE OnlyASR1603 standard branch
AT+FSSEEK OnlyASR1603 standard branch
AT+FSPOSITIO
N
OnlyASR1603 standard branch


## Página 287

A76XX Series_AT Command Manual_V1.09
www.simcom.com 286 /652
12.2 DetailedDescriptionofATCommandsforFileSystem
Thefilesystem isusedto storefiles in ahierarchical(tree)structure, andthereare somedefinitions and
conventions tousetheATcommands.
Local storage spaceis mapped to"C:","D:"forSDcard.(TheA7678 doesnot supportSDcard).
NOTE:Generalrules fornaming(both directories andfiles):
a)Thelengthof actual fullyqualified names offiles(C:/)can notexceed 115,alsoinclude fullDir.
b)Thelengthof actual fullyqualified names ofdirectories andfiles(D:/)cannotexceed250,also
include full Dir.
c)Directory and filenamescannot includethe following characters:\/:*?"<>|
d)Between directory nameandfile/directory name,usecharacter"/"as listseparator,soitcan not
appearindirectory nameorfile name.
e)Filenameson “C:/”drive cannot beginwith “.”or“blank”anditalsocan’tendwith “.”
f){none-ascii} input is only supported when the SD card function is supported. {none-ascii} input is still
not allowed when operating external flash and file systems.
g)The file name in drive D is case-insensitive, but can be displayed in case format.
Ifthe lastcharacter ofnames isperiod ".";the SDcard cansupport thischaracter,butthe compatibilityis
notgood.
12.2.1 AT+FSCD Selectdirectoryascurrentdirectory
This commandis used toselectadirectory.TheModulesupports absolutepath andrelative path.
AT+FSCD Selectdirectoryascurrentdirectory
TestCommand
AT+FSCD=?
Response
OK
Read Command
AT+FSCD?
Response
+FSCD:<curr_path>
OK
WriteCommand
AT+FSCD=<path>
Response
1)Ifsetcurrentdirectory successfully:
+FSCD:<curr_path>
OK
2)Ifsetcurrentdirectory failed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference


## Página 288

A76XX Series_AT Command Manual_V1.09
www.simcom.com 287 /652
DefinedValues
<path> Directory forselection.
<curr_path> Currentdirectory.
Examples
AT+FSCD=C:
+FSCD:C:/
OK
AT+FSCD=C:/
+FSCD:C:/
OK
AT+FSCD?
+FSCD:C:/
OK
AT+FSCD=D:
+FSCD:D:/
OK
If<path>is"..", itwill goback toprevious levelofdirectory.
12.2.2 AT+FSMKDIR Makenewdirectoryincurrentdirectory
This commandis used tocreateanewdirectory incurrentdirectory.Support"D:" and “C:”.
AT+FSMKDIR Makenewdirectoryincurrentdirectory
TestCommand
AT+FSMKDIR=?
Response
OK
WriteCommand
AT+FSMKDIR=<dir>
Response
1)Ifsuccessfully:
OK
NOTE


## Página 289

A76XX Series_AT Command Manual_V1.09
www.simcom.com 288 /652
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<dir> Directory namewhichdoes notalreadyexistin currentdirectory.
Examples
AT+FSMKDIR=SIMTech
OK
AT+FSCD?
+FSCD:D:/
OK
AT+FSLS
+FSLS:SUBDIRECTORIES:
SIMTech
OK
Support"C:"and“D:”.
12.2.3 AT+FSRMDIR Deletedirectoryincurrentdirectory
This commandis used todelete existing directory incurrentdirectory.Support"D:"and“C:”.
AT+FSRMDIR Deletedirectoryincurrentdirectory
TestCommand
AT+FSRMDIR=?
Response
OK
WriteCommand
AT+FSRMDIR=<dir>
Response
1)Ifsuccessfully:
NOTE


## Página 290

A76XX Series_AT Command Manual_V1.09
www.simcom.com 289 /652
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<dir> Thedirectorynamewhichalready existsincurrentdirectory.
Examples
AT+FSRMDIR=SIMTech
OK
AT+FSCD?
+FSCD:D:/
OK
AT+FSLS
+FSLS:SUBDIRECTORIES:
OK
Support"C:"and“D:”.
12.2.4 AT+FSLS Listdirectories/filesincurrentdirectory
This commandis used tolistinformations of directoriesand/or files incurrentdirectory.Support"C:","D:".
AT+FSLS Listdirectories/filesincurrentdirectory
TestCommand
AT+FSLS=?
Response
+FSLS:(listof supported<type>s)
OK
NOTE


## Página 291

A76XX Series_AT Command Manual_V1.09
www.simcom.com 290 /652
Read Command
AT+FSLS?
Response
+FSLS:SUBDIRECTORIES:<dir_num>,FILES:<file_num>
OK
WriteCommand
AT+FSLS=<type>
Response
[+FSLS:SUBDIRECTORIES:
<listof subdirectories>]
[+FSLS:FILES:
<listof files>]
OK
Execution Command
AT+FSLS
Response
[+FSLS:SUBDIRECTORIES:
<listof subdirectories>]
[+FSLS:FILES:
<listof files>]
OK
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<dir_num> Integertype,the numberofsubdirectories in currentdirectory.
<file_num> Integertype,the numberoffiles in currentdirectory.
<type> 0 list bothsubdirectories and files
1 list subdirectories only
2 list files only
Examples
AT+FSLS?
+FSLS:SUBDIRECTORIES:2,FILES:2
OK
AT+FSLS
+FSLS:SUBDIRECTORIES:
FirstDir
SecondDir


## Página 292

A76XX Series_AT Command Manual_V1.09
www.simcom.com 291 /652
+FSLS:FILES:
image_0.jpg
image_1.jpg
OK
AT+FSLS=2
+FSLS:FILES:
image_0.jpg
image_1.jpg
OK
12.2.5 AT+FSDEL Deletefileincurrentdirectory
This commandis used todelete afileincurrentdirectory.Before dothat, itneedsto useAT+FSCD select
the fatherdirectory as currentdirectory.Support"C:","D:".
AT+FSDEL Deletefileincurrentdirectory
TestCommand
AT+FSDEL=?
Response
OK
WriteCommand
AT+FSDEL=<filename>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filename> String with orwithout double quotes,filenamewhich isrelativeand
already existing.
Examples
AT+FSDEL=image_0.jpg
OK
NOTE


## Página 293

A76XX Series_AT Command Manual_V1.09
www.simcom.com 292 /652
If<filename>is *.*,itmeans delete allfiles in currentdirectory.
12.2.6 AT+FSRENAME Renamefileincurrentdirectory
This commandis used torenameafileincurrentdirectory.Support"C:", "D:".
AT+FSRENAME Renamefileincurrentdirectory
TestCommand
AT+FSRENAME=?
Response
OK
WriteCommand
AT+FSRENAME=<old_name
>,<new_name>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<old_name> String with orwithout double quotes,filenamewhich isexisted in
currentdirectory.
<new_name> New nameofspecified file,stringwith orwithout doublequotes.
Examples
AT+FSRENAME=image_0.jpg,image_1.jpg
OK
InCat 4modules,“D:”does notsupportAT+FSRENAME.
Cannot renamefiles that lengthis 0.
NOTE


## Página 294

A76XX Series_AT Command Manual_V1.09
www.simcom.com 293 /652
12.2.7 AT+FSATTRI Requestfileattributes
This commandis used torequestthe attributesof filewhichexists incurrentdirectory.Support"C:", "D:".
AT+FSATTRI Requestfileattributes
TestCommand
AT+FSATTRI=?
Response
OK
WriteCommand
AT+FSATTRI=<filename>
Response
1)Ifsuccessfully:
+FSATTRI:<file_size>
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filename> Stringwith orwithout doublequotes,file namewhichisin current
directory.
<file_size> Thesizeof specifiedfile, andthe unitis inByte.
Examples
AT+FSATTRI=image_0.jpg
+FSATTRI:8604
OK
12.2.8 AT+FSMEM Checkthesizeofavailablememory
Thiscommandisusedtocheckthesizeofavailablememory.Theresponsewilllisttotalsizeandusedsize
oflocal storage space ifpresentandmounted.Support"C:","D:".


## Página 295

A76XX Series_AT Command Manual_V1.09
www.simcom.com 294 /652
AT+FSMEM Checkthesizeofavailablememory
TestCommand
AT+FSMEM=?
Response:
OK
Execution Command
AT+FSMEM
Response:
1)Ifsuccessfully,currently C:/:
+FSMEM:C:(<total>,<used>)
OK
2)Ifsuccessfully,currently D:/:
+FSMEM:D:(<total>,<used>)
OK
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<total> Thetotal size oflocalstorage space.
<used> Theusedsize of localstoragespace.
Examples
AT+FSMEM
+FSMEM:C:(11348480,2201600)
OK
Theunit ofstoragespacesize isin Byte.
12.2.9 AT+FSCOPY Copyanappointedfile
This commandis used tocopyan appointed file onC:/to anappointed directory onC:/, the newfilename
shouldgive inparameter.Support"C:","D:".
NOTE


## Página 296

A76XX Series_AT Command Manual_V1.09
www.simcom.com 295 /652
AT+FSCOPY Copyanappointedfile
TestCommand
AT+FSCOPY=?
Response
OK
WriteCommand
AT+FSCOPY=<file1>,<file2>[
,<sync_mode>]
Response
1)Ifsuccessfully,synchronousmode:
+FSCOPY:<percent>
[+FSCOPY:<percent>]
OK
2)Ifsuccessfully,asynchronousmode:
OK
+FSCOPY:<percent>
[+FSCOPY:<percent>]
+FSCOPY:END
3)Ifany error:
SDCARD NOTPLUGGEDIN
FILEIS EXISTING
FILENOTEXISTING
DIRECTORYISEXISTED
DIRECTORYNOTEXISTED
INVALIDPATHNAME
INVALIDFILENAME
SDCARD HAVENOENOUGHMEMORY
EFSHAVENOENOUGHMEMORY
FILECREATE ERROR
READ FILEERROR
WRITEFILE ERROR
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<file1> Thesourcesfilenameorthe whole pathnamewith sourcesfilename.
<file2> Thedestination filenameorthe whole path namewith destinationfile
name.
<percent> Thepercentof copy done.Therange is0.0 to100.0
<sync_mode> Theexecution mode ofthecommand:


## Página 297

A76XX Series_AT Command Manual_V1.09
www.simcom.com 296 /652
0 synchronous mode
1 asynchronousmode
Examples
AT+FSCOPY=C:/TESTFILE,COPYFILE //Copy fileTESTFILEon C:/toC:/COPYFILE
+FSCOPY:0.0
+FSCOPY:9.7
+FSCOPY:19.4
…
+FSCOPY:100.0
OK
The<file1>and<file2>should give thewhole path andname,ifonly givenfilename,itwill referto
currentpath(AT+FSCD)andcheckthe file’svalidity.
If<file2>isa whole pathandname,makesure thedirectory exists, makesurethat the filenamedoes
notexistorthe filenameisnotthe samenameasthe sub foldername,otherwise return error.
<percent> reportrefer tothe copy filesize.Thebigfilemaybe reportmanytimes,andlittle filereport
less.
If<sync_mode>is1, the commandwillreturn OKimmediately,and reportfinal resultwith +FSCOPY:
END.
12.2.10AT+FSPRESET Movesthelocationofafile
This commandis used tomovean appointed fileon C:/to C:/simdir/,orfromC:/simdir toC:/.
AT+FSPRESET Move thelocationofafile
TestCommand
AT+FSPRESET=?
Response
OK
WriteCommand
AT+FSPRESET=<fileName>[,
<direction>]
Response
1)Ifsuccessfully
OK
NOTE


## Página 298

A76XX Series_AT Command Manual_V1.09
www.simcom.com 297 /652
2)Iferror
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<fileName> Thefilenametobe movedwithout the path.
<direction> Thedirectionin whichthe filewas moved
0 fromrootdirectory tothe userdirectory
1 fromuserdirectory tothe rootdirectory
Examples
AT+FSPRESET=test.txt,0 //move file fromrootdirectory tothe userdirectory
OK
 1.Justsupportedon inthe standard branchofASR1603 andASR1606.
 2.When<direction>=1,Ifthe filealready existsin the rootdirectory,deletethe fileand moveitto
the rootdirectory.
12.2.11AT+FSOPEN Openafile
This commandopens afileandgets the filehandleto beusedincommands suchas
AT+FSREAD,AT+FSWRITE,AT+FSSEEK,AT+FSPOSITION,AT+FSCLOSE.
AT+FSOPEN Openafile
TestCommand
AT+FSOPEN=?
Response
+FSOPEN:<filename>[,(0-2)]
OK
Read Command
AT+FSOPEN?
Response
+FSOPEN:<filename>,<filehandle>,<mode>
+FSOPEN:<filename>,<filehandle>,<mode>
[…]]
NOTE


## Página 299

A76XX Series_AT Command Manual_V1.09
www.simcom.com 298 /652
OK
WriteCommand
AT+FSOPEN
=<fileName>[,<mode>]
Response
1)Ifsuccessfully
+FSOPEN:<filehandle>
OK
2)Ifthere isan errorrelatedto ME functionality:
+CMEERROR: <err>
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filename> Thefilenametobe openedwith the path.Themaximum length is115
bytes.
<filehandle> Thehandle ofthefile.
<mode> Theopen mode ofthe file.
0 ifthe filedoes notexist,itwillbe created.Ifthe fileexists, itwill
be directlyopened.Andboth ofthem canbe readand written.
1 ifthe filedoes notexist,itwillbe created.Ifthe fileexists, itwill
be overwrittenand cleared.Andboth ofthem canbe read and written.
2 ifthefileexist,openitanditcanbereadonly.Whenthefiledoes
notexist, itwillrespondan error.
Examples
AT+FSOPEN=C:/test.txt,0
+FSOPEN:1
OK
12.2.12AT+FSREAD Readafile
This commandreads thedata ofafilewhich isspecified bythe handle.Thedata starts fromthe current
position ofthe pointerwhichbelongs tothe filehandle.
AT+FSREAD Readafile
TestCommand
AT+FSREAD=?
Response
+FSREAD:<filehandle>[,<length>]


## Página 300

A76XX Series_AT Command Manual_V1.09
www.simcom.com 299 /652
OK
WriteCommand
AT+FSREAD
=<filehandle>[,<length>]
Response
CONNECT<read_length>
TAswitches todata mode.Whenthe total size ofthe data reaches
<length>(unit:byte),TAwillreturn to commandmode, displaythe
resultand then reply the followingcodes:
OK
2)Ifthere isan errorrelatedto ME functionality:
+CMEERROR: <err>
3)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filehandle> Thehandle ofthefile.
<length> Thelength ofthe file toberead outandthe defaultis thefile length.
<read_length> Theactualread length.
Examples
AT+FSREAD=1,10
CONNECT10
1234567890
OK
12.2.13AT+FSWRITE Writeafile
This commandwrites data intoafile.Thedatastarts fromthe currentposition ofthe filepointerwhich
belongs tothe filehandle.
AT+FSWRITE Writeafile
TestCommand
AT+FSWRITE=?
Response
+FSWRITE:<filehandle>[,<length>[,<timeout>]]


## Página 301

A76XX Series_AT Command Manual_V1.09
www.simcom.com 300 /652
OK
WriteCommand
AT+FSWRITE=<filehandle>[,
<length>,[<timeout>]]
Response
CONNECT
TAswitches todata mode.Whenthe total size ofthe writtendata
reaches<length>(unit:byte)orthe timereaches <timeout>,TAwill
returnto commandmodeand reply thefollowing codes:
+FSWRITE:<written_length>,<total_length>
OK
2)Ifthere isan errorrelatedto ME functionality:
+CMEERROR: <err>
3)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filehandle> Thehandle ofthefile tobe operated.
<length> Thelength ofthe file tobewritten,and the defaultis10K.The
maximum valueof thisparameterisdetermined by<freesize>from
AT+FSMEM.
<timeout> Thetimewaiting fordata tobe inputtedto USB/UART.Thedefault
value is5.Unit:s.
<written_length> Theactualwrittenlength.
<total_length> Thetotal length ofthe file.
Examples
AT+FSWRITE=1,5,10
CONNECT
//inputdata
+FSWRITE:0,5
OK
12.2.14AT+FSSEEK SetafilePointertotheSpecifiedPosition


## Página 302

A76XX Series_AT Command Manual_V1.09
www.simcom.com 301 /652
This commandsetafilepointertothe specifiedposition.This willdecidethe starting positionof commands
suchas AT+FSREAD,AT+FSWRITE,AT+FSSEEK,AT+FSPOSITION.
AT+FSSEEK SetafilePointertotheSpecifiedPosition
TestCommand
AT+FSSEEK=?
Response
+FSSEEK:<filehandle>,<offset>[,<position>]
OK
WriteCommand
AT+FSSEEK=<filehandle>,<
offset>,[<position>]
Response
OK
2)Ifthere isan errorrelatedto ME functionality:
+CMEERROR: <err>
3)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filehandle> Thehandle ofthefile tobe operated.
<offset> Thenumberof bytesofthe filepointermovement.
<position> Pointermovement mode.
0 The beginningofthe file.
1 The currentposition ofthe pointer.
2 The end ofthe file.
Examples
AT+FSSEEK=1,0,0
OK
12.2.15AT+FSPOSITION GettheOffsetofaFilePointer
This commandgets the offsetofafilepointerfromthe beginning ofthe file.
AT+FSPRESET Move thelocationofafile
TestCommand
AT+FSPOSITION=?
Response
+FSPOSITION:<filehandle>


## Página 303

A76XX Series_AT Command Manual_V1.09
www.simcom.com 302 /652
OK
WriteCommand
AT+FSPOSITION=<filehandl
e>
Response
+FSPOSITION:<offset>
OK
2)Ifthere isan errorrelatedto ME functionality:
+CMEERROR: <err>
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filehandle> Thehandle ofthefile tobe operated.
<offset> Theoffsetfromthe beginning ofthe file.
Examples
AT+FSPOSITION=1
+FSPOSITION:0 //move file fromrootdirectory tothe userdirectory
OK
12.2.16AT+FSCLOSE Closeafile
This commandcloses afileand ends the operationto thefile.Afterthat, the filehandle isreleaseand
shouldnotbe used again,unlessthe fileis opened againby AT+FSOPEN.
AT+FSCLOSE Closeafile
TestCommand
AT+FSCLOSE=?
Response
+FSCLOSE:<filehandle>
OK
WriteCommand
AT+FSCLOSE=<filehandle>
Response
OK
2)Ifthere isan errorrelatedto ME functionality:
+CMEERROR: <err>
ParameterSaving Mode -


## Página 304

A76XX Series_AT Command Manual_V1.09
www.simcom.com 303 /652
Max ResponseTime -
Reference
DefinedValues
<filehandle> Thehandle ofthefile tobe operated.
Examples
AT+FSCLOSE=1 //move file fromrootdirectory tothe userdirectory
OK


## Página 305

A76XX Series_AT Command Manual_V1.09
www.simcom.com 304 /652
ATCommandsforFileTransmission
13.1 OverviewofATCommandsforFileTransmission
Command Description
AT+CFTRANRX TransferafiletoEFS
AT+CFTRANTX TransferafilefromEFS tohost
AT+CFTRXBUF Setsthe size of the buffertotransferfiles toEFS
Command Description SupportedProject
AT+CFTRXBUF A7600C1-XXXX(1601)
A7678(1603)
13.2 DetailedDescriptionofATCommandsforFileTransmission
13.2.1 AT+CFTRANRX TransferafiletoEFS
This commandis used totransferafiletoEFS.Support"C:", "D:".
AT+CFTRANRX TransferafiletoEFS
TestCommand
AT+CFTRANRX=?
Response
+CFTRANRX:[{non-ascii}]"FILEPATH"
OK
WriteCommand
AT+CFTRANRX=<filepath>,<l
en>[,<reserved>[,<location>]
]
Response
1)Ifsuccessfully:
>
OK
2)Iffailed:
>


## Página 306

A76XX Series_AT Command Manual_V1.09
www.simcom.com 305 /652
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filepath> Thepath ofthe fileon EFS
<len> Thelength ofthe file datatosend.
Becauseofthesystemresources,Thelengthcouldnotsettoolarge.If
usethe UARTto senddata,itmay cansetto3Mb. IfuseUSB tosend
data,itmay justcan setto200Kb. Iflimitthe sendspeed, itcan set
larger.Theactualsize could notensure. Usually itissaferto seta
smaller size.
<reserved> Thevalue is100.
<location> Theposition offsetfromthe start ofthe file.
Examples
AT+CFTRANRX="c:/t1.txt",10
>
OK
AT+CFTRANRX="d:/MyDir/t1.txt",10
>
OK
AT+CFTRANRX="C:/t2.txt",10,100,0
>
OK
1.The<filepath>mustbeafull pathwith the directory path,makesure thatthe filenamedoes notexist
underthe path.
2.Ifsending filefails,increasethe delay timebetween each256 bytereach to50ms,and then tryto
sendfileagain.
3.Only Cat4 modulesand SDcardsupport Non-ASCIIcharacters infilepath.
4.The<reserved>isusedbyindividualcustomersandif<location>isnotdefaultvalue,<reserved>will
be considered invalid.
5.The<location>exceed the file size,the command willreturn “ERROR”.
NOTE


## Página 307

A76XX Series_AT Command Manual_V1.09
www.simcom.com 306 /652
6.Whenthe <location>islessthan the filesize,itwillwrite data from <location>.
7.The<location>is justsupportedinASR1603 standard branches ,compatible SIM800 branchesand
ASR1803Sstandard branches.
13.2.2 AT+CFTRANTX TransferafilefromEFStohost
This commandis used totransferafilefrom EFStohost.
AT+CFTRANTX TransferafilefromEFStohost
TestCommand
AT+CFTRANTX=?
Response
+CFTRANTX:[{non-ascii}]"FILEPATH"
OK
WriteCommand
AT+CFTRANTX=<filepath>[,<
location>][,<size>][,<transMo
de>]
Response
1)Ifsuccessfully:
[+CFTRANTX:DATA,<len>
…
+CFTRANTX:DATA,<len>]
+CFTRANTX:0
OK
2)If<transMode>is1:
>…
OK
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<filepath> Thepath ofthe fileon EFS
<len> Thelength ofthe following filedata tooutput.
<location> Thebeginning ofthe filedata tooutput.
<size> Thelength ofthe file datatooutput.
<transMode> Whetherthere isno urc indata output
0 normalmode
1 data output directlywithout urc.


## Página 308

A76XX Series_AT Command Manual_V1.09
www.simcom.com 307 /652
Examples
AT+CFTRANTX="c:/t1.txt"
+CFTRANTX:DATA, 11
Testcontent
+CFTRANTX:0
OK
AT+CFTRANTX="d:/MyDir/t1.txt"
+CFTRANTX:DATA, 11
Testcontent
+CFTRANTX:0
OK
AT+CFTRANTX="d:/MyDir/t1.txt",1,4
+CFTRANTX:DATA, 4
estc
+CFTRANTX:0
OK
AT+CFTRANTX="c:/1.txt",0,10,1
>123456790
OK
The<filepath>mustbeafull pathwith the directory path.
Ifnotsetthe size,itmeans rangefromlocation tothe end ofthe file.
Ifthe (size +location)lagerthan the file size,itmeans rangefrom locationtothe endofthe file.
13.2.3 AT+CFTRXBUF SetsthesizeofthebuffertotransferfilestoEFS
This commandis used tosetthe size ofthe buffertotransferfiles toEFS.Support“C:”,“D:”.
AT+CFTRXBUF SetsthesizeofthebuffertotransferfilestoEFS
TestCommand
AT+CFTRXBUF=?
Response
+CFTRXBUF:(listofsupported<buffsize_level>s)
NOTE


## Página 309

A76XX Series_AT Command Manual_V1.09
www.simcom.com 308 /652
OK
Read Command
AT+CFTRXBUF?
Response
+CFTRXBUF:<buffsize_level>
OK
WriteCommand
AT+CFTRXBUF=<buffsize_le
vel>
Response
a)Ifsuccessfully:
OK
c)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<buffsize_level> Thesize of thebuffertotransferfiles toEFS.
0--20Kb(20,480bytes)
1--40Kb(40,960bytes)
Examples
AT+CFTRXBUF=0
OK
AT+CFTRXBUF?
+CFTRXBUF:0
OK
Thedefaultbuffersizeis20KB,whichissufficientforbasicfiletransferrequirements.Whentransferring
large files fails,ifyou areunwilling toextend thedelay timeinordertoensure the transferspeed,you
canincrease the buffersize to40KB,butthismay affectmultiple applicationscenarios.
NOTE


## Página 310

A76XX Series_AT Command Manual_V1.09
www.simcom.com 309 /652
ATCommandsforInternetService
14.1 OverviewofATCommandsforInternetService
Command Description
AT+CHTPSERV SetHTPserverinformation
AT+CHTPUPDATE Updatingdate timeusingHTPprotocol
AT+CNTP Update system time
14.2 DetailedDescriptionofATCommandsforInternetService
14.2.1 AT+CHTPSERV SetHTPserverinformation
This commandis used toaddordelete HTPserverinformation.There are maximum16 HTPservers.
AT+CHTPSERV SetHTPserverinformation
TestCommand
AT+CHTPSERV=?
Response
+CHTPSERV:"ADD","HOST",(1-65535),(0-1)[,"PROXY",(1-65535)]
+CHTPSERV:"DEL",(0-15)
OK
Read Command
AT+CHTPSERV?
Response
1)
OK
2)
+CHTPSERV:
<index><host>,<port>,<http_version>[,<proxy>,<proxy_port>]
…
+CHTPSERV:<index><host>,<port>[,<proxy>,<proxy_port>]
OK


## Página 311

A76XX Series_AT Command Manual_V1.09
www.simcom.com 310 /652
WriteCommand
AT+CHTPSERV=<cmd>,<ho
st_or_idx>[,<port>,<http_ver
sion>[,<proxy>,<proxy_port
>]]
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<cmd> This commandtooperate the HTPserver list.
"ADD" add aHTPserver item tothe list
"DEL" delete aHTPserver itemfrom the list
<host_or_idx> Ifthe<cmd>is“ADD”,thisfieldisthesameas<host>,lengthis1-255;
Ifthe <cmd>is“DEL”,this field isthe index ofthe HTPserveritem to
be deletedfrom thelist.
<host> TheHTPserveraddress,length is1-255.
<port> TheHTPserverport, the rangeis(1-65535).
<http_version> TheHTTPversionofthe HTPserver:
0 HTTP1.0
1 HTTP1.1
<proxy> Theproxy address,length is 1-255.
<proxy_port> Theportof the proxy,the rangeis(1-65535).
<index> TheHTPserverindex.
Examples
AT+CHTPSERV="ADD","www.google.com",80,1
OK
14.2.2 AT+CHTPUPDATE UpdatingdatetimeusingHTPprotocol
This commandis used toupdatingdate timeusing HTPprotocol.
AT+CHTPUPDATE UpdatingdatetimeusingHTPprotocol
TestCommand
AT+CHTPUPDATE=?
Response
OK
Read Command Response


## Página 312

A76XX Series_AT Command Manual_V1.09
www.simcom.com 311 /652
AT+CHTPUPDATE? +CHTPUPDATE:<status>
OK
Execute Command
AT+CHTPUPDATE
Response
1)Ifsuccessfully:
OK
+CHTPUPDATE:<err>
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<status> Thestatus ofHTPmodule:
Updating HTPmoduleis synchronizingdatetime
NULL HTPmoduleis idlenow
<err> Theresult ofthe HTPupdating
Examples
AT+CHTPUPDATE
OK
+CHTPUPDATE:0
14.2.3 AT+CNTP Updatesystemtime
This commandis used toupdate systemtimewith NTPserver.
AT+CNTP Updatesystemtime
TestCommand
AT+CNTP=?
Response
+CNTP:"HOST",(-96~96)
OK
Read Command
AT+CNTP?
Response
+CNTP:<host>,<timezone>


## Página 313

A76XX Series_AT Command Manual_V1.09
www.simcom.com 312 /652
OK
WriteCommand
AT+CNTP=<host>[,<timezon
e>]
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
Execute Command
AT+CNTP
Response
1)Ifsuccessfully:
OK
+CNTP:<err>
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<host> NTPserveraddress,length is0-255.
<timezone> Local timezone,therangeis (-96 to96),defaultvalue is32.
Examples
AT+CNTP="120.25.115.20",32
OK
AT+CNTP
OK
+CNTP:0
14.3 CommandResultCodes
14.3.1 Descriptionof<err>ofHTP
<err> Description


## Página 314

A76XX Series_AT Command Manual_V1.09
www.simcom.com 313 /652
0 Operationsucceeded
1 Unknown error
2 Wrongparameter
3 Wrongdate andtimecalculated
4 Networkerror
14.3.2 Descriptionof<err>ofNTP
<err> Description
0 Operationsucceeded
1 Unknown error
2 Wrongparameter
3 Wrongdate andtimecalculated
4 Networkerror
5 Timezoneerror
6 Timeouterror


## Página 315

A76XX Series_AT Command Manual_V1.09
www.simcom.com 314 /652
ATCommandsforTCP/IP
15.1 OverviewofATCommandsforTCP/IP
Command Description
AT+NETOPEN StartSocketService
AT+NETCLOSE Stop SocketService
AT+CIPOPEN Establish Connection inMulti-Socket Mode
AT+CIPSEND Send datathroughTCPorUDPConnection
AT+CIPRXGET Setthe Mode toRetrieve Data
AT+CIPCLOSE CloseTCPor UDPSocket
AT+IPADDR Inquire SocketPDPaddress
AT+CIPHEAD Addan IPHeaderWhenReceiving Data
AT+CIPSRIP ShowRemote IPAddressandPort
AT+CIPMODE SetTCP/IPApplication Mode
AT+CIPSENDMODE SetSendingMode
AT+CIPTIMEOUT SetTCP/IPTimeoutValue
AT+CIPCCFG Configure Parametersof Socket
AT+SERVERSTART StartupTCPSever
AT+SERVERSTOP StopTCPSever
AT+CIPACK QueryTCPConnection DataTransmitting Status
AT+CDNSGIP Query the IPAddress ofGiven DomainName
AT+CSOCKSETPN Setactive PDPcontext'sprofile
AT+CTCPKA ConigureTCPheartbeat
AT+CDNSCFG Configure Domain NameServer
AT+CSOC Setsomefeatures ofthe data service
AT+CIPCFG Configure ParametersofTcp
AT+CIPSENDSTR Send HEXString data
15.2 DetailedDescriptionofATCommandsforTCP/IP


## Página 316

A76XX Series_AT Command Manual_V1.09
www.simcom.com 315 /652
15.2.1 AT+NETOPEN StartSocketService
AT+NETOPENis used tostart serviceby activating PDPcontext.You mustexecuteAT+NETOPEN before
anyotherTCP/UDPrelated operations.
AT+NETOPEN StartSocketService
Read Command
AT+NETOPEN?
Response
+NETOPEN:<net_state>
OK
Execute Command
AT+NETOPEN
Response
1)Ifthe PDPcontexthas notbeen activatedorthe network closed
abnormally,response:
OK
+NETOPEN:<err>
2)Whenthe PDPcontexthas been activatedsuccessfully,ifyou
executeAT+NETOPENagain,response:
+IPERROR: Networkisalready opened
ERROR
3)other:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime
Range:3000ms-120000ms
default:120000ms
(itcanbe setbyAT+CIPTIMEOUT)
Reference 3GPPTS27.005
DefinedValues
<net_state> Integertype,indicatesthe stateof PDPcontext activation.
0 network close (deactivated)
1 network open(activated)
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 for
details
Examples
AT+NETOPEN?
+NETOPEN:1


## Página 317

A76XX Series_AT Command Manual_V1.09
www.simcom.com 316 /652
OK
AT+NETOPEN
OK
+NETOPEN:0
15.2.2 AT+NETCLOSE StopSocketService
AT+NETCLOSEisusedtostopservicebydeactivatingPDPcontext.Itcanalsoclosealltheopenedsocket
connections whenyou didn’tclosethese connections byAT+CIPCLOSE.
AT+NETCLOSE StopSocketService
TestCommand
AT+NETCLOSE=?
Response
OK
Execute Command
AT+NETCLOSE
Response
1)Ifthe PDPcontexthas been activated,response:
OK
+NETCLOSE:<err>
2)Ifthe PDPcontexthas been activatedandone connectionis in
non-transparentmode whenservice type isTCP,response:
OK
+CIPCLOSE:<link_num>,<err>
+NETCLOSE:<err>
3)Ifthe PDPcontexthas been activatedandone connectionis in
transparentmode whenservice type isTCP,response:
OK
CLOSED
+CIPCLOSE:<link_num>,<err>
+NETCLOSE:<err>
4)Ifthe PDPcontexthas been activatedandone connectionis in
non-transparentmode whenservice type is UDP,response:
+CIPCLOSE:<link_num>,<err>
OK


## Página 318

A76XX Series_AT Command Manual_V1.09
www.simcom.com 317 /652
+NETCLOSE:<err>
5)Ifthe PDPcontexthas notbeen activated,response:
+NETCLOSE:<err>
ERROR
6)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime
Range:3000ms-120000ms
default:120000ms
(itcanbe setbyAT+CIPTIMEOUT)
Reference
DefinedValues
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 for
details
Examples
AT+NETCLOSE
OK
+NETCLOSE:0
15.2.3 AT+CIPOPEN EstablishConnectioninMulti-SocketMode
YoucanuseAT+CIPOPENtoestablish aconnectionwithTCPserverandUDPserver,the maximumof the
connections is10.
AT+CIPOPEN EstablishConnectioninMulti-SocketMode
TestCommand
AT+CIPOPEN=?
Response
+CIPOPEN:(0-9),("TCP","UDP","UDPSERVER")
OK
Read Command
AT+CIPOPEN?
Response
+CIPOPEN:
<link_num>[,<type>,<serverIP>,<serverPort>,<index>]
+CIPOPEN:
<link_num>[,<type>,<serverIP>,<serverPort>,<index>]


## Página 319

A76XX Series_AT Command Manual_V1.09
www.simcom.com 318 /652
[…]
OK
Ifaconnectionidentified by<link_num>has notbeen established
successfully,only+CIPOPEN: <link_num>willbe returned.
WriteCommand
TCPconnection
AT+CIPOPEN=<link_num>,"TC
P",<serverIP>,<serverPort>[,<lo
calPort>]
Response
1)if PDPcontexthas been activated successfully,response:
OK
+CIPOPEN:<link_num>,<err>
2)when the <link_num>isgreaterthan9,response:
+IPERROR: Invalidparameter
ERROR
3)IfPDPcontexthas notbeen activated,ortheconnection has
been established,orparameteris incorrect,orwhen
AT+CIPMODE=1 isset,the <link_num>isgreaterthan 0,orother
errors,response:
+CIPOPEN:<link_num>,<err>
ERROR
4)Transparentmode forTCPconnection:
Whenyou wanttouse transparentmode totransmitdata,you
shouldsetAT+CIPMODE=1 beforeAT+NETOPEN.Andif
AT+CIPMODE=1 isset,the <link_num>isrestricted tobeonly 0.
ifsuccess
CONNECT[<text>]
iffailure
CONNECTFAIL
5)Others:
ERROR
WriteCommand
UDPConnection
AT+CIPOPEN=<link_num>,"UD
P",,,<localPort>
1)IfPDPcontexthas been activatedsuccessfully,response:
+CIPOPEN:<link_num>,0
OK
2)Whenthe <link_num>isgreaterthan9, response:
+IPERROR: Invalidparameter
ERROR
IfPDPcontexthasnotbeenactivated,ortheconnectionhasbeen
established,orparameteris incorrect,orothererrors,response:
+CIPOPEN:<link_num>,<err>
ERROR
3)Others:


## Página 320

A76XX Series_AT Command Manual_V1.09
www.simcom.com 319 /652
ERROR
WriteCommand
UDPserver Connection
AT+CIPOPEN=<link_num>,"UD
PSERVER",,,<localPort>
1)IfPDPcontexthas been activatedsuccessfully,response:
+CIPOPEN:<link_num>,0
OK
2)Whenthe <link_num>isgreaterthan9, response:
+IPERROR: Invalidparameter
ERROR
IfPDPcontexthasnotbeenactivated,ortheconnectionhasbeen
established,orparameteris incorrect,orothererrors,response:
+CIPOPEN:<link_num>,<err>
ERROR
3)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime
Range:3000ms-120000ms
default:120000ms
(itcanbe setbyAT+CIPTIMEOUT)
Reference
DefinedValues
<link_num> Integertype,identifiesa connection.Range is0-9.
IfAT+CIPMODE=1 is set,the <link_num>isrestrictedto beonly 0.
<type> String type,identifies the type oftransmissionprotocol.
TCP Transmission ControlProtocol
UDP UserDatagramProtocol
UDPSERVER User Datagram Protocolservice
<serverIP> String type,identifies the IPaddress ofserver.TheIPaddress format
consistsof 4octets,separatedby decimalpoint, like
"AAA.BBB.CCC.DDD".Also the domainnameis supportedhere.
<serverPort> Integertype,identifiesthe portofTCPserver,rangeis 0-65535.
NOTE:
Whenopen portasTCP,theport mustbethe openedTCPport;
Whenopen portas UDP,the portmay beany port.
<localPort> Integertype,identifiesthe portoflocalsocket, range is0-65535.
<index> Integertype,indicateswhetherthe moduleis used as aclientor
server.Whenusedas server,the range is0-3,<index>isthe server
index towhichthe clientis linked.
-1 TCPclient
0-3 TCPserver index
<text> String type,indicates CONNECTresultcode.


## Página 321

A76XX Series_AT Command Manual_V1.09
www.simcom.com 320 /652
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 for
details
Examples
AT+CIPOPEN=?
+CIPOPEN: (0-9),("TCP","UDP","UDPSERVER")
OK
AT+CIPOPEN?
+CIPOPEN: 0
+CIPOPEN: 1,"TCP","183.230.174.137",6031,-1
+CIPOPEN: 2
+CIPOPEN: 3
+CIPOPEN: 4
+CIPOPEN: 5,"UDP","183.230.174.137",6031,-1
+CIPOPEN: 6
+CIPOPEN: 7
+CIPOPEN: 8
+CIPOPEN: 9
OK
AT+CIPOPEN=0,"TCP","183.230.174.137",6031
OK
+CIPOPEN: 0,0
//TCPconnection
AT+CIPOPEN=5,"UDP",,,6031
+CIPOPEN: 5,0
OK
AT+CIPOPEN=2,"UDPSERVER",,,8888
+CIPOPEN: 2,0
OK
//UDPConnection
//UDPserver connection
15.2.4 AT+CIPSEND SenddatathroughTCPorUDPConnection
AT+CIPSENDisusedto send datatoremote side.Ifservice type isTCP,the datais firstlysenttothe
module’sinternalTCP/IPstack,andthensenttoserverbyprotocolstack.The<length>fieldmaybeempty.
Whileitis empty,each<Ctrl+Z>characterpresent inthe data should becoded as <ETX><Ctrl+Z>. Each
<ESC>characterpresentinthe data shouldbe codedas <ETX><ESC>.Each<ETX>characterwillbe


## Página 322

A76XX Series_AT Command Manual_V1.09
www.simcom.com 321 /652
coded as<ETX><ETX>. Single<Ctrl+Z>means end ofthe inputdata.Single <ESC>isused tocancelthe
sending.
<ETX>is0x03, and<Ctrl+Z>is0x1A,<ESC> is0x1B.
AT+CIPSEND SenddatathroughTCPorUDPConnection
TestCommand
AT+CIPSEND=?
Response
+CIPSEND:(0-9),(1-1500)
OK
WriteCommand
Ifservicetype is "TCP",send data
with changeable length
AT+CIPSEND=<link_num>
Response">",then type data to
send, tapCTRL+Z tosenddata,
tap ESCtocancel the operation
Response
1)Ifthe connection identified by<link_num>hasbeen established
successfully,response:
>
<inputdata>
CTRL+Z
OK
+CIPSEND:<link_num>,<reqSendLength>,<cnfSendLength>
2)If<reqSendLength>is equal<cnfSendLength>, itmeans that
the datahas beensenttoTCP/IPprotocolstack successfully.
3)Ifthe connection hasnotbeen established,abnormally closed,
orparameterisincorrect,response:
+CIPERROR:<err>
ERROR
4)Others:
ERROR
WriteCommand
Ifservicetype is "TCP",send data
with fixedlength
AT+CIPSEND=<link_num>,<len
gth>
Response
1)Ifthe connection identified by<link_num>hasbeen established
successfully,response:
>
<inputdatawithspecified length>
OK
+CIPSEND:<link_num>,<reqSendLength>,<cnfSendLength>
2)If<reqSendLength>is equal<cnfSendLength>, itmeans that
the datahas beensenttoTCP/IPprotocolstack successfully.
3)Ifthe connection hasnotbeen established,abnormally closed,
orparameterisincorrect,response:
+CIPERROR:<err>
ERROR
4)Others:
ERROR
WriteCommand Response


## Página 323

A76XX Series_AT Command Manual_V1.09
www.simcom.com 322 /652
Ifservicetypeis"UDP",senddata
with changeable length
AT+CIPSEND=<link_num>,,<ser
verIP>,<serverPort>
Response">",then type data to
send, tapCTRL+Z tosenddata,
tap ESCtocancel the operation
1)Ifthe connection identified by<link_num>hasbeen established
successfully,response:
>
<inputdata>
CTRL+Z
OK
+CIPSEND:<link_num>,<reqSendLength>,<cnfSendLength>
2)Ifthe connection hasnotbeen established,abnormally closed,
orparameterisincorrect,response:
+CIPERROR:<err>
ERROR
3)Others:
ERROR
WriteCommand
Ifservicetypeis"UDP",senddata
with fixedlength
AT+CIPSEND=<link_num>,<len
gth>,<serverIP>,<serverPort>
Response">",type datauntilthe
data lengthis equalto <length>
Response
1)Ifthe connection identified by<link_num>hasbeen established
successfully,response:
>
<inputdatawithspecified length>
OK
+CIPSEND:<link_num>,<reqSendLength>,<cnfSendLength>
2)Ifthe connection hasnotbeen established,abnormally closed,
orparameterisincorrect,response:
+CIPERROR:<err>
ERROR
3)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime
Range:3000ms-120000ms
default:120000ms
(itcanbe setbyAT+CIPTIMEOUT)
Reference
DefinedValues
<link_num> Integertype,identifiesa connection.Range is0-9.
<length> Integertype,indicatesthe lengthof sending data,range is1-1500.
<serverIP> String type,identifies the IPaddress ofserver.TheIPaddress format
consistsof 4octets,separatedby decimalpoint, like
"AAA.BBB.CCC.DDD".Also the domainnameis supportedhere.


## Página 324

A76XX Series_AT Command Manual_V1.09
www.simcom.com 323 /652
<serverPort> Integertype,identifiesthe portofTCPserver,rangeis 0-65535.
NOTE:
Whenopen portasTCP,theport mustbethe openedTCPport;
Whenopen portas UDP,the portmay beany port.
But,forQualcomm,connecting the port0isregarded as aninvalid
operation.
<reqSendLength> Integertype,the length ofthe data requestedtobe sent
<cnfSendLength> Integertype,the length ofthe data confirmedtohavebeen sent
-1 theconnection isdisconnected.
0own sendbufferor otherside’scongestionwindow arefull.
Note: Ifthe <cnfSendLength>isnotequal tothe <reqSendLength>,
the socketthen cannotbe used further.
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 for
details
Examples
AT+CIPSEND=?
+CIPSEND:(0-9),(1-1500)
OK
AT+CIPSEND=1,5
>12345
OK
+CIPSEND:1,5,5
//Ifservice type is "TCP",send datawith
fixedlength
AT+CIPSEND=8,5,"183.230.174.137",6031
>12345
OK
+CIPSEND:8,5,5
//Ifservice type is "UDP", senddata with
fixedlength
Ifyou use UDPto send morethan 1400 bytesof datawhen theserver does notreceivedata,this may
be the reasonforthe carrier,inthis casepleasesend no morethan 1400 bytesof data.
Ifyou useTCPtosenddata,theinstructioncanbefollowed byacommajustlike"AT+CIPSEND=0,"or
"AT+CIPSEND=0,10,"withoutan error,butitdoesn'tmakeany sense
NOTE


## Página 325

A76XX Series_AT Command Manual_V1.09
www.simcom.com 324 /652
15.2.5 AT+CIPRXGET SettheModetoRetrieveData
Ifset<mode>to 1,afterreceiving data,the modulewillbufferitand reportaURCas "+CIPRXGET:
1,<link_num>"tonotifythehost.Then hostcan retrieve databyAT+CIPRXGET.
Ifset<mode>to 0,the received datawillbe outputtedto COMportdirectly byURCas "RECVFROM:<IP
ADDRESS>:<PORT><CR><LF>+IPD(data length)<CR><LF><data>".
Thedefaultvalue of<mode>is 0.
AT+CIPRXGET SettheModetoRetrieveData
TestCommand
AT+CIPRXGET=?
Response
+CIPRXGET:(0-5),(0-9),(1-1500)
OK
Read Command
AT+CIPRXGET?
Response
+CIPRXGET:<mode>
OK
WriteCommand
AT+CIPRXGET=<mode>
Inthiscase,<mode>canonlybe0
or1
Response
1)Ifthe parameteris correct,response:
OK
2)Ifthe parameteris incorrectorothererror,response:
+IPERROR: <err_info>
ERROR
3)Others:
ERROR
WriteCommand
AT+CIPRXGET=2,<link_num>[,<
len>]
Retrievedata inACSIIform
1)If<len> field isempty,the defaultvalue toreadis 1500.
Ifthe bufferisnot empty,response:
+CIPRXGET:<mode>,<link_num>,<read_len>,<rest_len>
<data>ACSIIform
OK
2)Ifthe bufferisempty,response:
+IPERROR: Nodata
ERROR
3)Ifthe parameteris incorrectorothererror,response:
+IPERROR: <err_info>
ERROR
4)Others:
ERROR
WriteCommand Response


## Página 326

A76XX Series_AT Command Manual_V1.09
www.simcom.com 325 /652
AT+CIPRXGET=3,<link_num>[,<
len>]
Retrievedata inhex form
1)If<length>field is empty,the defaultvalueto readis 750.
Ifthe bufferisnot empty,response:
+CIPRXGET:<mode>,<link_num>,<read_len>,<rest_len>
<data>
hexform
OK
2)Ifthe bufferisempty,response:
+IPERROR: Nodata
ERROR
3)Ifthe parameteris incorrectorothererror,response:
+IPERROR: <err_info>
ERROR
4)Others:
ERROR
WriteCommand
AT+CIPRXGET=4,<link_num>
Response
1)Ifthe parameteris correct,response:
+CIPRXGET:4,<link_num>,<rest_len>
OK
2)Ifthe parameteris incorrectorothererror,response:
+IPERROR: <err_info>
ERROR
3)Others
ERROR
WriteCommand
AT+CIPRXGET=5
Response
1)Ifthe parameteris correct,response:
OK
2)Ifthe parameteris incorrectorothererror,response:
+IPERROR: <err_info>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9s
Reference
DefinedValues
<mode> Integertype,sets the mode toretrievedata


## Página 327

A76XX Series_AT Command Manual_V1.09
www.simcom.com 326 /652
0 set theway toget the networkdata automatically
1 set theway toget the networkdata manually
2 read data,the maxread length is1500
3 read datain HEXform,the maxread lengthis 750
4 get therestdata length
5 add IPheaderreportingbased on mode=1,the formatis
+CIPRXGETIP:<ip>:<port>
<link_num> Integertype,identifiesa connection.Range is0-9.
<len> Integertype,the data lengthto beread.
Not required,the defaultvalue is1500 when <mode>=2,and 750
when <mode>=3.
<read_len> Integertype,the length ofdata thathasbeen read.
<rest_len> Integertype,the length ofdata whichhas notbeen readin the buffer.
<err_info> String type,displays thecauseof occurringerror,pleaserefer to
Chapter15.3.1formoredetails.
Examples
AT+CIPRXGET=?
+CIPRXGET:(0-5),(0-9),(1-1500)
OK
AT+CIPRXGET?
+CIPRXGET:1
OK
AT+CIPRXGET=1
OK
AT+CIPRXGET=2,0
+CIPRXGET:2,0,6,0
123456
OK
AT+CIPRXGET=3,0
+CIPRXGET:3,0,6,0
313233343536
OK
AT+CIPRXGET=4,0
+CIPRXGET:4,0,18
OK
AT+CIPRXGET=5
OK


## Página 328

A76XX Series_AT Command Manual_V1.09
www.simcom.com 327 /652
1.Whendata isreceivedandreported, the maximumlength of<data length>is1500 eachtime.
2.WhenAT+CIPRXGET=1,ifmultiple tcp/udpconnections areconnected and the serveris sending
data allthetime, itis necessarytoread the datasentby the serverintime.
15.2.6 AT+CIPCLOSE CloseTCPorUDPSocket
AT+CIPCLOSEis used toclose aTCPorUDPSocket
AT+CIPCLOSE CloseTCPorUDPSocket
TestCommand
AT+CIPCLOSE=?
Response
+CIPCLOSE:(0-9)
OK
Read Command
AT+CIPCLOSE?
Response
+CIPCLOSE:
<link0_state>,<link1_state>,<link2_state>,<link3_state>,<link
4_state>,<link5_state>,<link6_state>,<link7_state>,<link8_sta
te>,<link9_state>
OK
WriteCommand
AT+CIPCLOSE=<link_num>
Response
1)Ifservice type isTCPand theconnection identified by
<link_num>hasbeen established,response
OK
+CIPCLOSE:<link_num>,<err>
2)IfservicetypeisTCPandtheaccessmodeistransparentmode,
response:
OK
CLOSED
+CIPCLOSE:<link_num>,<err>
3)Ifservice type is UDPand the connectionidentified by
<link_num>hasbeen established andclosedsuccessfully,
response:
+CIPCLOSE:<link_num>,0
NOTE


## Página 329

A76XX Series_AT Command Manual_V1.09
www.simcom.com 328 /652
OK
4)Ifservice type is UDPand access modeis transparentmode,
response:
CLOSED
+CIPCLOSE:<link_num>,<err>
OK
5)Ifthe connection hasnotbeen established,abnormally closed,
orparameterisincorrect,response:
+CIPCLOSE:<link_num>,<err>
ERROR
6)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime
Range:3000ms-120000ms
default:120000ms
(itcanbe setbyAT+CIPTIMEOUT)
Reference
DefinedValues
<link_num> Integertype,identifiesa connection.Range is0-9.
<linkX_state> Integertype,indicatesstate ofconnection identifiedby <link_num>.
Range is0-1.
0 disconnected
1 connected
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 for
details
Examples
AT+CIPCLOSE=?
+CIPCLOSE:(0-9)
OK
AT+CIPCLOSE?
+CIPCLOSE:0,0,0,0,0,1,0,0,1,0
OK


## Página 330

A76XX Series_AT Command Manual_V1.09
www.simcom.com 329 /652
AT+CIPCLOSE=0
OK
+CIPCLOSE:0,0
15.2.7 AT+IPADDR InquireSocketPDPaddress
AT+IPADDRisused toget active PDPaddress.
AT+IPADDR InquireSocketPDPAddress
TestCommand
AT+IPADDR=?
Response
OK
Execute Command
AT+IPADDR
Response
1)IfPDPcontexthas been activatedsuccessfully,response
+IPADDR:<ip_address>
OK
2)
+IPERROR: Networknotopened
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<ip_address> String type,identifies the IPaddress ofcurrent activesocket PDP.
Examples
AT+IPADDR
+IPADDR:10.84.17.161
OK


## Página 331

A76XX Series_AT Command Manual_V1.09
www.simcom.com 330 /652
15.2.8 AT+CIPHEAD AddanIPHeaderWhenReceivingData
AT+CIPHEADisusedto add an IPheaderwhenreceiving data.
AT+CIPHEAD AddanIPHeaderWhenReceivingData
TestCommand
AT+CIPHEAD=?
Response
+CIPHEAD:(0-1)
OK
Read Command
AT+CIPHEAD?
Response
+CIPHEAD:<mode>
OK
WriteCommand
AT+CIPHEAD=<mode>
Response
1)Ifthe parameteris correct,response:
OK
2)
ERROR
Execute Command
AT+CIPHEAD
Response
Setdefaultvalue:(<mode>=1)
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<mode> Integertype,indicateswhetheraddingan IPheaderornotwhen
receiving data
0 not add IPheader
1 add IPheader,the format is"+IPD(data length)"
Examples
AT+CIPHEAD=?
+CIPHEAD: (0-1)
OK
AT+CIPHEAD?
+CIPHEAD: 1
OK


## Página 332

A76XX Series_AT Command Manual_V1.09
www.simcom.com 331 /652
AT+CIPHEAD=1
OK
AT+CIPHEAD
OK
15.2.9 AT+CIPSRIP ShowRemoteIPAddressandPort
AT+CIPSRIPis usedtosetwhethertodisplay IPaddress andportofserver whenreceiving data.
AT+CIPSRIP ShowRemoteIPAddressandPort
TestCommand
AT+CIPSRIP=?
Response
+CIPSRIP:(0-1)
OK
Read Command
AT+CIPSRIP?
Response
+CIPSRIP:<mode>
OK
WriteCommand
AT+CIPSRIP=<mode>
Response
1)Ifthe parameteris correct,response:
OK
2)
ERROR
Execute Command
AT+CIPSRIP
Response
Setdefaultvalue:(<mode>=1)
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<mode> Integertype,indicateswhethertoshowIPaddressandportof server
or notwhen receivingdata.
0 not show
1 show,theformat isas follows:
"RECV FROM:<IPADDRESS>:<PORT>"
Examples


## Página 333

A76XX Series_AT Command Manual_V1.09
www.simcom.com 332 /652
AT+CIPSRIP=?
+CIPSRIP:(0-1)
OK
AT+CIPSRIP?
+CIPSRIP:1
OK
AT+CIPSRIP=0
OK
AT+CIPSRIP
OK
15.2.10AT+CIPMODE SetTCP/IPApplicationMode
AT+CIPMODEis used toselecttransparentmode(datamode) ornon-transparentmode(command
mode).Thedefaultmode isnon-transparentmode.
AT+CIPMODE SetTCP/IPApplicationMode
TestCommand
AT+CIPMODE=?
Response
+CIPMODE:(0-1)
OK
Read Command
AT+CIPMODE?
Response
+CIPMODE:<mode>
OK
WriteCommand
AT+CIPMODE=<mode>
Response
1)Ifthe parameteris correct,response:
OK
2)
ERROR
Execute Command
AT+CIPMODE
Response
1)Setdefaultvalue:(<mode>=0)
OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -


## Página 334

A76XX Series_AT Command Manual_V1.09
www.simcom.com 333 /652
DefinedValues
<mode> Integertype,setsTCP/IPapplication mode
0 Non transparentmode
1 Transparent mode
Examples
AT+CIPMODE=?
+CIPMODE:(0-1)
OK
AT+CIPMODE?
+CIPMODE:0
OK
AT+CIPMODE=1
OK
AT+CIPMODE
OK
Whenyou wantto usetransparentmodetotransmitdata,you shouldsetAT+CIPMODE=1before
AT+NETOPEN.
15.2.11AT+CIPSENDMODE SetSendingMode
AT+CIPSENDMODEis used toselectsending modewhen servicetype is"TCP".
Ifset<mode>to 1,whensending data byAT+CIPSEND,the URC"+CIPSEND:
<link_num>,<reqSendLength>,<cnfSendLength>"will not be returned until module receives the server’s
ACKmessage tothe sentdatalasttime.
Ifset<mode>to 0,the URC"+CIPSEND: <link_num>,<reqSendLength>,<cnfSendLength>"willbe
returnedIfthe data has been senttomodule’sinternalTCP/IPprotocol stack.Inthis case,the module
doesn’tneed towaitforthe server’sACK message.
Thedefaultmodeis sending withoutwaiting peerTCPACKmode.
AT+CIPSENDMODE SetSendingMode
TestCommand
AT+CIPSENDMODE=?
Response
+CIPSENDMODE:(0-1)
NOTE


## Página 335

A76XX Series_AT Command Manual_V1.09
www.simcom.com 334 /652
OK
Read Command
AT+CIPSENDMODE?
Response
+CIPSENDMODE:<mode>
OK
WriteCommand
AT+CIPSENDMODE=<mode>
Response
1)Ifthe parameteris correct,response:
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<mode> Integertype,sets sending mode
0 sending without waitingpeerTCPACKmode
1 sending waitpeerTCPACKmode
Examples
AT+CIPSENDMODE=?
+CIPSENDMODE:(0-1)
OK
AT+CIPSENDMODE=1
OK
AT+CIPSENDMODE?
+CIPSENDMODE:1
OK
15.2.12AT+CIPTIMEOUT SetTCP/IPTimeoutValue
AT+CIPTIMEOUTisusedto settimeoutvalueforAT+NETOPEN/AT+CIPOPEN/AT+CIPSEND.
AT+CIPTIMEOUT SetTCP/IPTimeoutValue
Read Command Response


## Página 336

A76XX Series_AT Command Manual_V1.09
www.simcom.com 335 /652
AT+CIPTIMEOUT? +CIPTIMEOUT:
<netopen_timeout>,<cipopen_timeout>,<cipsend_timeout>
OK
WriteCommand
AT+CIPTIMEOUT=[<netopen_ti
meout>][,[<cipopen_timeout>][,
[<cipsend_timeout>]]]
Response
1)Ifthe parameteris correct,response:
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<netopen_timeout> Integertype,timeoutvalue forAT+NETOPEN.
defaultis120000ms.Range is 3000ms-120000ms.
<cipopen_timeout> Integertype,timeoutvalue forAT+CIPOPEN.
defaultis120000ms.Range is 3000ms-120000ms.
<cipsend_timeout> Integertype,timeoutvalue forAT+CIPSEND.
defaultis120000ms.Range is 3000ms-120000ms.
Examples
AT+CIPTIMEOUT?
+CIPTIMEOUT:120000,120000,120000
OK
AT+CIPTIMEOUT=3000,3000,3000
OK
15.2.13AT+CIPCCFG ConfigureParametersofSocket
AT+CIPCCFGis usedtoconfigure parametersof socket.
AT+CIPCCFG ConfigureParametersofSocket
TestCommand
AT+CIPCCFG=?
Response
+CIPCCFG:(0-10),(0-1000),(0),(0-1),(0-1),(0-1),(500-120000),
(0-1)


## Página 337

A76XX Series_AT Command Manual_V1.09
www.simcom.com 336 /652
OK
Read Command
AT+CIPCCFG?
Response
+CIPCCFG:
<NmRetry>,<DelayTm>,<Ack>,<errMode>,<Header-Type>,<As
yncMode>,<TimeoutVal>,<udpRecvmode>
OK
WriteCommand
AT+CIPCCFG=[<NmRetry>][,[<
DelayTm>][,[<Ack>][,[<errMode
>][,[<HeaderType>][,[[<AsyncM
ode>][,[<TimeoutVal>][,[<udpRe
cvmode>]]]]]]]]]
Response
1)Ifthe parameteris correct,response:
OK
2)
ERROR
Execute Command
AT+CIPCCFG
Response
Setdefaultvalue:
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<NmRetry> Integer type, number of retransmission to be made for an IP
packet. Range is0-10.Thedefaultvalue is 10.
<DelayTm> Integer type, number of milliseconds to delay to output data of
Receiving.Rangeis 0-1000.Thedefaultvalue is0.
<Ack> Integer type, it can only be set to 0. It’s used to be compatible with
oldTCP/IPcommandset.
<errMode> Integertype,sets modeofreporting <err_info>,defaultvalue is1.
0 errorresult code with numericvalues
1 errorresult code with string values
<HeaderType> Integer type, select which data header is used when receiving
data,itonly takeseffectin multi-clientmode. Defaultvalue is0.
0 add dataheader,theformat is"+IPD<data length>"
1 add dataheader,theformat is"+RECEIVE,<linknum>,<data
length>"
<AsyncMode> Integertype,rangeis 0-1.Defaultvalue is0.
It’sused tobe compatiblewith oldTCP/IPcommand set.
<TimeoutVal> Integer type, set the minimum retransmission timeout value for
TCPconnection.Range is 500ms-120000ms.Default is 500ms.
<udpRecvmode> Integertype,rangeis0-1.Defaultvalue is0.Usedtosetthemode
of udp cachereceiving data.Thisparametermustbe setbefore
AT+NETOPEN.
0 Receive bybuf


## Página 338

A76XX Series_AT Command Manual_V1.09
www.simcom.com 337 /652
1 Receive bypackage
Examples
AT+CIPCCFG=?
+CIPCCFG:(0-10),(0-1000),(0),(0-1),(0-1),(0-1),(500-120000),(0-1)
OK
AT+CIPCCFG?
+CIPCCFG:10,0,0,1,0,0,500,0
OK
AT+CIPCCFG=2
OK
AT+CIPCCFG
OK
15.2.14AT+SERVERSTART StartupTCPSever
AT+SERVERSTARTis used tostartup aTCPserver,andthe server canreceivethe requestofTCPclient.
Afterthe command executessuccessfully,an unsolicited resultcode isreturned whenaclient triesto
connectwith moduleandmodule acceptsrequest.The unsolicitedresultcode is+CLIENT:
<link_num>,<server_index>,<client_IP>:<port>.
AT+SERVERSTART StartupTCPSever
TestCommand
AT+SERVERSTART=?
Response
+SERVERSTART:(0-65535),(0-3)
OK
Read Command
AT+SERVERSTART?
Response
1)Ifthe PDPcontexthas notbeen activatedsuccessfully,
response:
+CIPERROR:<err>
ERROR
2)Ifthere exists openedserver,response:
[+SERVERSTART:<server_index>,<port>
…]
OK
3)Others:


## Página 339

A76XX Series_AT Command Manual_V1.09
www.simcom.com 338 /652
ERROR
WriteCommand
AT+SERVERSTART=<port>,<se
rver_index>[,<backlog>]
Response
1)Ifthere isno error,response:
OK
2)Ifthe PDPcontexthas notbeen activated,orthe server
identifiedby <server_index>has been opened,orthe parameter
isnotcorrect,orothererrors,response:
+CIPERROR:<err>
ERROR
3)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<port> Integertype,identifies the listeningport ofmodule when used as a
TCPserver.Rangeis 0-65535.
<server_index> Integertype,theTCPserverindex,rangeis 0-3.
<backlog> Integertype,the maximum connectionscan bequeued inlistening
queue.Rangeis 1-3.Defaultis 3.
Examples
AT+SERVERSTART=?
+SERVERSTART:(0-65535),(0-3)
OK
AT+SERVERSTART?
OK
AT+SERVERSTART=8080,0
OK
15.2.15AT+SERVERSTOP StopTCPSever
AT+SERVERSTOPis used tostopTCPserver.Before stopping aTCPserver,allsockets <server_index>
ofwhichequalsto the closingTCPserverindexmust beclosedfirst.


## Página 340

A76XX Series_AT Command Manual_V1.09
www.simcom.com 339 /652
AT+SERVERSTOP StopTCPSever
WriteCommand
AT+SERVERSTOP=<server_ind
ex>
Response
1)Ifthere exists open connection with theserver identifiedby
<server_index>,ortheserveridentifiedby<server_index>hasnot
been opened,orthe parameteris incorrect,response:
+SERVERSTOP:<server_index>,<err>
ERROR
2)Ifthe server socketis closed immediately,response:
+SERVERSTOP:<server_index>,0
OK
(Ingeneral,the result isshown asbelow.)
3)Ifthe server socketstartstoclose,response:
OK
+SERVERSTOP:<server_index>,<err>
4)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<server_index> Integertype,theTCPserverindex,rangeis 0-3.
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 fordetails
Examples
AT+SERVERSTOP=0
OK
+SERVERSTOP:0,0
15.2.16AT+CIPACK QueryTCPConnectionDataTransmittingStatus
AT+CIPACKis used toqueryTCPconnection datatransmitting status.


## Página 341

A76XX Series_AT Command Manual_V1.09
www.simcom.com 340 /652
AT+CIPACK QueryConnectionDataTransmittingState
TestCommand
AT+CIPACK=?
Response
+CIPACK:(rangeofsupported <link_num>s)
OK
WriteCommand
AT+CIPACK=<link_num>
Response
1)Ifthe PDPcontexthasnotbeen activated,orthe connection
identifiedby <link_num>has notbeenestablished, abnormally closed,
ortheparameteris incorrect,orothererrors,response:
+IPERROR:<err_info>
ERROR
2)Ifthe connection hasbeen established,and theservice type is
"TCP",response:
+CIPACK:<sent_data_size>,<ack_data_size>,<recv_data_size>
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime default:9000ms
Reference -
DefinedValues
<link_num> Integertype,identifiesa connection.Range is0-9.
<sent_data_size> Integertype,the total lengthof sentdata
<ack_data_size> Integertype,the total lengthof acknowledgeddata.
<recv_data_size> Integertype,the total lengthof received data
<err> Integertype,the resultof operation.
0is success,othervalue isfailure,pleasereferto Chapter15.3.2 fordetails
<err_info> String type,displays thecauseof occurringerror,pleaserefer toChapter3
fordetails.
Examples
AT+CIPACK=?
+CIPACK:(0-9)
OK
AT+CIPACK=0
+CIPACK:10,10,5
OK


## Página 342

A76XX Series_AT Command Manual_V1.09
www.simcom.com 341 /652
15.2.17AT+CDNSGIP QuerytheIPAddressofGivenDomainName
AT+CDNSGIPisusedto query the IPaddress ofgiven domainname.
AT+CDNSGIP QuerytheIPAddressofGivenDomainName
TestCommand
AT+CDNSGIP=?
Response
OK
WriteCommand
AT+CDNSGIP=<domain name>
Response
1)Ifthe given domainnamehas relatedIP,response:
+CDNSGIP:1,<domainname>,<IPaddress>
+CDNSGIP:2,<domainname>,<IPaddress>
[…]
OK
2)Ifthe given namehasno related IP,response:
+CDNSGIP:0,<dns errorcode>
ERROR
3)Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime default:6s
Reference -
DefinedValues
<domain name> String type (string should be included in quotation marks), indicates the
domainname.Themaximum lengthof domain nameis 254.
Valid characters allowed in the domain name area include a-z, A-Z, 0-9,
"-" (hyphen)and ".". A domain name is made up of one label name or
morelabel names separatedby "."(eg:AT+CDNSGIP="aa.bb.cc").
For label names separated by ".", length of each label must be no more
than 63 characters. The beginning character of the domain name and of
labels shouldbe analphanumeric character.
<IPaddress> String type, indicatesthe IPaddresscorresponding to the domain name.
<dns errorcode> Integertype, indicatesthe errorcode.
10 DNSGENERALERROR
Examples


## Página 343

A76XX Series_AT Command Manual_V1.09
www.simcom.com 342 /652
AT+CDNSGIP=?
OK
AT+CDNSGIP="www.baidu.com"
+CDNSGIP:1,"www.baidu.com","61.135.169.121"
OK
15.2.18AT+CSOCKSETPN Setactive PDPcontext'sprofile
This command sets default active PDP context's profile number and type. When we activate PDP by using
AT+NETOPENcommand,weneedusethedefaultprofilenumberandtype.,andthecontextofthisprofileis
setbyAT+CGDCONTcommand.
AT+CSOCKSETPN Setacitve PDPcontext'sprofile
TestCommand
AT+CSOCKSETPN=?
Response
+CSOCKSETPN:1,(1,6)
OK
Read Command
AT+CSOCKSETPN?
Response
+CSOCKSETPN:<profile_num>,<ip_family>
OK
WriteCommand
AT+CSOCKSETPN=<profile_nu
m>[,<ip_family>]
Response
1)Ifthe parameteriscorrect,response:
OK
2)If the parameter is wrong,or NETOPEN is already active,
response:
ERROR
ParameterSaving Mode NO_SAVE
Maximum ResponseTime default:9000ms
Reference -
DefinedValues
<profile_num> PacketDataProtocolcontext’s profilenumber.
Now only 1issupportedforthisparametervalue.
<ip_family> PacketDataProtocoltype
1 IPV4
6 IPV6


## Página 344

A76XX Series_AT Command Manual_V1.09
www.simcom.com 343 /652
The A7600 series(1601) cannot change the configuration file number of the PDP context;
<profile_num>canonly be setto1.
Examples
AT+CSOCKSETPN=?
+CSOCKSETPN:1,(1,6)
OK
AT+CSOCKSETPN?
+CSOCKSETPN:1,1
OK
AT+CSOCKSETPN=1,6
OK
15.2.19AT+CTCPKA ConfigureTCPheartbeat
This command is used to set TCP heartbeat parameters. Set this up after we activate PDP by using
AT+NETOPENcommand.
AT+CTCPKA ConfigureTCPheartbeat
TestCommand
AT+CTCPKA=?
Response
OK
Read Command
AT+CTCPKA?
Response
+CTCPKA:
<keepalive>,<keepidle>,<keepcount>,<keepinterval>
OK
WriteCommand
AT+CTCPKA=<keepalive>,<kee
pidle>,<keepcount>[,<keepinter
val>]
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Maximum ResponseTime default:9000ms
Reference -
NOTE


## Página 345

A76XX Series_AT Command Manual_V1.09
www.simcom.com 344 /652
DefinedValues
<keepalive> SetTCPkeepaliveoption.
0DisableTCPkeep alive mechanism
1EnableTCPkeepalive mechanism
<keepidle> Theunit isminute. Ifthereis nodata interactionwithinthisperiod,the
probeis performed.(1-120)
<keepcount> Numberofproberetries.Ifalltimesout,the connection isconsidered
Invalid.(1-10)
<keepinterval> Theunit isminute. Intervalforsending probepackets during probe.
(1-100)
Examples
AT+CTCPKA=1,2,5,1
OK
AT+CTCPKA?
+CTCPKA: 1,2,5,1
OK
15.2.20AT+CDNSCFGConfigureDomainNameServer
This commandis used toconfigureDomain NameServer.
AT+CDNSCFG ConfigureDomainNameServer
TestCommand
AT+CDNSCFG=?
Response
+CDNSCFG:("Primary DNS"),("SecondaryDNS"),type
OK
Read Command
AT+CDNSCFG?
Response
1)Ifsuccessfully:
PrimaryIPv4DNS: <pri_dns>,Secondary IPv4DNS:<pri_dns>
PrimaryIPv6DNS: <pri_dns>,Secondary IPv6DNS:<pri_dns>
OK
2)Iffailed:
ERROR
WriteCommand
AT+CDNSCFG=<pri_dns>[,<sec
Response
1)Ifsuccessfully:


## Página 346

A76XX Series_AT Command Manual_V1.09
www.simcom.com 345 /652
_dns>][,<type>] OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Maximum ResponseTime default:9000ms
Reference -
DefinedValues
<pri_dns> Astringparameter which indicatesthe IPaddress ofthe primary
domain nameserver.
<sec_dns> Astringparameter which indicatesthe IPaddress ofthe
secondary domain nameserver.
<type> 0Setthe serverforthe ipv4 network
1Setthe serverforthe ipv6 network
Examples
AT+CDNSCFG?
Primary IPv4 DNS: 183.230.126.224,Secondary IPv4
DNS:183.230.126.225
Primary IPv6 DNS: 2409:8060:20EA:101::1,Secondary
IPv6DNS:2409:8060:20EA:201::1
OK
AT+CDNSCFG=183.230.126.224,183.230.126.225,0
OK
15.2.21AT+CSOC Setsomefeaturesofthedataservice
This commandis used tosetsomefeatures ofthe dataservice.Onlysupports in Cat1modules.
AT+CSOC Setsomefeaturesofthedataservice
TestCommand
AT+CSOC=?
Response
OK
Read Command
AT+CSOC?
Response
+CSOC:"Tcpsrxt",<srxt>,<tcp_sync_backoff>
+CSOC:"Tcprxt",<rtx>,<tcp_backoff>
+CSOC:"Mss",<mtu>
+CSOC:"RecvWin",<win>
+CSOC:
"DnsTmr",<dnsMaxWaitTime>,<dnsMaxRetry>,<dns_wait_tmr>


## Página 347

A76XX Series_AT Command Manual_V1.09
www.simcom.com 346 /652
OK
WriteCommand
AT+CSOC="Tcp
srxt",<tcp_sync_backoff>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+CSOC="Tcp
rxt",<tcp_backoff>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+CSOC="Mss",<mtu>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+CSOC="RecvWin",<win>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+CSOC="DnsTmr",<dns_
wait_tmr>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<srxt> Maximum numberofTCPSYN packetsretransmitted.
Themaximum valueis 12.Thedefaultis 6.
<tcp_sync_backoff> Setthe intervalforTCPSYN packetretransmission.
tcp_sync_backoff[13].defaults to {1,1,2,3, 3,6,12,24,48,96,120,
120, 120}
<rtx> Maximum numberofTCPdata packets retransmitted.
Themaximum valueis 12.Thedefaultis 6.
<tcp_backoff> Setthe intervalforTCPdatapackageretransmission.
tcp_backoff_default [13].defaults to {1,1, 2,2,3, 4,5,6,7,7, 7,7,7}
<mtu> Integertype,Maximumvalue 1500,minimumvalue 640.


## Página 348

A76XX Series_AT Command Manual_V1.09
www.simcom.com 347 /652
<win> TCPSlidingWindow.
<dnsMaxWaitTime> Maximum timeoutforDNSresolution retries
<dnsMaxRetry> Maximum numberofDNSresolution retries
Note:Thevalueranges from2 to8.
<dns_wait_tmr> Setthe DNSretryinterval.Unitis second.
Dns_wait_tmr[8]defaults to{2,3, 4,8, 8,8,8,8}.
Note:Thefirstintervalcannotbe lessthan 2seconds.
Examples
AT+CSOC?
+CSOC:"Tcpsrxt",6,1,1,2,3,3,6,12,24,48,96,120,120,120
+CSOC:"Tcprxt",6,1,1,2,2,3,4,5,6,7,7,7,7,7
+CSOC:"Mss",1500
+CSOC:"RecvWin",64240
+CSOC:"DnsTmr",5,2,2,3,4,8,8,8,8,8
OK
AT+CSOC="DnsTmr","2,2,0,0,0,0,0,0"
OK
15.2.22AT+CIPCFG ConfigureParametersofTcp
AT+CIPCFG isusedtoconfigureparameters ofTcp.
AT+CIPCFG ConfigureParametersofTcp
TestCommand
AT+CIPCFG=?
Response
+CIPCFG:"transwaittm",(5-1000)
+CIPCFG:"DATAECHO","ON/OFF"
OK
Read Command
AT+CIPCFG?
Response
+CIPCFG:"transwaittm",<transwaittime>
+CIPCFG:"DATAECHO",<dataecho>
OK
WriteCommand
AT+CIPCFG="transwaittm
",<transwaittime>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand Response


## Página 349

A76XX Series_AT Command Manual_V1.09
www.simcom.com 348 /652
AT+CIPCFG="DATAECHO",<
dataecho>
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<transwaittime> Setthe transparenttransmissiontimeoutwaiting time,integertype.
Range 5-1000ms,defaultvalueis 100ms.
<dataecho> Setthe echo ofdata contentsentbyCIPSEND, String type. Default
value isON.
ON Turnon echo
OFF Turnoffecho
Examples
AT+CIPCFG=?
+CIPCFG:"transwaittm",(5-1000)
+CIPCFG:"DATAECHO","ON/OFF"
OK
AT+CIPCFG?
+CIPCFG:"transwaittm",100
+CIPCFG:"DATAECHO",ON
OK
AT+CIPCFG="DATAECHO",OFF
OK
15.2.23AT+CIPSENDSTR SendHEXStringData
This commandis used tosendhex string data,and is notsuitable forthe "UDPserver"and"TCPserver"
types ofSocket services.
AT+CIPSENDSTR SendHEXStringData
TestCommand Response


## Página 350

A76XX Series_AT Command Manual_V1.09
www.simcom.com 349 /652
AT+CIPSENDSTR=? +CIPSENDSTR:(0-9),<hex_string>
OK
WriteCommand
AT+CIPSENDSTR=<link_num
>,<hex_string>
Response
Ifthe datais successfully sent
OK
+CIPSENDSTR:<link_num>,<data_len>
Others:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<link_num> Integertype,identifiesa connection.Range is0-9.
<hex_string> Stringtype,hex string,the max length is500.
<data_len> Integertype,length ofhex_string sent.
Examples
AT+CIPSENDSTR=?
+CIPSENDSTR:(0-9),<hex_string>
OK
AT+CIPOPEN=0,"TCP","183.230.174.137",6044
OK
+CIPOPEN: 0,0
AT+CIPSENDSTR=0,"4142434445"
OK
+CIPSENDSTR:0,5


## Página 351

A76XX Series_AT Command Manual_V1.09
www.simcom.com 350 /652
15.3 CommandResultCodes
15.3.1 Descriptionof<err_info>
The fourth parameter <errMode> of AT+CIPCCFG (TODO)is used to determine how <err_info> is
displayed.
If<errMode>is setto0, the<err_info>is displayed with numericvalue.
If<errMode>is setto 1,the <err_info>is displayed with stringvalue.
Thedefaultis displayed with string value.
NumericValue StringValue
0 Connectiontimeout
1 Bind portfailed
2 Portoverflow
3 Create socketfailed
4 Networkis alreadyopened
5 Networkis alreadyclosed
6 Noclients connected
7 Noactive client
8 Networknotopened
9 Clientindex overflow
10 Connectionis already created
11 Connectionis not created
12 Invalid parameter
13 Operation notsupported
14 DNSquery failed
15 TCPbusy
16 Net closefailed forsocketopened
17 Sending timeout
18 Sending failurefornetworkerror
19 Open failurefornetworkerror
20 Serveris already listening
21 Operation failed
22 Nodata
15.3.2 Descriptionof<err>
<err> Descriptionof<err>


## Página 352

A76XX Series_AT Command Manual_V1.09
www.simcom.com 351 /652
0 operation succeeded
1 Networkfailure
2 Networknotopened
3 Wrong parameter
4 Operation notsupported
5 Failed tocreate socket
6 Failed tobind socket
7 TCPserveris alreadylistening
8 Busy
9 Sockets opened
10 Timeout
11 DNSparse failedforAT+CIPOPEN
12 Unknown error
15.4 UnsolicitedResultCodes
URC Description
+CIPEVENT:NETWORK CLOSEDUNEXPECTEDLY Network isclosedfornetworkerror(Out of
service,etc). When thiseventhappens,
user’sapplicationneeds tocheckand close
allopened sockets,andthen uses
AT+NETCLOSEtorelease thenetwork
libraryifAT+NETOPEN? shows the network
libraryis stillopened.
+IPCLOSE:<client_index>,<close_reason> Socketis closedpassively.
<client_index>is thelink number.
<close_reason>:
0 Closedby local, active
1 Closedby remote,passive
2 Closedforsending timeoutorDTRoff
+CLIENT:
<link_num>,<server_index>,<client_IP>:<port>
TCPserveracceptedanewsocketclient,the
indexis<link_num>,theTCPserver indexis
<server_index>.ThepeerIPaddressis
<client_IP>,the peerportis <port>.


## Página 353

A76XX Series_AT Command Manual_V1.09
www.simcom.com 352 /652
ATCommandsforHTTP(S)
16.1 OverviewofATCommandsforHTTP(S)
Command Description
AT+HTTPINIT StartHTTPservice
AT+HTTPTERM Stop HTTPService
AT+HTTPPARA SetHTTPParameters value
AT+HTTPACTION HTTPMethodAction
AT+HTTPHEAD Read the HTTPHeaderInformationofServerResponse
AT+HTTPREAD Read the responseinformationofHTTPServer
AT+HTTPDATA InputHTTPData
AT+HTTPPOSTFILE Send HTTPRequestto HTTP(S)serverby File
AT+HTTPREADFILE Receive HTTPResponseContenttoafile
16.2 DetailedDescriptionofATCommandsforHTTP(S)
16.2.1 AT+HTTPINIT StartHTTPService
AT+HTTPINITisusedto startHTTPservice by activatingPDPcontext.YoumustexecuteAT+HTTPINIT
beforeany otherHTTPrelatedoperations.
AT+HTTPINIT StartHTTPService
TestCommand
AT+HTTPINIT=?
Response
OK
Execute Command
AT+HTTPINIT
Response
1)Ifstart HTTPservicesuccessfully:
OK
2)Iffailed:
ERROR


## Página 354

A76XX Series_AT Command Manual_V1.09
www.simcom.com 353 /652
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<err> Thetype oferrorpleaserefer toSection16.4
Examples
AT+HTTPINIT
OK
16.2.2 AT+HTTPTERM StopHTTPService
AT+HTTPTERMisused tostop HTTPservice.
AT+HTTPTERM StopHTTPService
TestCommand
AT+HTTPTERM=?
Response
OK
Execute Command
AT+HTTPTERM
Response
1)Ifstop HTTPservice successfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
Examples
AT+HTTPTERM
OK


## Página 355

A76XX Series_AT Command Manual_V1.09
www.simcom.com 354 /652
16.2.3 AT+HTTPPARA SetHTTPParametersvalue
AT+HTTPPARAis used tosetHTTPparameters value.Whenyou wantto accessto aHTTPserver,you
shouldinput<value>likehttp://’server’/’path’:’tcpPort’.Inaddition,https://’server’/’path’:’tcpPort’is used to
access toaHTTPSserver.
AT+HTTPPARA SetHTTPParametersvalue
TestCommand
AT+HTTPPARA=?
Response
OK
WriteCommand
AT+HTTPPARA="URL",<url>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+HTTPPARA="CONNECTTO"
,<conn_timeout>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+HTTPPARA="RECVTO",<re
cv_timeout>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+HTTPPARA="CONTENT",<c
ontent_type>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+HTTPPARA="ACCEPT",<ac
cept-type>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+HTTPPARA="SSLCFG",<ssl
cfg_id>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
WriteCommand
AT+HTTPPARA="USERDATA",<
user_data>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:


## Página 356

A76XX Series_AT Command Manual_V1.09
www.simcom.com 355 /652
ERROR
WriteCommand
AT+HTTPPARA="READMODE",
<readmode>
Response
1)Ifparameter formatisright:
OK
2)Ifparameter formatisnotright orothererrors occur:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<url> URLof networkresource.String,startwith "http://" or"https://"
a)http://’server’:’tcpPort’/’path’.
b)https://’server’:’tcpPort’/’path’.
"server" DNSdomainnameorIPaddress
"path" path toafileordirectory ofaserver
"tcpPort" httpdefaultvalue is80,https defaultvalueis 443.(can be
omitted)
<conn_timeout> Timeout foraccessingserver,Numeric type,rangeis 20-120s,default
is 120s.
<recv_timeout> Timeoutforreceivingdatafromserver,Numerictyperangeis2s-120s,
defaultis 20s.
<content_type> This is forHTTP"Content-Type"tag,String type,maxlength is 256,
anddefault is"text/plain".
<accept-type> ThisisforHTTP"Accept-type"tag,Stringtype,maxlengthis256,and
defaultis "*/*".
<sslcfg_id> This is setting SSLcontext id,Numeric type,rangeis 0-9.Defaultis
0.Pleasereferto Chapter19 ofthis document.
<user_data> Thecustomized HTTPheaderinformation. String type,max length is
256.
<readmode> ForHTTPREAD,Numeric type, itcanbe setto0or1. Ifsetto1,you
canread the responsecontent datafromthe sameposition repeatly.
Thelimitis thatthe size ofHTTPserver responsecontentshould be
shorterthan1M.Default is0.
Whenyou wantto usecontent-type mutipart/form-datatotransferdata,you shouldset
AT+HTTPPARA="CONTENT","mutipart/form-data".And we willconstructboundaryheader.
NOTE


## Página 357

A76XX Series_AT Command Manual_V1.09
www.simcom.com 356 /652
Examples
AT+HTTPPARA="URL","http://www.baidu.com"
OK
16.2.4 AT+HTTPACTION HTTPMethodAction
AT+HTTPACTIONis used toperform aHTTPMethod.Youcanuse HTTPACTIONtosend aget/post
requestto aHTTP/HTTPSserver.
AT+HTTPACTION HTTPMethodAction
TestCommand
AT+HTTPACTION=?
Response
+HTTPACTION:(0-4)
OK
WriteCommand
AT+HTTPACTION=<method>
Response
1)Ifparameter formatisright:
OK
+HTTPACTION:<method>,<statuscode>,<datalen>
2)Ifparameterformatisrightbutserverconnectedunsuccessfully:
OK
+HTTPACTION:<method>,<errcode>,<datalen>
3)Ifparameter formatisnotright orothererrors occur:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<method> HTTPmethod specification:
0 GET
1 POST
2 HEAD
3 DELETE
4 PUT
<statuscode> Pleasereferto the end ofthis chapter
<datalen> Thelength ofdata received


## Página 358

A76XX Series_AT Command Manual_V1.09
www.simcom.com 357 /652
Examples
AT+HTTPACTION=?
+HTTPACTION: (0-4)
OK
AT+HTTPACTION=0
OK
+HTTPACTION: 0,200,104220
16.2.5 AT+HTTPHEAD ReadtheHTTPHeaderInformationofServerResponse
AT+HTTPHEADisusedtoreadtheHTTPheaderinformationofserverresponsewhenmodulereceivesthe
responsedata fromserver.
AT+HTTPHEAD ReadtheHTTPHeaderInformationofServerResponse
TestCommand
AT+HTTPHEAD=?
Response
OK
Execute Command
AT+HTTPHEAD
Response
1)Ifread the headerinformation successfully:
+HTTPHEAD:<data_len>
<data>
OK
2)Ifread failed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<data_len> Thelength ofHTTPheader
<data> Theheaderinformationof HTTPresponse
Examples
AT+HTTPHEAD
+HTTPHEAD:653


## Página 359

A76XX Series_AT Command Manual_V1.09
www.simcom.com 358 /652
HTTP/1.1 200 OK
Content-Type:text/html
Connection:keep-alive
X-Cache:MISSfrom PDcache-04:opinion.people.com.cn
Date:Tue,24 Mar2020 03:12:09GMT
Powered-By-ChinaCache:HITfrom CNC-WB-b-D24
Powered-By-ChinaCache:HITfrom CNC-WV-b-D1C
ETag:W/"5b7379f5-57e9"
x-cc-via:CNC-WB-b-D24[H,1],CNC-WV-b-D1C[H,62]
d-cc-upstream: CNC-WV-b-D1C
CACHE:TCP_HIT
Vary:Accept-Encoding
Last-Modified:Wed, 15Aug 2018 00:55:17GMT
Expires:Tue,24 Mar2020 03:17:09 GMT
x-cc-req-id:f4b9e1793697d1ef2950f530aeec4519
Content-Length:22505
Age:0
Accept-Ranges: bytes
Server:nginx
X-Frame-Options:ALLOW-FROM .*
CC_CACHE:TCP_REFRESH_HIT
OK
16.2.6 AT+HTTPREAD ReadtheresponseinformationofHTTPServer
Aftersending HTTP(S)GET/POSTrequests,you canretrieveHTTP(S)response information from
HTTP(S)serverviaUART/USBportbyAT+HTTPREAD.Whenthe <datalen>of"+HTTPACTION:
<method>,<statuscode>,<datalen>"isnotequalto 0,Youcan execute
AT+HTTPREAD=<start_offset>,<byte_size>toread outdatato port.Ifparameter<byte_size>is set
greaterthanthe size ofdata saved inbuffer,alldata incache willoutputtoport.
AT+HTTPREAD ReadtheresponseinformationofHTTPServer
TestCommand
AT+HTTPREAD=?
Response
OK
Read Command
AT+HTTPREAD?
Response
1)Ifchecksuccessfully:
+HTTPREAD:LEN,<len>
OK
2)Iffailed (nomore data othererror):
ERROR
WriteCommand Response


## Página 360

A76XX Series_AT Command Manual_V1.09
www.simcom.com 359 /652
AT+HTTPREAD=[<start_offset>,
]<byte_size>
1)Ifread the responseinfosuccessfully:
OK
+HTTPREAD:<data_len>
<data>
+HTTPREAD:0
If<byte_size>isbiggerthanthe data size received,modulewill
onlyreturn actualdata size.
2)Ifread failed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<start_offset> Thestart position ofreading
<byte_size> Thelength ofdata toread
<data_len> Theactuallength ofread data
<data> Responsecontent fromHTTPserver
<len> Totalsize of datasaved inbuffer.
Examples
AT+HTTPREAD?
+HTTPREAD:LEN,22505
OK
AT+HTTPREAD=0,500
OK
+HTTPREAD:500
\0\0\0\0\0\0\0\0\0\0\0\0\0\0<!DOCTYPEhtmlPUBLIC"-//W3C//DTD XHTML1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<htmlxmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type"content="text/html;charset=GB2312"/>
<meta http-equiv="Content-Language"content="utf-8" />
<meta content="all" name="robots" />
<title>人民日报钟声:牢记历史是为了更好开创未来--观点--人民网 </title>
<meta name="keywords"content="" />
<meta name="description"content=" 日方应在正确对待历史?
+HTTPREAD:0


## Página 361

A76XX Series_AT Command Manual_V1.09
www.simcom.com 360 /652
Theresponsecontent received fromserverwillbe saved incache,and would notbecleaned up by
AT+HTTPREAD.
Due tothe max size ofprotocol stackis 64K bytes(the CAT4module is10Kbytes),when thetotal size
ofthe data fromserveris biggerthanthatand ‘READMODE’is 0,you should readthe data quickly,or
you willfailto readit.
16.2.7 AT+HTTPDATA InputHTTPData
YoucanuseAT+HTTPDATAtoinputdata topostwhenyou send aHTTP/HTTPSPOSTrequest.
AT+HTTPDATA InputHTTPData
TestCommand
AT+HTTPDATA=?
Response
OK
WriteCommand
AT+HTTPDATA=<size>,<time>
Response
1)ifparameterformatisright:
DOWNLOAD
<inputdatahere>
Whenthe totalsize ofthe inputted datareaches <size>,TAwill
reportthe followingcode.Otherwise, the serial portwillbe
blocked.
OK
2)Ifparameter formatiswrong orothererrorsoccur:
ERROR
ParameterSaving Mode
Max ResponseTime
Reference
DefinedValues
<size> Size inbytesof the datato post.range is1- 153600 (bytes)
<time> Maximum timein secondstoinputdata.range is 10-65535
Examples
NOTE


## Página 362

A76XX Series_AT Command Manual_V1.09
www.simcom.com 361 /652
AT+HTTPDATA=18,1000
DOWNLOAD
Message=helloworld
OK
16.2.8 AT+HTTPPOSTFILE SendHTTPRequesttoHTTP(S)serverbyFile
YoualsocansendHTTPrequest inafileviaAT+HTTPPOSTFILEcommand.TheURLmustbe setby
AT+HTTPPARAbefore executingAT+HTTPPOSTFILEcommand.Theparameter<path>can beusedto
setthe filedirectory.When modem hasreceivedresponsefrom HTTPserver,itwill reportthefollowing
URC:
+HTTPPOSTFILE:<statuscode>,<datalen>
AT+HTTPPOSTFILE SendHTTPRequesttoHTTP(S)serverbyFile
TestCommand
AT+HTTPPOSTFILE=?
Response
+HTTPPOSTFILE:<filename>[,(1-2)[,(0-4)[,(0-1)]]]
OK
WriteCommand
AT+HTTPPOSTFILE=<filename>
[,<path>[,<method>[,<send_hea
der>]]]
Response
1)if parameterformatisright andserver connected successfully:
a)ifparameter<method>isvalid:
OK
+HTTPPOSTFILE:<method>,<statuscode>,<datalen>
b)if parameter<method>isignored:
OK
+HTTPPOSTFILE:<statuscode>,<datalen>
2)ifparameterformatisrightbutserverconnectedunsuccessfully:
a)ifparameter<method>isvalid:
OK
+HTTPPOSTFILE:<method>,<errcode>,0
b)if parameter<method>isignored:
OK
+HTTPPOSTFILE:<errcode>,0
3)ifparameterformatisnotright oranyothererroroccurs:
ERROR
ParameterSaving Mode
Max ResponseTime
Reference


## Página 363

A76XX Series_AT Command Manual_V1.09
www.simcom.com 362 /652
DefinedValues
<filename> String type,filename,the maxlength is112.unit:byte.
<path> Thedirectory where the sentfile saved.Numeric type, rangeis1-2
1 C:/ (localstorage)
2 D:/(sd card)
<method> HTTPmethodspecification:
0 GET
1 POST
2 HEAD
3 DELETE
4 PUT
Ifthis valueis notprovided, itissametothe valuedescribed in the
postfile.
<send_header> Send file as HTTP header and Body or Only as Body. Numeric type,
the rangeis 0-1,the defaultis 0.
0 Send fileas Body
1 Send fileas HTTPheaderand body
<statuscode> Pleasereferto the end ofthis chapter
<datalen> Thelength ofdata received
Examples
AT+HTTPPOSTFILE=?
+HTTPPOSTFILE:<filename>[,(1-2)[,(0-4)[,(0-1)]]]
OK
AT+HTTPPOSTFILE="getbaidu.txt",1
OK
+HTTPPOSTFILE:200,14615
AT+HTTPPOSTFILE="getbaidu.txt",1,1,1
OK
+HTTPPOSTFILE:1,200,14615
16.2.9 AT+HTTPREADFILE ReceiveHTTPResponseContenttoafile
AfterexecuteAT+HTTPACTION/AT+HTTPOSTFILEcommand.YoucanreceivetheHTTPserverresponse


## Página 364

A76XX Series_AT Command Manual_V1.09
www.simcom.com 363 /652
content toafile viaAT+HTTPREADFILE.
BeforeAT+HTTPREADFILEexecuted,"+HTTPACTION:<method>,<statuscode>,<datalen>"or
"+HTTPPOSTFILE:<statuscode>,<datalen>" mustbereceived.Theparameter<path>canbe usedtoset
the directory where tosave the file.Ifomitparameter<path>,the filewillbe save tolocalstorage.
AT+HTTPREADFILE ReceiveHTTPResponseContenttoaFile
TestCommand
AT+HTTPREADFILE=?
Response
+HTTPREADFILE:<filename>[,(1-2)]
OK
WriteCommand
AT+HTTPREADFILE=<filename
>[,<path>]
Response
1)ifparameterformatisright:
OK
+HTTPREADFILE:<errcode>
2)iffailed:
OK
+HTTPREADFILE:<errcode>
3)ifparameterformatisnotright oranyothererroroccurs:
ERROR
ParameterSaving Mode
Max ResponseTime
Reference
DefinedValues
<filename> String type,filename,the maxlength is112.unit:byte.
<path> Thedirectory where the readfilesaved.Numeric type,range is1-2.
1 C:/(local storage)
2 D:/(sd card)
Examples
AT+HTTPREADFILE=?
+HTTPREADFILE:<filename>[,(1-2)]
OK
AT+HTTPREADFILE="readbaidu.dat"
OK
+HTTPREADFILE:0


## Página 365

A76XX Series_AT Command Manual_V1.09
www.simcom.com 364 /652
16.3 CommandResultCodes
16.3.1 Descriptionof<statuscode>
<statuscode> Description
100 Continue
101 Switching Protocols
200 OK
201 Created
202 Accepted
203 Non-AuthoritativeInformation
204 NoContent
205 ResetContent
206 Partial Content
300 Multiple Choices
301 Moved Permanently
302 Found
303 SeeOther
304 Not Modified
305 UseProxy
307 Temporary Redirect
400 BadRequest
401 Unauthorized
402 Payment Required
403 Forbidden
404 Not Found
405 MethodNot Allowed
406 Not Acceptable
407 Proxy Authentication Required
408 RequestTimeout
409 Conflict
410 Gone
411 LengthRequired
412 Precondition Failed
413 RequestEntity TooLarge
414 Request-URITooLarge


## Página 366

A76XX Series_AT Command Manual_V1.09
www.simcom.com 365 /652
415 UnsupportedMedia Type
416 Requested rangenotsatisfiable
417 Expectation Failed
500 InternalServerError
501 Not Implemented
502 BadGateway
503 ServiceUnavailable
504 Gateway timeout
505 HTTPVersionnotsupported
600 Not HTTPPDU
601 NetworkError
602 Nomemory
603 DNSError
604 StackBusy
16.3.2 Descriptionof<errcode>
<errcode> Meaning
0 Success
701 Alertstate
702 Unknown error
703 Busy
704 Connectionclosederror
705 Timeout
706 Receive/sendsocket datafailed
707 Filenotexists orothermemoryerror
708 Invalidparameter
709 Networkerror
710 startanewsslsessionfailed
711 Wrongstate
712 Failed tocreatesocket
713 GetDNSfailed
714 Connectsocketfailed
715 Handshakefailed
716 Closesocketfailed
717 Nonetwork error
718 Senddata timeout
719 CAmissed


## Página 367

A76XX Series_AT Command Manual_V1.09
www.simcom.com 366 /652
16.4 UnsolicitedResultCodes
URC Description
+HTTP_PEER_CLOSED It’sa notificationmessage.Whilereceived,itmeans the
connection has been closedby server.
+HTTP_NONET_EVENT It’sa notificationmessage.Whilereceived,itmeans nowthe
networkis unavailable.


## Página 368

A76XX Series_AT Command Manual_V1.09
www.simcom.com 367 /652
ATCommandsforFTP(S)
17.1 OverviewofATCommandsforFTP(S)
Command Description
AT+CFTPSSTART StartFTP(S)service
AT+CFTPSSTOP Stop FTP(S)Service
AT+CFTPSLOGIN Login toaFTP(S)server
AT+CFTPSLOGOUT LogoutaFTP(S)server
AT+CFTPSLIST Listthe itemsin thedirectory on FTP(S)server
AT+CFTPSMKD Create anewdirectory onFTP(S)server
AT+CFTPSRMD Delete adirectory on FTP(S)server
AT+CFTPSCWD Change the currentdirectoryon FTP(S)server
AT+CFTPSPWD Getthe currentdirectory on FTP(S)server
AT+CFTPSDELE Delete afile onFTP(S)server
AT+CFTPSGETFILE Download afilefromFTP(S)serverto module
AT+CFTPSPUTFILE Upload afile frommodule toFTP(S)server
AT+CFTPSGET Getafile from FTP(S)servertoserial port
AT+CFTPSPUT PutafiletoFTP(S)server through serialport
AT+CFTPSSIZE Getthe filesize onFTP(S)server
AT+CFTPSSINGLEIP SetFTP(S)data socketaddresstype
AT+CFTPSTYPE Setthe transfertype onFTP(S)server
AT+CFTPSSLCFG Setthe SSLcontext idforFTPSsession
AT+CFTPSMODE SetActive orPassiveFTPMode
17.2 DetailedDescriptionofATCommandsforFTP(S)
17.2.1 AT+CFTPSSTART StartFTP(S)service


## Página 369

A76XX Series_AT Command Manual_V1.09
www.simcom.com 368 /652
AT+CFTPSSTARTisusedto startFTP(S)service byactivating PDPcontext.Youmustexecute
AT+CFTPSSTARTbeforeanyotherFTP(S)related operations.
AT+CFTPSSTART StartFTP(S)service
TestCommand
AT+CFTPSSTART=?
Response
OK
Execution Command
AT+CFTPSSTART
Response
1)
OK
+CFTPSSTART: 0
2)
OK
+CFTPSSTART: <errcode>
3)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<errcode> Theresult ofstartFTP(S)service,0issuccess,others arefailure.
Pleasereferto errcode list.
Examples
AT+CFTPSSTART
OK
+CFTPSSTART:0
17.2.2 AT+CFTPSSTOP StopFTP(S)Service
AT+CFTPSSTOPisusedtostopFTP(S)servicebydeactivatingPDPcontextWhenyouarenolongerusing
the FTP(S)service,usethiscommand.
AT+CFTPSSTOP StopFTP(S)Service


## Página 370

A76XX Series_AT Command Manual_V1.09
www.simcom.com 369 /652
TestCommand
AT+CFTPSSTOP=?
Response
OK
Execution Command
AT+CFTPSSTOP
Response
1)
OK
+CFTPSSTOP:0
2)
OK
+CFTPSSTOP:<errcode>
3)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<errcode> Theresult ofstopFTP(S)service,0is success,others are failure.
Pleasereferto errcode list.
Examples
AT+CFTPSSTOP
OK
+CFTPSSTOP:0
17.2.3 AT+CFTPSLOGIN LogintoaFTP(S)server
AT+CFTPSLOGINisused tologin toaFTP(S)server,you can logintoaFTPserverby setparameter
<server_type> to0,login toanimplicit FTPSserverby set<server_type>to3and login toanexplicitFTPS
serverby set<server_type> to1 or2.About<sever_type>,more detailspleaserefer toDefined Values
<server_type>.
AT+CFTPSLOGIN LogintoaFTP(S)server
TestCommand
AT+CFTPSLOGIN=?
Response
+CFTPSLOGIN:
"ADDRESS",(1-65535),"USERNAME","PASSWORD"[,(0-3)]


## Página 371

A76XX Series_AT Command Manual_V1.09
www.simcom.com 370 /652
OK
Read Command
AT+CFTPSLOGIN?
Response
1)iIfthestatus isnot logged in
+CFTPSLOGIN:0
OK
2)Ifitisloggedin
+CFTPSLOGIN:1
OK
3)
ERROR
WriteCommand
AT+CFTPSLOGIN=<host>,<port
>,<username>,<password>[<se
rver_type>]
Response
1)
OK
+CFTPSLOGIN:0
2)
OK
+CFTPSLOGIN:<errcode>
3)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<host> Host address,string type, maximumlength is128
<port> ThehostlisteningportforFTP(S), therangeis from1to65535
<username> FTP(S)user name,string type,maximum length is128
<password> Theuserpassword,stringtype,maximum lengthis 128
<servet_type> FTP(S)server type,numeric,from 0-3,defaultis3
0 FTPserver.
1 Explicit FTPSserverwithAUTHSSL.
2 Explicit FTPSserverwithAUTHTLS.
3 Implicit FTPSserver.
<errcode> Theresult code ofthe FTP/FTPSlogin. 0is success.Others are
failure,pleasereferto chapter17.3.
Examples


## Página 372

A76XX Series_AT Command Manual_V1.09
www.simcom.com 371 /652
AT+CFTPSLOGIN=?
+CFTPSLOGIN:
"ADDRESS",(1-65535),"USERNAME","PASSWORD"[,(0-3)]
OK
AT+CFTPSLOGIN?
+CFTPSLOGIN: 0
OK
AT+CFTPSLOGIN="serveraddr",21,"username","password",0
OK
+CFTPSLOGIN: 0
17.2.4 AT+CFTPSLOGOUT LogoutaFTP(S)server
AT+CFTPSLOGOUTisusedtologoutaFTP(S)sever,makesureyou loginaFTP(S)severbefore you
executeAT+CFTPSLOGOUTcommand.
AT+CFTPSLOGOUT LogoutaFTP(S)server
TestCommand
AT+CFTPSLOGOUT=?
Response
OK
Execute Command
AT+CFTPSLOGOUT
Response
1)
OK
+CFTPSLOGOUT:0
2)
OK
+CFTPSLOGOUT:<errcode>
3)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues


## Página 373

A76XX Series_AT Command Manual_V1.09
www.simcom.com 372 /652
<errcode> Theresult code ofthe FTP/FTPSlogout.0is success.Others are
failure,pleasereferto chapter17.3.
Examples
AT+CFTPSLOGOUT=?
OK
AT+CFTPSLOGOUT
OK
+CFTPSLOGOUT:0
Whenyou wantto stopthe FTP(S)service,please useAT+CFTPSLOGOUTtologoutof the
FTP(S)server,thenuseAT+CFTPSSTOPtostop FTP,ifyou only useAT+CFTPSSTOP,itwillreport
ERROR.
17.2.5 AT+CFTPSLIST ListtheitemsinthedirectoryonFTP(S)server
This commandis used tolistthe itemsin the specified directoryon FTP(S)server.Modulewilloutput the
itemstoserialportwhen listitemssuccessfully.Makesure thatyou have login toFTP(S)server
successfully.
AT+CFTPSLIST ListtheitemsinthedirectoryonFTP(S)server
TestCommand
AT+CFTPSLIST=?
Response
OK
WriteCommand
AT+CFTPSLIST=<dir>
Response
1)
OK
+CFTPSLIST: DATA,<len>
…
+CFTPSLIST: 0
2)
OK
+CFTPSLIST: <errcode>
3)
NOTE


## Página 374

A76XX Series_AT Command Manual_V1.09
www.simcom.com 373 /652
ERROR
4)
+CFTPSLIST: <errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<dir> Thedirectory to becreated, string type,maximumlength is112.
<errcode> Theresult ofcreatedirectory,0is success,others are failure,please
refer tochapter17.3
<len> TheserverAll filesand related information inthe directory.
Examples
AT+CFTPSLIST="/"
OK
+CFTPSLIST:DATA,175
-rw-r--r--1ftp ftp 121Mar1116:24124.txt
drwxr-xr-x1ftpftp 0Jan 132020
TEST113
drwxr-xr-x1ftpftp 0Jan 192020
TEST1155
+CFTPSLIST:0
17.2.6 AT+CFTPSMKD CreateanewdirectoryonFTP(S)server
AT+CFTPSMKDisused tocreatea newdirectory on aFTP(S)server.Pleasemakesure loginto the
FTP(S)serversuccessfully before createadirectory.
AT+CFTPSMKD CreateanewdirectoryonFTP(S)server
TestCommand
AT+CFTPSMKD=?
Response
+CFTPSMKD:"DIR"
OK


## Página 375

A76XX Series_AT Command Manual_V1.09
www.simcom.com 374 /652
WriteCommand
AT+CFTPSMKD=<dir>
Response
1)
OK
+CFTPSMKD:0
2)
OK
+CFTPSMKD:<errcode>
3)
ERROR
4)
+CFTPSMKD:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<dir> Thedirectory to becreated, string type,maximumlength is112.
<errcode> Theresult ofcreatedirectory,0is success,others are failure,please
refer tochapter17.3
Examples
AT+CFTPSMKD=?
+CFTPSMKD:"DIR"
OK
AT+CFTPSMKD="test"
OK
+CFTPSMKD:0
17.2.7 AT+CFTPSRMD DeleteadirectoryonFTP(S)server
AT+CFTPSRMDisusedtodeleteadirectory onFTP(S)server,pleasemakesurelogintotheFTP(S)server
successfully before deleteadirectory.


## Página 376

A76XX Series_AT Command Manual_V1.09
www.simcom.com 375 /652
AT+CFTPSRMD DeleteadirectoryonFTP(S)server
TestCommand
AT+CFTPSRMD=?
Response
+CFTPSRMD:"DIR"
OK
WriteCommand
AT+CFTPSRMD=<dir>
Response
1)
OK
+CFTPSRMD:0
2)
OK
+CFTPSRMD:<errcode>
3)
ERROR
4)
+CFTPSRMD:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<dir> Thedirectory to bedeleted,string type,maximumlength is 112.
<errcode> Theresult ofcreatedirectory,0is success,others are failure,please
refer tochapter17.3
Examples
AT+CFTPSRMD=?
+CFTPSRMD:"DIR"
OK
AT+CFTPSRMD="test"
OK
+CFTPSRMD:0


## Página 377

A76XX Series_AT Command Manual_V1.09
www.simcom.com 376 /652
17.2.8 AT+CFTPSCWD ChangethecurrentdirectoryonFTP(S)server
Youcanuse thiscommand tochangethe currentdirectory onFTP(S)sever.Makesure you have login to
FTP(S)serversuccessfully beforeAT+CFTPSCWD
AT+CFTPSCWD ChangethecurrentdirectoryonFTP(S)server
TestCommand
AT+CFTPSCWD=?
Response
+CFTPSCWD:"DIR"
OK
WriteCommand
AT+CFTPSCWD=<dir>
Response
1)
OK
+CFTPSCWD:0
2)
OK
+CFTPSCWD:<errcode>
3)
ERROR
4)
+CFTPSCWD:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<dir> Thedirectory to bechanged,string type, maximumlength is 112.
<errcode> Theresult ofcreatedirectory,0is success,others are failure,please
refer tochapter17.3
Examples
AT+CFTPSCWD=?
+CFTPSCWD:"DIR"
OK
AT+CFTPSCWD="test"


## Página 378

A76XX Series_AT Command Manual_V1.09
www.simcom.com 377 /652
OK
+CFTPSCWD:0
17.2.9 AT+CFTPSPWD GetthecurrentdirectoryonFTP(S)server
This commandis used togetthe currentdirectoryon FTPSserver.BeforeAT+CFTPSPWD,pleasemake
sureyou have loginto FTP(S)serversuccessfully
AT+CFTPSPWD GetthecurrentdirectoryonFTP(S)server
TestCommand
AT+CFTPSPWD=?
Response
+CFTPSPWD:
OK
Execute Command
AT+CFTPSPWD
Response
1)
OK
+CFTPSPWD:<dir>
2)
OK
+CFTPSPWD:<errcode>
3)
ERROR
4)
+CFTPSPWD:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<dir> Thedirectory to begot,string type.
<errcode> Theresult ofcreatedirectory,0is success,others are failure,please
refer tochapter17.3
Examples


## Página 379

A76XX Series_AT Command Manual_V1.09
www.simcom.com 378 /652
AT+CFTPSPWD
OK
+CFTPSPWD:"/"
17.2.10AT+CFTPSDELE DeleteafileonFTP(S)server
YoucanuseAT+CFTPSDELEdelete afileon FTP(S)server,pleasemakesure login tothe FTP(S)server
successfully before deleteafile.
AT+CFTPSDELE DeleteafileonFTP(S)server
TestCommand
AT+CFTPSDELE=?
Response
+CFTPSDELE:"FILENAME"
OK
WriteCommand
AT+CFTPSDELE=<filename>
Response
1)
OK
+CFTPSDELE:0
2)
OK
+CFTPSDELE:<errcode>
3)
ERROR
4)
+CFTPSDELE:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<filename> Thenameof the filetobe deleted. Stringtype, themaximum length is
112
<errcode> Theresult ofcreatedirectory,0is success,others are failure,please
refer tochapter17.3


## Página 380

A76XX Series_AT Command Manual_V1.09
www.simcom.com 379 /652
Examples
AT+CFTPSDELE=?
+CFTPSDELE:"FILENAME"
OK
AT+CFTPSDELE="testfile"
OK
+CFTPSDELE:0
17.2.11AT+CFTPSGETFILE DownloadafilefromFTP(S)servertomodule
Youcandownload afilefrom FTP(S)serverto module,by setting parameter<dir>,you can selectthe
directory whereto savethe downloadedfile. Defaultthe downloadedfile willbe saved tolocalstorage.
Makesure thatyou havelogin toFTP(S)serversuccessfully beforeAT+CFTPSGETFILE.
AT+CFTPSGETFILE DownloadafilefromFTP(S)servertomodule
TestCommand
AT+CFTPSGETFILE=?
Response
+CFTPSGETFILE:"FILEPATH"[,(1-2)]
OK
WriteCommand
AT+CFTPSGETFILE=<filepath>[
,<dir>[,<offset>]]
Response
1)
OK
+CFTPSGETFILE:0
2)
OK
+CFTPSGETFILE:<errcode>
3)
ERROR
4)
+CFTPSGETFILE:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference


## Página 381

A76XX Series_AT Command Manual_V1.09
www.simcom.com 380 /652
DefinedValues
<filepath> Theremote filepath.String type,maximum lengthis 112
<dir> Thedirectory to save the downloaded file.Numeric type,rangeis 1-2,
defaultis 1(local storage)
1 C:/(local storage)
2 D:/(sd card)
<offset>
ThevalueforFTP"REST"commandwhichisusedforbrokentransfer
when transferringfailed lasttime.Ifthe fileis complete, thefile length
is notincreased.Numeric type,the rangeis from0to 2147483647.
Note:Ifoffsetis not0,makesurethe filealready exists,otherwiseitwill
fail.
Note:Thisparameterisonlysupportedon the1603,1606 and
1803 platforms.
<errcode> Theresult code ofdownload filefromFTP(s)server.0is success,
others arefailure,please refertochapter17.3.
Examples
AT+CFTPSGETFILE=?
+CFTPSGETFILE:"FILEPATH"[,(1-2)]
OK
AT+CFTPSGETFILE="test.txt",1
OK
+CFTPSGETFILE:0
17.2.12AT+CFTPSPUTFILE UploadafilefrommoduletoFTP(S)server
Youcanuse thiscommand touploadafiletoFTP(S)server frommodule. Bysettingparameter <dir>you
canselectthe directory thatcontainsthe filetobe uploaded.Make surethatyou have logintothe
FTP(S)serversuccessfully beforeAT+CFTPSPUTFILE.
AT+CFTPSPUTFILE UploadafilefrommoduletoFTP(S)server
TestCommand
AT+CFTPSPUTFILE=?
Response
+CFTPSPUTFILE:"FILEPATH"[,(rangeofsupported
<dir>s),(range ofsupported <rest_size>s)]


## Página 382

A76XX Series_AT Command Manual_V1.09
www.simcom.com 381 /652
OK
WriteCommand
AT+CFTPSPUTFILE=<filepath>[
,<dir>[,<rest_size>]]
Response
1)
OK
+CFTPSPUTFILE:0
2)
OK
+CFTPSPUTFILE:<errcode>
3)
ERROR
4)
+CFTPSPUTFILE:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<filepath> Theremote filepath.String type,maximum lengthis 112
<dir> Thedirectory thatcontainsthe uploadedfile.Numeric type,range is
1-2,default is1(localstorage)
1 C:/(local storage)
2 D:/(sd card)
<rest_size> ThevalueforFTP"REST"commandwhichisusedforbrokentransfer
when transferringfailed lasttime.Ifthe fileis complete, thefile length
is notincreased.Numeric type, the range isfrom 0to2147483647.
<errcode> Theresult code ofdownload filefromFTP(s)server.0is success,
others arefailure,please refertochapter17.3.
Examples
AT+CFTPSPUTFILE=?
+CFTPSPUTFILE:"FILEPATH"[,(1-2),(0-2147483647)]
OK
AT+CFTPSPUTFILE="test.txt",1
OK
+CFTPSPUTFILE:0


## Página 383

A76XX Series_AT Command Manual_V1.09
www.simcom.com 382 /652
17.2.13AT+CFTPSGET GetafilefromFTP(S)servertoserialport
Youcanuse thiscommand togetafilefrom FTP(S)servertoserial port.
AT+CFTPSGET GetafilefromFTP(S)servertoserialport
TestCommand
AT+CFTPSGET=?
Response
+CFTPSGET:"FILEPATH"[,<rest_size>]
OK
WriteCommand
AT+CFTPSGET=<filepath>[,<re
st_size>]
Response
1)
OK
+CFTPSGET:DATA,<len>
…
+CFTPSGET:DATA,<len>
…
+CFTPSGET:0
2)
OK
+CFTPSGET: <errcode>
3)
ERROR
4)
+CFTPSGET:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<filepath> Theremote filepath.String type,maximum lengthis 112.
<rest_size> ThevalueforFTP"REST"commandwhichisusedforbrokentransfer
when transferringfailed lasttime.Numeric type, therange isfrom0to
2147483647
<len> Theactuallength ofthe fileread.Integertype,the maximumlength of
eachpackage is1024.


## Página 384

A76XX Series_AT Command Manual_V1.09
www.simcom.com 383 /652
<errcode> Theresult code ofdownload filefromFTP(s)server.0is success,
others arefailure,please refertochapter17.3.
Examples
AT+CFTPSGET=?
+CFTPSGET:"FILEPATH"[,<rest_size>]
OK
AT+CFTPSGET="test.txt"
OK
+CFTPSGET:DATA,3
321
+CFTPSGET:0
17.2.14AT+CFTPSPUT PutafiletoFTP(S)serverthroughserialport
Youcanputa fileto FTP(S)serverthroughserial port.Makesure thatyou havelogin toFTP(S)server
successfully.
AT+CFTPSPUT PutafiletoFTP(S)serverthroughserialport
TestCommand
AT+CFTPSPUT=?
Response
+CFTPSPUT:"FILEPATH"[,<data_len>[,<rest_size>]]
OK
WriteCommand
AT+CFTPSPUT=<filepath>[,<dat
a_len>[,<rest_size>]]
Response
1)ifupload filethrough serialportsuccessfully:
OK
+CFTPSPUT:0
2)iffailedbeforeinputdata:
ERROR
+CFTPSPUT:<errcode>
3)iffailedafterinputdata:
OK
+CFTPSPUT:<errcode>
4)
ERROR


## Página 385

A76XX Series_AT Command Manual_V1.09
www.simcom.com 384 /652
5)
+CFTPSPUT:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 600000ms
Reference
DefinedValues
<filepath> Theremote filepath.String type,maximum lengthis 112.
<data_len> Numeric type,Thelengthof the datatosend, the maximumlength is
2048.ifparameter<data_len> isomitted, Each <Ctrl+Z>character
presentin the dataflowof serialportwhendownloading FTPdata will
be coded as<ETX><Ctrl+Z>.Each<ETX>character willbe coded as
<ETX><ETX>. Single<Ctrl+Z>means endof the FTPdata.<ETX> is
0x03, and<Ctrl+Z>is0x1A.
<rest_size> ThevalueforFTP"REST"commandwhichisusedforbrokentransfer
when transferringfailed lasttime.Numeric type, therange isfrom0to
2147483647
<errcode> Theresult code ofdownload filefromFTP(s)server.0is success,
others arefailure,please refertochapter17.3.
Examples
AT+CFTPSPUT=?
+CFTPSPUT:
"FILEPATH"[,<data_len>[,<rest_size>]]
OK
AT+CFTPSPUT="test.txt",4
>
data
OK
+CFTPSPUT:0
17.2.15AT+CFTPSSINGLEIP SetFTP(S)datasocketaddresstype
This commandis used tosetFTPSserverdata socketIPaddresstype.ForsomeFTP(S)server,itis


## Página 386

A76XX Series_AT Command Manual_V1.09
www.simcom.com 385 /652
needed tosetAT+CFTPSSINGLEIP=1.Please makesureto setAT+CFTPSSINGLEIPbefore
AT+CFTPSLOGIN.
AT+CFTPSSINGLEIP SetFTP(S)datasocketaddresstype
TestCommand
AT+CFTPSSINGLEIP=?
Response
+CFTPSSINGLEIP:(0,1)
OK
Read Command
AT+CFTPSSINGLEIP?
+CFTPSSINGLEIP:<singleip>
OK
WriteCommand
AT+CFTPSSINGLEIP=<singleip
>
Response
1)
OK
2)
ERROR
3)
+CFTPSSINGLEIP:<singleip>
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<singleip> TheFTPSdata socketIPaddress type:
0 decided by PORTresponse from FTPSserver
1 the sameas thecontrolsocket.
Examples
AT+CFTPSSINGLEIP=?
+CFTPSSINGLEIP:(0,1)
OK
AT+CFTPSSINGLEIP?
+CFTPSSINGLEIP:0
OK
AT+CFTPSSINGLEIP=0
OK


## Página 387

A76XX Series_AT Command Manual_V1.09
www.simcom.com 386 /652
17.2.16AT+CFTPSSIZE GetthefilesizeonFTP(S)server
Youcanuse thiscommand togetthe filesize on FTP(S)server.Pleasemake sureyou have login to
FTP(S)serverbeforeAT+CFTPSSIZE.
AT+CFTPSSIZE GetthefilesizeonFTP(S)server
TestCommand
AT+CFTPSSIZE=?
Response
+CFTPSSIZE:"FILEPATH"
OK
WriteCommand
AT+CFTPSSIZE=<filepath>
Response
1)
OK
+CFTPSSIZE:<filesize>
2)
ERROR
+CFTPSSIZE:<errcode>
3)
ERROR
4)
+CFTPSSIZE:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<filepath> Theremote filepath onFTP(S)server.String type, max length is112
<filesize> Numeric type,size ofthe remotefileon FTP(S)server
<errcode> Theresult ofsettype,0issuccess,others arefailure,pleaserefer to
chapter4
Examples
AT+CFTPSSIZE=?
+CFTPSSIZE:"FILEPATH"


## Página 388

A76XX Series_AT Command Manual_V1.09
www.simcom.com 387 /652
OK
AT+CFTPSSIZE="test"
OK
+CFTPSSIZE:3
17.2.17AT+CFTPSTYPE Setthetransfertype onFTP(S)server
This commandis used tosetthe transfertype on FTP(S)server,pleasemake sureyou havelogin to
FTP(S)serverbeforeAT+CFTPSTYPE.
AT+CFTPSTYPE Setthetransfertype onFTP(S)server
TestCommand
AT+CFTPSTYPE=?
Response
+CFTPSTYPE:(A,I)
OK
Read Command
AT+CFTPSTYPE?
+CFTPSTYPE:<type>
OK
WriteCommand
AT+CFTPSTYPE=<type>
Response
1)
OK
+CFTPSTYPE:0
2)
OK
+CFTPSTYPE:<errcode>
3)
ERROR
4)
+CFTPSTYPE:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues


## Página 389

A76XX Series_AT Command Manual_V1.09
www.simcom.com 388 /652
<type> Thetype oftransferring:
A ASCII
I Binary
<errcode> Theresult ofsettype,0issuccess,others arefailure,pleaserefer to
chapter17.3
Examples
AT+CFTPSTYPE=?
+CFTPSTYPE:(A,I)
OK
AT+CFTPSTYPE?
+CFTPSTYPE:I
OK
AT+CFTPSTYPE=A
OK
+CFTPSTYPE:0
17.2.18AT+CFTPSSLCFG SettheSSLcontextidforFTPSsession
Youcanuse thiscommand tosetthe SSLcontext id forFTPSsession.
AT+CFTPSSLCFG SettheSSLcontextidforFTPSsession
TestCommand
AT+CFTPSSLCFG=?
Response
+CFTPSSLCFG:(0,1),(0-9)
OK
WriteCommand
AT+CFTPSSLCFG=<session_id
>,<ssl_id>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues


## Página 390

A76XX Series_AT Command Manual_V1.09
www.simcom.com 389 /652
<session_id> Numeric type,0forcontrolsession, 1fordata session.
<ssl_id> Numeric type,SSLcontextIDduring 0-9.
Examples
AT+CFTPSSLCFG=?
+CFTPSSLCFG:(0,1),(0-9)
OK
AT+CFTPSSLCFG=0,1
OK
17.2.19AT+CFTPSMODE SetActiveorPassive FTPMode
Youcanuse thiscommand tosetActive orPassive FTPMode.
AT+CFTPSMODE SetActiveorPassive FTPMode
TestCommand
AT+CFTPSMODE=?
Response
+CFTPSMODE:(0,1)
OK
Read Command
AT+CFTPSMODE?
+CFTPSMODE:<mode>
OK
WriteCommand
AT+CFTPSMODE=<mode>
Response
1)
OK
2)
ERROR
3)
+CFTPSMODE:<errcode>
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues


## Página 391

A76XX Series_AT Command Manual_V1.09
www.simcom.com 390 /652
<mode> Numeric type
0 ActiveFTPmode
1 Passive FTPmode
Examples
AT+CFTPSMODE=?
+CFTPSMODE:(0,1)
OK
AT+CFTPSMODE=1
OK
AT+CFTPSMODE?
+CFTPSMODE:1
OK
AT+CFTPSMODEis onlysupported onthe 1603,1606 and 1803 platforms.
17.3 CommandResultCodes
17.3.1 Descriptionof<errcode>
<errcode> Description
0 Success
1 SSLalert
2 Unknown error
3 Busy
4 Connectionclosed byserver
5 Timeout
6 Transferfailed
7 Filenotexists oranyothermemoryerror
8 Invalid parameter
NOTE


## Página 392

A76XX Series_AT Command Manual_V1.09
www.simcom.com 391 /652
9 Operation rejected byserver
10 Networkerror
11 State error
12 Failed toparseserver name
13 Create socketerror
14 Connectsocket failed
15 Closesocketfailed
16 SSLsession closed
17 Fileerror,filenotexistor othererror.
421 Server response connection time out, while received error code 421,
you need do AT+CFTPSLOGOUT to logout server then
AT+CFTPSLOGINagainforfurtheroperations.
17.4 UnsolicitedResultcodes
Unsolicitedcodes Description
+CFTPSNOTIFY:PEERCLOSED Whenclientdisconnectpassively,URC"+CFTPSNOTIFY:
PEERCLOSED"will bereported, thenuserneed to
executeAT+CFTPSLOGOUTand loginagain.
+CFTPSNOTIFY: FTPSDISCONNECT DuringtheFTPclientandFTPserverisconnecting,thenet
disconnectwill report"+CFTPSNOTIFY:FTPS
DISCONNECT"toURC,then userneed excute
AT+CFTPSTARTandlogin again
+CFTPSERRORBUF:<errorbuf> Returns failurecausewhen servererroroccurs,URC
"+CFTPSERRORBUF:<errorbuf>"willbe reported,then
userneed toexecuteAT+CFTPSLOGOUTandlog in
again.


## Página 393

A76XX Series_AT Command Manual_V1.09
www.simcom.com 392 /652
ATCommandsforMQTT(S)
18.1 OverviewofATCommandsforMQTT(S)
Command Description
AT+CMQTTSTART StartMQTTservice
AT+CMQTTSTOP StopMQTTservice
AT+CMQTTACCQ Acquireaclient
AT+CMQTTREL Releaseaclient
AT+CMQTTSSLCFG Setthe SSLcontext(only forSSL/TLSMQTT)
AT+CMQTTWILLTOPIC Inputthe topic of willmessage
AT+CMQTTWILLMSG Inputthe will message
AT+CMQTTCONNECT ConnecttoMQTTserver
AT+CMQTTDISC Disconnectfrom server
AT+CMQTTTOPIC Inputthe topic of publishmessage
AT+CMQTTPAYLOAD Inputthe publish message
AT+CMQTTPUB Publish amessage toserver
AT+CMQTTSUBTOPIC Inputthe topic of subscribemessage
AT+CMQTTSUB Subscribe amessage toserver
AT+CMQTTUNSUBTOPIC Inputthe topic of unsubscribemessage
AT+CMQTTUNSUB Unsubscribe amessagetoserver
AT+CMQTTCFG Configure the MQTTContext
18.2 DetailedDescriptionofATCommandsforMQTT(S)
18.2.1 AT+CMQTTSTART StartMQTTservice
AT+CMQTTSTARTis used tostartMQTTserviceby activating PDPcontext.Youmustexecute this
command beforeany otherMQTTrelatedoperations.


## Página 394

A76XX Series_AT Command Manual_V1.09
www.simcom.com 393 /652
AT+CMQTTSTART StartMQTTservice
Execute Command
AT+CMQTTSTART
Response
1)IfstartMQTTservicesuccessfully:
OK
+CMQTTSTART:0
2)Iffailed:
OK
+CMQTTSTART:<err>
3)IfMQTTservicehave started successfully andyou executed
AT+CMQTTSTARTagain:
ERROR
Max ResponseTime 12000ms
ParameterSaving Mode -
Reference
DefinedValues
<err> Theresult code,pleaserefertoChapter18.3
Examples
AT+CMQTTSTART
OK
+CMQTTSTART:0
AT+CMQTTSTARTis used tostartMQTTserviceby activating PDPcontext.Youmustexecute this
command beforeany otherMQTTrelatedoperations.
Ifyou don’texecuteAT+CMQTTSTART,theWrite/Read Command ofanyotherMQTTwill return
ERRORimmediately.
18.2.2 AT+CMQTTSTOP StopMQTTservice
AT+CMQTTSTOPisused tostopMQTTservice.
NOTE


## Página 395

A76XX Series_AT Command Manual_V1.09
www.simcom.com 394 /652
AT+CMQTTSTOP StopMQTTservice
Execute Command
AT+CMQTTSTOP
Response
1)Ifstop MQTTservicesuccessfully:
OK
+CMQTTSTOP:0
2)Iffailed:
+CMQTTSTOP:<err>
ERROR
3)IfMQTTservicehave stopped successfully and you executed
AT+CMQTTSTOPagain:
ERROR
Max ResponseTime 12000ms
ParameterSaving Mode -
Reference
DefinedValues
<err> Theresult code,pleaserefertochapter 18.3
Examples
AT+CMQTTSTOP
OK
+CMQTTSTOP:0
AT+CMQTTSTOPisused tostopMQTTservice.Youcanexecute this commandafter
AT+CMQTTDISCandAT+CMQTTREL.
18.2.3 AT+CMQTTACCQ Acquireaclient
AT+CMQTTACCQis used toacquireaMQTTclient.Itmustbecalled before allcommands aboutMQTT
connectandafterAT+CMQTTSTART.
NOTE


## Página 396

A76XX Series_AT Command Manual_V1.09
www.simcom.com 395 /652
AT+CMQTTACCQ Acquireaclient
TestCommand
AT+CMQTTACCQ=?
Response
+CMQTTACCQ:(0-1),(1-128)[,(0-1)]
OK
Read Command
AT+CMQTTACCQ?
Response
+CMQTTACCQ:<client_index>,<clientID>,<server_type>
+CMQTTACCQ:<client_index>,<clientID>,<server_type>
OK
WriteCommand
AT+CMQTTACCQ=<client_in
dex>,<clientID>[<server_type
>]
Response
1)Ifsuccessfully:
OK
2)Iffailed:
+CMQTTACCQ:<client_index>,<err>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<clientID> TheUTF-encoded string.Itspecifies auniqueidentifierforthe client.
Thestring lengthis from1to128 bytes.
<server_type> Anumeric parameterthatidentifies the servertype.Thedefault value
is 0.
0 MQTTserverwithTCP
1 MQTTserverwith SSL/TLS
<err> Theresult code,pleaserefertochapter 18.3
Examples
AT+CMQTTACCQ=0,"a12mmmm",0
OK
AT+CMQTTACCQ?
+CMQTTACCQ:0,"a12mmmm",0


## Página 397

A76XX Series_AT Command Manual_V1.09
www.simcom.com 396 /652
+CMQTTACCQ:1,"",0
OK
AT+CMQTTACCQ=?
+CMQTTACCQ:(0-1),(1-128)[,(0-1)]
OK
18.2.4 AT+CMQTTREL Releaseaclient
AT+CMQTTRELisusedto releaseaMQTTclient.Itmustbe called afterAT+CMQTTDISCandbefore
AT+CMQTTSTOP.
AT+CMQTTREL Releaseaclient
TestCommand
AT+CMQTTREL=?
Response
+CMQTTREL:(0-1)
OK
Read Command
AT+CMQTTREL?
Response
1)Ifsuccessfully:
OK
2)ifMQTTnotstart
ERROR
WriteCommand
AT+CMQTTREL=<client_inde
x>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
+CMQTTREL:<client_index>,<err>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues


## Página 398

A76XX Series_AT Command Manual_V1.09
www.simcom.com 397 /652
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<err> Theresult code,pleaserefertochapter 18.3
Examples
AT+CMQTTREL=?
+CMQTTREL:(0-1)
OK
AT+CMQTTREL=0
OK
AT+CMQTTREL?
OK
18.2.5 AT+CMQTTSSLCFG SettheSSLcontext(onlyforSSL/TLSMQTT)
AT+CMQTTSSLCFG isused tosetthe SSLcontext whichtobeused inthe SSLconnectionwhenit will
connecttoaSSL/TLS MQTTserver.Itmustbe calledbeforeAT+CMQTTCONNECTandafter
AT+CMQTTSTART.Thesetting will beclearedafterAT+CMQTTCONNECTfailedorAT+CMQTTDISC.
AT+CMQTTSSLCFG SettheSSLcontext(onlyforSSL/TLSMQTT)
TestCommand
AT+CMQTTSSLCFG=?
Response
+CMQTTSSLCFG:(0,1),(0-9)
OK
Read Command
AT+CMQTTSSLCFG?
Response
+CMQTTSSLCFG:<session_id>,[<ssl_ctx_index>]
+CMQTTSSLCFG:<session_id>,[<ssl_ctx_index>]
OK
WriteCommand
AT+CMQTTSSLCFG=<sessio
n_id>,<ssl_ctx_index>
Response
1)If successfully:
OK
2)If failed:
ERROR
ParameterSaving Mode -


## Página 399

A76XX Series_AT Command Manual_V1.09
www.simcom.com 398 /652
Max ResponseTime -
Reference -
DefinedValues
<session_id> Thesession_id tooperate. It’sfrom 0to1
<ssl_ctx_index> TheSSLcontextIDwhichwillbeusedinthe SSLconnection.Referto
the <ssl_ctx_index>ofAT+CSSLCFG
Examples
AT+CMQTTSSLCFG?
+CMQTTSSLCFG:0,0
+CMQTTSSLCFG:1,0
OK
AT+CMQTTSSLCFG=?
+CMQTTSSLCFG:(0,1),(0-9)
OK
AT+CMQTTSSLCFG=0,1
OK
18.2.6 AT+CMQTTWILLTOPIC Inputthetopicofwillmessage
AT+CMQTTWILLTOPICis used toinputthe topicof will message.
AT+CMQTTWILLTOPIC Inputthetopicofwillmessage
TestCommand
AT+CMQTTWILLTOPIC=?
Response
+CMQTTWILLTOPIC:(0-1),(1-1024)
OK
WriteCommand
AT+CMQTTWILLTOPIC=<clie
nt_index>,<req_length>
Response
1)If successfully:
>
<inputdatahere>
OK
2)If failed:
+CMQTTWILLTOPIC:<client_index>,<err>


## Página 400

A76XX Series_AT Command Manual_V1.09
www.simcom.com 399 /652
ERROR
3)If failed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputtopic.ThewilltopicshouldbeUTF-encodedstring.
Therangeis from1to1024 bytes.
<err> Theresult code,pleaserefertochapter 18.3
Examples
AT+CMQTTWILLTOPIC=0,10
>
OK
18.2.1 AT+CMQTTWILLMSG Inputthewillmessage
AT+CMQTTWILLMSG is used toinputthe message body ofwill message.
AT+CMQTTWILLMSG Inputthewillmessage
TestCommand
AT+CMQTTWILLMSG=?
Response
+CMQTTWILLMSG:(0-1),(1-1024),(0-2)
OK
WriteCommand
AT+CMQTTWILLMSG=<clien
t_index>,<req_length>,<qos>
Response
1)Ifsuccessfully:
>
<inputdata here>
OK
2)Iffailed:
+CMQTTWILLMSG:<client_index>,<err>


## Página 401

A76XX Series_AT Command Manual_V1.09
www.simcom.com 400 /652
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputdata.Thewill message shouldbe UTF-encoded
string.Therange isfrom1to 1024bytes.
<qos> Theqos valueofthe willmessage.Therangeis from0to2.
Examples
AT+CMQTTWILLMSG=0,6,1
>
OK
18.2.2 AT+CMQTTCONNECT ConnecttoMQTTserver
AT+CMQTTCONNECTisusedto connectto aMQTTserver.
AT+CMQTTCONNECT ConnecttoMQTTserver
TestCommand
AT+CMQTTCONNECT=?
Response
+CMQTTCONNECT:
(0-1),(9-256),(1-64800),(0-1)[,<user_name>,<pass_word>]
OK
Read Command
AT+CMQTTCONNECT?
Response
+CMQTTCONNECT:
0[,<server_addr>,<keepalive_time>,<clean_session>[,<user_na
me>[,<pass_word>]]]
+CMQTTCONNECT:
1[,<server_addr>,<keepalive_time>,<clean_session>[,<user_na
me>[,<pass_word>]]]
OK


## Página 402

A76XX Series_AT Command Manual_V1.09
www.simcom.com 401 /652
WriteCommand
AT+CMQTTCONNECT=<clien
t_index>,<server_addr>,<kee
palive_time>,<clean_session
>[,<user_name>[,<pass_word
>]]
Response
1)Ifsuccessfully:
OK
+CMQTTCONNECT:<client_index>,0
2)Iffailed:
OK
+CMQTTCONNECT:<client_index>,<err>
3)Iffailed:
ERROR
+CMQTTCONNECT:<client_index>,<err>
3)Iffailed:
ERROR
+CMQTTCONNECT:<client_index>,<err>
4) Iffailed:
+CMQTTCONNECT:<client_index>,<err>
ERROR
5)If failed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeof permitted
values is0to1.
<server_addr>
Thestringthatdescribed the serveraddress andport.Therangeof
the string lengthis 9to256bytes.Thestringshouldbe likethis
"tcp://116.247.119.165:5141",mustbeginwith "tcp://". Ifthe
<server_addr>notinclude theport, the defaultportis1883.
<keepalive_time>
Thetimeinterval between two messagesreceivedfroma client.The
clientwillsend akeep-alivepacketwhen thereis no messagesent to
serveraftersong longtime.Therange isfrom 1sto64800s (18
hours).
<clean_session>
Thecleansessionflag.Thevalue rangeisfrom 0to 1,anddefault
valueis 0.
0 the servermuststore the subscriptions ofthe clientafter it
disconnected.Thisincludes continuingtostore QoS1and
QoS2messages forthe subscribedtopicssothatthey canbe


## Página 403

A76XX Series_AT Command Manual_V1.09
www.simcom.com 402 /652
deliveredwhen the clientreconnects.The servermustalso
maintainthe stateofin-flight messages beingdeliveredat the
pointthe connection islost.This information mustbe keptuntil
the clientreconnects.
1 the servermustdiscardany previously maintainedinformation
abouttheclientandtreattheconnectionas"clean".Theserver
mustalsodiscard any statewhen the client disconnects.
<user_name>
Theusernameidentifies the nameofthe userwhichcanbe used for
authentication whenconnecting toserver.Thestring lengthis from1
to256bytes.
<pass_word>
Thepasswordcorresponding tothe userwhichcanbe usedfor
authentication whenconnecting toserver.Thestring lengthis from1
to256bytes.
<err> Theresultcode:0issuccess.Othervaluesarefailure.Pleasereferto
chapter18.3.
Examples
AT+CMQTTCONNECT=0,"tcp://120.27.2.154:1883",20,1
OK
+CMQTTCONNECT:0,0
AT+CMQTTCONNECT?
+CMQTTCONNECT:0,"tcp://120.27.2.154:1883",20,1
+CMQTTCONNECT:1
OK
AT+CMQTTCONNECTisusedto connectto aMQTTserver.
Ifyou don’tsettheSSLcontextbyAT+CMQTTSSLCFGbeforeconnectingaSSL/TLSMQTTserverby
AT+CMQTTCONNECT,itwillusethe <client_index>(the 1stparameterof
AT+CMQTTCONNNECT)SSLcontextwhen connecting totheserver.
18.2.3 AT+CMQTTDISC Disconnectfromserver
AT+CMQTTDISCisusedto disconnectfrom the server.
NOTE


## Página 404

A76XX Series_AT Command Manual_V1.09
www.simcom.com 403 /652
AT+CMQTTDISC Disconnectfromserver
TestCommand
AT+CMQTTDISC=?
Response:
+CMQTTDISC:(0-1),(0,1-180)
OK
Read Command
AT+CMQTTDISC?
Response:
+CMQTTDISC:0,<disc_state>
+CMQTTDISC:1,<disc_state>
OK
WriteCommand
AT+CMQTTDISC=<client_in
dex>,<timeout>
Response
1)Ifdisconnectsuccessfully:
+CMQTTDISC:<client_index>,0
OK
2)Ifdisconnectsuccessfully:
OK
+CMQTTDISC:<client_index>,0
3)Iffailed:
OK
+CMQTTDISC:<client_index>,<err>
4)Iffailed:
ERROR
5)Iffailed:
+CMQTTDISC:<client_index>,<err>
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<timeout> Thetimeout valuefordisconnection.Theunitis second.Therangeis
1s to180s.Thedefaultvalue is0s (notsetthe timeoutvalue).
<disc_state> 1 disconnection
0 connection
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.


## Página 405

A76XX Series_AT Command Manual_V1.09
www.simcom.com 404 /652
Examples
AT+CMQTTDISC=0,120
OK
+CMQTTDISC:0,0
18.2.4 AT+CMQTTTOPIC Inputthetopicofpublishmessage
AT+CMQTTTOPICisusedto inputthe topic ofapublishmessage.
AT+CMQTTTOPIC Inputthetopicofpublishmessage
TestCommand
AT+CMQTTTOPIC=?
Response
+CMQTTTOPIC:(0-1),(1-1024)
OK
WriteCommand
AT+CMQTTTOPIC=<client_i
ndex>,<req_length>
Response
1)Ifsuccessfully:
>
<inputdatahere>
OK
2)Iffailed:
+CMQTTTOPIC:<client_index>,<err>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputtopicdata.Thepublish message topic shouldbe
UTF-encoded string.Therange isfrom1to 1024 bytes.
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.


## Página 406

A76XX Series_AT Command Manual_V1.09
www.simcom.com 405 /652
Examples
AT+CMQTTTOPIC=0,9
>
OK
Thetopicwillbe cleanafterexecuteAT+CMQTTPUB.
18.2.5 AT+CMQTTPAYLOAD Inputthepublishmessage
AT+CMQTTPAYLOADis used toinputthe messagebody of apublishmessage.
AT+CMQTTPAYLOAD Inputthepublishmessage
TestCommand
AT+CMQTTPAYLOAD=?
Response
+CMQTTPAYLOAD:(0-1),(1-10240)
OK
WriteCommand
AT+CMQTTPAYLOAD=<clien
t_index>,<req_length>
Response
1)Ifsuccessfully:
>
<inputdatahere>
OK
2)Iffailed:
+CMQTTPAYLOAD:<client_index>,<err>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
NOTE


## Página 407

A76XX Series_AT Command Manual_V1.09
www.simcom.com 406 /652
values is0to1.
<req_length> Thelength ofinputmessage data.Thepublish messageshouldbe
UTF-encoded string.Therange isfrom1to 10240bytes.
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.
Examples
AT+CMQTTPAYLOAD=0,6
>
OK
Thetopicwillbe cleanafterexecuteAT+CMQTTPUB.
18.2.6 AT+CMQTTPUB Publishamessagetoserver
AT+CMQTTPUBisusedto publishamessageto MQTTserver.
AT+CMQTTPUB Publishamessagetoserver
TestCommand
AT+CMQTTPUB=?
Response
+CMQTTPUB:(0-1),(0-2),(1-180),(0-1),(0-1)
OK
WriteCommand
AT+CMQTTPUB=<client_ind
ex>,<qos>,<pub_timeout>[,<
ratained>[,<dup>]]
Response
1)Ifsuccessfully:
OK
+CMQTTPUB:<client_index>,0
2)Iffailed:
OK
+CMQTTPUB:<client_index>,<err>
3)Iffailed:
+CMQTTPUB:<client_index>,<err>
ERROR
NOTE


## Página 408

A76XX Series_AT Command Manual_V1.09
www.simcom.com 407 /652
4)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<qos> Thepublish message’sqos.Therangeis from0to2.
0 at mostonce
1 at leastonce
2 exactly once
<pub_timeout> Thepublishing timeoutintervalvalue.Sincethe client publisha
message toserver,itwill reportfailed ifthe client receiveno response
from serverafterthe timeoutvalue seconds.Therangeis from1s to
180s.
<ratained> Theretainflagofthepublishmessage.Thevalueis0or1.Thedefault
value is0.
Whenaclient sends aPUBLISHtoa server,if theretainflag isset to
1,theservershouldholdontothemessageafterithasbeendelivered
to thecurrentsubscribers.
<dup> Thedupflag tothe message.Thevalue is 0or 1.Thedefaultvalue is
0.Theflagis setwhenthe client orserver attempts tore-delivera
message.
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.
Examples
AT+CMQTTPUB=0,1,60
OK
+CMQTTPUB:0,0
Thetopicandpayload willbe clean afterexecuteAT+CMQTTPUB.
NOTE


## Página 409

A76XX Series_AT Command Manual_V1.09
www.simcom.com 408 /652
18.2.7 AT+CMQTTSUBTOPIC Inputthetopicofsubscribemessage
AT+CMQTTSUBTOPICis used to inputthe topic ofasubscribemessage.
AT+CMQTTSUBTOPIC Inputthetopicofsubscribemessage
TestCommand
AT+CMQTTSUBTOPIC=?
Response
+CMQTTSUBTOPIC:(0-1),(1-1024),(0-2)
OK
WriteCommand
AT+CMQTTSUBTOPIC=<clie
nt_index>,<req_length>,<qo
s>
Response
1)Ifsuccessfully:
>
<inputdatahere>
OK
2)Iffailed:
+CMQTTSUBTOPIC:<client_index>,<err>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputtopicdata.Thepublish message topic shouldbe
UTF-encoded string.Therange isfrom1to 1024 bytes.
<qos> Thepublish message’sqos.Therangeis from0to2.
0 at mostonce
1 at leastonce
2 exactly once
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.
Examples
AT+CMQTTSUBTOPIC=0,9,1
>


## Página 410

A76XX Series_AT Command Manual_V1.09
www.simcom.com 409 /652
OK
Thetopicwillbe cleanafterexecuteAT+CMQTTSUB.
18.2.8 AT+CMQTTSUB Subscribeamessagetoserver
AT+CMQTTSUBisusedto subscribeamessage toMQTTserver.
AT+CMQTTSUB Subscribeamessagetoserver
TestCommand
AT+CMQTTSUB=?
Response
+CMQTTSUB:(0-1),(1-1024),(0-2),(0-1)
OK
Read Command
AT+CMQTTSUB?
Response
+CMQTTSUB:
[<topic>]
OK
WriteCommand
/*subscribeoneormoretopics
whichinputby
AT+CMQTTSUBTOPIC*/
AT+CMQTTSUB=<client_ind
ex>[,<dup>]
Response
1)Ifsuccessfully:
OK
+CMQTTSUB:<client_index>,0
2)Iffailed:
OK
+CMQTTSUB:<client_index>,<err>
3)Iffailed:
+CMQTTSUB:<client_index>,<err>
ERROR
4)Iffailed:
ERROR
WriteCommand
/*subcribe onetopic*/
AT+CMQTTSUB=<client_ind
ex>,<reqLength>,<qos>[,<d
up>]
Response
1)Ifsuccessfully:
>
<inputdatahere>
OK
NOTE


## Página 411

A76XX Series_AT Command Manual_V1.09
www.simcom.com 410 /652
+CMQTTSUB:<client_index>,0
2)Iffailed:
OK
+CMQTTSUB:<client_index>,<err>
3)Iffailed:
+CMQTTSUB:<client_index>,<err>
ERROR
4)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputtopicdata.Themessagetopic should be
UTF-encoded string.Therange isfrom1to 1024 bytes.
<qos> Thepublish message’sqos.Therangeis from0to2.
0 at mostonce
1 at leastonce
2 exactly once
<dup> Thedupflag tothe message.Thevalue is 0or 1.Thedefaultvalue is
0.Theflagis setwhenthe client orserver attempts tore-delivera
message.
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.
<topic> Topics towhichyou havesubscribed
Examples
AT+CMQTTSUB=0,9,1
>
OK
+CMQTTSUB:0,0
AT+CMQTTSUB=0,1
OK


## Página 412

A76XX Series_AT Command Manual_V1.09
www.simcom.com 411 /652
+CMQTTSUB:0,0
Thetopicwillbe cleanafterexecuteAT+CMQTTSUB.
18.2.9 AT+CMQTTUNSUBTOPIC Inputthetopicofunsubscribemessage
AT+CMQTTUNSUBTOPICis usedto inputthe topic ofaunsubscribemessage.
AT+CMQTTUNSUBTOPIC Inputthetopicofunsubscribemessage
TestCommand
AT+CMQTTUNSUBTOPIC=?
Response
+CMQTTUNSUBTOPIC:(0-1),(1-1024)
OK
WriteCommand
AT+CMQTTUNSUBTOPIC=<
client_index>,<req_length>
Response
1)Ifsuccessfully:
>
<inputdatahere>
OK
2)Iffailed:
+CMQTTUNSUBTOPIC:<client_index>,<err>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputtopicdata.Thepublish message topic shouldbe
UTF-encoded string.Therange isfrom1to 1024 bytes.
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.
NOTE


## Página 413

A76XX Series_AT Command Manual_V1.09
www.simcom.com 412 /652
Examples
AT+CMQTTUNSUBTOPIC=0,9
>
OK
Thetopicwillbe cleanafterexecuteAT+CMQTTUNSUB.
18.2.10AT+CMQTTUNSUB Unsubscribeamessagetoserver
AT+CMQTTUNSUBis used tounsubscribeamessageto MQTTserver.
AT+CMQTTUNSUB Unsubscribeamessagetoserver
TestCommand
AT+CMQTTUNSUB=?
Response
+CMQTTUNSUB:(0-1),(1-1024),(0-1)
OK
WriteCommand
/*unsubscribeone ormore
topicswhichinputby
AT+CMQTTUNSUBTOPIC*/
AT+CMQTTUNSUB=<client_i
ndex>,<dup>
Response
1)Ifsuccessfully:
OK
+CMQTTUNSUB:<client_index>,0
2)Iffailed:
OK
+CMQTTUNSUB:<client_index>,<err>
3)Iffailed:
+CMQTTUNSUB:<client_index>,<err>
ERROR
4)Iffailed:
ERROR
WriteCommand
/*unsubscribe onetopic*/
AT+CMQTTUNSUB=<client_i
ndex>,<reqLength>,<dup>
Response
1)Ifsuccessfully:
>
<inputdatahere>
NOTE


## Página 414

A76XX Series_AT Command Manual_V1.09
www.simcom.com 413 /652
OK
+CMQTTUNSUB:<client_index>,0
2)Iffailed:
OK
+CMQTTUNSUB:<client_index>,<err>
3)Iffailed:
+CMQTTUNSUB:<client_index>,<err>
ERROR
4)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<req_length> Thelength ofinputtopicdata.Themessagetopic should be
UTF-encoded string.Therange isfrom1to 1024 bytes.
<dup> Thedupflag tothe message.Thevalue is 0or 1.Thedefaultvalue is
0.Theflagis setwhenthe client orserver attempts tore-delivera
message.
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter18.3.
Examples
AT+CMQTTUNSUBTOPIC=0,9
>
OK
AT+CMQTTUNSUB=0,1
OK
+CMQTTUNSUB:0,0
NOTE


## Página 415

A76XX Series_AT Command Manual_V1.09
www.simcom.com 414 /652
Thetopicwillbe cleanafterexecuteAT+CMQTTUNSUB.
18.2.11AT+CMQTTCFG ConfiguretheMQTTContext
AT+CMQTTCFGisusedtoconfigurethe MQTTcontext.Itmustbecalled beforeAT+CMQTTCONNECT
andafterAT+CMQTTACCQ.Thesetting will becleared afterAT+CMQTTREL.
AT+CMQTTCFG ConfiguretheMQTTContext
TestCommand
AT+CMQTTCFG=?
Response
+CMQTTCFG:"checkUTF8",(0-1),(0-1)
+CMQTTCFG:"optimeout",(0-1),(20-120)
+CMQTTCFG:
"aliauth",(0-1),"productkey","devicename","devicesecret"
+CMQTTCFG:"version",(0-1),(3-4)
+CMQTTCFG:"argtopic",(0-1),(0-1)
OK
Read Command
AT+CMQTTCFG?
Response
+CMQTTCFG:0,<checkUTF8_flag>,<optimeout_val>
+CMQTTCFG:1,<checkUTF8_flag>,<optimeout_val>
OK
WriteCommand
/*Configurethe checkUTF8
flagof the specifiedMQTT
client context*/
AT+CMQTTCFG="checkUTF
8",<index>,<checkUTF8_flag
>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe max timeout
interval ofthe send orreceive
data operation*/
AT+CMQTTCFG="optimeout
",<index>,<optimeout_val>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues


## Página 416

A76XX Series_AT Command Manual_V1.09
www.simcom.com 415 /652
<checkUTF8_flag> Theflag toindicate whether tocheck thestring isUTF8 coding ornot,
the defaultvalue is1.
0 Not check UTF8coding.
1 Check UTF8coding.
<optimeout_val> Themax timeoutinterval ofsending orreceivingdata operation.The
rangeis from20 seconds to120 seconds,the defaultvalueis 120
seconds.
+CMQTTCFG:
"aliauth",(0-1),"productkey"
,"devicename","devicesecr
et"
(0-1):Anumericparameterthatidentifies aclient.Therange of
permittedvalues is0to1.
"productkey":Thestring isproductkeyofAlibabaCloud platform.
"devicename":Thestring isdevicenameofAlibaba Cloud platform.
"devicesecret":The stringisdevicesecret ofAlibabaCloud platform.
+CMQTTCFG:
"version",(0-1),(3-4)
(0-1):Anumericparameterthatidentifies aclient.Therange of
permittedvalues is0to1.
(3-4): VersionofMQTT.
3:MQTT3.1.Thedefaultvalue is3.
4: MQTT3.1.1.
+CMQTTCFG:
"argtopic",(0-1),(0-1)
Thecommand isusedto configure MQTTATofextension,please
refer to“A76XX Series MQTT_EX_ATCommand
Manual_V2.01.docx”.
(0-1):Anumericparameterthatidentifies aclient.Therange of
permittedvalues is0to1.
(0-1):Theflag toenablewhethertoshowlength ofreceived.
0: Notshowlengthof received.
1: Showlengthof received.
Examples
AT+CMQTTCFG?
+CMQTTCFG:0,1,120
+CMQTTCFG:1,1,120
OK
AT+CMQTTCFG="optimeout",0,24
OK
AT+CMQTTCFG="checkUTF8",0,0
OK
AT+CMQTTCFG?
+CMQTTCFG:0,0,24
+CMQTTCFG:1,1,120
OK


## Página 417

A76XX Series_AT Command Manual_V1.09
www.simcom.com 416 /652
Thesettingwillbe clearedafterAT+CMQTTREL.
18.3 CommandResultCodes
18.3.1 Descriptionof<err>
<err> Description
0 operation succeeded
1 failed
2 bad UTF-8string
3 sockconnectfail
4 sockcreatefail
5 sockclosefail
6 messagereceivefail
7 networkopen fail
8 networkclose fail
9 networknotopened
10 clientindexerror
11 noconnection
12 invalidparameter
13 notsupported operation
14 clientisbusy
15 requireconnection fail
16 socksending fail
17 timeout
18 topicis empty
19 clientisused
20 clientnotacquired
21 clientnotreleased
22 lengthoutofrange
23 networkis opened
24 packetfail
25 DNSerror
NOTE


## Página 418

A76XX Series_AT Command Manual_V1.09
www.simcom.com 417 /652
26 socketisclosedby server
27 connectionrefused: unacceptedprotocol version
28 connectionrefused: identifierrejected
29 connectionrefused: serverunavailable
30 connectionrefused: bad usernameor password
31 connectionrefused: notauthorized
32 handshakefail
33 notsetcertificate
34 Opensessionfailed
35 Disconnectfromserverfailed
18.4 UnsolicitedResultCodes
URC Description
+CMQTTCONNLOST:<client_index>,<cause>
Whenclient disconnectpassively,
URC"+CMQTTCONNLOST"willbe
reported,then userneed toconnect
MQTTserveragain.
+CMQTTNONET
When the network is become no
network, the module will report this
URC.
Ifreceivedthis message,you
shouldrestartthe MQTTserviceby
AT+CMQTTSTART.
+CMQTTRXSTART:
<client_index>,<topic_total_len>,<payload_total_len>
+CMQTTRXTOPIC:<client_index>,<sub_topic_len>
<sub_topic>
/*forlongtopic,splittomultiple packets toreport*/
[<CR><LF>+CMQTTRXTOPIC:
<client_index>,<sub_topic_len>
<sub_topic>]
+CMQTTRXPAYLOAD:<client_index>,<sub_payload_len>
<sub_payload>
/*forlongpayload, splitto multiple packets toreport*/
[+CMQTTRXPAYLOAD:<client_index>,<sub_payload_len>
<sub_payload>]
+CMQTTRXEND:<client_index>
Ifaclientsubscribes toone ormore
topics,any messagepublished to
those topicsare sentby theserver
tothe client.The followingURCis
usedfortransmittingthe message
published fromserver toclient.
1)+CMQTTRXSTART:
<client_index>,<topic_total_len>,<p
ayload_total_len>\r\n
Atthe beginning ofreceiving
published message,the module will
reportthis touser,andindicate
client indexwith <client_index>,the
topictotal length with
<topic_total_len>andthe payload
total lengthwith


## Página 419

A76XX Series_AT Command Manual_V1.09
www.simcom.com 418 /652
<payload_total_len>after "\r\n".
2)+CMQTTRXTOPIC:
<client_index>,<sub_topic_len>\r\n
<sub_topic>
Afterthe command
"+CMQTTRXSTART"received,the
module willreportthe second
messagetouser,andindicateclient
index with <client_index>, thetopic
packetlength with <sub_topic_len>
andthe topiccontent with
<sub_topic>after "\r\n".
Forlongtopic,itwillbesplitto
multiple packets toreportandthe
command "+CMQTTRXTOPIC"will
be send morethan oncewith the
restoftopic content.Thesumof
<sub_topic_len>isequalto
<topic_total_len>.
3)+CMQTTRXPAYLOAD:
<client_index>,<sub_payload_len>\
r\n<sub_payload>
Afterthe command
"+CMQTTRXTOPIC"received,the
module willsendthird messageto
user,andindicate client indexwith
<client_index>,the payload packet
length with <sub_payload_len>and
the payload contentwith
<sub_payload>after"\r\n".
Forlongpayload, the sameas
"+CMQTTRXTOPIC".
4)+CMQTTRXEND:<client_index>
Atlast,the modulewillsend fourth
messageto userand indicatethe
topicandpayload havebeen
transmittedcompletely.
DefinedValues
<client_index> Anumeric parameterthatidentifies aclient.Therangeofpermitted
values is0to1.
<cause> Thecauseofdisconnection.
1 Socketis closedpassively.


## Página 420

A76XX Series_AT Command Manual_V1.09
www.simcom.com 419 /652
2 Socketis reset.
3 Network isclosed.
<topic_total_len> ThelengthofmessagetopicreceivedfromMQTTserver.Therangeis
from 1to1024 bytes.
<payload_total_len> ThelengthofmessagebodyreceivedfromMQTTserver.Therangeis
from 1to10240 bytes.
<sub_topic_len> Thesub topic packetlength,Thesumof<sub_topic_len>isequal to
<topic_total_len>.
<sub_topic> Thesub topic content.
<sub_payload_len> Thesubmessagebodypacketlength,Thesumof<sub_payload_len>
is equalto<payload_total_len>.
<sub_payload> Thesub messagebody content.


## Página 421

A76XX Series_AT Command Manual_V1.09
www.simcom.com 420 /652
ATCommandsforSSL
19.1 OverviewofATCommandsforSSL
Command Description
AT+CSSLCFG Configure the SSLContext
AT+CCERTDOWN Download certificate into the module
AT+CCERTLIST Listcertificates
AT+CCERTDELE Delete certificates
AT+CCHSET Configure the reportmodeofsending andreceiving data
AT+CCHMODE Configure the modeofsending andreceiving data
AT+CCHSTART StartSSLservice
AT+CCHSTOP Stop SSLservice
AT+CCHADDR Getthe IPv4address
AT+CCHSSLCFG Setthe SSLcontext
AT+CCHCFG Configure the ClientContext
AT+CCHOPEN Connectto server
AT+CCHCLOSE Disconnect from server
AT+CCHSEND Send datato server
AT+CCHRECV Read the cacheddata thatreceived fromthe server
AT+CCERTMOVE Movethe certfrom filesystem tocertcontent
19.2 DetailedDescriptionofATCommandsforSSL
19.2.1 AT+CSSLCFG ConfiguretheSSLContext
AT+CSSLCFG ConfiguretheSSLContext
TestCommand
AT+CSSLCFG=?
Response
+CSSLCFG:"sslversion",(0-9),(0-4)


## Página 422

A76XX Series_AT Command Manual_V1.09
www.simcom.com 421 /652
+CSSLCFG:"authmode",(0-9),(0-3)
+CSSLCFG:"ignorelocaltime",(0-9),(0,1)
+CSSLCFG:"negotiatetime",(0-9),(10-300)
+CSSLCFG:"cacert",(0-9),(5-108)
+CSSLCFG:"clientcert",(0-9),(5-108)
+CSSLCFG:"clientkey",(0-9),(5-108)
+CSSLCFG:"password",(0-9),(5-108)
+CSSLCFG:"enableSNI",(0-9),(0,1)
OK
Read Command
AT+CSSLCFG?
Response
+CSSLCFG:
0,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
1,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
2,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
3,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
4,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
5,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
6,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
7,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
8,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca


## Página 423

A76XX Series_AT Command Manual_V1.09
www.simcom.com 422 /652
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
+CSSLCFG:
9,<sslversion>,<authmode>,<ignoreltime>,<negotiatetime>,<ca
_file>,<clientcert_file>,<clientkey_file>,<password_file>,<enabl
eSNI_flag>
OK
WriteCommand
/*Query the configurationofthe
specified SSLcontext*/
AT+CSSLCFG=<ssl_ctx_inde
x>
Response
+CSSLCFG:
<ssl_ctxindex>,<sslversion>,<authmode>,<ignoreltime>,<nego
tiatetime>,<ca_file>,<clientcert_file>,<clientkey_file>,<passwor
d_file>,<enableSNI_flag>
OK
WriteCommand
/*Configurethe versionof the
specified SSLcontext*/
AT+CSSLCFG="sslversion",<
ssl_ctx_index>,<sslversion>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe authentication
modeof the specifiedSSL
context*/
AT+CSSLCFG="authmode",<
ssl_ctx_index>,<authmode>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe ignore localtime
flagof the specifiedSSL
context*/
AT+CSSLCFG="ignorelocalti
me",<ssl_ctx_index>,<ignorel
time>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe negotiate
timeoutvalue ofthe specified
SSLcontext*/
AT+CSSLCFG="negotiatetime
",<ssl_ctx_index>,<negotiatet
ime>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*ConfiguretheserverrootCAof
the specified SSLcontext*/
AT+CSSLCFG="cacert",<ssl_
ctx_index>,<ca_file>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR


## Página 424

A76XX Series_AT Command Manual_V1.09
www.simcom.com 423 /652
WriteCommand
/*Configurethe client certificate
ofthe specified SSLcontext*/
AT+CSSLCFG="clientcert",<s
sl_ctx_index>,<clientcert_file
>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe client keyofthe
specified SSLcontext*/
AT+CSSLCFG="clientkey",<s
sl_ctx_index>,<clientkey_file>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe password ofthe
specified SSLcontext*/
AT+CSSLCFG="password",<s
sl_ctx_index>,<password_file
>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*Configurethe enableSNIflag
ofthe specified SSLcontext */
AT+CSSLCFG="enableSNI",<
ssl_ctx_index>,<enableSNI_fl
ag>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<ssl_ctx_index> TheSSLcontext ID.Therangeis 0-9.
<sslversion> TheSSLversion,the defaultvalueis 4.
0 SSL3.0
1 TLS1.0
2 TLS1.1
3 TLS1.2
4 All
Theconfigured versionshouldbe supportby server.Soyou should
usethe defaultvalue ifyou are notsurethat theversion which the
serversupported.
<authmode> Theauthenticationmode, thedefaultvalue is0.
0 noauthentication.
1 serverauthentication. Itneedsthe rootCAof the server.
2 serverand clientauthentication. Itneedsthe rootCAof the


## Página 425

A76XX Series_AT Command Manual_V1.09
www.simcom.com 424 /652
server,thecert andkey ofthe client.(Ifthe server does notneedto
authenticatethe client,it isequivalentto value1.)
3 clientauthentication and no serverauthentication. Itneeds the
certandkey ofthe client.((Ifthe serverdoes notneed to
authenticatethe client,it isequivalentto value0.)
<ignoreltime> Theflagtoindicate howtodeal with expired certificate,the default
valueis 1.
0 careabout timecheckforcertification.
1 ignoretimecheck forcertification
Whensetthevalueto0,itneedtosettherightcurrentdateandtime
byAT+CCLKwhenneed SSLcertification.
<negotiatetime> ThetimeoutvalueusedinSSLnegotiatestage.Therangeis10-300
seconds.Thedefaultvalueis 300.
<ca_file> TherootCAfilenameof SSLcontext.Thefilenamemusthavetype
like".pem"or".der".Thelength offilenameis from5to108bytes.
Ifthe filenamecontains non-ASCIIcharacters,the filepath
parametershouldcontain aprefix of{non-ascii}andthe quotation
mark(Thestringinthequotationmarkshouldbehexadecimalofthe
filename’sUTF8code).
Thereare two ways to download certificatefiles tomodule:
1.ByAT+CCERTDOWN.
2.ByFTPSorHTTPScommands.PleaserefertoChapter16&17of
thisdocument.
<clientcert_file> Theclientcert filenameofSSLcontext.Thefilenamemusthave
type like ".pem"or".der".Thelength offilenameis from5to108
bytes.
Ifthe filenamecontains non-ASCIIcharacters,the filepath
parametershouldcontain aprefix of{non-ascii}andthe quotation
mark(Thestringinthequotationmarkshouldbehexadecimalofthe
filename’sUTF8code).
Thereare two ways to download certificatefiles tomodule:
1.ByAT+CCERTDOWN.
2.ByFTPSorHTTPScommands.PleaserefertoChapter16&17of
thisdocument.
<clientkey_file> Theclientkey file nameofSSLcontext.Thefilenamemusthave
type like ".pem"or".der".Thelength offilenameis from5to108
bytes.
Ifthe filenamecontains non-ASCIIcharacters,the filepath
parametershouldcontain aprefix of{non-ascii}andthe quotation
mark(Thestringinthequotationmarkshouldbehexadecimalofthe
filename’sUTF8code).
Thereare two ways to download certificatefiles tomodule:


## Página 426

A76XX Series_AT Command Manual_V1.09
www.simcom.com 425 /652
1.ByAT+CCERTDOWN.
2.ByFTPSorHTTPScommands.PleaserefertoChapter16&17of
thisdocument.
<password_file> The password file nameofSSLcontext.this isusedtodecrypt the
clientkey.Thefilenamemusthavetype like ".pem"or".der".The
lengthoffilenameisfrom 5to 108bytes.
Ifthe filenamecontains non-ASCIIcharacters,the filepath
parametershouldcontain aprefix of{non-ascii}andthe quotation
mark(Thestringinthequotationmarkshouldbehexadecimalofthe
filename’sUTF8code).Pleasenote the password option only
supportedinASR1603.
Thereare two ways to download certificatefiles tomodule:
1.ByAT+CCERTDOWN.
2.ByFTPSorHTTPScommands.PleaserefertoChapter16&17of
thisdocument.
<enableSNI_flag> Theflagtoindicate that enable the SNIflagornot,the defaultvalue
is0.
0 notenableSNI.
1 enable SNI.
Examples
AT+CSSLCFG=?
+CSSLCFG:"sslversion",(0-9),(0-4)
+CSSLCFG:"authmode",(0-9),(0-3)
+CSSLCFG:"ignorelocaltime",(0-9),(0,1)
+CSSLCFG:"negotiatetime",(0-9),(10-300)
+CSSLCFG:"cacert",(0-9),(5-108)
+CSSLCFG:"clientcert",(0-9),(5-108)
+CSSLCFG:"clientkey",(0-9),(5-108)
+CSSLCFG:"password",(0-9),(5-108)
+CSSLCFG:"enableSNI",(0-9),(0,1)
OK
AT+CSSLCFG?
+CSSLCFG:0,4,0,1,300,"","","","",0
+CSSLCFG:1,4,0,1,300,"","","","",0
+CSSLCFG:2,4,0,1,300,"","","","",0
+CSSLCFG:3,4,0,1,300,"","","","",0
+CSSLCFG:4,4,0,1,300,"","","","",0
+CSSLCFG:5,4,0,1,300,"","","","",0
+CSSLCFG:6,4,0,1,300,"","","","",0
+CSSLCFG:7,4,0,1,300,"","","","",0


## Página 427

A76XX Series_AT Command Manual_V1.09
www.simcom.com 426 /652
+CSSLCFG:8,4,0,1,300,"","","","",0
+CSSLCFG:9,4,0,1,300,"","","","",0
OK
AT+CSSLCFG="authmode",0,0
OK
AT+CSSLCFG=6
+CSSLCFG:6,4,0,1,300,"","","","",0
OK
19.2.2 AT+CCERTDOWN Downloadcertificateintothemodule
AT+CCERTDOWN Downloadcertificateintothemodule
TestCommand
AT+CCERTDOWN=?
Response
+CCERTDOWN:(5-108),(1-10240)
OK
WriteCommand
AT+CCERTDOWN=<filename>,<
len>
Response
1)Ifit canbedownload:
>
<inputdatahere>
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<filename> Thenameofthecertificate/key/passwordfile.Thefilenamemusthave
typelike".pem"or".der".Thelengthoffilenameisfrom5to108bytes.
Ifthefilenamecontainsnon-ASCII characters,the filepath parameter
should containaprefixof {non-ascii}andthe quotationmark (The
stringinthe quotation markshouldbe hexadecimalof the filename’s
UTF8code).
ForExamples:Ifyou wantto downloadafile with name"中华.pem"in
UTF8,you’d betterconvert the "中华.pem"to UTF8coding
(&#x4E2D;&#x534E;.pem),then inputthe hexadecimal


## Página 428

A76XX Series_AT Command Manual_V1.09
www.simcom.com 427 /652
(262378344532443B262378353334453B2E70656D)ofUTF8coding.
<len> Thelengthofthefiledata tosend.Therangeisfrom1to10240bytes.
Usershould notethan everypacketdata shouldbe no largerthan
3072bytes.
Examples
AT+CCERTDOWN=?
+CCERTDOWN:(5-108),(1-10240)
OK
AT+CCERTDOWN="ls.pem",1970
>
OK
19.2.3 AT+CCERTLIST Listcertificates
AT+CCERTLIST Listcertificates
Execute Command
AT+CCERTLIST
Response
[+CCERTLIST:<file_name>
[+CCERTLIST:<file_name>]
…
]
OK
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<filename> Thecertificate/key/password files whichhasbeen downloadedto
the module.
Ifthe filenamecontains non-ASCIIcharacters,itwillshow the
non-ASCIIcharactersas UTF8code.
Examples


## Página 429

A76XX Series_AT Command Manual_V1.09
www.simcom.com 428 /652
AT+CCERTLIST
+CCERTLIST: "ls.pem"
OK
19.2.4 AT+CCERTDELE Deletecertificates
AT+CCERTDELE Deletecertificates
WriteCommand
AT+CCERTDELE=<filename>
Response
1)Ifremove thefile successfully:
OK
2)Else
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<filename> Thenameofthe certificate/keypassword file.Thefilenamemust
have type like ".pem"or".der".Thelength offilenameisfrom 5to
108bytes.
Ifthe filenamecontains non-ASCIIcharacters,the filepath
parametershouldcontain aprefix of{non-ascii}andthe quotation
mark(Thestringinthequotationmarkshouldbehexadecimalofthe
filename’sUTF8code).
ForExamples:Ifyou wantto download afile with name"中华.pem",
you shouldconvert the"中华.pem"to UTF8coding
(&#x4E2D;&#x534E;.pem),then inputthe hexadecimal
(262378344532443B262378353334453B2E70656D)ofUTF8
coding.
Examples
AT+CCERTDELE="ls.pem"
OK


## Página 430

A76XX Series_AT Command Manual_V1.09
www.simcom.com 429 /652
19.2.5 AT+CCHSET Configurethereportmodeofsendingandreceivingdata
AT+CCHSETis used toconfigurethe modeof sendingandreceiving data.Itmustbe calledbefore
AT+CCHSTART.
AT+CCHSET Configurethereportmodeofsendingandreceivingdata
TestCommand
AT+CCHSET=?
Response
+CCHSET:(0,1),(0,1)
OK
Read Command
AT+CCHSET?
Response
+CCHSET:<report_send_result>,<recv_mode>
OK
WriteCommand
AT+CCHSET=<report_send_res
ult>[,<recv_mode>]
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<report_send_result> Whetherto reportresultofCCHSEND, the defaultvalue is 0:
0 No.
1 Yes.Modulewillreport+CCHSEND:<session_id>,<err>toMCU
whencompletesending data.
<recv_mode> Thereceivingmode,the defaultvalueis 0:
0 Outputthe datato MCUwheneverreceived data.
1 Modulecaches thereceiveddata andnotifies MCUwith
+CCHEVENT:<session_id>,RECV EVENT.
MCUcan useAT+CCHRECV toreceivethe cached data(only in
manualreceivingmode).
Examples
AT+CCHSET=?
+CCHSET:(0,1),(0,1)
OK


## Página 431

A76XX Series_AT Command Manual_V1.09
www.simcom.com 430 /652
AT+CCHSET?
+CCHSET:0,0
OK
AT+CCHSET=1,1
OK
19.2.6 AT+CCHMODE Configurethemodeofsendingandreceivingdata
AT+CCHMODEisusedtoselecttransparentmode(datamode)ornon-transparentmode(commandmode).
Thedefaultmodeis non-transparentmode.ThisATcommandmustbe called beforecalling
AT+CCHSTART.
AT+CCHMODE Configurethemodeofsendingandreceivingdata
TestCommand
AT+CCHMODE=?
Response
+CCHMODE:(0,1)
OK
Read Command
AT+CCHMODE?
Response
+CCHMODE:<mode>
OK
WriteCommand
AT+CCHMODE=<mode>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<mode> Themodevalue:
0 Normal
1 Transparentmode
Examples
AT+CCHMODE=?


## Página 432

A76XX Series_AT Command Manual_V1.09
www.simcom.com 431 /652
+CCHMODE:(0,1)
OK
AT+CCHMODE?
+CCHMODE:0
OK
AT+CCHMODE=1
OK
There isonly one session inthe transparentmode,it’sthe firstsession.
19.2.7 AT+CCHSTART StartSSLservice
AT+CCHSTARTis used to startSSLservice byactivating PDPcontext.YoumustexecuteAT+CCHSTART
beforeany otherSSLrelated operations.
AT+CCHSTART StartSSLservice
Execute Command
AT+CCHSTART
Response
1)Ifstart SSLservice successfully:
OK
+CCHSTART:0
2)Iffailed:
ERROR
3)Iffailed:
ERROR
+CCHSTART:<err>
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<err> Theresult code,pleaserefertothe endofthis chapter
NOTE


## Página 433

A76XX Series_AT Command Manual_V1.09
www.simcom.com 432 /652
Examples
AT+CCHSTART
OK
+CCHSTART:0
19.2.8 AT+CCHSTOP StopSSLservice
AT+CCHSTOPis used tostop SSLservice.
AT+CCHSTOP StopSSLservice
Execute Command
AT+CCHSTOP
Response
1)Ifstop SSL servicesuccessfully:
OK
+CCHSTOP:0
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<err> Theresult code,pleaserefertothe endofthis chapter
Examples
AT+CCHSTOP
OK
+CCHSTOP:0


## Página 434

A76XX Series_AT Command Manual_V1.09
www.simcom.com 433 /652
19.2.9 AT+CCHADDR GettheIPv4address
AT+CCHADDRis used togetthe IPv4 addressafter callingAT+CCHSTART.
AT+CCHADDR GettheIPv4address
Execute Command
AT+CCHADDR
Response
1)ifsuccessfully,response
+CCHADDR:<ip_address>
OK
2)ifpdp has notbeen activated,response
ERROR
ParameterSaving Mode -
Max ResponseTime 12000ms
Reference -
DefinedValues
<ipaddress> Astringparameter thatidentifiesthe IPv4 addressafter PDP
activated.
Examples
AT+CCHADDR
+CCHADDR:10.43.71.130
OK
19.2.10AT+CCHSSLCFG SettheSSLcontext
AT+CCHSSLCFGis used tosetthe SSLcontextwhichtobe usedinthe SSLconnection.Itmustbecalled
beforeAT+CCHOPENandafterAT+CCHSTART.Thesetting will becleared afterAT+CCHOPEN failedor
AT+CCHCLOSE.
AT+CCHSSLCFG SettheSSLcontext
TestCommand
AT+CCHSSLCFG=?
Response
+CCHSSLCFG:(0,1),(0-9)


## Página 435

A76XX Series_AT Command Manual_V1.09
www.simcom.com 434 /652
OK
Read Command
AT+CCHSSLCFG?
Response
+CCHSSLCFG:<session_id>,[<ssl_ctx_index>]
+CCHSSLCFG:<session_id>,[<ssl_ctx_index>]
OK
WriteCommand
AT+CCHSSLCFG=<session_id>
,<ssl_ctx_index>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<session_id> Thesession_idto operate.It’s from 0to1.
<ssl_ctx_index> TheSSLcontext IDwhichwillbe used inthe SSL
connection.Refer tothe <ssl_ctx_index>ofAT+CSSLCFG.
Examples
AT+CCHSSLCFG=?
+CCHSSLCFG:(0,1),(0-9)
OK
AT+CCHSSLCFG?
+CCHSSLCFG:0,
+CCHSSLCFG:1,
OK
AT+CCHSSLCFG=0,1
OK
AT+CCHSSLCFGis used tosetthe SSLcontextwhichtobe usedinthe SSLconnection.Itmustbe
calledbeforeAT+CCHOPENandafterAT+CCHSTART.ThesettingwillbeclearedafterAT+CCHOPEN
failed orAT+CCHCLOSE
Ifyou don’tset theSSLcontextby thiscommand beforeconnecting toSSL/TLS serverby
NOTE


## Página 436

A76XX Series_AT Command Manual_V1.09
www.simcom.com 435 /652
AT+CCHOPEN,theCCHOPENoperationwillusetheSSLcontextassameasindex<session_id>(the
1stparameterofAT+CCHOPEN)when connecting tothe server.
19.2.11AT+CCHCFG ConfiguretheClientContext
AT+CCHCFGisused tosetthe clientsessioncontext.Itmustbe called beforeAT+CCHOPEN and after
AT+CCHSTART.Thesettingwillbe clearedafterAT+CCHOPENfailedorAT+CCHCLOSE.
AT+CCHCFG ConfiguretheClientContext
TestCommand
AT+CCHCFG=?
Response
+CCHCFG:"sendtimeout",(0-1),(60-150)
+CCHCFG: "sslctx",(0-1),(0-9)
OK
Read Command
AT+CCHCFG?
Response
+CCHCFG:0,<sendtimeout_val>,<sslctx_index>
+CCHCFG:1,<sendtimeout_val>,<sslctx_index>
OK
WriteCommand
/*Configurethe timeoutvalue of
the specified clientwhen sending
data*/
AT+CCHCFG="sendtimeout",<s
ession_id>,<sendtimeout_val>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
WriteCommand
/*ConfiguretheSSLcontextindex,
it’sas sameas
AT+CCHSSLCFG*/
AT+CCHCFG="sslctx",<session
_id>,<sslctx_index>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<session_id> Thesession_idto operate.It’s from0to1.
<sendtimeout_val> The timeout value used in sending data stage. The range is 60-150
seconds.Thedefaultvalueis 150.


## Página 437

A76XX Series_AT Command Manual_V1.09
www.simcom.com 436 /652
<sslctx_index> TheSSLcontext IDwhichwillbe usedin the SSLconnection.Refer to
the <ssl_ctx_index>ofAT+CSSLCFG.
Examples
AT+CCHCFG=?
+CCHCFG:"sendtimeout",(0-1),(60-150)
+CCHCFG:"sslctx",(0-1),(0-9)
OK
AT+CCHCFG?
+CCHCFG:0,150,
+CCHCFG:1,150,
OK
AT+CCHCFG="sendtimeout",0,120
OK
AT+CCHCFG="sslctx",0,3
OK
19.2.12AT+CCHOPEN Connecttoserver
AT+CCHOPENis used toconnectthe server.
AT+CCHOPEN Connecttoserver
TestCommand
AT+CCHOPEN=?
Response
+CCHOPEN:(0,1),"ADDRESS",(1-65535)[,(1-2)[,(1-65535)]]
OK
Read Command
AT+CCHOPEN?
Response
Ifconnectto aserver,itwillshowthe connectedinformation.
Otherwise,the connectedinformationis empty.
+CCHOPEN:0,<host>,<port>,<client_type>,<bind_port>
+CCHOPEN:1,<host>,<port>,<client_type>,<bind_port>
OK
WriteCommand
AT+CCHOPEN=<session_id>,<
host>,<port>[,<client_type>,[<bi
nd_port>]]
Response
1)Ifconnectsuccessfully:
OK
+CCHOPEN:<session_id>,0


## Página 438

A76XX Series_AT Command Manual_V1.09
www.simcom.com 437 /652
2)Ifconnectsuccessfully intransparentmode:
CONNECT[<text>]
3)Iffailed:
OK
+CCHOPEN:<session_id>,<err>
4)Iffailed:
ERROR
5)Iffailed intransparentmode:
CONNECTFAIL
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<session_id> Thesessionindexto operate.It’s from0to1.
<host> Theserveraddress,maximum lengthis 256bytes.
<port> Theserverport whichto beconnected, the range isfrom 1to
65535.
<client_type>
Thetype ofclient,defaultvalueis 2:
1 TCPclient.
2 SSL/TLSclient.
<bind_port> Thelocalportforchannel,the range is from 1to65535.
<text> CONNECTresultcode string;the stringformats please referATX
command.
<err> Theresultcode: 0issuccess.Othervaluesare failure.Please
refertochapter 19.3
Examples
AT+CCHOPEN=?
+CCHOPEN:(0,1),"ADDRESS",(1-65535)[,(1-2)[,(1-65535)]]
OK
AT+CCHOPEN=0,"183.230.174.137",6043,1
OK
+CCHOPEN:0,0
AT+CCHOPEN?
+CCHOPEN:0,"183.230.174.137",6043,1,
+CCHOPEN:1,"",,,


## Página 439

A76XX Series_AT Command Manual_V1.09
www.simcom.com 438 /652
OK
Ifyou don’tset theSSLcontextbyAT+CCHSSLCFGbefore connectingaSSL/TLS serverby
AT+CCHOPEN, itwill usethe <session_id>(the 1’stparameterofAT+CCHOPEN)SSLcontextwhen
connecting tothe server.
19.2.13AT+CCHCLOSE Disconnectfromserver
AT+CCHCLOSEisused todisconnectfrom the server.
AT+CCHCLOSE Disconnectfromserver
WriteCommand
AT+CCHCLOSE=<session_id>
Response
1)Ifsuccessfully:
OK
+CCHCLOSE:<session_id>,0
2)Ifsuccessfully in transparent mode:
OK
CLOSED
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<session_id> Thesessionindexto operate.It’s from0to1.
<err> Theresultcode: 0issuccess.Othervaluesare failure.Please
refertothe end ofthischapter.
Examples
NOTE


## Página 440

A76XX Series_AT Command Manual_V1.09
www.simcom.com 439 /652
AT+CCHCLOSE=0
OK
+CCHCLOSE:0,0
19.2.14AT+CCHSEND Senddatatoserver
AT+CCHSEND Senddatatoserver
TestCommand
AT+CCHSEND=?
Response
+CCHSEND:(0,1),(1-2048)
OK
Read Command
AT+CCHSEND?
Response
+CCHSEND:0,<unsent_len_0>,1,<unsent_len_1>
OK
WriteCommand
AT+CCHSEND=<session_id>,<l
en>
Response
1)ifparameterisright:
>
<inputdatahere>
Whenthe totalsize ofthe inputted datareaches <len>,TAwill
reportthe followingcode.Otherwise, the serial portwillbe
blocked.
OK
2)Ifparameter iswrong orothererrors occur:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<session_id> Thesession_id tooperate.It’sfrom0to 1.
<len> Thelength ofdata tosend.Its range is from 1to2048 bytes.
<unsent_len_0> Thedataofconnection 0cached insendingbufferwhichiswaiting
tobe sent.
<unsent_len_1> Thedataofconnection 1cached insendingbufferwhichiswaiting
tobe sent.
Examples


## Página 441

A76XX Series_AT Command Manual_V1.09
www.simcom.com 440 /652
AT+CCHSEND=?
+CCHSEND:(0,1),(1-2048)
OK
AT+CCHSEND?
+CCHSEND:0,0,1,0
OK
AT+CCHSEND=0,121
>GET /HTTP/1.1
Host:www.baidu.com
User-Agent:MAUIhtpUserAgent
Proxy-Connection:keep-alive
Content-Length:0
OK
19.2.15AT+CCHRECV Readthecacheddatathatreceivedfromtheserver
AT+CCHRECV Readthecacheddatathatreceivedfromtheserver
Read Command
AT+CCHRECV?
Response
+CCHRECV:LEN,<cache_len_0>,<cache_len_1>
OK
WriteCommand
AT+CCHRECV=<session_id>[,<
max_recv_len>]
Response
1)ifparameterisright and there arecached data:
OK
[+CCHRECV:DATA,<session_id>,<len>
…
+CCHRECV:DATA,<session_id>,<len>
…]
+CCHRECV:<session_id>,<err>
2)ifparameterisnotrightor any othererroroccurs:
+CCHRECV:<session_id>,<err>
ERROR
3)others:


## Página 442

A76XX Series_AT Command Manual_V1.09
www.simcom.com 441 /652
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<session_id> Thesessionidtooperate. It’sfrom 0to1.
<max_recv_len> Maximumbytes ofdata toreceivein the currentAT+CCHRECV
calling.Thevalueranges from 0to2048.
0means itwillreceivealldatafrom the currentcache.
Thedefaultvalueis 0anditwillreceiveallofRXdata cached for
session<session_id>.
Itwillbenotallowed when there isnodata inthe cache.
<cache_len_0> Thelength ofRXdatacached forconnection 0.
<cache_len_1> Thelength ofRXdatacached forconnection 1.
<len> Thelength ofdata followed.
<err> Theresultcode: 0issuccess.Othervaluesare failure.Please
refertochapter 19.3
Examples
AT+CCHRECV?
+CCHRECV:LEN,3072,0
OK
AT+CCHRECV=0
OK
+CCHRECV:DATA,0,1024
HTTP/1.1 200 OK
Bdpagetype:1
Bdqid: 0x9821f6dd000060aa
Cache-Control: private
Connection:keep-alive
Content-Type:text/html;charset=utf-8
Date:Tue,24 Mar2020 02:27:10GMT
Expires:Tue,24 Mar2020 02:26:31 GMT
P3p:CP="OTIDSP CORIVAOURINDCOM "
P3p:CP="OTIDSP CORIVAOURINDCOM "
Server:BWS/1.1
Set-Cookie:BAIDUID=F0CD980BA0927350B147AB1064A3423D:FG=1;expires=Thu,31-Dec-37


## Página 443

A76XX Series_AT Command Manual_V1.09
www.simcom.com 442 /652
23:55:55GMT;max-age=2147483647; path=/;domain=.baidu.com
Set-Cookie:BIDUPSID=F0CD980BA0927350B147AB1064A3423D;expires=Thu,31-Dec-37 23:55:55
GMT;max-age=2147483647;path=/;domain=.baidu.com
Set-Cookie:PSTM=1585016830;expires=Thu,31-Dec-37 23:55:55 GMT;max-age=2147483647;
path=/;domain=.baidu.com
Set-Cookie:BAIDUID=F0CD980BA0927350739AA64356C3CB13:FG=1;max-age=31536000;
expires=Wed,24-Mar-21 02:27:10GMT;domain=.baidu.com;path=/;version=1;comment=bd
Set-Cookie:BDSVRTM=0;path=/
Set-Cookie:BD_HOME=1;path=/
Set-Cookie:H_PS_PSSID=30972_1467_21116_30823;path=/;domain=.baidu.com
Traceid
+CCHRECV:DATA,0,1024
:1585016830040414772210962314397044727978
Vary:Accept-Encoding
Vary:Accept-Encoding
X-Ua-Compatible: IE=Edge,chrome=1
Transfer-Encoding: chunked
b5e
<!DOCTYPE html><!--STATUSOK--><html><head><meta http-equiv="Content-Type"
content="text/html;charset=utf-8"><meta http-equiv="X-UA-Compatible"
content="IE=edge,chrome=1"><meta content="always"name="referrer"><meta
name="theme-color" content="#2932e1"><link rel="shortcuticon" href="/favicon.ico"
type="image/x-icon"/><link rel="search"type="application/opensearchdescription+xml"
href="/content-search.xml"title="鐧惧害鎼滅储"/><linkrel="icon"sizes="any" mask
href="//www.baidu.com/img/baidu_85beaf5496f291521eb75ba38eacbd87.svg"><link
rel="dns-prefetch"href="//dss0.bdstatic.com"/><linkrel="dns-prefetch"
href="//dss1.bdstatic.com"/><linkrel="dns-prefetch"href="//ss1.bdstatic.com"/><link
rel="dns-prefetch"href="//sp0.baidu.com"/><linkrel="dns-prefetch" href="//sp1.baidu.com"/><link
rel="dns-prefetch"href="//sp2.baidu.com"/><title>鐧惧害涓€涓?
+CCHRECV:DATA,0,1024
紝浣犲氨鐭ラ亾</title><style type="text/css"id="css_index"
index="index">body,html{height:100%}html{overflow-y:auto}body{font:12px
arial;background:#fff}body,form,li,p,ul{margin:0;padding:0;list-style:none}#fm,body,form{position:
relative}td{text-align:left}img{border:0}a{text-decoration:none}a:active{color:#f60}input{border:0;p
adding:0}.clearfix:after{content:'\20';display:block;height:0;clear:both}.clearfix{zoom:1}#wrapper{p
osition:relative;min-height:100%}#head{padding-bottom:100px;text-align:center;*z-index:1}#ftCon{
height:50px;position:absolute;text-align:left;width:100%;margin:0
auto;z-index:0;overflow:hidden}#ftConw{display:inline-block;text-align:left;margin-left:33px;line-he
ight:22px;position:relative;top:-2px;*float:right;*margin-left:0;*position:static}#ftConw,#ftConw
a{color:#999}#ftConw{text-align:center;margin-left:0}.bg{background-image:url(http://ss.bdimg.co
m/static/superman/img/icons-5859e577e2.png);background-repeat:no-repeat;_background-image:u
rl(http://ss.bdimg.com/static/superman/img/icon
+CCHRECV:0,0


## Página 444

A76XX Series_AT Command Manual_V1.09
www.simcom.com 443 /652
+CCHEVENT: 0,RECVEVENT
Ifconnection isclosedby server,the cacheddata willnotbecleaned.
19.2.16AT+CCERTMOVE Move thecertfromfilesystemtocertcontent
AT+CCERTMOVE Move thecertfromfilesystemtocertcontent
TestCommand
AT+CCERTMOVE=?
Response
+CCERTMOVE:"FILENAME"
OK
WriteCommand
AT+CCERTMOVE=<filename>
Response
1)ifparameterisright and the fileneed tomove isexist:
OK
2)ifparameterisnotrightor any othererroroccurs:
ERROR
3)others:
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<filename> Thefilenameexistin filesystem,canbefound byAT+FSLS.The
filenamemusthavetype like ".pem"or".der".Thelength of
filenameis from 5to108bytes.
Examples
AT+CCERTMOVE="baidu.der"
OK
NOTE


## Página 445

A76XX Series_AT Command Manual_V1.09
www.simcom.com 444 /652
19.3 CommandResultCodes
19.3.1 Descriptionof<err>
Resultcodes Description
0 Operationsucceeded
1 Alerting state(reserved)
2 Unknown error
3 Busy
4 Peerclosed
5 Operationtimeout
6 Transferfailed
7 Memoryerror
8 Invalidparameter
9 Networkerror
10 Opensessionerror
11 Stateerror
12 Create socketerror
13 GetDNSerror
14 Connectsocketerror
15 Handshakeerror
16 Closesocketerror
17 Nonet
18 Senddata timeout
19 Notsetcertificates
19.4 UnsolicitedResultCodes
URC Description
+CCHEVENT:<session_id>,RECVEVENT
Inmanualreceiving mode,when newdataofa
connectionarrivingto the module,this unsolicited
resultcode willbe reported toMCU.


## Página 446

A76XX Series_AT Command Manual_V1.09
www.simcom.com 445 /652
+CCH_RECV_CLOSED:<session_id>,<err> Whenreceive data occurredany error,this
unsolicitedresult code willbereportedto MCU.
+CCHSEND:<session_id>,<err> Whensend datafaild,errorcodewillbe reported.
+CCH_PEER_CLOSED:<session_id> Theconnectionis closedby the server.
+CCH:CCHSTOP CCHstoppedcaused bynetwork error.


## Página 447

A76XX Series_AT Command Manual_V1.09
www.simcom.com 446 /652
ATCommandsforTTS
20.1 OverviewofATCommandsforTTS
Command Description
AT+CTTS TTSoperation
AT+CTTSPARAM SetTTSparameters
AT+CDTAM SetTTSPlayPath
20.2 DetailedDescriptionofATCommandsforTTS
20.2.1 AT+CTTS TTSoperation
Thewritecommand isusedto play/decode/pauseTTS.
AT+CTTS TTSoperation
TestCommand
AT+CTTS=?
Response
OK
Read Command
AT+CTTS?
Response
+CTTS:<status>
OK
WriteCommand
AT+CTTS=<mode>,[<text>],[<fil
ename>]
Response
1)If<mode>is 0,andtts is playing:
+CTTS:0
OK
2)If<mode>is 0,andtts is not playing:
OK
3)If<mode>is 1or2:
+CTTS:


## Página 448

A76XX Series_AT Command Manual_V1.09
www.simcom.com 447 /652
OK
+CTTS:0 //transform end
4)If<mode>is 3or4:
+CTTS:
OK
+CTTS:0 //transformend
5)
ERROR
ParameterSaving Mode -
Max ResponseTime 120000ms
Reference -
DefinedValues
<status> 0 NO_WORKING
1 TTS_WORKING
<mode> 0 Stopthe speechplay
1 Starttosynth andplay,<text>isin UCS2coding format.
2 Starttosynth andplay,<text>isinASCIIcodingformat,
Chinsestextis inGBKcoding format.
3 TTSTowav format,<text>isinASCII coding format,Chinese
textisin GBKcodingformat.
4 TTSTowav format,<text>isin UCS2coding format.
<text> Whenmode is1or2,the <text>isinASCIIcodingformatwhichis
synthetized tospeed tobe played,maximumdata length is512
bytes.(including "").And<text>isin UCS2 codingformat,
maximumdata lengthis 510bytes.(including ""),because every
fourcharacterscorrespondto oneChinesecharacter.Whenmode
is3or4,<text>maximumdata length is50bytesbecauseof the
memory.ForTOUNGTONETTSChinesepolyphonic characters,
pronunciationcan be specified.format: 多音字<pinyin=发音读音+
声调>.ForIFLYTTSChinese polyphonic characters,pronunciation
canbe specified. format: 多音字[=发音读音+声调].
<filename> Enterpath andfilename,ifno path isadded,savein C:by default.
Maximum filenamelength is60 bytes.
Currentlyonly .wav formatfile storage issupported.
NOTE


## Página 449

A76XX Series_AT Command Manual_V1.09
www.simcom.com 448 /652
1.Lowercase Englishstrings arepredicted tobe pronouncedaccording toEnglish words.Ifyou want
long lowercasestrings tobe pronounced correctly,pleaseadd aspace aftereachletter.
2.WhenusingIFlytekTTS,itisrecommendedthatyouinputmeasurementunitsandcombinedunitsin
the text inChinese. Englishinputmaynotbe able toreadcorrectly.
Examples
AT+CTTS=?
OK
AT+CTTS?
+CTTS:0
OK
AT+CTTS=1,"6B228FCE4F7F75288BED97F3540862107CFB7EDF"
+CTTS:
OK
+CTTS:0
AT+CTTS=2,"去朝<pinyin=chao2>阳，看朝<pinyin=zhao1>阳"
+CTTS:
OK
+CTTS:0
AT+CTTS=3,"hello world","C:/12.wav"
+CTTS:
OK
+CTTS:0
20.2.2 AT+CTTSPARAM SetTTSParametersforYOUNGTONETTS
Thewritecommand isusedto SetTTSParameters forYOUNGTONETTS.
AT+CTTSPARAM SetTTSParametersforYOUNGTONETTS
TestCommand
AT+CTTSPARAM=?
Response
+CTTSPARAM:(0-2),(0-3),(0-3),(0-2),(0-2|10-30),(0-1)


## Página 450

A76XX Series_AT Command Manual_V1.09
www.simcom.com 449 /652
OK
Read Command
AT+CTTSPARAM?
Response
+CTTSPARAM:
<volume>,<sysvolume>,<digitmode>,<pitch>,<speed>,<digitr
eading>
OK
WriteCommand
AT+CTTSPARAM=<volume>[,<s
ysvolume>[,<digitmode>[,<pitc
h>[,<speed>[,<digitreading>]]]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode Just<volume>isAUTO_SAVE
Max ResponseTime 9000ms
Reference Ventor
DefinedValues
<volume> 0 Themin volume
1 Thenormalvolume
2 Themax volume
<sysvolume> 0 Themin system volume
1 Thesmall systemvolume
2 Thenormalsystem volume
3 Themax system volume
<digitmode> 0 Read digitbased ondefaultmode
1 Read digitbased ontelephone number
2 Read digitbased ondigit
3 Read digitbased onnumber
<pitch> 0 Themin voicetone
1 Thenormalvoice tone
2 Themax voice tone
<speed> Two kindsof speedregulation methods:
1 rough speedregulation.Thethree grades rangefrom0to2. (0is
the minimumspeed, 1is the generaldefaultspeed,and2 isthe
maximum speed)
2 precision speed regulation.20levels rangefrom 10to 30.(10 is
minimum speed,30 ismaximum speed).Justsupportedon 1601.
<digitreading> 0 Read the numbersin Chinese
1 Read the numbersin English
Examples
AT+CTTSPARAM=?


## Página 451

A76XX Series_AT Command Manual_V1.09
www.simcom.com 450 /652
+CTTSPARAM:(0-2),(0-3),(0-3),(0-2),(0-2|10-30),(0-1)
OK
AT+CTTSPARAM?
+CTTSPARAM:1,3,0,1,1,0
OK
AT+CTTSPARAM=2,3,0,1,1,1
OK
AT+CTTSPARAM=2,3,0,1,10,1
OK
20.2.3 AT+CTTSPARAM SetTTSParametersforIFLYTTS
Thewritecommand isusedto SetTTSParameters forIFLYTTS.
AT+CTTSPARAM SetTTSParametersforIFLYTTS
TestCommand
AT+CTTSPARAM=?
Response
+CTTSPARAM:(0-7),(0-2),(0-2),(0-2),(0-2),(0-1)
OK
Read Command
AT+CTTSPARAM?
Response
+CTTSPARAM:
<volume>,<sysvolume>,<digitmode>,<pitch>,<speed>,<ttslib
>
OK
WriteCommand
AT+CTTSPARAM=<volume>[,<s
ysvolume>[,<digitmode>[,<pitc
h>[,<speed>[,<ttslib>]]]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode Just<volume>isAUTO_SAVE
Max ResponseTime 9000ms
Reference Ventor
DefinedValues
<volume> Setttingsystem volumelike AT+COUTGAIN,default valueis 4.
<sysvolume> 0 Themin system volume


## Página 452

A76XX Series_AT Command Manual_V1.09
www.simcom.com 451 /652
1 Thesmall systemvolume
2 Thenormalsystem volume
<digitmode> 0 Read digitbased ondefaultmode
1 Read digitbased ondigit
2 Read digitbased onnumber
<pitch> 0 Themin voicetone
1 Thenormalvoice tone
2 Themax voice tone
<speed> 0 Themin voicespeed
1 Thenormalvoice speed
2 Themax voice speed
<ttslib> 0 Supportboth Chineseand EnglishTTSlibraries,andChinese
pronunciation is thebest.
1 Supportonly EnglishTTSlibrary.
Examples
AT+CTTSPARAM=?
+CTTSPARAM:(0-7),(0-2),(0-2),(0-2),(0-2),(0-1)
OK
AT+CTTSPARAM?
+CTTSPARAM:1,2,0,1,1,0
OK
AT+CTTSPARAM=2,2,0,1,1,1
OK
AT+CTTSPARAM=7,2,0,1,1,1
OK
<ttslib>fucntionis notsupporting atpresent.
20.2.4 AT+CDTAM SetLocalorRemoteAudioPlay
Thewritecommand isusedto SetTTSPlay Path.
AT+CDTAM SetTTSPlayPath
TestCommand
AT+CDTAM=?
Response
+CDTAM:(0-1)
NOTE


## Página 453

A76XX Series_AT Command Manual_V1.09
www.simcom.com 452 /652
OK
Read Command
AT+CDTAM?
Response
+CDTAM:<status>
OK
WriteCommand
AT+CDTAM=<mode>
Response
1)If<mode>is 0or1:
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime -
Reference -
DefinedValues
<status> 0 LocalPlayer
1 Remote Player
<mode> 0 LocalPath
1 Remote Path
Examples
AT+CDTAM=?
+CDTAM:(0-1)
OK
AT+CDTAM?
+CDTAM:0
OK
AT+CDTAM=1
OK


## Página 454

A76XX Series_AT Command Manual_V1.09
www.simcom.com 453 /652
ATCommandsforAudio
21.1 OverviewofATCommandsforAudio
Command Description
AT+CCMXPLAY play an audiofile
AT+CCMXSTOP stop playing audiofile
AT+CREC record audiofile
AT+CRTSWITCH close ringtone
AT+CRINGSET setringfile
21.2 DetailedDescriptionofATCommandsforAudio
21.2.1 AT+CCMXPLAY Playaudiofile
This commandis used toplay an audiofile.
AT+CCMXPLAY Playaudiofile
TestCommand
AT+CCMXPLAY=?
Response
+CCMXPLAY:(list ofsupported<play_path>s),(listof supported
<repeat>s)
OK
WriteCommand
AT+CCMXPLAY=<file_na
me>,<play_path>,<repeat>
Response
1)
+CCMXPLAY:
OK
+AUDIOSTATE:audio play


## Página 455

A76XX Series_AT Command Manual_V1.09
www.simcom.com 454 /652
+AUDIOSTATE:audio playstop
2)
ERROR
ParameterSaving Mode
Max ResponseTime
Reference
DefinedValues
<file_name> Thenameof audiofile.
Supportaudio fileformat amr,wav,mp3andpcm.
<play_path> 0 local path
1 remote path (justsupportvoice call)
<repeat> 0 don’tplay repeat.play only once.
1…255 play repeattimes.E.g.<repeat>=1,audio will play twice.
Examples
AT+CCMXPLAY=?
+CCMXPLAY:(0-1),(0-255)
OK
AT+CCMXPLAY="c:/recording.amr",0,255
+CCMXPLAY:
OK
+AUDIOSTATE:audio play
+AUDIOSTATE:audio playstop
AT+CCMXPLAY="c:/recording.wav",0,255
+CCMXPLAY:
OK
+AUDIOSTATE:audio play
+AUDIOSTATE:audio playstop
NOTE


## Página 456

A76XX Series_AT Command Manual_V1.09
www.simcom.com 455 /652
1.support file type:AMR,WAV,MP3,PCM
2.PCM filemusthaveaheadertoplay,otherwise playbackis invalid.


## Página 457

A76XX Series_AT Command Manual_V1.09
www.simcom.com 456 /652
21.2.2 AT+CCMXSTOP Stopplayingaudiofile
Thecommand isusedtostop playing audiofile. Executethis commandduringaudio playing.If
audio filewas played end inthe past,when you execute "AT+CCMXSTOP",there is no
"+AUDIOSTATE:audioplay stop".
AT+CCMXSTOP Stopplayingaudiofile
TestCommand
AT+CCMXSTOP=?
Response
OK
Execution Command
AT+CCMXSTOP
Response
1)
+CCMXSTOP:
OK
+AUDIOSTATE:audio playstop
2)
OK
ParameterSaving Mode
Max ResponseTime
Reference
Examples
AT+CCMXSTOP
+CCMXSTOP:
OK
+AUDIOSTATE:audio playstop
21.2.3 AT+CREC RecordaudioFile
This commandis used torecordawav/amraudio file.Itcanrecord wav/amrfile duringacall ornot,the
recordfile should beputinto the"c:/".Thesupported fileformatis WAVandAMR.The recordpath 3only
supported onA7600 Series(1601)andA7678Series(1603)andothers support1and2. Only SDcard
supportNon-ASCII characters infilepath.


## Página 458

A76XX Series_AT Command Manual_V1.09
www.simcom.com 457 /652
AT+CREC RecordaudioFile
TestCommand
AT+CREC=?
Response
+CREC:(0-3), [{non-ascii}]"FILEPATH"
OK
Read Command
AT+CREC?
Response
+CREC:(listcurrent<status>s)
OK
WriteCommand
AT+CREC=<record_path>,<file_
name>
Response
1)
+CREC:1
OK
2)
+CREC:2
OK
3)
+CREC:3
OK
4)
ERROR
WriteCommand
AT+CREC=<mode>
Response
1)
+CREC:0
OK
2)Ifit isrecording:
+CREC:0
OK
+CREC:crecstop
3)
ERROR
ParameterSaving Mode
Max ResponseTime
Reference
DefinedValues


## Página 459

A76XX Series_AT Command Manual_V1.09
www.simcom.com 458 /652
<record_path> 1 local path
2 remote path (getvoicefromcs call)
3 mixd(localandremote)
<file_name> Thenameof wav/amraudiofile.(MAXis 60 bytes)
<status> 0 free
1 busy
<mode> 0 stop record
Examples
AT+CREC=?
+CREC:(0-3), [{non-ascii}]"FILEPATH"
OK
AT+CREC?
+CREC:0
OK
AT+CREC=1,"c:/recording.wav"
+CREC:1
OK
+CREC:file full
AT+CREC=2,"c:/recording.wav"
+CREC:2
OK
AT+CREC=0
+CREC:0
OK
+CREC:crec stop
AT+CREC=1,"c:/recording.amr"
+CREC:1
OK
+CREC:file full


## Página 460

A76XX Series_AT Command Manual_V1.09
www.simcom.com 459 /652
AT+CREC=2,"c:/recording.amr"
+CREC:2
OK
AT+CREC=0
+CREC:0
OK
+CREC:crec stop
AT+CREC=3,"c:/recording3.amr"
+CREC:3
OK
AT+CREC=0
+CREC:0
OK
+CREC:crec stop
 Whenthe fileisrecoding full, Response"+CREC:filefull"isdisplayed.
 Maximum size ofwavefile is768KBandmaximum size ofamrfileis 512KB.Whenthe filesystem
freesize islessthan the maximum size ofrecording file,the maximum sizeof recording fileisfile
system free size.
 Mixdrecordingfunction(record_path setvalue3)is justforA7600Series(1600) andA7678
Series(1603).
NOTE


## Página 461

A76XX Series_AT Command Manual_V1.09
www.simcom.com 460 /652
21.2.4 AT+CRTSWITCH CloseRingTone
This commandis used toSetringtoneon oroff.
AT+CRTSWITCH CloseRingTone
TestCommand
AT+CRTSWITCH=?
Response
+CRTSWITCH:(0-1)
OK
Read Command
AT+CRTSWITCH?
Response
+CRTSWITCH:(listcurrent<status>s)
OK
WriteCommand
AT+CRTSWITCH=<status>
Response
1)
OK
2)
ERROR
ParameterSaving Mode
Max ResponseTime
Reference
DefinedValues
<status> 0 close Ringtone
1 open,default value.
Examples
AT+CRTSWITCH=?
+CRTSWITCH:(0-1)
OK
AT+CRTSWITCH?
+CRTSWITCH:1
OK
AT+CRTSWITCH=0
+CRTSWITCH:0
OK
NOTE


## Página 462

A76XX Series_AT Command Manual_V1.09
www.simcom.com 461 /652
 Andthe commandjustsupported onA7678 Series(1603)Standardversion .
21.2.5 AT+CRINGSET SetRingFile
This commandis used toSelectringtone settings.SupportAMR,WAV,MP3 format.
AT+CRINGSET SetRingFile
TestCommand
AT+CRINGSET=?
Response
+CRINGSET:<fileName>
OK
Read Command
AT+CRINGSET?
Response
+CRINGSET:<fileName>
OK
WriteCommand
AT+CRINGSET
=<fileName>[,<mode>]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime
Reference
DefinedValues
<fileName> Ringtone file forsetting.Thedefaultfilenameis“iPhone_Ring.mp3”.
<mode> Whetherthe ringfunction is enable.
1 Enable toplay ringtone.
0 Disable toplay.
Examples
AT+CRINGSET=?
+CRINGSET:<fileName>
OK
AT+CRINGSET?
+CRINGSET:iPhone_Ring.mp3


## Página 463

A76XX Series_AT Command Manual_V1.09
www.simcom.com 462 /652
OK
AT+CRINGSET="C:/ring.amr"
OK
 Andthe commandjustsupported onA7678 Series(1603)Standardversion .
21.2.6 AT+CCODECSWITCH Switchcodectype
This commandis used toSwtichcodec type.Ourmodel supportbuilt-incodec andexternel codec.Built-in
codec byASRchip support,externel codec onlysupport nau8810.
AT+CCODECSWTICH Setcodectype
Read Command
AT+CCODECSWITCH?
Response
+CCODESWITCH:<type>
OK
WriteCommand
AT+CCODECSWITCH=<type>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE_REBOOT:
Max ResponseTime
Reference
DefinedValues
<type> Codec type,0 isexternel codec,1is built-incodec
Examples
AT+CCODECSWITCH?
+CCODECSWITCH:1
OK
NOTE


## Página 464

A76XX Series_AT Command Manual_V1.09
www.simcom.com 463 /652
AT+CCODECSWITCH=0
OK
 Andthe commandjustsupported onA7678 Series(1603)Standardversion .Only supports
firmware after2022.03.21.
21.2.7 AT+CLDTMF LocalDTMFToneGeneration
This commandis used toplay DTMFtone.
AT+CLDTMF LocalDTMFToneGeneration
TestCommand
AT+CLDTMF=?
Response
+CLDTMF:(1-100),(0-9,A,B,C,D,*,#),(50-500),(0-2)
OK
Execution Command
AT+CLDTMF
Response
OK
Abort any DTMF tone currently being generated and any DTMF
tonesequence
WriteCommand
AT+CLDTMF=<n>,<DTMF
string>[,<timeBase>][,<path>]
Response
1)
OK
+CLDTMF:0 //play end
2)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime
Reference
DefinedValues
<n> Anumeric parameter(1-100)whichindicates the durationof allDTMF
tones.
<DTMF string> Astringparameter (stringshould be includedinquotation marks)
whichhas amaxlength of20 charsofform <DTMF>,separatedby
NOTE


## Página 465

A76XX Series_AT Command Manual_V1.09
www.simcom.com 464 /652
commas.
<timeBase> timeBaseto generateDTMF sound.the DTMFontimeis
<n>*<timeBase> DTMFofftimeis timeBase,thedefaultvalue is
100ms.
<path> 0 local path
1 remote path(justsupportvoice call)
2 both remote and local(justsupportvoice call)
Examples
AT+CLDTMF=?
+CLDTMF:
(1-100),(0-9,A,B,C,D,*,#),(50-500),(0-2)
OK
AT+CLDTMF=1,”A,B,C,D,*,#”,100,0
OK
+CLDTMF:0
 JustsupportedonASR1603 standard branchnow.
 Thetotal durationof dtmfplayback duringacall cannotexceed 10s.
21.2.8 AT+SIMTONE GenerateSpecificallyTone
This commandis used toplay generate specifically tone.
AT+SIMTONEGenerateSpecificallyTone
TestCommand
AT+SIMTONE=?
Response
+SIMTONE:
(0-1),(20-4000),(50-25500),(0,40-25500),(50-500000)
OK
WriteCommand
AT+SIMTONE=<mode>[,<freque
ncy>][,<periodOn>][,<periodOff
>][,<duration>]
Response
1)
OK
+SIMTONE:0 //play end
NOTE


## Página 466

A76XX Series_AT Command Manual_V1.09
www.simcom.com 465 /652
2)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime
Reference
DefinedValues
<mode > 0 Stop playingtone.
1 start toplay tone.
<frequency> Thefrequency oftone tobe generated.
<periodOn> Theperiod ofgenerating tone,mustbe multipleof50.
<periodOff> Theperiod ofstoppingtone,mustbe multipleof 40.
<duration> Durationof tones inmilliseconds
Examples
AT+SIMTONE=?
+SIMTONE:
(0-1),(20-4000),(50-25500),(0,40-25500),(50-5000
00)
OK
AT+SIMTONE=1,1400,200,200,5000
OK
+SIMTONE:0
AT+SIMTONE=1,1400,200,200,5000
OK
AT+SIMTONE=0
OK
+SIMTONE:0 //stopsimtone with urc.
 Stopping the simtone halfwayalsotakes the URCwithit.
 JustsupportedonASR1603 standard branchnow.
NOTE


## Página 467

A76XX Series_AT Command Manual_V1.09
www.simcom.com 466 /652
21.2.9 AT+STTONE PlaySIMToolkitTone
This commandis used toplay SIM toolkittone.
AT+STTONE PlaySIMToolkitTone
TestCommand
AT+STTONE=?
Response
+STTONE:(0-1),(1-8,16-20),(50-15300000)
OK
WriteCommand
AT+STTONE=<mode>[,<tone>][,
<duration>]
Response
1)
OK
+STTONE:0 //play end
2)
+CMEERROR: <err>
ParameterSaving Mode NO_SAVE
Max ResponseTime
Reference
DefinedValues
<mode> 0 Stop playingtone.
1 start toplay tone.
<tone> Numberic type
1 Dialtone
2 Called SubscriberBusy
3 Congestion
4 Radio PathAcknowledge
5 Radio Path NotAvailable/CallDropped
6 Error/Specialinformation
7 CallWaitingTone
8 RingingTone
9 16 General Beep
17 PositiveAcknowledgementTone
18 NegativeAcknowledgementorErrorTone
19 Indian DialTone
20 American DialTone
<duration> Numeric type,in milliseconds.
Max requestedvalue =255*60*1000 =15300000ms
(supported range=50-15300000)


## Página 468

A76XX Series_AT Command Manual_V1.09
www.simcom.com 467 /652
Examples
AT+STTONE=?
+STTONE:(0-1),(1-8,16-20),(50-15300000)
OK
AT+STTONE=1,8,10000
OK
+STTONE:0 //play end
 JustsupportedonASR1603 standard branchnow.
NOTE


## Página 469

A76XX Series_AT Command Manual_V1.09
www.simcom.com 468 /652
ATCommandsforFOTA
22.1 OverviewofATCommandforFOTA
Command Description
AT+CFOTA StartFOTAService
AT+LFOTA StartLocal FOTAService
Currently,only CAT1modules supportatcommandsforFOTA.
22.2 DetailedDescriptionofATCommandforFOTA
22.2.1 AT+CFOTA StartFOTAservice
AT+CFOTA StartFOTAService
WriteCommand
AT+CFOTA=<channel>,<mo
de>,<destination_ip/url>,<us
ername>,<password>
Response
1)
<CR><LF>OK<CR><LF>
+CFOTA: <err>
2)
<CR><LF>ERROR:<err><CR><LF>
ParameterSaving Mode NO_SAVE
Max ResponseTime -
Reference -
NOTE


## Página 470

A76XX Series_AT Command Manual_V1.09
www.simcom.com 469 /652
DefinedValues
<channel> 0–5 means the channel number
<mode> 0 FTPway
1 HTTPway
<destination_ip:port/url> Theremote site server’sIPaddress orURLaddress.
IPaddress shouldbe inthe formatofthe dotted decimalnotation:
XXX.XXX.XXX.XXX.
URLaddress should beASCIIcharacters,the maxmumofthe
length is256 bytes.
NOTE:If<port>are omitted,the defaultFTPportis 21andthe default
HTTPport is80.
<username> Thelogin username,itshould beASCIIcharacters,and the maxmum
of thelength is 128bytes.
<password> Thelogin password,itshouldbeASCII characters,and the maxmum
of thelength is 128bytes.
Examples
AT+CFOTA=0,0,"183.230.174.137:6047/fbf_dfota.bin",simcom,simcom
+CFOTA: FOTA,START
+CFOTA: DOWNLOADING:17
+CFOTA: DOWNLOADING:50
+CFOTA: DOWNLOADING:83
+CFOTA: DOWNLOADING:99
+CFOTA: DOWNLOADING:100
AT+CFOTA=0,1,"183.230.174.137:6022/bin/fbf_dfota.bin",simcom,simcom
+CFOTA: FOTA,START
+CFOTA: DOWNLOADING:17
+CFOTA: DOWNLOADING:50
+CFOTA: DOWNLOADING:83
+CFOTA: DOWNLOADING:99


## Página 471

A76XX Series_AT Command Manual_V1.09
www.simcom.com 470 /652
+CFOTA: DOWNLOADING:100
22.2.2 AT+LFOTA StartLocalFotaService
AT+LFOTA StartLocalFotaService
TestCommand
AT+LFOTA=?
Response
+LFOTA:<0-1>,<FileSize>
OK
Read Command
AT+LFOTA?
Response
1)ifdata passcheck
+LFOTA: 1
OK
2)ifdata doesn’tpasscheck
+LFOTA: 0
OK
3)ifdata hasnottransferyet.Itwillbe initialvalue -1
+LFOTA: -1
OK
WriteCommand
AT+LFOTA=<ops>,<File
Size>
Response
1)Ifsuccessfully:
>
OK
2)Iffailed:
>
ERROR
3)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<ops> 0 initial parameters
1 start transfer
<FileSize> Thebytes ofthe filedata tosend.


## Página 472

A76XX Series_AT Command Manual_V1.09
www.simcom.com 471 /652
Examples
AT+LFOTA=0,5358979
OK
AT+LFOTA=1,5358979
>
OK
IfUARTis used forLFOTA,please makesurethatthe delaytimebetween each 256byte reachtoat
least50ms.
Ifsendingfilecrash,restartmoduleandincreasethedelaytimebetweeneach256bytereachto50ms,
andthen try tosend fileagain
22.3 UnsolicitedResultCodes
URC Description
+CFOTA: 100 FOTACOMPLETE,itwillrestartin 8s.
+CFOTA: 1001 FOTAURLis invalid,maybe PDPwas active.
+CFOTA: 1002 FOTAtimeout
+CFOTA: 1003 FOTAURLis unknown
+CFOTA: 1004 FOTAusernameorpasswordis error
+CFOTA: 1005 FOTAfileis not exist
+CFOTA: 1006 Thesize ofFOTAfile isinvalid
+CFOTA: 1007 Getfilefailed
+CFOTA: 1008 Checkfileerror
+CFOTA: 1009 FOTAinternalerror
+CFOTA: 1010 Fotafiletoo large
+CFOTA: 1011 Fota setflagerror
+CFOTA: 1012 Fotaparametersize error
NOTE


## Página 473

A76XX Series_AT Command Manual_V1.09
www.simcom.com 472 /652
ATCommandsforSCFOTA
23.1 OverviewofATCommandsforSCFOTA
Command Description
AT+CAPFOTA Start/CloseFOTAservice
AT+CSCFOTA Configure parameters anddownload upgradepackage
Currently,only CAT4modules supportatcommandsforSCFOTA
23.2 DetailedDescriptionofATCommandsforSCFOTA
23.2.1 AT+CAPFOTA Start/CloseFOTAservice
AT+CAPFOTA Start/CloseFOTAservice
TestCommand
AT+CAPFOTA=?
Response
+CAPFOTA:(0-1)
OK
Read Command
AT+CAPFOTA?
Response
1)
+CAPFOTA:0
OK
2)
+CAPFOTA:1
NOTE


## Página 474

A76XX Series_AT Command Manual_V1.09
www.simcom.com 473 /652
OK
WriteCommand
/*SettingFOTAservicestatus*/
AT+CAPFOTA=<on/off>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<on/off> Theservice status on/off,the defaultvalue is 0.
0 Close FOTAprogram
1 ActiveFOTAprogram
Thefunction willtake effectimmediately.
Examples
AT+CAPFOTA=?
+CAPFOTA: (0-1)
OK
AT+CAPFOTA?
+CAPFOTA: 0
OK
AT+CAPFOTA=1
OK
23.2.2 AT+CSCFOTA Configureparametersanddownloadupgradepackage
AT+CSCFOTA Configureparametersanddownloadupgradepackage
WriteCommand
AT+CSCFOTA=<OEM>,<models
>,<product ID>,<product
Secret>,<targetversion>
Response
1)Ifsuccessfully:
OK
Ifitcanbe downloaded:
+CSCFOTA:2
+CSCFOTA:3
Ifdownloadpartial isfinished:


## Página 475

A76XX Series_AT Command Manual_V1.09
www.simcom.com 474 /652
+CSCFOTA:0
Ifthereis nonewversiondetected:
+CSCFOTA:5
Ifdetectversionfailed:
+CSCFOTA: <err>
Ifitcannotbedownloaded:
+CSCFOTA: <err>
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<OEM> Thenameofprojectdesigncompany.Thisnamemustbethesameas
the OEM created on thecloud platform.Otherwise,itwillcause
upgrade failed.
<models> Thenameof the devicemodel.Thisnamemustbe the sameas the
device modelcreatedon the cloud platform.Otherwise, itwillcause
upgrade failed.
<productID> TheproductIDthatmust bethe sameas the productIDgenerated on
the cloud platform.
<productSecret> Theproductsecret is usedto confirmthe identity andusage rights of
the user.Itmustbe the sameas theproduct secretgenerated onthe
cloud platform.
<targetversion> Theversionthatneedstobeupgradedto.Thisversionispublishedby
the cloud platform.
Examples
AT+CSCFOTA="SIMCom","A7600C","1540907004","f9bbb0d76f894da090b6b69253616561","A760
0C_A39_190327_V1.00"
OK
+CSCFOTA: 2
+CSCFOTA: 3
+CSCFOTA: 0


## Página 476

A76XX Series_AT Command Manual_V1.09
www.simcom.com 475 /652
23.3 CommandResultCodes
23.3.1 CommandResultReportCodes
Resultcodes Description
2 Checkversionis finished
3 Downloadis finished
4 Downloadpartial finished
5 Nonew version
23.3.2 Descriptionof<err>
<err> Description
0 OK
1 unknown error(contactsupplier)
301 Noenough memory
302 Invalidparameter
303 Invalidoperation
304 IOfailed
305 IOtimeout
306 Downloadfileverification failed
307 gotcanceled
308 Interfacenesting error
401 Invaliddeviceinformation
402 Invalidplatforminformation
403 Missing deviceinformation
404 Version numberis notconfigured
405 Internalerror(contactsupplier)
501 InvalidURL
502 Unableto resolve domainname
503 cannotconnectto the server
504 Invalidrequest,server returnederror
505 Notinrange
506 HTTPPOST requesterror
507 Re-downloadstart error


## Página 477

A76XX Series_AT Command Manual_V1.09
www.simcom.com 476 /652
508 Operationis aborted
509 Operationnotcompleted
510 Toomanyretargeting times
511 Unableto getdata fromSOCKET
512 Errorsendingdata viaSOCKET
513 Errorreceivingdatavia SOCKET
514 InvalidSOCKETconnection


## Página 478

A76XX Series_AT Command Manual_V1.09
www.simcom.com 477 /652
ATCommandsforGNSS
24.1 OverviewofATCommandsforGNSS
Command Description
AT+CGNSSPWR GNSS powercontrolandAP-Flash control
AT+CGPSCOLD Cold startGPS
AT+CGPSWARM WarmstartGPS
AT+CGPSHOT Hot startGPS
AT+CGNSSIPR Configure the baud rate ofUART3andGPSmodule
AT+CGNSSMODE Configure GNSSsupport mode
AT+CGNSSNMEA Configure NMEAsentence type
AT+CGPSNMEARATE SetNMEAoutput rate
AT+CGPSFTM StartGPStest mode
AT+CGPSINFO GetGPS fixed position information
AT+CGNSSINFO GetGNSS fixedposition information
AT+CGNSSCMD Send command toGNSS
AT+CGNSSTST Send datareceivedfrom UART3toNMEAport
AT+CGNSSPORTSWITCH Selectthe outputportforNMEAsentence
AT+CAGPS GetAGPSdata fromtheAGNSS serverforassisted positioning
AT+CGNSSPROD Getthe production ofGNSS
24.2 DetailedDescriptionofATCommandsforGNSS
24.2.1 AT+CGNSSPWR GNSSpowercontrolandAP-Flashcontrol
InASR1603 andASR1803,this commandcancontrol theGNSSmodule by pullingup/down the power pin,
andit canalso control whetherthe GNSSmodule canquickly hotstarttheAP-Flash. Ifyou only wantto
enable/disable GNSScontrol,pleaseexecuteAT+CGNSSPWR=1orAT+CGNSSPWR=0.Ifyou wantto
enableGNSSand wanttouseAP_Flashfasthotstartmode, pleaseexecuteAT+CGNSSPWR=1,1 or


## Página 479

A76XX Series_AT Command Manual_V1.09
www.simcom.com 478 /652
AT+CGNSSPWR=0,1.
WhenusingAP_Flashfasthotstartmode, you need toexecuteAT+CGNSSPWR=0,1 tostore the
positioning datain the moduleafter theGNSSis settothe upperposition forthe firsttime.When
AT+CGNSSPWR=1,1 is executednexttime,the positioning data will beloadedintoGNSS again.
.IfyouwanttoenableGNSSandwanttouseGNSSdynamic_load,pleaseexecuteAT+CGNSSPWR=1,1,1
orAT+CGNSSPWR=1,0,1,Thethird parameterdefaults to1and itisoptional
InASR1601, thiscommand canonly controlthe GNSSmodule bypullingup/down the powerpin.
AT+CGNSSPWR GNSSpowercontrolandAP-Flashcontrol
TestCommand
AT+CGNSSPWR=?
Response
+CGNSSPWR:
<GNSS_Power_status>,<AP_Flash_status>,<GNSS
_dynamic_load>,<Loader_extend>
OK
Read Command
AT+CGNSSPWR?
Response
+CGNSSPWR:
<GNSS_Power_status>,<AP_Flash_status>,<GNSS
_dynamic_load>,<Loader_extend>
OK
WriteCommand
AT+CGNSSPWR=<GNSS_Powe
r_status>[,<AP_Flash_status>]
[,<GNSS_dynamic_load>]
[,<Loader_extend>]
Response
1)Ifsuccessfully:
OK
2)ifGNSScanwork properly:
+CGNSSPWR:READY!
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<GNSS_Power_status> 0 Close GNSS
1 ActiveGNSS
Thefunction willtake effectimmediately.
<AP_Flash_status> 0 Close GNSSAP_Flashfasthotstartmode
1 ActiveGNSSAP_Flashfasthot startmode
Thefunction willtake effectimmediately.
<GNSS _dynamic_load> 0 Close GNSS_dynamic_load
1 ActiveGNSS_dynamic_load
Thefunction willtake effectimmediately.
<Loader_extend> 0 Usedefaultloader


## Página 480

A76XX Series_AT Command Manual_V1.09
www.simcom.com 479 /652
1 Usecustomloader
Thefunction willtake effectimmediately.
1.InASR1601,only the GNSS_Power_status field issupported.
2.InASR1603,GNSSwilltake about9secondstoupdata theversion ofGNSS, pleasesee
"+CGNSSPWR:READY!"before controllingthe GNSS.
3.AP_Flash_status is supportedafter 27/9/21.
4."+CGNSSPWR:READY! "is supportedafter27/9/21.
5.A7670C-BASS_DTUuses1601 GPSchip,and the syntax rules are thesameas1601GNSS.
6.Loader_extend issupported after7/3/23.
Examples
AT+CGNSSPWR=?
+CGNSSPWR:(0,1),(0,1),(0,1),(0,1)
OK
AT+CGNSSPWR?
+CGNSSPWR:1,1,1,0
OK
AT+CGNSSPWR=1,1,1,0
OK
+CGNSSPWR:READY!
24.2.2 AT+CGNSSTST SenddatareceivedfromUART3toNMEAport
AT+CGNSSTSTisusedtoprint rawGPSdata tothe NMEAport.
AT+CGNSSTST SenddatareceivedfromUART3toNMEAport
TestCommand
AT+CGNSSTST=?
Response
+CGNSSTST:(0,1)
OK
Read Command
AT+CGNSSTST?
Response
+CGNSSTST:<on/off>
NOTE


## Página 481

A76XX Series_AT Command Manual_V1.09
www.simcom.com 480 /652
OK
WriteCommand
AT+CGNSSTST=<on/off>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<on/off> 0 Stop sending datareceivedfromUART3to NMEAport.
1 Startsending data received fromUART3toNMEAport.
Thefunction willtake effectimmediately.
Ifyou want togetNMEAdata by NMEAport,you should toexcute
AT+CGNSSTST=1first.
Examples
AT+CGNSSTST=?
+CGNSSTST:(0,1)
OK
AT+CGNSSTST?
+CGNSSTST:0
OK
AT+CGNSSTST=1
OK
24.2.3 AT+CGPSCOLD ColdstartGPS
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGPSCOLD ColdstartGPS
TestCommand
AT+CGPSCOLD=?
Response
OK
Execution Command
AT+CGPSCOLD
Response
OK


## Página 482

A76XX Series_AT Command Manual_V1.09
www.simcom.com 481 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
Examples
AT+CGPSCOLD=?
OK
AT+CGPSCOLD
OK
24.2.4 AT+CGPSWARM WarmstartGPS
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGPSCOLD WarmstartGPS
TestCommand
AT+CGPSWARM=?
Response
OK
Execution Command
AT+CGPSWARM
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
1.This commandis valid ontheASR1603projects andASR1803Sprojects.
2.A7670C-BASS_DTUuses1601 GPSchip,and the syntax rules are thesameas1601GNSS.
Examples
AT+CGPSWARM=?
OK
AT+CGPSWARM
OK
NOTE


## Página 483

A76XX Series_AT Command Manual_V1.09
www.simcom.com 482 /652
24.2.5 AT+CGPSHOT HotstartGPS
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGPSHOT HotstartGPS
TestCommand
AT+CGPSHOT=?
Response
OK
Execution Command
AT+CGPSHOT
Response
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
Examples
AT+CGPSHOT=?
OK
AT+CGPSHOT
OK
24.2.6 AT+CGNSSIPR ConfigurethebaudrateofUART3 andGPSmodule
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGNSSIPR ConfigurethebaudrateofUART3 andGPSmodule
TestCommand
AT+CGNSSIPR=?
Response
+CGNSSIPR:(listofsupported<baud-rate>s)
OK
Read Command
AT+CGNSSIPR?
Response
+CGNSSIPR:<baud-rate>
OK
WriteCommand
AT+CGNSSIPR=<baud-rate>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
Execution Command
AT+CGNSSIPR
Response
Setdefaultvalue


## Página 484

A76XX Series_AT Command Manual_V1.09
www.simcom.com 483 /652
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<baud-rate> 9600
115200
230400
Thefunction willtake effectimmediately.
1.Thebaud-rate issupported bytheASR1601projects:
4800,9600,19200,38400,57600,115200
2.IntheASR1603projectsandASR1803Sproject,thebaudratedefaultsto115200afterGPSdynamic
loading and upgradingthe firmware,and 9600 withoutGPSdynamicloading.
3.A7670C-BASS_DTUuses1601 GPSchip,and the syntax rules are thesameas1601GNSS.
Examples
AT+CGNSSIPR=?
+CGNSSIPR:(9600,115200,230400)
OK
AT+CGNSSIPR?
+CGNSSIPR:9600
OK
AT+CGNSSIPR=9600
OK
24.2.7 AT+CGNSSMODE ConfigureGNSSsupportmode
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGNSSMODE ConfigureGNSSsupportmode
TestCommand Response
NOTE


## Página 485

A76XX Series_AT Command Manual_V1.09
www.simcom.com 484 /652
AT+CGNSSMODE=? +CGNSSMODE:(1-7)
OK
Read Command
AT+CGNSSMODE?
Response
+CGNSSMODE:<mode>
OK
WriteCommand
AT+CGNSSMODE=<mode>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
Execution Command
AT+CGNSSMODE
Response
Setdefaultvalue 3
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<mode> 1GPSL1 +BDS B1+QZSS
2BDS B1
3GPSL1+QZSS
Themodeis supported inforeign module:
1GPSL1+SBAS+QZSS
2BDS B1
3GPS+GLONASS+GALILEO+SBAS+QZSS
4GPS_BDS_GALILEO
Thefunction willtake effectimmediately.
Themodeis supportedby theASR1603projects andASR1803Sprojects inthe domesticmodule:
1 GPS
2 BDS
3 GPS+BDS
4 GLONASS
5 GPS+GLONASS
6 BDS+GLONASS
7 GPS+BDS+GLONASS
A7670C-BASS_DTUuses 1601 GPSchip,andthe syntax rulesare the sameas 1601GNSS.
NOTE


## Página 486

A76XX Series_AT Command Manual_V1.09
www.simcom.com 485 /652
Examples
AT+CGNSSMODE=?
+CGNSSMODE:(1-3)
OK
AT+CGNSSMODE?
+CGNSSMODE:1
OK
AT+CGNSSMODE=1
OK
24.2.8 AT+CGNSSNMEA ConfigureNMEAsentencetype
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGNSSNMEA ConfigureNMEAsentencetype
TestCommand
AT+CGNSSNMEA=?
Response
+CGNSSNMEA:(0-1),(0-1),(0-1),(0-1),(0-1),(0-1),(0-1),(0-1)
OK
Read Command
AT+CGNSSNMEA?
Response
+CGNSSNMEA:1,1,1,1,1,1,0,0
OK
WriteCommand
AT+CGNSSNMEA=[nGGA,[nGL
L,[nGSA,[nGSV,[nRMC,[nVTG,[
nZDA,[nGST]]]]]]]]
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
[nGGA,[nGLL,[nGSA,[nGSV,
[nRMC,[nVTG,[nZDA,[nGST,
]]]]]]]]
Therangeof nis0-1.Itmeans thatthe sentenceisoutput every n
times,0means no output,null means tosavethe original
configuration.


## Página 487

A76XX Series_AT Command Manual_V1.09
www.simcom.com 486 /652
nGGA GGAoutput rate,defaultis1
nGLL GLLoutput rate,defaultis1
nGSA GSAoutput rate,defaultis 1
nGSV GSVoutput rate,defaultis 1
nRMC RMCoutput rate,defaultis1
nVTG VTGoutput rate,defaultis 1
nZDA ZDAoutput rate,defaultis0
nGST GSToutput rate,defaultis 0
Thefunction willtake effectimmediately.
TheNMEAfields are supportedby theASR1601projects:
nGGA GGAoutputrate,defaultis 1
nGLL GLLoutputrate,defaultis 1
nGSA GSAoutput rate,defaultis 1
nGSV GSVoutput rate,defaultis1
nRMC RMCoutput rate,defaultis1
nVTG VTGoutput rate,defaultis1
nZDA ZDAoutput rate,defaultis1
nANT ANToutput rate,defaultis 1
nDHV DHVoutput rate,defaultis0
nLPS LPSoutput rate,defaultis 0(nonsupport)
res1 reserved,defaultis 0
res2 reserved,defaultis 0
nUTC UTCoutput rate,defaultis0(nonsupport)
nGST GSToutput rate,defaultis0
A7670C-BASS_DTUuses 1601 GPSchip,andthe syntax rulesare the sameas 1601GNSS.
Examples
AT+CGNSSNMEA=?
+CGNSSNMEA:(0-1),(0-1),(0-1),(0-1),(0-1),(0-1),(0-1),(0-1)
OK
AT+CGNSSNMEA?
+CGNSSNMEA:1,1,1,1,1,1,0,0
OK
AT+CGNSSNMEA=1,0,0,0,0,0,0,0
OK
NOTE


## Página 488

A76XX Series_AT Command Manual_V1.09
www.simcom.com 487 /652
24.2.9 AT+CGPSNMEARATE SetNMEAoutputrate
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGPSNMEARATE SetNMEAoutputrate
TestCommand
AT+CGPSNMEARATE=?
Response
+CGPSNMEARATE:(1,2,4,5,10)
OK
Read Command
AT+CGPSNMEARATE?
Response
+CGPSNMEARATE:<rate>
OK
WriteCommand
AT+CGPSNMEARATE=<rate>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
Execution Command
AT+CGPSNMEARATE
Response
Setdefaultvalue 1
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<rate> 1 1Hz, one anchorpointisoutput persecond
2 2Hz, two anchorpoints isoutput persecond
5 5Hz, five anchorpoints isoutput persecond
10 10Hz, ten anchorpointsis output persecond
Thefunction willtake effectimmediately.
Therateis supportedby theASR1601 projects:
1 1Hz, one anchorpointisoutput persecond
2 2Hz, two anchorpoints isoutput persecond
4 4Hz, four anchorpoints is outputpersecond
NOTE


## Página 489

A76XX Series_AT Command Manual_V1.09
www.simcom.com 488 /652
5 5Hz, five anchorpoints isoutput persecond
10 10Hz, ten anchorpoints isoutput persecond
Examples
AT+CGPSNMEARATE=?
+CGPSNMEARATE:(1,2,5,10)
OK
AT+CGPSNMEARATE?
+CGPSNMEARATE:1
OK
AT+CGPSNMEARATE=2
OK
24.2.10AT+CGPSFTM StartGPStestmode
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGPSFTM StartGPStestmode
TestCommand
AT+CGPSFTM=?
Response
OK
Read Command
AT+CGPSFTM?
Response
+CGPSFTM:0/1
OK
WriteCommand
AT+CGPSFTM=<on/off>
Response
1)Ifsuccessfully:
OK
2)Iffailed:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<on/off> 0 Close testmode


## Página 490

A76XX Series_AT Command Manual_V1.09
www.simcom.com 489 /652
1 Starttest mode
Thefunction willtake effectimmediately.
1.GBGSV fieldwillreplace BDGSVfield afterdynamic loadingiscomplete inASR1603andASR1803
Seriesprojects.
2.GAGSVfield will supportedby theASR1603 andASR1803 Seriesprojects after dynamicloadingis
complete.
3.GLGSVand GAGSVfield willsupported by theASR1603 andASR1803 Series projectsinforeign
modules afterdynamicloading is complete.
Examples
AT+CGPSFTM?
+CGPSFTM:0
OK
AT+CGPSFTM=1
OK
+GLGSV,78,20.6,66,25.6,77,21.6,79,21.9,67,26.2,
68,23.6
+GPGSV,10,36.3,12,33.5,14,26.5,15,27.0,18,30.6
,20,29.4,21,14.9,24,32.8,25,30.6,31,29.1,32,27.0
+BDGSV,201,28.7,204,29.0,206,27.3,207,25.9,20
9,25.0,210,18.5
24.2.11AT+CGPSINFO GetGPSfixedpositioninformation
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGPSINFO GetGPSfixedpositioninformation
TestCommand
AT+CGPSINFO=?
Response
+CGPSINFO:(0-255)
OK
Read Command
AT+CGPSINFO?
Response
+CGPSINFO:<time>
NOTE


## Página 491

A76XX Series_AT Command Manual_V1.09
www.simcom.com 490 /652
OK
WriteCommand
AT+CGPSINFO=<time>
Response
1)Ifsuccessfully:
OK
+CGPSINFO:[<lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC
time>],[<alt>],[<speed>],[<course>]
2)If<time>=0:
OK
3)Iffailed:
ERROR
Execution Command
AT+CGPSINFO
Response
+CGPSINFO:[<lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC
time>],[<alt>],[<speed>],[<course>]
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<time> Therang is0-255, unitis second.after set<time>willreportthe GPS
information every the seconds.
Thefunction willtake effectimmediately.
<lat> Latitude ofcurrentposition.Outputformatis ddmm.mmmmmm.
<N/S> N/SIndicator,N=north orS=south.
<log> Longitude ofcurrentposition.Outputformat isdddmm.mmmmmm.
<E/W> E/W Indicator,E=eastorW=west.
<date> Date. Outputformatisddmmyy.
<UTCtime> UTCTime.Output formatishhmmss.ss.
<alt> MSLAltitude. Unitismeters.
<speed> Speed OverGround. Unitisknots.
<course> Course.Degrees.
Examples
AT+CGPSINFO=?
+CGPSINFO:(0-255)
OK
AT+CGPSINFO?
+CGPSINFO:0


## Página 492

A76XX Series_AT Command Manual_V1.09
www.simcom.com 491 /652
OK
AT+CGPSINFO
+CGPSINFO:3113.343286,N,12121.234064,E,250311,072809.33,44.1,0.0,0
OK
24.2.12AT+CGNSSINFO GetGNSSfixedpositioninformation
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGNSSINFO GetGNSSfixedpositioninformation
TestCommand
AT+CGNSSINFO=?
Response
+CGNSSINFO:(0-255)
OK
Read Command
AT+CGNSSINFO?
Response
+CGNSSINFO:<time>
OK
WriteCommand
AT+CGNSSINFO=<time>
Response
1)Ifsuccessfully:
OK
+CGNSSINFO:
[<mode>],[<GPS-SVs>],[<GLONASS-SVs>],[BEIDOU-SVs],[<l
at>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC-time>],[<alt>],[<s
peed>],[<course>],[<PDOP>],[HDOP],[VDOP]
2)If<time>=0:
OK
3)Iffailed:
ERROR
Execution Command
AT+CGNSSINFO
Response
+CGNSSINFO:
[<mode>],[<GPS-SVs>],[<GLONASS-SVs>],[BEIDOU-SVs],[<l
at>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC-time>],[<alt>],[<s
peed>],[<course>],[<PDOP>],[HDOP],[VDOP]
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -


## Página 493

A76XX Series_AT Command Manual_V1.09
www.simcom.com 492 /652
DefinedValues
<time> Therang is0-255, unitis second.after set<time>willreportthe
GNSSinformation every the seconds.
Thefunction willtake effectimmediately.
<mode> Fix mode 2=2Dfix 3=3D fix
<GPS-SVs> GPSsatellitevisible numbers
<GLONASS-SVs> GLONASS satellite visiblenumbers
<BEIDOU-SVs> BEIDOUsatellitevisiblenumbers
<lat> Latitude ofcurrentposition.Outputformatis dd.ddddd
<N/S> N/SIndicator,N=north orS=south.
<log> Longitude ofcurrentposition.Outputformat isddd.ddddd
<E/W> E/W Indicator,E=eastorW=west.
<date> Date. Outputformatisddmmyy.
<UTC-time> UTCTime.Output formatishhmmss.ss.
<alt> MSLAltitude. Unitismeters.
<speed> Speed OverGround. Unitisknots.
<course> Course.Degrees.
<PDOP> Position DilutionOfPrecision.
<HDOP> HorizontalDilution OfPrecision.
<VDOP> VerticalDilution OfPrecision.
Examples
AT+CGNSSINFO=?
+CGNSSINFO:(0-255)
OK
AT+CGNSSINFO?
+CGNSSINFO:0
OK
AT+CGNSSINFO
+CGNSSINFO:
2,09,05,00,3113.330650,N,12121.262554,E,131117,091918.00,32.9,0.0,255.0,1.1,0.8,0.7
OK
AT+CGNSSINFO (ifnotfix,willreportnull)
+CGNSSINFO:,,,,,,,,,,,,,,,
OK
NOTE


## Página 494

A76XX Series_AT Command Manual_V1.09
www.simcom.com 493 /652
IntheASR1603andASR1803Splatforms,thedomestic versionof+CGNSSINFOreportedinformation
as follows:+CGNSSINFO:
[<mode>],[<GPS-SVs>],[BEIDOU-SVs],[<lat>],[<N/S >],[<log>],[<E/W>],[<date>],[<UTC-time>],[<alt>],[
<speed>],[<course>],[ <PDOP>],[HDOP],[VDOP].[<valid-SVs>]
Theinformationreportedby the foreign version of+CGNSSINFOis as follows:+CGNSSINFO:
[<mode>],[<GPS-SVs>],[BEIDOU-SVs],[<GLONASS-SVs>],[<GALILEO-SVs>],[<
lat>],[<N/S>],[<log>],[<E/W>],[<date>],[<UTC-time>],[<alt>],[<speed> ],[<course>],[<PDOP>],[HDOP],[
VDOP].
24.2.13AT+CGNSSCMD SendcommandtoGNSS
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGNSSCMD SendcommandtoGNSS
TestCommand
AT+CGNSSCMD=?
Response
+CGNSSCMD:"CmdString"
OK
WriteCommand
AT+CGNSSCMD=<CmdString>
Response
1)IfsendOK:
OK
2)Ifsendfalse:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<CmdString> Command string, maxlength ofstring is510.
Forexample:ifyou wanttosend"$PCAS02,1000*2E<CR><LF>"
command toGNSS.
Youcan use:
AT+CGNSSCMD=0,"$PCAS02,1000*2E"
Or:
AT+CGNSSCMD=1,"245043415330322C313030302A3245"
Examples


## Página 495

A76XX Series_AT Command Manual_V1.09
www.simcom.com 494 /652
AT+CGNSSCMD=?
+CGNSSCMD:"CmdString"
OK
AT+CGNSSCMD="$PCAS02,1000*2E"
OK
24.2.14AT+CGNSSPORTSWITCH SelecttheoutputportforNMEAsentence
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.AT+CGNSSTST=1isusedto
output originalNMEAdatatoUSBNMEAportorUARTport.
AT+CGNSSPORTSWITCH SelecttheoutputportforNMEAsentence
TestCommand
AT+CGNSSPORTSWITCH=?
Response
+CGNSSPORTSWITCH:(0,1),(0,1)
OK
Read Command
AT+CGNSSPORTSWITCH?
Response
+CGNSSPORTSWITCH:
<parse_data_port>,<nmea_data_port>
OK
WriteCommand
AT+CGNSSPORTSWITCH=<par
se_data_port>[,<nmea_data_po
rt>]
Response
1)IfsendOK:
OK
2)Ifsendfalse:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<parse_data_port> 0 output the parsed dataof NMEAtoUSBATport.
1 output the parsed dataof NMEAtoUARTport.
<nmea_data_port> 0 output rawNMEAdatatoUSB NMEAport.
1 output rawNMEAdatatoUARTport.
Examples


## Página 496

A76XX Series_AT Command Manual_V1.09
www.simcom.com 495 /652
AT+CGNSSPORTSWITCH=?
+CGNSSPORTSWITCH:(0,1),(0,1)
OK
AT+CGNSSPORTSWITCH=0,1
OK
24.2.15AT+CAGPS GetAGPSdatafromtheAGNSSserverforassistedpositioning
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CAGPS GetAGPSdatafromtheAGNSSserverforassistedpositioning
Execution Command
AT+CAGPS
Response
1)Ifsuccessfully:
OK
+AGPS:success.
2)Iffailed:
ERROR
3)Iffailed:
OK
+AGPS:<errorcode>.
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<errorcode> 101 open socketunsuccessfully.
102 gettheAGNSSserver unsuccessfully.
103 connecttoAGNSS serverunsuccessfully.
104 write informationtosocket unsuccessfully.
105 readAGPS datafrom socketunsuccessfully.
Examples
AT+CAGPS
OK


## Página 497

A76XX Series_AT Command Manual_V1.09
www.simcom.com 496 /652
+AGPS:success.
24.2.16AT+CGNSSPRODGettheproductionofGNSS
This commandis validafterthe URCreports “+CGNSSPWR:READY!”.
AT+CGNSSPROD GettheproductionofGNSS
TestCommand
AT+CGNSSPROD=?
Response
OK
Execution Command
AT+CGNSSPROD
Response
1)Ifsuccessfully:
PRODUCT:<prodname>,<model>,<vers>
OK
2)Ifthe GNSSis poweroff:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<prodname> Theproduction ofGNSS,there are CASICandUNICORECOMM
currently.
<model> Theproductmodel ofGNSS.
<vers> Thefirewarm versionof GNSS.
Examples
AT+CGNSSPROD=?
OK
AT+CGNSSPROD
PRODUCT:CASIC
OK
AT+CGNSSPROD
PRODUCT:UNICORECOMM,UC6226NIS,R3.2.10.0Build8016
OK
AT+CGNSSPROD
ERROR


## Página 498

A76XX Series_AT Command Manual_V1.09
www.simcom.com 497 /652
ATCommandsforWIFI
25.1 OverviewofATCommandsforWIFI
Command Description
AT+CWSTASCAN Scan WIFInetwork
AT+CWSTASCANEX Scan WIFInetwork extension command
AT+CWSTASCANSYN Asynchronous controlcommand ofscanwifinetworK
AT+CWMAP Open/Close WIFI
AT+CWSSID SSIDsetting
AT+CWAUTH Authentication setting
AT+CWMOCH 80211mode and channelsetting
AT+CWISO Clientisolation setting
AT+CWMACADDR GetMACaddress
AT+CWNETCNCT Query the connectionto the network
25.2 DetailedDescriptionofATCommandsforWIFI
25.2.1 AT+CWSTASCAN ScanWIFInetwork
AT+CWSTASCAN ScanWIFI network
TestCommand
AT+CWSTASCAN=?
Response
+CWSTASCAN:(0-1)
OK
Read Command
AT+CWSTASCAN?
Response
+CWSTASCAN:<flag_show_signal>
OK
WriteCommand Response


## Página 499

A76XX Series_AT Command Manual_V1.09
www.simcom.com 498 /652
AT+CWSTASCAN=<flag_sh
ow_signal>
1)ifthe modeis 0or1:
OK
2)
ERROR
Execution Command
AT+CWSTASCAN
Response
[+CWSTASCAN:<bssid>,<channel_num>,[signal]
[……]]
OK
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<flag_show_signal> 0 Don’tshowthe signallevel.
1 Show thesignallevel. It’s thedefault value.
<bssid> TheMACaddress ofexternalwireless network.
<channel_num> Thechannelnumberof externalwirelessnetwork.
<signal> Thesignallevelof externalwireless network.
Examples
AT+CWSTASCAN=?
+CWSTASCAN:(0-1)
OK
AT+CWSTASCAN=1
OK
AT+CWSTASCAN?
+CWSTASCAN:1
OK
AT+CWSTASCAN
+CWSTASCAN:
50:FA:84:AF:C8:B9,11,-61
86:40:BB:00:2E:AD,11,-65
1C:15:1F:55:56:7A,1,-76
B0:D5:9D:AF:57:A1,6,-79


## Página 500

A76XX Series_AT Command Manual_V1.09
www.simcom.com 499 /652
30:7B:AC:6C:F9:B0,1,-81
OK
25.2.2 AT+CWSTASCANEX ScanWIFInetworkextensioncommand
AT+CWSTASCANEX ScanWIFInetworkextension command
TestCommand
AT+CWSTASCANEX=?
Response
+CWSTASCANEX:(0-1),(1-3),(4-10),(0-255),(0-1)
OK
Read Command
AT+CWSTASCANEX?
Response
+CWSTASCANEX:
<flag_show_signal>,<scan_round_num>,<scan_max_bssid_num
>,<scan_timeout>,<scan_priority>
OK
WriteCommand
AT+CWSTASCANEX=<flag_
show_signal>[,<scan_roun
d_num>[,<scan_max_bssid
_num>[,<scan_timeout>[,<s
can_priority>]]]]
Response
1)
OK
2)
ERROR
Execution Command
AT+CWSTASCANEX
Response
[+CWSTASCANEX:<bssid>,<channel_num>,[signal]
[……]]
OK
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<flag_show_signal> 0 Don’tshowthe signallevel.
1 Showthe signallevel. It’sthe defaultvalue.
<scan_round_num> Therangeis1-3,means thenumber ofrounds ofWIFIscan.
<scan_max_bssid_num> Therangeis4-10,maximum numberofbssid perWIFIscan.
<scan_timeout> Therangeis0-255, timeout.


## Página 501

A76XX Series_AT Command Manual_V1.09
www.simcom.com 500 /652
<scan_priority> Therangeis0-1,priority.
<bssid> TheMACaddressof externalwireless network.
<channel_num> Thechannelnumberofexternal wirelessnetwork.
<signal> Thesignallevelofexternal wirelessnetwork.
Examples
AT+CWSTASCANEX=?
+CWSTASCANEX:(0-1),(1-3),(4-10),(0-255),(0-1)
OK
AT+CWSTASCANEX=1,3,4,25,0
OK
AT+CWSTASCANEX?
+CWSTASCANEX:1,3,4,25,0
OK
AT+CWSTASCANEX
+CWSTASCANEX:
08:4F:0A:CA:45:80,6,-64
92:32:4B:9F:E2:EB,1,-66
08:4F:0A:CA:45:40,1,-79
1C:15:1F:FD:C7:6C,6,-83
OK
25.2.3 AT+CWSTASCANSYNAsynchronouscontrolcommandofscanwifinetwork
AT+CWSTASCANSYN Asynchronouscontrolcommand ofscan wifinetwork
TestCommand
AT+CWSTASCANSYN=?
Response
+CWSTASCANSYN:(0-1)
OK
WriteCommand
AT+CWSTASCANSYN=<op>
Response
1) If op==1 and parameter format is right, response 0 indicates the
endof the scanresponse


## Página 502

A76XX Series_AT Command Manual_V1.09
www.simcom.com 501 /652
OK
[+CWSTASCANSYN:<mac_addr>,<channel_number>,<rssi>
,[……]]
+CWSTASCANSYN:0
2) Ifop==0 andparameterformatis right,
OK
3)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<op> 0 Stopscan wifinetwork.
1 Startscanwifinetwork.
<mac_addr> TheMACaddressof externalwireless network.
<channel_number> Thechannelnumberofexternal wirelessnetwork.
<rssi> Thesignallevelofexternal wirelessnetwork.
Examples
AT+CWSTASCANSYN=?
+CWSTASCANSYN:(0-1)
OK
AT+CWSTASCANSYN=1
OK
+CWSTASCANSYN:"08:4F:0A:CA:45:80",6,-64
+CWSTASCANSYN:"92:32:4B:9F:E2:EB",1,-66
+CWSTASCANSYN:"1C:15:1F:FD:C7:6C",6,-83
+CWSTASCANSYN:0
AT+CWSTASCANSYN=0
OK


## Página 503

A76XX Series_AT Command Manual_V1.09
www.simcom.com 502 /652
25.2.4 AT+CWMAPOpen/CloseWIFI
AT+CWMAP Open/Close WIFI
TestCommand
AT+CWMAP=?
Response
+CWMAP:(0-1)
OK
Read Command
AT+CWMAP?
Response
+CWMAP:<flag>
OK
WriteCommand
AT+CWMAP=<flag>
Response
1)ifthe modeis 0or1:
OK
2)
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<flag> 0 Close WIFI
1 Open WIFI
Examples
AT+CWMAP?
+CWMAP:1
OK
AT+CWMAP=0
OK
25.2.5 AT+CWSSIDSSIDsetting
AT+CWSSIDSSIDsetting
Read Command
AT+CWSSID?
Response
+CWSSID:<ssid>


## Página 504

A76XX Series_AT Command Manual_V1.09
www.simcom.com 503 /652
OK
WriteCommand
AT+CWSSID=<ssid>
Response
OK
or
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<ssid> newssidstring
1.Themaxlength of<ssid> is128bytes whenthe <ssid>include
onlyASCIIcharacters.
Thedefaultvalue isMrvl-uAP-X-XXX.
Examples
AT+CWSSID?
+CWSSID:Mrvl-uAP-X-C7FC
OK
25.2.6 AT+CWAUTHAuthenticationsetting
AT+CWAUTHAuthenticationsetting
Read Command
AT+CWAUTH=?
Response
+CWAUTH:<auth>,<encrypt>[,<password>]
OK
WriteCommand
AT+CWAUTH=<auth>,<encr
ypt>[,<password>]
Response
OK
or
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference


## Página 505

A76XX Series_AT Command Manual_V1.09
www.simcom.com 504 /652
DefinedValues
<auth> 0–open/share
1–open
2–share
3–wpa
4–wpa2
5–wpa/wpa2
<encrypt> 0–null
1–WEP
2–TKIP
3–AES
<password> password string, thelength is 5orbetwwen 8to 64.Thecharinthe
password is onlyallowtheASCII’sdecimalcodebetwwen 32 to126.
Theparameterneed tomeetthe following conditions:
1.If(auth =0) then(encrypt =0)
1.If(auth =1) then(encrypt =1)
2.If(auth =2) then(encrypt =1)
3.If(auth >=3)then (encrypt >=2)
4.If(encrypt =0)then (password isnull)
5.If(encrypt =1)then
{
1)password can’tbe setnull
2)password format:(5ASCIIcharacter)or(10 hexadecimalnumber)or(13ASCIIcharacter)
or(26hexadecimalnumber)
}
6.if(encrypt >=2)then
{
1)password can’tbe setnull
2)password format:(8~63ASCIIcharacteror64 hexadecimalnumber)
}
Examples
AT+CWAUTH?
+CWAUTH:0,1,"11111"
OK
NOTE


## Página 506

A76XX Series_AT Command Manual_V1.09
www.simcom.com 505 /652
AT+CWAUTH?
+CWAUTH:5,4,"12345678"
OK
AT+CWAUTH=0,0
OK //Auth:open/shareencrypt:null
AT+CWAUTH=1,1,"11111"
OK //Auth:openencrypt:WEP
AT+CWAUTH=2,1,"12345"
OK //Auth:share encrypt:WEP
//(ASCII characterpassword:12345)
AT+CWAUTH=2,1,"3132333435"
OK //Auth:share encrypt:WEP
//(sixteen hexadecimalnumber: password12345)
AT+CWAUTH=5,4,"abcd1234"
OK //Auth:WPA/WPA2 encrypt:TIKP-AES
25.2.7 AT+CWMOCH80211modeandchannelsetting
AT+CWMAP mode andchannelsetting
Read Command
AT+CWMOCH?
Response
+CWMOCH:<mode>,<channel>
OK
WriteCommand
AT+CWMOCH=<mode>,<ch
annel>
Response
OK
or
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<mode> 2–b 2.4Gmode
3–b/g 2.4Gmode
4–b/g/n 2.4Gmode
<channel> 0–auto select
1~11–2.4Gmode channel number


## Página 507

A76XX Series_AT Command Manual_V1.09
www.simcom.com 506 /652
Examples
AT+CWMOCH?
+CWMOCH:4,0
OK
AT+CWMOCH=3,1
OK
25.2.8 AT+CWISOClientisolationsetting
AT+CWISOClientisolation setting
TestCommand
AT+CWISO=?
Response
+CWISO:(0-1)
OK
Read Command
AT+CWISO?
Response
+CWMOCH:<mode>,<channel>
OK
WriteCommand
AT+CWISO=<isolation>
Response
OK
or
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<isolation> 0 Close
1 Open
Examples
AT+CWISO?
+CWISO:1


## Página 508

A76XX Series_AT Command Manual_V1.09
www.simcom.com 507 /652
OK
AT+CWISO=0
OK
25.2.9 AT+CWMACADDRGetMACaddress
AT+CWISOClientisolation setting
TestCommand
AT+CWMACADDR=?
Response
[<number>,<mac_addr>
[……]]
OK
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<number> 0–hostmacaddr
1–client macaddr
… –clientmacaddr
<mac_addr> Devicemac address
Examples
AT+CWMACADDR?
0,00:0A:F5:88:88:8F
1,74:23:44:8f:64:fd
OK
25.2.10AT+CWCLICNTGetclientnumberconnectedtotheWIFI
AT+CWISOClientisolation setting


## Página 509

A76XX Series_AT Command Manual_V1.09
www.simcom.com 508 /652
Read Command
AT+CWCLICNT?
Response
+CWCLICNT:<cnt>
OK
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<cnt> the connectedclient count,range isfrom0to 32.
Examples
AT+CWCLICNT?
+CWCLICNT:1
OK


## Página 510

A76XX Series_AT Command Manual_V1.09
www.simcom.com 509 /652
ATCommandsforBluetooth
26.1 OverviewofATCommandsforBluetooth
Command Description
AT+BLEPOWER Power on/offBle Device
AT+BLESTATUS Inquiry currentBLEconnectstatus
AT+BLEHOST Getor Sethostname
AT+BLEADDR Getor Setble address
AT+BLESREG RegisterGATTServer
AT+BLESDREG DeregisterGATTServer
AT+BLESSAD Addaservice
AT+BLESSRM Removeaservice
AT+BLESSCAD Addacharacteristic toan existedservice
AT+BLESSCRM Remove acharacteristic
AT+BLESSDAD Addadescriptortoanexisted characteristic
AT+BLESSDRM Remove adescriptor
AT+BLESSSTART Startaserver
AT+BLESSSTOP Stop aserver
AT+BLESSETADVDATA Setadvertingpackage
AT+BLESCLRADVDATA Clear advertingpackage
AT+BLESSETADVPARAM Setadvertingparameters
AT+BLESLSTART Startadvertising
AT+BLESLSTOP Stop advertising
AT+BLEADV Setadvertingparameters
AT+BLEDISCONN Disconnect BLEconnection
AT+BLESIND Send anindication toaclient
AT+BLESNTY Send anotice toaclient
AT+BLESRSP Send aResponseto aClient’SRead orWrite Operation
+BLESRREQRead requestreceived fromremote device
+BLESWREQWriterequest received from remotedevice
+BLESCON NotifyWhenaConnection’sStatusChange
+BLEMTUExchange mturequestreceivedfrom remotedevice
AT+BLECREG RegisterGATTClient


## Página 511

A76XX Series_AT Command Manual_V1.09
www.simcom.com 510 /652
AT+BLECDREG DeregisterGATTClient
AT+BLESCAN Scan Surrounding BLEDevice
+BLESCANRSTNotifyWhenFind aBLEDevice
AT+BLECGDT GetDeviceType
AT+BLECCON ConnectGATTClienttoRemote LE/Dual-mode Device
AT+BLECDISC Disconnect GATTClienttoRemote LE/Dual-mode Device
AT+BLECSS Search Peer’sService
AT+BLECGC Search Peer’sCharacteristic
AT+BLECGD Search Peer’sCharacteristicDescriptor
AT+BLECRC Read Peer’sCharacteristic
AT+BLECWC WritePeer’sCharacteristic
AT+BLECRD Read Peer’sDescriptor
AT+BLECWD WritePeer’sDescriptor
+BLECNTYNotifyWhenGet aNoticationfrom Peer’sDevice
+BLECINDNotifyWhenGetaIndication fromPeer’sDevice
AT+BTPOWER Open/Close BTDevice
AT+BTHOST Get/SetBTDevice Name
AT+BTADDR Get/SetBTDeviceAddress
AT+BTSCAN ScanBTDevice
AT+BTIOCAP Get/SetBTDevice IOCapability
AT+BTPAIR PairWithOtherBTDevice
AT+BTUNPAIR UnpairWithThePaired BTDevice
AT+BTPAIRED GetPaired BTDevice
AT+BTSPPSRV Active/Deactive LocalSPPService
AT+BTSPPPROF GetRemote BTDevice SPPServiceStatus
AT+BTSPPCONN Establish/ReleaseSPPConnection
AT+BTSPPSEND SPPSend Data
+BTSPPRECVSPPReceive Data
Currently,onlyA7678Series supportATcommands forBLE.ASR1603_011_051 versionSDK support
ATcommand forBT.
26.2 DetailedDescriptionofATCommandsforBLE
Theclient and server canbe createdand used atthe sametime,butonly oneconnection is supported.
NOTE


## Página 512

A76XX Series_AT Command Manual_V1.09
www.simcom.com 511 /652
26.2.1 AT+BLEPOWER Poweron/offBleDevice
AT+BLEPOWER=1isusedtopower onble device.YoumustexecuteAT+BLEPOWER=1before any other
blerelated operations.
AT+BLEPOWER Poweron/offBleDevice
TestCommand
AT+BLEPOWER=?
Response
OK
Read Command
AT+BLEPOWER?
Response
+BLEPOWER:<status>
OK
Or
ERROR
WriteCommand
AT+BLEPOWER=<op>
Response
OK
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 12000ms
Reference -
DefinedValues
<status> This parameterhasthe following two values:
0 the currentdeviceis closed.
1 the currentdeviceis open.
<op> This parameterhasthe following two values:
0 power offthe bledevice
1 power on the ble device
Examples
AT+BLEPOWER=?
OK
AT+BLEPOWER=1
OK
Whenthe Bluetoothisturned off,the client information is cleared andthe connection statusof the
serveris reset.
NOTE


## Página 513

A76XX Series_AT Command Manual_V1.09
www.simcom.com 512 /652
26.2.2 AT+BLESTATUS InquiryCurrentBLEConnectStatus
AT+BLESTATUS InquiryCurrentBLEConnectStatus
TestCommand
AT+BLESTATUS=?
Response
OK
Read Command
AT+BLESTATUS?
Response
Ifble has no connection:
OK
else:
+BLESTATUS:<conn_id>,<gatts_type>,<user_id>,<addr>
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<conn_id> Theconnection idof currentconnection
<gatts_type> Thevalues areas follows:
0 None.
1 GattServer.
2 GattClient.
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset {‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
<addr> Addressof the peerdevice.
Examples
AT+BLESTATUS=?
OK
AT+BLESTATUS?
+BLESTATUS:0,1,ABCDEF00,c0:65:29:48:56:ef
OK


## Página 514

A76XX Series_AT Command Manual_V1.09
www.simcom.com 513 /652
26.2.3 AT+BLEHOST InquiryandSetHostDeviceName
AT+BLEHOST InquiryandSetHostDeviceName
TestCommand
AT+BLEHOST=?
Response
OK
Read Command
AT+BLEHOST?
Response
+BLEHOST:<name>,<address>
OK
Write Command
AT+BLEHOST=<name>
Response
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<name> Devicename,maxlength ofitis 18bytes
<address> Deviceaddress
Examples
AT+BLEHOST=?
OK
AT+BLEHOST?
+BLEHOST:SIMCOM BLE,"df:45:e6:29:65:c0"
OK
AT+BLEHOST="SIMCOMBLE"
OK
26.2.4 AT+BLEADDRInquiryandSetDeviceAddress
AT+BLEADDR InquiryandSetDeviceAddress
TestCommand
AT+BLEADDR=?
Response
OK
Read Command
AT+BLEADDR?
Response
+BLEADDR:<address>


## Página 515

A76XX Series_AT Command Manual_V1.09
www.simcom.com 514 /652
OK
WriteCommand
AT+BLEADDR=<address>
Response
+BLEADDR:<address>
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<address> Deviceaddress
Examples
AT+BLEADDR=?
OK
AT+BLEADDR?
+BLEADDR:"df:45:e6:29:65:c0"
OK
AT+BLEADDR="C0:00:00:00:00:01"
+BLEADDR:"c0:00:00:00:00:01"
OK
26.2.5 AT+BLESREG RegisterGATTServer
AT+BLESREG RegisterGATTServer
TestCommand
AT+BLESREG=?
Response
OK
Read Command
AT+BLESREG?
Response
Ifthe serverhasalready beenregistered,response
+BLESREG:<server_index>,<user_id>
…
+BLESREG:<server_index>,<user_id>
OK
elseonly response


## Página 516

A76XX Series_AT Command Manual_V1.09
www.simcom.com 515 /652
OK
Execution Command
AT+BLESREG
Response
+BLESREG:<server_index>,<user_id>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex,the numberofregistered servers islimitedto 64.
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.
AHex valuestring. Eachcharofitshould inset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is 8.
Examples
AT+BLESREG=?
OK
AT+BLESREG
+BLESREG:0,ABCDEF50
OK
AT+BLESREG?
+BLESREG:0,ABCDEF50
OK
26.2.6 AT+BLESDREG DeregisterGATTServer
AT+BLESDREG DeregisterGATTServer
TestCommand
AT+BLESDREG=?
Response
OK
Read Command
AT+BLESDREG?
Response
OK
WriteCommand
AT+BLESDREG=<server_index
Response
+BLESDREG:<server_index>,<user_id>


## Página 517

A76XX Series_AT Command Manual_V1.09
www.simcom.com 516 /652
>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.
AHex valuestring. Eachcharofitshould inset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is 8.
Examples
AT+BLESDREG=?
OK
AT+BLESDREG?
OK
AT+BLESDREG=0
+BLESDREG:0,ABCDEF00
OK
26.2.7 AT+BLESSAD AddaService
AT+BLESSAD AddaService
TestCommand
AT+BLESSAD=?
Response
OK
Read Command
AT+BLESSAD?
Response
Ifthe serverhasalready beenregistered,response
+BLESSAD:
<service_index>,<user_id>,<uuid>,<is_primary>,<inst>,<serv
ice_handle>
…
+BLESSAD:
<service_index>,<user_id>,<uuid>,<is_primary>,<inst>,<serv
ice_handle>


## Página 518

A76XX Series_AT Command Manual_V1.09
www.simcom.com 517 /652
OK
elseonly response
OK
WriteCommand
AT+BLESSAD=<server_index>,
<uuid>,<num_handles>,<is_pri
mary>,<inst>
Response
+BLESSAD:
<service_index>,<user_id>,<uuid>,<is_primary>,<inst>,<serv
ice_handle>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> ServerIndex.Generated whenthe server iscreated.
<service_index> ServiceIndex,the numberofregistered services islimitedto 64.
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.
AHex valuestring. Eachcharofitshould inset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is 8.
<uuid> Theuuid of theservice.Max lengthis 32.
<num_handles> Reserve.
<is_primary> Thevalues areas follows:
1 primary service.
<inst> Reserve.
<service_handle> Thehandle ofthisservice.Dec format.
Examples
AT+BLESSAD=?
OK
AT+BLESSAD=0,"1802",30,1,0
+BLESSAD:0,ABCDEF50,1802,1,0,0
OK
AT+BLESSAD?
+BLESSAD:0,ABCDEF50,1802,1,0,0
OK


## Página 519

A76XX Series_AT Command Manual_V1.09
www.simcom.com 518 /652
26.2.8 AT+BLESSRM RemoveaService
AT+BLESSRM RemoveaService
TestCommand
AT+BLESSRM=?
Response
OK
Read Command
AT+BLESSRM?
Response
OK
WriteCommand
AT+BLESSRM=<service_index>
Response
+BLESSRM:
<service_index>,<user_id>,<uuid>,<service_handle>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<service_index> ServiceIndex
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.
AHex valuestring. Eachcharofitshould inset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is 8.
<uuid> TheUUIDof the service, astring with hexvalue,length is4.
<service_handle> Thehandle ofthisservice.Dec format.
Examples
AT+BLESSRM=?
OK
AT+BLESSRM?
OK
AT+BLESSRM=0
+BLESSRM:0,ABCDEF50,1802,0
OK


## Página 520

A76XX Series_AT Command Manual_V1.09
www.simcom.com 519 /652
26.2.9 AT+BLESSCAD AddaCharacteristictoanExistedService
AT+BLESSCAD AddaCharacteristictoanExistedService
TestCommand
AT+BLESSCAD=?
Response
OK
Read Command
AT+BLESSCAD?
Response
Ifthe serverhasalready beenregistered,response
+BLESSCAD:
<char_index>,<user_id>,<char_uuid>,<inst>,<char_handle>
…
+BLESSCAD:
<char_index>,<user_id>,<char_uuid>,<inst>,<char_handle>
OK
elseonly response
OK
WriteCommand
AT+BLESSCAD=<service_inde
x>,<char_uuid>,<inst>,<pro
p>,<permission>
Response
+BLESSCAD:
<char_index>,<user_id>,<char_uuid>,<inst>,<char_handle>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<service_index> ServiceIndex
<char_index> Characteristicindex,thenumberofregisteredcharacteristicsislimited
to 64.
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string. Eachcharofitshould inset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
<char_uuid> TheUUIDof the characteristic.Maxlength is32.
<inst> Not use.
<prop> Thecharacteristic’sproperties.Itshould beacombinationof the
following values:
1 Broadcast
2 Read
4 Writewithout response
8 Write


## Página 521

A76XX Series_AT Command Manual_V1.09
www.simcom.com 520 /652
16 Notify
32 Indicate
64 Authenticated SignedWrites
128 Extended properties
<permission> Permissionof thischaracteristic.
Itshouldbe acombinationofthe following values:
Read 1
Write2
<char_handle> Thehandle ofthisCharacteristic.Decformat.
Examples
AT+BLESSCAD=?
OK
AT+BLESSCAD=0,"2A06",4,38,3
+BLESSCAD:0,ABCDEF50,2A06,4,0
OK
AT+BLESSCAD?
+BLESSCAD:0,ABCDEF50,2A06,4,38,3,0
OK
26.2.10AT+BLESSCRM RemoveaCharacteristic
AT+BLESSCRM RemoveaCharacteristic
TestCommand
AT+BLESSCRM=?
Response
OK
WriteCommand
AT+BLESSCRM=<char_index>
Response
+BLESSCRM:
<char_index>,<user_id>,<char_uuid>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues


## Página 522

A76XX Series_AT Command Manual_V1.09
www.simcom.com 521 /652
<char_index> Characteristic index
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string. Eachcharofitshould inset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
<char_uuid> TheUUIDof the characteristic,astringwith hex value,length is4.
Examples
AT+BLESSCRM=?
OK
AT+BLESSCRM=0
+BLESSCRM:0,ABCDEF50,2A06
OK
26.2.11AT+BLESSDAD AddaDescriptortoanExistedCharacteristic
AT+BLESSDAD AddaDescriptortoanExistedCharacteristic
TestCommand
AT+BLESSDAD=?
Response
OK
Read Command
AT+BLESSDAD?
Response
Ifthe serverhasalready beenregistered,response
+BLESSDAD:
<desc_index>,<char_uuid>,<desc_uuid>,<inst>,<desc_handl
e>
…
+BLESSDAD:
<desc_index>,<char_uuid>,<desc_uuid>,<inst>,<desc_handl
e>
OK
elseonly response
OK
WriteCommand
AT+BLESSDAD=<char_index
>,<desc_uuid>,<inst>,<pe
rmission>
Response
+BLESSDAD:
<desc_index>,<char_uuid>,<desc_uuid>,<inst>,<desc_handl
e>
OK
or
ERROR


## Página 523

A76XX Series_AT Command Manual_V1.09
www.simcom.com 522 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<char_index> Characteristic index
<desc_index> DescriptorIndex
<char_uuid> TheUUIDof the characteristic,maxlength is32.
<desc_uuid> TheUUIDof the descriptor,the rules asfollows:
1.can’tadd multiplethe samedescriptortoacharacteristic.
2.can’tadd descriptortoacharacteristic ifserver isalreadyactive.
3.max lengthis 32.
4.available valueis 2900,2901,2902,2903,2904,2905.
<inst> Not use.
<permission> Permissionof thischaracteristic.Dec format.
Itshouldbe acombinationofthe following values:
Read 1
Write2
<desc_handle> Handle ofthis descriptor.Dec format.
Examples
AT+BLEPOWER=1
OK
AT+BLESREG?
OK
AT+BLESREG
+BLESREG:0,ABCDEF00
OK
AT+BLESSAD?
OK
AT+BLESSAD=0,"1603",30,1,4
+BLESSAD:0,ABCDEF00,1603,1,4,0
OK
AT+BLESSCAD=0,"8901",4,54,3


## Página 524

A76XX Series_AT Command Manual_V1.09
www.simcom.com 523 /652
+BLESSCAD:0,ABCDEF00,8901,0,0
OK
AT+BLESSDAD=0,"2901",4,0
+BLESSDAD:0,8901,2901,0,0
OK
AT+BLESSDAD=0,"2902",4,0
+BLESSDAD:1,8901,2902,0,0
OK
AT+BLESSDAD=0,"2901",4,0
ERROR
AT+BLEADV=0,1,1,1,0
+BLEADV:ABCDEF00
OK
AT+BLESSSTART=0,0
+BLESSSTART:0,ABCDEF00,0
OK
AT+BLESLSTART=0
+BLESLSTART:0,ABCDEF00
OK
AT+BLESSDAD=0,"2903",4,0
ERROR
26.2.12AT+BLESSDRM RemoveaDescriptor
AT+BLESSDRM AddaDescriptortoanExistedService
TestCommand
AT+BLESSDRM=?
Response
OK
WriteCommand
AT+BLESSDRM=<desc_index>
Response
+BLESSDRM:<desc_index>,<desc_uuid>


## Página 525

A76XX Series_AT Command Manual_V1.09
www.simcom.com 524 /652
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<desc_index> DescriptorIndex
<desc_uuid> TheUUIDof the descriptor,astring with hexvalue,length is4.
Examples
AT+BLESSDRM=?
OK
AT+BLESSDRM=0
+BLESSDRM:0,0210
OK
26.2.13AT+BLESSSTART StartaServer
AT+BLESSSTART StartaServer
TestCommand
AT+BLESSSTART=?
Response
OK
Read Command
AT+BLESSSTART?
Response
Ifnostarted device:
OK
Else
+BLESSSTART:
<server_index>,<user_id>,<server_ble_link_handle>
OK
WriteCommand
AT+BLESSSTART=<server_ind
ex>,<transport>
Response
+BLESSSTART:
<server_index>,<user_id>,<server_ble_link_handle>
OK


## Página 526

A76XX Series_AT Command Manual_V1.09
www.simcom.com 525 /652
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex.Generated whenthe serveriscreated.
<transport> Not use.
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string. Eachcharofitshould inset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
<server_ble_link_handle> Theblelink handleofthis server.Dec format.
Examples
AT+BLESSSTART=?
OK
AT+BLESSSTART=0,0
+BLESSSTART:0,ABCDEF50,0
OK
26.2.14AT+BLESSSTOP StopaServer
AT+BLESSSTOP StopaServer
TestCommand
AT+BLESSSTOP=?
Response
OK
Read Command
AT+BLESSSTOP?
Response
OK
WriteCommand
AT+BLESSSTOP=<server_inde
x>
Response
+BLESSSTOP:
<server_index>,<user_id>,<server_ble_link_handle>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms


## Página 527

A76XX Series_AT Command Manual_V1.09
www.simcom.com 526 /652
Reference -
DefinedValues
<server_index> Serverindex.Generated whenthe serveriscreated.(servershould
stop afterbleadvertising stop)
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string. Eachcharofitshould inset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
<server_ble_link_handle> Theblelink handleofthis server.Dec format.
Examples
AT+BLEPOWER=1
OK
AT+BLESREG?
OK
AT+BLESREG
+BLESREG:0,ABCDEF00
OK
AT+BLESSAD?
OK
AT+BLESSAD=0,"1603",30,1,4
+BLESSAD:0,ABCDEF00,1603,1,4,0
OK
AT+BLESSCAD=0,"8901",4,54,3
+BLESSCAD:0,ABCDEF00,8901,0,0
OK
AT+BLESSDAD=0,"2901",4,0
+BLESSDAD:0,8901,2901,0,0
OK
AT+BLESSDAD=0,"2902",4,0
+BLESSDAD:1,8901,2902,0,0


## Página 528

A76XX Series_AT Command Manual_V1.09
www.simcom.com 527 /652
OK
AT+BLEADV=0,1,1,1,0
+BLEADV:ABCDEF00
OK
AT+BLESSSTART=0,0
+BLESSSTART:0,ABCDEF00,0
OK
AT+BLESLSTART=0
+BLESLSTART:0,ABCDEF00
OK
AT+BLESSSTOP=0
ERROR
AT+BLESLSTOP=0
+BLESLSTOP:0,ABCDEF00
OK
AT+BLESSSTOP=0
+BLESSSTOP:0,ABCDEF00,0
OK
26.2.15AT+BLESSETADVDATA SetAdvertingPackage
This commandis incompatiblewith the +BLEHOSTand +BLEADVcommand.Ifthe above commandsare
usedat sametime,only thiscommandwilltake effect.
AT+BLESSETADVDATA SetAdvertingPackage
TestCommand
AT+BLESSETADVDATA=?
Response
+BLESSETADVDATA: <server_index>,<type>,<value>
OK
WriteCommand
AT+BLESSETADVDATA=<serve
Response
1)if<type>and <value>is ignored:


## Página 529

A76XX Series_AT Command Manual_V1.09
www.simcom.com 528 /652
r_index>[,<type>,<value>] +BLESSETDAVDATA: [adv_data]
OK
2)else:
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex,generatedwhen the serveris register.
<type> LE advertingpackagetype, consistentwith Bluetoothstandard
documents.Commonvaluesare as follows:
0x01: Flags
0x09: Complete LocalName
0xFF:ManufacturerSpecific Data
Forothertypes ofvalues,pleaserefertothe description ofgeneric
access profile inthe standard document
<value> LEadvertingpackagedata.StringType,maxlengthis29.Iftheformat
is the sameas HEX{DATA},andDATAis ahexadecimalstring, DATA
willbe transcoded andset.
<adv_data> Adverting packagecurrently set, isahexadecimalstring.
Examples
AT+BLESSETADVDATA=?
+BLESSETADVDATA:
<server_index>,<type>,<value>
OK
AT+BLESSETADVDATA=0
+BLESSETADVDATA:
OK
AT+BLESSETADVDATA=0,1,"HEX{01}"
OK


## Página 530

A76XX Series_AT Command Manual_V1.09
www.simcom.com 529 /652
26.2.16AT+BLESCLRADVDATA ClearAdvertingpackage
Thiscommandis only usedto clearthe advertingpackagesetby +BLESSETADVDATAcommand.
AT+BLESCLRADVDATAClearAdvertingpackage
TestCommand
AT+BLESCLRADVDATA=?
Response
OK
WriteCommand
AT+BLESCLRADVDATA=<serve
r_index>
Response
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serviceindex,generated whenthe serveris register.
Examples
AT+BLESCLRADVDATA=?
OK
AT+BLESCLRADVDATA=0
OK
26.2.17AT+BLESSETADVPARAM SetAdvertingParamters
AT+BLESSETADVPARAM SetAdvertingParameters
TestCommand
AT+BLESSETADVPARAM=?
Response
+BLESSETADVPARAM:
<server_index>,<adv_interval_min>,<adv_interval_max>,<ad
v_type>,<own_address_type>,<peer_address_type>,<peer_a
ddress>,<adv_channel_map>,<adv_filter_policy>,<adv_tx_po
wer>
OK
WriteCommand
AT+BLESSETADVPARAM=<ser
ver_index>[,<adv_interval_min
Response
+BLESSETDAVDPARAM:
<adv_interval_min>,<adv_interval_max>,<adv_type>,<own_a


## Página 531

A76XX Series_AT Command Manual_V1.09
www.simcom.com 530 /652
>,<adv_interval_max>[,<adv_ty
pe>[,<own_address_type>[,<pe
er_address_type>,<peer_addre
ss>[,<adv_channel_map>[,<adv
_filter_policy>[,<adv_tx_power
>]]]]]]]
ddress_type>,<peer_address_type>,<peer_address>,<adv_c
hannel_map>,<adv_filter_policy>,<adv_tx_power_dBm>
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex,generated whenthe server isregister.
<adv_interval_min>
Hexademicalnumbertype.Minimumadvertisingintervalforundirectedand
lowduty cycledirected advertising. Range:0x0020 to0x4000.Default:
0x0800(1.28 s).Time=N*0.625 ms.Forexample:f0.
<adv_interval_max>
Hexademicalnumbertype.Minimumadvertisingintervalforundirectedand
lowduty cycledirected advertising. Range:0x0020 to0x4000.Default:
0x0800(1.28 s).Time=N*0.625 ms.Forexample: f0.
<adv_type>
Itsvalue isthe oneoffollowing:
0x00:Connectableand scannableundirected advertising (ADV_IND)
(default).
0x01:Connectablehigh duty cycle directedadvertising
(ADV_DIRECT_IND,highduty cycle).
0x02:Scannableundirected advertising (ADV_SCAN_IND).
0x03:Non connectable undirected advertising(ADV_NONCONN_IND).
0x04:Connectablelowduty cycle directedadvertising
(ADV_DIRECT_IND,lowduty cycle).
Othervalues:Reserved forfuture use.
<own_address_type >
Itsvalue isthe oneoffollowing:
0x00:Publicdevice address(default).
0x01:Random deviceaddress.
Othervalues:Reserved forfuture use.
<peer_address_type>
Itsvalue isthe oneoffollowing:
0x00:PublicDeviceAddress(default).
0x01:Random DeviceAddress.
Othervalues:Reserved forfuture use.
<peer_address> Stringtype.Addressof thedevice tobe connected.Forexample:
“02:12:65:ef:d5:f0”.
<adv_channel_map> Reservedforfuture use.
<adv_filter_policy>
Itsvalue isthe oneoffollowing:
0x00:Processscanand connectionrequests fromalldevices (i.e., the
White Listisnotin use)(default).
0x01:Processconnection requestsfrom alldevices andscanrequests


## Página 532

A76XX Series_AT Command Manual_V1.09
www.simcom.com 531 /652
onlyfromdevices thatarein the WhiteList.
0x02:Processscanrequests from alldevices and connectionrequests
onlyfromdevices thatarein the WhiteList.
0x03:Processscanand connectionrequests only fromdevices in the
White List.
Othervalues:Reverved forfuture use.
<adv_tx_power>
Itsvalue isthe oneoffollowing:
0:setadvertisingtx powerto -12dBm
1:setadvertisingtx powerto -8dBm
2:setadvertisingtx powerto -2dBm
3:setadvertisingtx powerto 0dBm
4:setadvertisingtx powerto +6dBm
<adv_tx_power_dBm>
Itsvalue isthe oneoffollowing:
244:indicate thatcurrentadvertisingtx poweris -12dBm
248:indicate thatcurrentadvertisingtx poweris -8dBm
254:indicate thatcurrentadvertisingtx poweris -2dBm
0:indicate thatcurrentadvertisingtxpower is 0dBm
6:indicate thatcurrentadvertisingtxpower is +6dBm
Examples
AT+BLESSETADVPARAM=?
+BLESSETADVPARAM:
<server_index>,<adv_interval_min>,<adv_interval_max>,<adv_type>,<own_address_type>,<peer_
address_type>,<peer_address>,<adv_channel_map>,<adv_filter_policy>,<adv_tx_power>
OK
AT+BLESSETADVPARAM=0
+BLESSETADVPARAM:0x0800,0x0800,0,0,0,"00:00:00:00:00:00",7,0,0
OK
AT+BLESSETADVPARAM=0,80,f0
+BLESSETADVPARAM:0x0080,0x00f0,0,0,0,"00:00:00:00:00:00",7,0,0
OK
AT+BLESSETADVPARAM=0,320,320,0,0,0,"00:00:00:00:00:00",7,0,2
+BLESSETADVPARAM:0x0320,0x0320,0,0,0,"00:00:00:00:00:00",7,0,254
OK
AT+BLESSETADVPARAM=0,320,320,0,0,0,"00:00:00:00:00:00",7,0,4
+BLESSETADVPARAM:0x0320,0x0320,0,0,0,"00:00:00:00:00:00",7,0,6
OK


## Página 533

A76XX Series_AT Command Manual_V1.09
www.simcom.com 532 /652
26.2.18AT+BLESLSTART StartAdvertising
AT+BLESLSTART StartAdvertising
TestCommand
AT+BLESLSTART=?
Response
OK
Read Command
AT+BLESLSTART?
Response
Ifnodevicein adversing.
OK
Else
+BLESLSTART:<server_index>,<user_id>
OK
WriteCommand
AT+BLESLSTART=<server_inde
x>
Response
+BLESLSTART:<server_index>,<user_id>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex.Generated whenthe serveriscreated.(ble advertising
should startafterserver start)
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset {‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
Examples
AT+BLEPOWER=1
OK
AT+BLESREG?
OK
AT+BLESREG
+BLESREG:0,ABCDEF00


## Página 534

A76XX Series_AT Command Manual_V1.09
www.simcom.com 533 /652
OK
AT+BLESSAD?
OK
AT+BLESSAD=0,"1603",30,1,4
+BLESSAD:0,ABCDEF00,1603,1,4,0
OK
AT+BLESSCAD=0,"8901",4,54,3
+BLESSCAD:0,ABCDEF00,8901,0,0
OK
AT+BLESSDAD=0,"2901",4,0
+BLESSDAD:0,8901,2901,0,0
OK
AT+BLESSDAD=0,"2902",4,0
+BLESSDAD:1,8901,2902,0,0
OK
AT+BLEADV=0,1,1,1,0
+BLEADV:ABCDEF00
OK
AT+BLESLSTART=0
ERROR
AT+BLESSSTART=0,0
+BLESSSTART:0,ABCDEF00,0
OK
AT+BLESLSTART=0
+BLESLSTART:0,ABCDEF00
OK


## Página 535

A76XX Series_AT Command Manual_V1.09
www.simcom.com 534 /652
26.2.19AT+BLESLSTOP StopAdvertising
AT+BLESLSTOP StopAdvertising
TestCommand
AT+BLESLSTOP=?
Response
OK
Read Command
AT+BLESLSTOP?
Response
OK
WriteCommand
AT+BLESLSTOP=<server_index
>
Response
+BLESLSTOP:<server_index>,<user_id>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex.Generated whenthe serveriscreated.(disallowstop
advertising ifblelink exist)
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset {‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
Examples
AT+BLEPOWER=1
OK
AT+BLESREG?
OK
AT+BLESREG
+BLESREG:0,ABCDEF00
OK
AT+BLESSAD?
OK
AT+BLESSAD=0,"1603",30,1,4
+BLESSAD:0,ABCDEF00,1603,1,4,0


## Página 536

A76XX Series_AT Command Manual_V1.09
www.simcom.com 535 /652
OK
AT+BLESSCAD=0,"8901",4,54,3
+BLESSCAD:0,ABCDEF00,8901,0,0
OK
AT+BLESSDAD=0,"2901",4,0
+BLESSDAD:0,8901,2901,0,0
OK
AT+BLESSDAD=0,"2902",4,0
+BLESSDAD:1,8901,2902,0,0
OK
AT+BLEADV=0,1,1,1,0
+BLEADV:ABCDEF00
OK
AT+BLESSSTART=0,0
+BLESSSTART:0,ABCDEF00,0
OK
AT+BLESLSTART=0
+BLESLSTART:0,ABCDEF00
OK
+BLESCON:1,ABCDEF00,7e:c3:ed:71:e5:55,1
AT+BLESLSTOP=0
ERROR
+BLESCON:0,ABCDEF00,7e:c3:ed:71:e5:55,1
AT+BLESLSTOP=0
+BLESLSTOP:0,ABCDEF00
OK


## Página 537

A76XX Series_AT Command Manual_V1.09
www.simcom.com 536 /652
26.2.20AT+BLEADV SetAdvertingParameters
AT+BLEADV SetAdvertingParameters
TestCommand
AT+BLEADV=?
Response
OK
WriteCommand
AT+BLEADV=<server_index>,<i
nclude_flag>,<include_name>,<
include_txpower>,<appearance
>[,<manufacturer_data>,<servic
e_data>,<service_uuid>]
Response
+BLEADV:<user_id>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Serverindex
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset {‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
<include_flag> Include flagparameterornot
0 Not include
1 Include
<include_name> Include BTname
0 Not include
1 Include
<include_txpower> IncludeTxpowerLevel
0 Not include
1 Include
<appearance> Setappearance,0~16384
<manufacturer_data> Setmanufacturer,AHex value string(“HEX{data}”)oradecimal value
string.Maxlength ofitis 56.
<service_data> Setservice_datauuid,AHex value string,eachchar
of itshould inset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.The lengthof itshouldbe 0or
4~32.
<service_uuid> Setcompleteservicesuuid,AHex valuestring,each
charofitshouldin set{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Thelength ofitshouldbe
0or4~32.
Examples


## Página 538

A76XX Series_AT Command Manual_V1.09
www.simcom.com 537 /652
AT+BLEADV=?
OK
AT+BLEADV=0,1,1,1,0
+BLEADV:ABCDEF50
OK
26.2.21AT+BLEDISCONN DisconnectBLEConnection
AT+BLEDISCONN DisconnectBLEConnection
TestCommand
AT+BLEDISCONN=?
Response
OK
Read Command
AT+BLEDISCONN?
Response
OK
Write Command
AT+BLEDISCONN=<conn_id>
Response
OK
+BLESCON:<op>,<user_id>,<addr>,<conn_id>
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<op> 0 Disconnect
1 Connect
<conn_id> Theconnection idof currentconnection
<addr> Addressof the peerdevice.
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset {‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is8.
Examples
AT+BLEDISCONN=?
OK
AT+BLEDISCONN=0
OK


## Página 539

A76XX Series_AT Command Manual_V1.09
www.simcom.com 538 /652
+BLESCON:0,ABCDEF50,df:45:e6:29:65:c1,0
26.2.22AT+BLESIND SendanIndicationtoaClient
AT+BLESIND SendanIndicationtoaClient
TestCommand
AT+BLESIND=?
Response
OK
Write Command
AT+BLESIND=<char_index>,<v
alue>
Response
OK
+BLESIND:<result>,<user_id>,<conn_id>,<attr_handle>
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<char_index> Characteristic index
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string,eachcharofitshouldinset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Max lengthof
itis 8.
<conn_id> Theconnection idof currentconnection.
<attr_handle> Thehandle ofthecharacteristic value.Dec format.
<value> Thevalue need tobe notified. StringType,max length is(MTU–3).If
the formatis the sameas HEX{DATA},and DATAisahexadecimal
string, DATAwillbe transcoded and sent.Pleaserefer tochapter
26.2.27about MTU.
<result> 0 Success
1 Fail
Examples
AT+BLESIND=?
OK
AT+BLESIND=0,"HEX{123456}"
+BLESIND:0,ABCDEF00,1,17
OK


## Página 540

A76XX Series_AT Command Manual_V1.09
www.simcom.com 539 /652
26.2.23AT+BLESNTY SendannoticetoaClient
AT+BLESNTY SendanNoticetoaClient
TestCommand
AT+BLESNTY=?
Response
OK
Write Command
AT+BLESNTY=<char_index>,<v
alue>
Response
+BLESNTY:<result>,<user_id>,<conn_id>,<attr_handle>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<char_index> Characteristic index
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string,eachcharofitshouldinset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Max lengthof
itis 8.
<conn_id> Theconnection idof currentconnection.
<attr_handle> Thehandle ofthecharacteristic value.Dec format.
<value> Thevalue need tobe notified. StringType,max length is(MTU–3).If
the formatis the sameas HEX{DATA},and DATAisahexadecimal
string, DATAwillbe transcoded and sent.Pleaserefer tochapter
26.2.27about MTU.
<result> 0 Success
1 Fail.
Examples
AT+BLESNTY=?
OK
AT+BLESNTY=0,"HEX{123456}"
+BLESNTY:0,ABCDEF00,1,17
OK


## Página 541

A76XX Series_AT Command Manual_V1.09
www.simcom.com 540 /652
26.2.24AT+BLESRSP SendaResponsetoaClient’SReadorWriteOperation
AT+BLESRSP SendaResponsetoaClient’SReadorWriteOperation
Write Command
AT+BLESRSP=<switch>,<value
>
Response
+BLESRSP:<result>,<user_id>,<conn_id>,<attr_handle>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<switch> 0 Read
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string,eachcharofitshouldinset{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Max lengthof
itis 8.
<conn_id> Theconnection idof currentconnection.
<attr_handle> Thehandle ofthecharacteristic value.Dec format.
<value> Thevalue need toresponseread request.StringType, maxlength is
(MTU–3).Ifthe format isthe sameas HEX{DATA},and DATAisa
hexadecimalstring,DATAwillbe transcodedandsent.Pleasereferto
chapter26.2.27 aboutMTU.
<result> 0 Success
1 Fail
Examples
AT+BLESRSP=0,"HEX{123456}"
+BLESRSP:0,ABCDEF50,1,17
OK
26.2.25+BLESRREQ Readrequestreceivedfromremotedevice
AT+BLESRREQ Readrequestreceivedfromremotedevice
URC
ifthereis incoming aread request:
+BLESRREQ:
<user_id>,<conn_id>,<trans_id>,<addr>,<attr_handle>,<is_lo


## Página 542

A76XX Series_AT Command Manual_V1.09
www.simcom.com 541 /652
DefinedValues
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Max length ofitis 8.
<conn_id> Theconnection idof currentconnection.
<trans_id> Theid ofcurrenttransaction.0~65535
<addr> Addressof the peerdevice.
<attr_handle> Handle ofattribute.
<is_long> Tellserverthatthe requestis oneorseveral requests.
<offset> Offsetof the request.0~65535
Examples
+BLESRREQ:
ABCDEF50,1,0,"90:f0:6a:3a:4f:41",17,19,0
26.2.26+BLESWREQ Writerequestreceivedfromremotedevice
DefinedValues
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Max length ofitis 8.
<conn_id> Theconnection idof currentconnection.
<trans_id> Theid ofcurrenttransaction.0~65535
<addr> Addressof the peerdevice.
ng>,<offset>
Reference -
AT+BLESWREQ Writerequestreceivedfromremotedevice
URC
ifthereis incoming awriterequest:
+BLESWREQ:
<user_id>,<conn_id>,<trans_id>,<addr>,<attr_handle>,<value
>,<need_rsp>,<is_prep>,<offset>
Reference -


## Página 543

A76XX Series_AT Command Manual_V1.09
www.simcom.com 542 /652
<attr_handle> Handle ofattribute.
<value> Thevalue need tobe write,Hex format
<need_rsp> Whetherclient need server'sresponse
1 Yes
0 No
<is_prep> Whetheror notserver executerequest immediately
0 No
1 Yse
<offset> Offsetof the request.0~65535
Examples
+BLESWREQ:ABCDEF50,1,0,"21:e8:5a:c2:8d:47",17,1234,0,0,0
26.2.27+BLESCON Notify WhenaConnection’sStatusChange
DefinedValues
<op> 0 Disconnect
1 Connect
<user_id> Userid ofGATTserver,or thenameofthe GATTserver.AHex value
string, eachcharof itshould inset{ ‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Max length of
itis 8.
<addr> Addressof the peerdevice.
<conn_id> Theconnection idof currentconnection.
Examples
+BLESCON:1,ABCDEF50,21:e8:5a:c2:8d:47,1
+BLESCONNotifyWhenaConnection’sStatusChange
Response
+BLESCON:<op>,<user_id>,<addr>,<conn_id>
Reference -


## Página 544

A76XX Series_AT Command Manual_V1.09
www.simcom.com 543 /652
26.2.28+BLEMTU Exchangemturequestreceivedfromremotedevice
DefinedValues
<conn_id> Theconnection idof currentconnection.
<mtu> Negotiated MTUSize.Thedefaultis23.
Examples
+BLEMTU:1,185
26.2.29AT+BLECREG RegisterGATTClient
AT+BLECREG RegisterGATTClient
TestCommand
AT+BLECREG=?
Response
OK
Read Command
AT+BLECREG?
Response
Ifthe clienthasalready been registered,response
+BLECREG:<client_index>,<user_id>
…
+BLECREG:<client_index>,<user_id>
OK
elseonly response
OK
Execution Command
AT+BLECREG
Response
+BLECREG:<client_index>,<user_id>
OK
or
ERROR
+BLEMTUExchangemturequestreceivedfromremotedevice
Response
+BLEMTU:<conn_id>,<mtu>
Reference -


## Página 545

A76XX Series_AT Command Manual_V1.09
www.simcom.com 544 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<client_index> Clientindex,the numberofregistered servers is limited to64.
<user_id> UseridofGATTClient.AHexvaluestring.Eachcharofitshouldinset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is 8.
Examples
AT+BLECREG=?
OK
AT+BLECREG
+BLECREG:0, ABCDEF50
OK
AT+BLECREG?
+BLECREG:0,ABCDEF50
OK
26.2.30AT+BLECDREG DeregisterGATTClient
AT+BLECDREG DeregisterGATTClient
TestCommand
AT+BLECDREG=?
Response
OK
WriteCommand
AT+BLECDREG=<client_index>
Response
+BLECDREG:<client_index>,<user_id>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -


## Página 546

A76XX Series_AT Command Manual_V1.09
www.simcom.com 545 /652
DefinedValues
<client_index> ClientIndex,generatedwhen registering GATTclient.
<user_id> UseridofGATTClient.AHexvaluestring.Eachcharofitshouldinset
{‘0’~‘9’,‘a’~‘f’,‘A’~‘F’}.Length is 8.
Examples
AT+BLECDREG=?
OK
AT+BLECDREG?
OK
AT+BLECDREG=0
+BLECDREG:0,ABCDEF00
OK
26.2.31AT+BLESCAN ScanSurroundingBLEDevice
YoumustexecuteAT+BLESCANafter poweron the device.
AT+BLESCAN SCAN SurroundingBLEDevice
TestCommand
AT+BLESCAN=?
Response
OK
Read Command
AT+BLESCAN?
Response
Ifhasthe devices scaned,response
+BLESCAN:
<client_index>,<server_index>,<remote_address>
…
+BLESCAN:
<client_index>,<server_index>,<remote_address>
OK
elseonly response
OK
WriteCommand
AT+BLESCAN=<client_index>,<
operation>
Response
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -


## Página 547

A76XX Series_AT Command Manual_V1.09
www.simcom.com 546 /652
DefinedValues
<client_index> Clientindex,generated whenregistering GATTclient.
<server_index> Remote deviceindex,generatedwhen scansurroundingdevice.
<remote_address> Remote deviceaddress.
<operation> This parameterhasthe following two values:
0 stop scan. Ifthedevice isnotscanning,the command response
error.
1 start scan.Ifthe deviceis scanning currently,the command
response error.
Examples
AT+BLESCAN=?
OK
AT+BLESCAN?
OK
AT+BLESCAN=0,1
OK
26.2.32+BLESCANRST NotifyWhenFindaBLEDevice
+BLESCANRST NotifyWhenFindaBLEDevice
Response
+BLESCANRST:
<client_index>,<server_index>,<remote_address>,<rssi>,<ad
v_data>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<client_index> Clientindex,generated whenregistering GATTclient.
<server_index> Remote deviceindex,generatedwhen scansurroundingdevice.
<remote_address> Remote deviceaddress.
<rssi> ReceivedSignal Strength Indication.
<adv_data> Remote device’sadvertisingdata.Hex StringType.


## Página 548

A76XX Series_AT Command Manual_V1.09
www.simcom.com 547 /652
Examples
+BLESCANRST:0,0,"1f:50:24:38:96:20",197,"02011A020A080BFF4C0010063A"
26.2.33AT+BLECGDT GetDeviceType
AT+BLECGDT GetDeviceType
TestCommand
AT+BLECGDT=?
Response
OK
WriteCommand
AT+BLECGDT=<server_index>
Response
+BLECGDT:<server_index>,<device_type>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Remote deviceindex,generatedwhen scansurroundingdevice.
<device_type> This parameterhasthe following values:
0 Unknown.
1 Classic.
2 Le
3 Dual
Examples
AT+BLECGDT=0
+BLECGDT:0,2
OK
26.2.34AT+BLECCON ConnectGATTClienttoRemoteLE/Dual-modeDevice
AT+BLECCON ConnectGATTClienttoRemoteLE/Dual-modeDevice


## Página 549

A76XX Series_AT Command Manual_V1.09
www.simcom.com 548 /652
TestCommand
AT+BLECCON=?
Response
OK
WriteCommand
AT+BLECCON=<server_index>
Response
OK
+BLECCON:<connect_id>,<remote_address>
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Remote deviceindex,generatedwhen scansurroundingdevice.
<connect_id> Theconnection idof currentconnection.
<remote_address> Remote deviceaddress.
Examples
AT+BLECCON=0
OK
+BLECCON:0,"2b:3c:42:10:23:58"
26.2.35AT+BLECDISC DisconnectGATTClienttoRemoteLE/Dual-modeDevice
AT+BLECDISC DisconnectGATTClienttoRemoteLE/Dual-modeDevice
TestCommand
AT+BLECDISC=?
Response
OK
WriteCommand
AT+BLECDISC=<connect_id>
Response
OK
+BLECDISC:<connect_id>,<remote_address>
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -


## Página 550

A76XX Series_AT Command Manual_V1.09
www.simcom.com 549 /652
DefinedValues
<connect_id> Theconnection idof currentconnection.
<remote_address> Remote deviceaddress.
Examples
AT+BLECDISC=0
OK
+BLEDISC:0,"2b:3c:42:10:23:58"
26.2.36AT+BLECSS SearchPeer’sService
AT+BLECSS SearchPeer’sService
TestCommand
AT+BLECSS=?
Response
OK
Read Command
AT+BLECSS?
Response
Ifhasthe servicessearched,response
+BLECSS:<server_index>,<service_index>,<uuid>
…
+BLECSS:<server_index>,<service_index>,<uuid>
OK
elseonly response
OK
WriteCommand
AT+BLECSS=<service_index>
Response
+BLECSS:<server_index>,<service_index>,<uuid>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<server_index> Remote deviceindex,generatedwhen scansurroundingdevice.
<service_index> Remote device’sserviceindex,generated whensearchservices.
<uuid> Theuuid of theservice.Thelengthis 4or32 bytes.Hex StringType.


## Página 551

A76XX Series_AT Command Manual_V1.09
www.simcom.com 550 /652
Examples
AT+BLECSS?
+BLECSS:0,0,0x1800
+BLECSS:0,1,0x1801
+BLECSS:0,2,0x8900
OK
26.2.37AT+BLECGC SearchPeer’sCharacteristic
AT+BLECGC SearchPeer’sCharacteristic
TestCommand
AT+BLECGC=?
Response
OK
Read Command
AT+BLECGC?
Response
Ifhasthe servicessearched,response
+BLECGC:
<service_index>,<characteristic_index>,<propertis>,<uuid>
…
+BLECGC:
<service_index>,<characteristic_index>,<propertis>,<uuid>
OK
elseonly response
OK
WriteCommand
AT+BLECGC=<characteristic_i
ndex>
Response
+BLECGC:
<service_index>,<characteristic_index>,<properties>,<uuid>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<service_index> Remote device’sserviceindex,generated whensearchservices.
<characteristic_index> Remote device’scharacteristic index,generated whensearch
characteristic.


## Página 552

A76XX Series_AT Command Manual_V1.09
www.simcom.com 551 /652
<properties> Thecharacteristic’sproperties.Itshould beacombinationof the
following values:
1 Broadcast
2 Read
4 Writewithout response
8 Write
16 Notify
32 Indicate
64 Authenticated SignedWrites
128 Extended properties
<uuid> Theuuid of thecharacteristic.Thelengthis 4or32 bytes.Hex String
Type.
Examples
AT+BLECGC?
+BLECGC:0,0,0x2A00
+BLECGC:0,1,0x2A01
+BLECGC:0,2,0x2A02
+BLECGC:0,3,0x2A03
+BLECGC:0,4,0x2A04
+BLECGC:1,5,0x2A05
+BLECGC:2,6,0x8901
OK
26.2.38AT+BLECGD SearchPeer’sCharacteristicDescriptor
AT+BLECGD SearchPeer’sCharacteristicDescriptor
TestCommand
AT+BLECGD=?
Response
OK
Read Command
AT+BLECGD?
Response
Ifhasthe servicessearched,response
+BLECGD:
<characteristic_index>,<descriptor_index>,<uuid>
…
+BLECGC:
<characteristic_index>,<descriptor_index>,<uuid>
OK
elseonly response
OK
WriteCommand Response


## Página 553

A76XX Series_AT Command Manual_V1.09
www.simcom.com 552 /652
AT+BLECGD=<descriptor_inde
x>
+BLECGC:
<characteristic_index>,<descriptor_index>,<uuid>
OK
or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<characteristic_index> Remote device’scharacteristic index,generated whensearch
characteristic.
<descriptor_index> Remote device’sdescriptorindex,generated whensearchdescriptor.
<uuid> Theuuid of thedescriptor.Thelength is4or32 bytes.HexString
Type.
Examples
AT+BLECGD?
+BLECGC:6,0,0x2902
OK
26.2.39AT+BLECRC ReadPeer’sCharacteristic
AT+BLECRC ReadPeer’sCharacteristic
TestCommand
AT+BLECRC=?
Response
OK
WriteCommand
AT+BLECRC=<characteristic_in
dex>
Response
1) Ifthe serverresponse theread requestquickly:
OK
+BLECRC:<characteristic_index>,<value>
2) Ifsend readrequestsuccefully anddon’treceive read
responsewhthinthe specified time:
OK
3)Anerroroccurred:
ERROR
ParameterSaving Mode NO_SAVE


## Página 554

A76XX Series_AT Command Manual_V1.09
www.simcom.com 553 /652
Max ResponseTime 9000ms
Reference -
DefinedValues
<characteristic_index> Remote device’scharacteristic index,generated whensearchpeer’s
characteristic.
<value> Serverresponse data.Hex StringType.
Examples
AT+BLECRC=6
OK
+BLECRC:6,"313233"
26.2.40AT+BLECWC WritePeer’sCharacteristic
AT+BLECWC WritePeer’sCharacteristic
TestCommand
AT+BLECWC=?
Response
OK
WriteCommand
AT+BLECWC=<characteristic_i
ndex>,<write_type>,<value>
Response
1)Ifwrite the characteristicsuccefully:
OK
2)Othererroroccurred:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<characteristic_index> Remote device’scharacteristic index,generated whensearchpeer’s
characteristic.
<write_type> This parameterhasthe following values:
0 writewithout response
1 write
<value> Thevalue need towrite.StringType,max lengthis (MTU–3).Ifthe
format isthe sameas HEX{DATA},and DATAis ahexadecimal string,
DATAwillbe transcoded and sent.Pleasereferto chapter26.2.27


## Página 555

A76XX Series_AT Command Manual_V1.09
www.simcom.com 554 /652
aboutMTU.
<error_code> Pleasereferto chapter26.3.
Examples
AT+BLECWC=6,0,"HEX{123456}"
OK
26.2.41AT+BLECRD ReadPeer’sCharacteristicDescriptor
AT+BLECRD ReadPeer’sCharacteristicDescriptor
TestCommand
AT+BLECRD=?
Response
OK
WriteCommand
AT+BLECRD=<descriptor_inde
x>
Response
3) Ifthe serverresponsethe read requestquickly:
OK
+BLECRD:<descriptor_index>,<value>
2) Ifsend readrequestsuccefully anddon’treceive read
responsewhthinthe specified time:
OK
3)Anerroroccurred:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<descriptor_index> Remote device’sdescriptorindex,generated whensearchpeer’s
characteristic descriptor.
<value> Serverresponse data.Hex StringType.
Examples
AT+BLECRD=0
OK
+BLECRD:0,"0000"


## Página 556

A76XX Series_AT Command Manual_V1.09
www.simcom.com 555 /652
26.2.42AT+BLECWD WritePeer’sCharacteristicDescriptor
AT+BLECWD WritePeer’sCharacteristicDescriptor
TestCommand
AT+BLECWD=?
Response
OK
WriteCommand
AT+BLECWD=<descriptor_inde
x>,<value>
Response
1)Ifwrite the characteristicsuccefully:
OK
2)Othererroroccurred:
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<descriptor_index> Remote device’sdescriptorindex,generated whensearchpeer’s
characteristic descriptor.
<value> Thevalue need towrite.StringType,max lengthis (MTU–3).Ifthe
format isthe sameas HEX{DATA},and DATAis ahexadecimal string,
DATAwillbe transcoded and sent.Pleasereferto chapter26.2.27
aboutMTU.
<error_code> Pleasereferto chapter26.3.
Examples
AT+BLECWD=0,"HEX{0100}"
OK
26.2.43+BLECNTY NotifyWhenGetaNoticationfromPeer’sDevice
+BLECNTY NotifyWhenGetaNoticationfromPeer’sDevice
Response
+BLECNTY:<connect_id>,<characteristic_index>,<value>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues


## Página 557

A76XX Series_AT Command Manual_V1.09
www.simcom.com 556 /652
<connect_id> Theconnection idof currentconnection.
<characteristic_index> Remote device’scharacteristic index,generated whensearch
characteristic.
<value> Serverresponse data.Hex StringType.
Examples
+BLECNTY:0,6,"02011A020A080BFF4C0010063A"
26.2.44+BLECIND NotifyWhenGetaIndicationfromPeer’sDevice
+BLECIND NotifyWhenGetaIndicationfromPeer’sDevice
Response
+BLECIND:<connect_id>,<characteristic_index>,<value>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<connect_id> Theconnection idof currentconnection.
<characteristic_index> Remote device’scharacteristic index,generated whensearch
characteristic.
<value> Serverresponse data.Hex StringType.
Examples
+BLECIND: 0,6,"02011A020A080BFF4C0010063A"
26.3 DetailedDescriptionofATCommandsforBT
Following commandsare only supportedby specificFW,fordetailed informationpleasecontactwith
SIMCom FAE


## Página 558

A76XX Series_AT Command Manual_V1.09
www.simcom.com 557 /652
26.3.1 AT+BTPOWER Open/CloseBTDevice
This commandis used toopen/closebt device.Afteropeningbt device,btdevice canbe discoveredand
connectedby otherbtdevice.BTdevicemustin openstatus before executingAT+BTSCAN,AT+BTPAIR,
AT+UNPAIR,AT+BTSPPSRV,AT+BTSPPPROF,AT+BTSPPCONNandAT+BTSPPSENDcommand.
AT+BTPOWER Open/CloseBTDevice
TestCommand
AT+BTPOWER=?
Response
+BTPOWER:(0-1)
OK
Read Command
AT+BTPOWER?
Response
+BTPOWER:<status>
OK
WriteCommand
AT+BTPOWER=<flag>[,<debug
>]
Response
OK
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<flag> This parameterhasthe following two values:
0 closebtdevice.
1 open bt device.
<debug> Currentnotsupport.
<status> This parameterhasthe following two values:
0 bt devicehasbeen closed.
1 bt devicehasbeen opened.
Examples
AT+BTPOWER=?
+BTPOWER:(0-1)
OK
AT+BTPOWER?
+BTPOWER:0
OK


## Página 559

A76XX Series_AT Command Manual_V1.09
www.simcom.com 558 /652
AT+BTPOWER=1
OK
AT+BTPOWER?
+BTPOWER:1
OK
26.3.2 AT+BTHOST Get/SetBTDeviceName
This commandis used togetbtdevicenameandbtdevice address,and canalsosetbtdevice name.This
commandcanbeexecutedatanytime.Theparameterwillbesavedinflash.Ifbtdevicenameissetduring
btdevicein open status,then need toreopen btdeviceby executingAT+BTPOWERcommandto makebt
devicenamevalid,otherwise btdevicenameinquiried byotherbt deviceisthe sameas previous bt device
name.
AT+BTHOST Get/SetBTDeviceName
Read Command
AT+BTHOST?
Response
+BTHOST:<bt_name>,<bt_addr>
OK
WriteCommand
AT+BTHOST=<new_bt_name>
Response
OK
Or
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<new_bt_name> New btdevicename.(max247bytes)
<bt_name> Currentbt devicename.
<bt_addr> Currentbt deviceaddress.
Examples
AT+BTHOST?
+BTHOST:SIMCOM BT,11:22:23:33:33:80
OK
AT+BTHOST=SIMCOM_BT_DEVICE


## Página 560

A76XX Series_AT Command Manual_V1.09
www.simcom.com 559 /652
OK
AT+BTHOST?
+BTHOST:SIMCOM_BT_DEVICE,11:22:23:33:33:80
OK
26.3.3 AT+BTADDR Get/SetBTDeviceAddress
This commandis used toget/setbtdevice address.This commandcanbe executed atany time.The
parameterwillbe saved inflash.Ifbtdevice addressis setduring btdeviceinopen status, thenneed to
reopen btdevice byexecutingAT+BTPOWERcommand tomakebt deviceaddressvalid, otherwise bt
deviceaddress notchange.
AT+BTADDR Get/SetBTDeviceAddress
Read Command
AT+BTADDR?
Response
+BTADDR:<bt_addr>
OK
WriteCommand
AT+BTADDR=<new_bt_addr>
Response
OK
Or
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<new_bt_addr> New btdeviceaddress.
<bt_addr> Currentbt deviceaddress.
Examples
AT+BTADDR?
+BTADDR:38:08:17:26:36:45
OK
AT+BTADDR=112223333380
OK
AT+BTADDR?


## Página 561

A76XX Series_AT Command Manual_V1.09
www.simcom.com 560 /652
+BTADDR:11:22:23:33:33:80
OK
26.3.4 AT+BTSCAN ScanBTdevice
This commandis used todiscoverotherbtdevice. BTdevice mustin open statusbefore executing this
command.
AT+BTSCAN ScanBTDevice
TestCommand
AT+BTSCAN=?
Response
+BTSCAN:(0-1),(0-1),(6-48)
OK
WriteCommand
AT+BTSCAN=<flag>[,<mode>[,
<timeout>]]
Response
OK
+BTSCAN:
<scan_status>[,<index>,<bt_name>,<bt_addr>,<rssi_level>]
…
<scan_status>[,<index>,<bt_name>,<bt_addr>,<rssi_level>]
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<flag> This parameterhasthe following two values:
0 stop scan.
1 start scan.
<mode> This parameterhasthe following two values:
0 don’thide paired device.
1 hide paired device.
<timeout> Durationof thescan.
duration =1.28s*timeout.(default:12.8s)
<scan_status> This parameterhasthe following two values:
0 scanning.
1 scanend.
<index> Theindex ofdiscoveredbt device.(startwith 1)
<bt_name> Thenameof discoveredbtdevice.
<bt_addr> Theaddress ofdiscovered btdevice.


## Página 562

A76XX Series_AT Command Manual_V1.09
www.simcom.com 561 /652
<rssi_level> Received signalstrength indication
Examples
AT+BTPOWER=1
OK
AT+BTSCAN=1,0,20
OK
+BTSCAN:0,1,Mi Note3,F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,HonorV10,90:94:97:DA:3C:73,176
+BTSCAN:0,4,OPPOA91,1C:02:19:8F:8A:7C,180
+BTSCAN:1
AT+BTSCAN=1,0,20
OK
+BTSCAN:0,1,Mi Note3, F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,HonorV10,90:94:97:DA:3C:73,176
AT+BTSCAN=0
OK
+BTSCAN:1
26.3.5 AT+BTIOCAP Get/SetBTDeviceIOCapalibity
This commandis used toget/setbtdevice iocapability.This commandcan be executedatany time.The
parameterwillbesavedinflash.Ifbtdeviceiocapabilityissetduringbtdeviceinopen status,thenneedto
reopen btdevice byexecutingAT+BTPOWERcommand tomakebt deviceiocapabilityvalid, otherwise bt
deviceio capability not change.
AT+BTIOCAP Get/SetBTDeviceIOCapability
TestCommand
AT+BTIOCAP=?
Response
+BTIOCAP:(0-3)
OK
TestCommand
AT+BTIOCAP?
Response
+BTIOCAP:<mode>
OK
WriteCommand Response


## Página 563

A76XX Series_AT Command Manual_V1.09
www.simcom.com 562 /652
AT+BTIOCAP=<mode> +BTIOCAP:1
OK
Or
ERROR
ParameterSaving Mode SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<mode> This parameterhasthe following fourvalues:
0 display only.
1 display andyes orno.
2 keyboard only.
3 no display and no keyboard.
Examples
AT+BTIOCAP=?
+BTIOCAP:(0-3)
OK
AT+BTIOCAP?
+BTIOCAP:1
OK
AT+BTIOCAP=3
+BTIOCAP:1
OK
AT+BTIOCAP?
+BTIOCAP:3
OK
26.3.6 AT+BTPAIR PairWithOtherBTDevice
This commandis used topairwith otherbtdevice.Afterpairingsuccess,the pairing information will be
saved inflashand the bluetoothconnectionwillbe disconnected actively.YoucanuseAT+BTPAIRED
command toquery the pairedbt device. BTdevicemustinopen statusbefore executingthis command.


## Página 564

A76XX Series_AT Command Manual_V1.09
www.simcom.com 563 /652
AT+BTPAIR PairWithOtherBTDevice
TestCommand
AT+BTPAIR=?
Response
+BTPAIR:(index)
OK
WriteCommand
AT+BTPAIR=0,<index>
Response
OK
+BTPAIRING:<mode>,<bt_name>,<bt_addr>[,<key>]
Or
ERROR
WriteCommand
AT+BTPAIR=<mode>,<accept>[,
<key>]
Response
OK
+BTPAIR:<result>,<bt_name>,<bt_addr>
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<mode> This parameterhasthe following sixvalues:
1 compare mode.
2 passkey mode.
3 rebond mode.(Current notsupport)
4 notifymode. (Currentnotsupport)
5 justwork mode.(Support, but invisibletouser,userdo nothing)
6 pin code mode.(Currentnotsupport)
<accept> Whenpairingis initiated,itisthe index ofthe remote device;And as a
confirmation ofthe pairingrequest
<key> Random generation 6-digitverificationcode.(default: 123456)
<index> AT+BTSCANcommandresponse index.
<bt_name> Thenameof remote btdevice.
<bt_addr> Theaddress ofremote btdevice.
<result> This parameterhasthe following two values:
0 pair fail.
1 pair success.
Examples
AT+BTIOCAP=1
+BTIOCAP:1


## Página 565

A76XX Series_AT Command Manual_V1.09
www.simcom.com 564 /652
OK
AT+BTPOWER=1
OK
AT+BTSCAN=1,0,20
OK
+BTSCAN:0,1,Mi Note3,F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,KK,04:8C:9A:D7:90:4C,180
+BTSCAN:1
AT+BTPAIR=0,3
OK
+BTPAIRING:1,KK,04:8C:9A:D7:90:4C,466622
AT+BTPAIR=1,1
OK
+BTPAIR:1,KK,04:8C:9A:D7:90:4C
AT+BTIOCAP=2
+BTIOCAP:1
OK
AT+BTPOWER=0
OK
AT+BTPOWER=1
OK
AT+BTSCAN=1,0,20
OK
+BTSCAN:0,1,Mi Note3,F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,KK,04:8C:9A:D7:90:4C,180
+BTSCAN:1
AT+BTPAIR=0,3
OK
+BTPAIRING:2,peer btname,04:8C:9A:D7:90:4C //forpasskey mode,bt_name fixedas
"peerbtname"
AT+BTPAIR=2,1,123456 //"123456",remote btdevice displayed
OK
+BTPAIR:1,KK,04:8C:9A:D7:90:4C


## Página 566

A76XX Series_AT Command Manual_V1.09
www.simcom.com 565 /652
26.3.7 AT+BTUNPAIR UnpairWithThePairedBTDevice
This commandis used toUnpairwith the pairedbt device.Youshould makesure the bluetoothconnection
isdisconnectedbeforeunpairing.Afterunpairingsuccess,thepairinginformationwillberemovedfromflash.
BTdevice mustinopen status beforeexecuting this command.
AT+BTUNPAIR UnpairWithThePairedBTDevice
TestCommand
AT+BTUNPAIR=?
Response
+BTUNPAIR:(index)
OK
WriteCommand
AT+BTUNPAIR=<index>
Response
OK
+BTUNPAIR:<status>
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<index> AT+BTPAIREDcommandresponse index.
<status> This parameterhasthe following two values:
0 unpairfail.
1 unpairsuccess.
Examples
AT+BTPOWER=1
OK
AT+BTPAIRED?
OK
+BTPAIRED:2,1,KK,04:8C:9A:D7:90:4C
+BTPAIRED:2,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F
AT+BTUNPAIR=1
OK
+BTUNPAIR:1


## Página 567

A76XX Series_AT Command Manual_V1.09
www.simcom.com 566 /652
AT+BTPAIRED?
OK
+BTPAIRED:1,1,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F
26.3.8 AT+BTPAIRED GetPairedBTDevice
This commandis used togetpaired btdevice.Thiscommand canbe executedatany time.
AT+BTPAIRED GetPairedBTDevice
Read Command
AT+BTPAIRED?
Response
OK
+BTPAIRED:<paired_num>,<index>,<bt_name>,<bt_addr>
…
+BTPAIRED:<paired_num>,<index>,<bt_name>,<bt_addr>
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<paired_num> Thetotal numberofpaired btdevice.
<index> Theindex ofpaired btdevice.
<bt_name> Thenameof pairedbtdevice.
<bt_addr> Theaddress ofpaired btdevice.
Examples
AT+BTPAIRED?
OK
+BTPAIRED:2,1,KK,04:8C:9A:D7:90:4C
+BTPAIRED:2,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F
26.3.9 AT+BTSPPSRV Active/DeactiveLocalSPPService
Thiscommandisusedtoactive/deactivelocalSPPservice.BTdevicemustinopenstatusbeforeexecuting
this command.


## Página 568

A76XX Series_AT Command Manual_V1.09
www.simcom.com 567 /652
AT+BTSPPSRV Active/DeactiveLocalSPPService
TestCommand
AT+BTSPPSRV=?
Response
+BTSPPSRV:(0-1)
OK
RestCommand
AT+BTSPPSRV?
Response
+BTSPPSRV:<status>
OK
WriteCommand
AT+BTSPPSRV=<flag>
Response
+BTSPPSRV:<result>
OK
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<flag> This parameterhasthe following two values:
0 deactive localsppservice.(Currentnotsupport)
1 active localspp service.
<status> This parameterhasthe following two values:
0 local notsupportsppservice.
1 local supportsppservice.
<result> This parameterhasthe following two values:
0 local sppservice isdeactived.
1 local sppservice isactive.
Examples
AT+BTPOWER=1
OK
AT+BTSPPSRV=?
+BTSPPSRV:(0-1)
OK
AT+BTSPPSRV?
+BTSPPSRV:1
OK
AT+BTSPPSRV=0


## Página 569

A76XX Series_AT Command Manual_V1.09
www.simcom.com 568 /652
ERROR
AT+BTSPPSRV=1
+BTSPPSRV:1
OK
26.3.10 AT+BTSPPPROF GetRemoteBTDeviceSPPServiceStatus
This commandis used tocheckwhetherthe remotebtdevice supportsppservice.BTdevice mustinopen
status beforeexecuting thiscommand.
AT+BTSPPPROF GetRemoteBTDeviceSPPServiceStatus
WriteCommand
AT+BTSPPPROF=<index>
Response
OK
+BTSPPPROF:<status>
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<index> AT+BTPAIREDcommandresponse index.
<status> This parameterhasthe following two values:
0 remote btdevice sppservice isnotactive.
1 remote btdevice sppservice isactive.(Currentfixedreturn)
Examples
AT+BTPOWER=1
OK
AT+BTPAIRED?
OK
+BTPAIRED:2,1,KK,04:8C:9A:D7:90:4C
+BTPAIRED:2,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F
AT+BTSPPPROF=1


## Página 570

A76XX Series_AT Command Manual_V1.09
www.simcom.com 569 /652
OK
+BTSPPPROF:1
26.3.11 AT+BTSPPCONN Establish/ReleaseSPPConnection
This commandis used toestablish/releasesppconnection.Iflocalbt devicewhichdoes’thave pairing
informationofpeer btdevice initiate sppconnection,thenpairingprocedure need tobe executedbefore
establishing sppconnection.Thepairingprocedurecan betriggered manually byAT+BTPAIRcommand. If
localbt devicewhichhaspairinginformation ofpeerbtdevice initiatesppconnection,then sppconnection
procedurecan betriggered directly byAT+BTSPPCONNcommandwithout manuallyperforming pairing
procedureagain.BTdevicemustinopen status beforeexecuting this command.
AT+BTSPPCONN Establish/ReleaseSPPConnection
TestCommand
AT+BTSPPCONN=?
Response
+BTSPPCONN:(0-1)
OK
RestCommand
AT+BTSPPCONN?
Response
+BTSPPCONN:<status>
OK
WriteCommand
AT+BTSPPCONN=<action>[,<in
dex>]
Response
OK
+BTSPPCONN:<result>[,<max_frame_size>,<bt_addr>]
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<action> This parameterhasthe following two values:
0 release sppconnection.
1 establish sppconnection.
<index> AT+BTPAIREDcommandresponse index.(default:1)
<status> This parameterhasthe following two values:
0 spp connectionhas been release.
1 spp connectionhas been established.
<result> This parameterhasthe following two values:
0 spp connectionreleasesuccess.
1 spp connectionestablish success.


## Página 571

A76XX Series_AT Command Manual_V1.09
www.simcom.com 570 /652
<max_frame_size> Maximum framesize.(unit:byte)
<bt_addr> Theaddress ofconnectedbt device.
Examples
AT+BTPOWER=1
OK
AT+BTSCAN=1,0,20
OK
+BTSCAN:0,1,Mi Note3, F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,KK,04:8C:9A:D7:90:4C,180
+BTSCAN:1
AT+BTPAIR=0,3
OK
+BTPAIRING:1,KK,04:8C:9A:D7:90:4C,466622
AT+BTPAIR=1,1
OK
+BTPAIR:1,KK,04:8C:9A:D7:90:4C
AT+BTPAIRED?
OK
+BTPAIRED:1,1,KK,04:8C:9A:D7:90:4C
AT+BTSPPCONN=1,1
OK
+BTSPPCONN:1,990,04:8C:9A:D7:90:4C
AT+BTSPPCONN=0
OK
+BTSPPCONN:0
26.3.12 AT+BTSPPSEND SPPSendData
This commandis used tosenddata byspp.SPPconnectionmust beestablishedbeforesending data.
AT+BTSPPSEND SPPSendData


## Página 572

A76XX Series_AT Command Manual_V1.09
www.simcom.com 571 /652
WriteCommand
AT+BTSPPSEND=<data>
Response
OK
+BTSPPSEND:<result>
Or
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference -
DefinedValues
<data> Thedata tobe sent.Thefollowing itemsneedto notice:
Ifmax_frame_size of +BTSPPCONNeventmore than200, then SPP
MTUis equalto200.
Ifmax_frame_size of +BTSPPCONNeventlessthan 200,then SPP
MTUis equaltomax_frame_size.
Theamountof datasentby usershouldlessthan orequal toSPP
MTU,orreturn ERROR.
<result> This parameterhasthe following two values:
0 sendfail.
1 sendsuccess.
Examples
AT+BTPOWER=1
OK
AT+BTSCAN=1,0,20
OK
+BTSCAN:0,1,Mi Note3,F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,KK,04:8C:9A:D7:90:4C,180
+BTSCAN:1
AT+BTPAIR=0,3
OK
+BTPAIRING:1,KK,04:8C:9A:D7:90:4C,466622
AT+BTPAIR=1,1
OK
+BTPAIR:1,KK,04:8C:9A:D7:90:4C


## Página 573

A76XX Series_AT Command Manual_V1.09
www.simcom.com 572 /652
AT+BTPAIRED?
OK
+BTPAIRED:1,1,KK,04:8C:9A:D7:90:4C
AT+BTSPPCONN=1,1
OK
+BTSPPCONN:1,990,04:8C:9A:D7:90:4C
AT+BTSPPSEND=003100320033006100620063
OK
+BTSPPSEND:1
26.3.13 +BTSPPRECV SPPReceiveData
This urc isusedtoindicate spp hasreceiveddata.SPPconnectionmustbe establishedbeforereceiving
data.
DefinedValues
<data_len> Thelength ofspp received data.
<data> SPPreceiveddata.Thefollowing itemsneed tonotice:
Ifmax_frame_size of +BTSPPCONNeventmore than200, then SPP
MTUis equalto200.
Ifmax_frame_size of +BTSPPCONNeventlessthan 200,then SPP
MTUis equaltomax_frame_size.
Theamountof datareceivedfrom remoteshould lessthan orequal to
SPPMTU,orreturn +BTSPPRECV:ERROR.
Examples
AT+BTPOWER=1
OK
AT+BTSCAN=1,0,20
+BTSPPRECV SPPReceiveData
URC
+BTSPPRECV:<data_len>,<data>
Or
+BTSPPRECV:ERROR
Reference -


## Página 574

A76XX Series_AT Command Manual_V1.09
www.simcom.com 573 /652
OK
+BTSCAN:0,1,Mi Note3,F4:F5:DB:C9:03:2C,178
+BTSCAN:0,2,HUAWEIWATCHGT2-A5F,A0:D8:07:A6:7A:5F,194
+BTSCAN:0,3,KK,04:8C:9A:D7:90:4C,180
+BTSCAN:1
AT+BTPAIR=0,3
OK
+BTPAIRING:1,KK,04:8C:9A:D7:90:4C,466622
AT+BTPAIR=1,1
OK
+BTPAIR:1,KK,04:8C:9A:D7:90:4C
AT+BTPAIRED?
OK
+BTPAIRED:1,1,KK,04:8C:9A:D7:90:4C
AT+BTSPPCONN=1,1
OK
+BTSPPCONN:1,990,04:8C:9A:D7:90:4C
+BTSPPRECV:24,003100320033006100620063
26.4 Descriptionof<error_code>
Errorcodes Description
1 InvalidHandle
2 ReadNot Permitted
3 WriteNot Permiited
4 InvalidPDU
5 InsufficientAuthentication
6 RequestNotSupported
7 InvalidOffset
8 InsufficientAuthorizaztion
9 PrepareQueueFull
10 AttributeNotFound
11 AttributeNotLong
12 InsufficientEncryption Key Size
13 InvalidAttributeValueLength
14 UnlikelyError


## Página 575

A76XX Series_AT Command Manual_V1.09
www.simcom.com 574 /652
15 InsufficientEncryption
16 UnsupportedGroupType
17 InsufficientResources
18 DatabaseOutOfSync
19 ValueNotAllowed
ATCommandsforCTBURST
27.1 OverviewofATCommandsforCTBURST
Command Description
AT+CTBURST TheRFTXBurstTest
27.2 Supportsthe 1802 platform.
27.3 Supportsthe 1601,1603,1606 and1803 platforms.
27.2 DetailedDescriptionofATCommandsforCTBURST(CAT4)
27.2.1 AT+CTBURST TheRFTXBurstTest
AT+CTBURST The RFTXBurstTest
TestCommand Response
NOTE


## Página 576

A76XX Series_AT Command Manual_V1.09
www.simcom.com 575 /652
AT+CTBURST=? +CTBURST=0-2,0-142,1-65535,-5000-3500,0-5
OK
WriteCommand
AT+CTBURST=<option>,<b
and>,<power>,<gsmband>,
<para>
Response
1)LTERFTXsuccessfully:
*RADIOPOWER:0
OK
+CTBURST:0
2)GSM RFTXsuccessfully:
*RADIOPOWER:0
+CTBURST:0
*GSMTR:-31355
OK
RFTXfailed:
*RADIOPOWER:0
+CMEERROR: unknownerror
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<option> 0–default.
<band> 1–LTEfrequency=18300.
2–LTEfrequency=18900.
3–LTEfrequency=19500.
4–LTEfrequency=20175.
5–LTEfrequency=20525.
6–LTEfrequency=20700.
7–LTEfrequency=21100.
8–LTEfrequency=21625.
17 –LTEfrequency =23790.
20 –LTEfrequency =24300.
38 –LTEfrequency =38000.
39 –LTEfrequency =38450.
40 –LTEfrequency =39150.
101–GSMTX.


## Página 577

A76XX Series_AT Command Manual_V1.09
www.simcom.com 576 /652
<power> 0–LTEmaxpower.
1–LTE10dBm.
9–LTEturn offTX.
<gsmband> 0–GSM 900.
1–GSM 1800.
2–GSM 1900.
3–GSM 850.
4–WCDMAband1.
5–WCDMAband2.
7–WCDMAband5.
8–WCDMAband8.
9–GSM turn offTX.
<para> 0–GSMTX.
9–GSM turn offTX.
Examples
//Example ofGSMTX
AT+CFUN=0
+SIMCARD:NOTAVAILABLE
+CGEV:MEDETACH
OK
*RADIOPOWER:0
AT+CTBURST=0,101,0,1,0 //GSM 1800 MHZMaximum power emission
*RADIOPOWER:0
+CTBURST:0
*GSMTR:-31355
OK
AT+CTBURST=0,101,0,9,9 //GSM turn offemission.
*RADIOPOWER:0
+CTBURST:0
*GSMTR:-31355
OK


## Página 578

A76XX Series_AT Command Manual_V1.09
www.simcom.com 577 /652
//Example ofLTETX
AT+CFUN=0
+SIMCARD:NOTAVAILABLE
+CGEV:MEDETACH
OK
*RADIOPOWER:0
AT+CTBURST=0,38,0 //B38 Maximumpower emission
*RADIOPOWER:0
OK
+CTBURST:0
AT+CTBURST=0,1,9 //LTEturnoffemission.
*RADIOPOWER:0
OK
+CTBURST:0
Totesteach item,closethe previous item first.
WhentestingLTE,you needto restartthe module.
Thesecondandthird parameters are mainly forLTE,andthe fourthandfifthparametersare mainly
forGSM
27.3 DetailedDescriptionofATCommandsforCTBURST(CAT1&CAT4)
27.3.1 AT+CTBURST TheTX/RXBurstTest
AT+CTBURST The RFTXBurstTest
TestCommand
AT+CTBURST=?
Response
+CTBURST=0-2,0-142,1-65535,-5000-3500,0-5
NOTE


## Página 579

A76XX Series_AT Command Manual_V1.09
www.simcom.com 578 /652
OK
WriteCommand
AT+CTBURST=<mode>[,<b
and>,<channel>,<power>[,
<bandwith>]]
Response
Ifmodeis 0
+CTBURST:TX/RXOFF
OK
Ifmodeis 1
+CTBURST:TXON
OK
Ifmodeis 2
Forgsm/wcdma
+CTBURST:RX[rssiValue]
OK
ForLTE
+CTBURST:RX: [mainRssiValue],[secRssiValue]
OK
ParameterSaving Mode NO_SAVE
Max ResponseTime -
Reference
DefinedValues
<mode> Start/stopTX/RX theburst/waveform
0–stop RFTX/RX
1–start RFTX
2–start RFRX
<band> Theband of burst/waveform tobe sent
0–GSM 850 Band
1–GSM 900 Band
2–GSM DCS1800 Band
3–GSM PCS1900 Band
10 –WCDMAIMT2000 Band
11–WCDMAPCS1900 Band
12 –WCDMA800 Band
13 –WCDMA850 Band
14 –WCDMA900 Band
101–LTE1Band
102–LTE2Band
103–LTE3Band
104–LTE4Band


## Página 580

A76XX Series_AT Command Manual_V1.09
www.simcom.com 579 /652
105–LTE5Band
106–LTE6Band
107–LTE7Band
108–LTE8Band
109–LTE9Band
110–LTE10 Band
111–LTE11Band
112–LTE12 Band
113–LTE13 Band
114–LTE14 Band
117–LTE17 Band
118–LTE18 Band
119–LTE19 Band
120–LTE20Band
121–LTE21Band
122–LTE22Band
123–LTE23Band
124–LTE24Band
125–LTE25Band
126–LTE26Band
127–LTE27Band
128–LTE28Band
131–LTE31Band
133–LTE33Band
134–LTE34Band
135–LTE35Band
136–LTE36Band
137–LTE37Band
138–LTE38Band
139–LTE39Band
140–LTE40Band
141–LTE41Band
142–LTE42Band
172–LTE72Band
<channel> Frequency channel,the rangeis differentaccording to differentband
GSM 850:128~251
GSM 900:1~124, 975~1023
GSM DCS1800:512~885
GSM PCS1900:512~810
WCDMAIMT2000: 9612~9888
WCDMAPCS1900:9262~9538
WCDMA800: 4132~4233,782~862
WCDMA850: 4132~4233,782~862
WCDMA900: 2712~2863
LTE1: 18000~18599


## Página 581

A76XX Series_AT Command Manual_V1.09
www.simcom.com 580 /652
LTE2: 18600~19199
LTE3: 19200~19949
LTE4: 19950~20399
LTE5: 20400~20649
LTE6: 20650~20749
LTE7: 20750~21449
LTE8: 21450~21799
LTE9: 21800~22149
LTE10: 22150~22749
LTE11:22750~22949
LTE12: 23010~23179
LTE13: 23180~23279
LTE14: 23280~23379
LTE17: 23730~23849
LTE18: 23850~23999
LTE19: 24000~24149
LTE20: 24150~24449
LTE21: 24450~24599
LTE22: 24600~25399
LTE23: 25500~25699
LTE24: 25700~26039
LTE25: 26040~26689
LTE26: 26690~27039
LTE27: 27040~27209
LTE28: 27210~27659
LTE31: 27760~27809
LTE33: 36000~36199
LTE34: 36200~36349
LTE35: 36350~36949
LTE36: 36950~37549
LTE37: 37550~37749
LTE38: 37750~38249
LTE39: 38250~38649
LTE40: 38650~39649
LTE41: 39650~41589
LTE42: 41590~43589
LTE72: 133472~133521
<power> ForLTE:Thepower between 0~2000apc,the valueis differentwith
differentband
ForGSM:Thepower means afcDac,the valuebetween 0and1023.
suggestedrangeis(200-700)(toobiggerwillcauseTxsaturated,and
equipmentcould notdetectit)
<bandwith> Rx bandwidth:0~5.if itisWCDMA,this value mustbe setto 0.
01.4M
13M


## Página 582

A76XX Series_AT Command Manual_V1.09
www.simcom.com 581 /652
25M
310M
415M
520M
<rssiValue> TheRx Power forGSM/WCDMA
<mainRssiValue > TheMain antRxPower forLTE
<secRssiValue> TheSecantRxPower forLTE
Examples
AT+CFUN=0 //Minimumfunctionality
OK
AT+CTBURST=1,101,18300,2000 //Start RF TX Power of LTE BAND1 the arfcn is
18300the power is2000apc
+CTBURST:TXON
OK
AT+CTBURST=0 //CloseTX/RXCTBURST
+CTBURST:TX/RXOFF
OK
AT+CTBURST=2,101,18300,2000,5 //Start RF RX Power of LTE BAND1 the arfcn is
18300,mainrssiis -60 secrssiis 0
+CTBURST:RX:-60,0
OK
AT+CTBURST=0 //CloseTX/RXCTBURST
+CTBURST:TX/RXOFF
OK
AT+CTBURST=1,10,9750,2000 //Start RF TX Power of WCDMA2000 the arfcn is
9750 the poweris 2000apc
+CTBURST:TXON
OK
AT+CTBURST=0 //CloseTX/RXCTBURST
+CTBURST:TX/RXOFF
OK
AT+CTBURST=2,10,9750,2000,0
+CTBURST:RX:-60 //Start RF RX Power of WCDMA2000 the arfcn is


## Página 583

A76XX Series_AT Command Manual_V1.09
www.simcom.com 582 /652
OK
9750
AT+CTBURST=0 //CloseTX/RXCTBURST
+CTBURST:TX/RXOFF
OK
Totesteach item,closethe previous item first.
NOTE


## Página 584

A76XX Series_AT Command Manual_V1.09
www.simcom.com 583 /652
SummaryofERRORCodes
28.1 VerboseCodesandNumericCodes
Verboseresultcode Numeric(V0set) Description
OK 0 Command executed,no errors,Wakeupafter
reset
CONNECT 1 Linkestablished
RING 2 Ringdetected
NOCARRIER 3 Linknotestablishedordisconnected
ERROR 4 Invalidcommandorcommand linetoo long
NODIALTONE 6 Nodialtone,dialingimpossible,wrong mode
BUSY 7 Remotestationbusy
NOANSWER 8 Connectioncompletion timeout
28.2 ResponseStringofAT+CEER
Number Responsestring
CSinternalcause
0 Unknown
1 Unassigned number
3 Noroute todestination
6 Channelunacceptable
8 Operatordeterminedbarring
16 Normal callclearing
17 Userbusy
18 Nouserresponding
19 Useralerting: no answer
21 Callrejected
22 Numberchanged
25 Preemption


## Página 585

A76XX Series_AT Command Manual_V1.09
www.simcom.com 584 /652
26 Non selecteduserclearing
27 Destinationoutoforder
28 Invalidnumberformat
29 Facility rejected
30 Responseto STATUSENQUIRY
31 Normal unspecified
34 Nocircuit/channel available
38 Networkoutof order
41 Temporary failure
42 Switching equipmentcongestion
43 Access information discarded
44 Requested circuit/channelunavailable
47 Resourceunavailable
49 QoS unavailable
50 Requested facilitynotsubscribed
55 Incoming calls barredwithinCUG
57 Bearercapabilitynotauthorized
58 Bearercapabilitynotavailable
63 Servicenotavailable
65 Bearerservice notimplemented
68 ACM MAXreached
69 Facility notimplemented
70 Only RDIbearercapability available
79 Servicenotimplemented
81 Invalidtransaction ID
87 UsernotmemberofCUG
88 Incompatible destination
91 Invalidtransit network selection
95 Incorrectmessage
96 Invalidmandatory information
97 Message type non-existent
98 Message type wrong state
99 Informationelement not-existent
100 ConditionalIEerror
101 Message wrongstate
102 Recovery aftertimerexpiry
111 Protocolerror: unspecified
127 Interworking:unspecified
224 Callbarring
241 FDNBlocked


## Página 586

A76XX Series_AT Command Manual_V1.09
www.simcom.com 585 /652
CSnetworkcause
1 Unassigned/unallocatednumber
3 Noroute todestination
6 Channelunacceptable
8 Operatordeterminedbarring
16 Normal callclearing
17 Userbusy
18 Nouserresponding
19 Useralerting, no answer
21 Callrejected
22 Numberchanged
26 Non selecteduserclearing
27 Destinationoutoforder
28 Invalid/incomplete number
29 Facility rejected
30 Responseto StatusEnquiry
31 Normal, unspecified
34 Nocircuit/channel available
38 Networkoutof order
41 Temporary failure
42 Switching equipmentcongestion
43 Access information discarded
44 Requested circuit/channelnotavailable
47 Resources unavailable,unspecified
49 Quality ofserviceunavailable
50 Requested facilitynotsubscribed
55 Incoming calls barredwithinthe CUG
57 Bearercapabilitynotauthorized
58 Bearercapabilitynotavailable
63 Service/option notavailable
65 Bearerservice notimplemented
68 ACM >=ACMmax
69 Requested facilitynotimplemented
70 Only RDIbearerisavailable
79 Service/option notimplemented
81 Invalidtransaction identifiervalue
87 UsernotmemberofCUG
88 Incompatible destination
91 Invalidtransit network selection
95 Semanticallyincorrectmessage
96 Invalidmandatory information


## Página 587

A76XX Series_AT Command Manual_V1.09
www.simcom.com 586 /652
97 Message non-existent/not implemented
98 Message type notcompatiblewith state
99 IEnon-existent/notimplemented
100 ConditionalIEerror
101 Message notcompatible with state
102 Recovery on timerexpiry
111 Protocolerror,unspecified
117 Interworking,unspecified
CSnetworkreject
2 IMSI unknown inHLR
3 IllegalMS
4 IMSI unknown inVLR
5 IMEI notaccepted
6 IllegalME
7 GPRSservicesnot allowed
8 GPRS&nonGPRS servicesnotallowed
9 MS identitycannotbe derived
10 Implicitly detached
11 PLMNnot allowed
12 LocationArea notallowed
13 Roaming not allowed
14 GPRSservicesnot allowed inPLMN
15 NoSuitable Cells InLocationArea
16 MSCtemporarilynotreachable
17 Networkfailure
20 MACfailure
21 Synchfailure
22 Congestion
23 GSM authentication unacceptable
32 Serviceoption notsupported
33 Requested serviceoption notsubscribed
34 Serviceoption temporarilyoutoforder
38 Callcannotbe identified
40 NoPDPcontextactivated
95 Semanticallyincorrectmessage
96 Invalidmandatory information
97 Message type non-existent
98 Message type notcompatiblewith state
99 Informationelement non-existent
101 Message notcompatible with state


## Página 588

A76XX Series_AT Command Manual_V1.09
www.simcom.com 587 /652
161 RRreleaseindication
162 RRrandom accessfailure
163 RRCreleaseindication
164 RRCclosesession indication
165 RRCopen session failure
166 Lowlevelfailure
167 Lowlevelfailure no redialallowed
168 InvalidSIM
169 Noservice
170 TimerT3230 expired
171 Nocell available
172 Wrong state
173 Access classblocked
174 Abort messagereceived
175 Othercause
176 TimerT303 expired
177 Noresources
178 Releasepending
179 Invaliduserdata
PSinternal causelookup
0 Invalidconnection identifier
1 InvalidNSAPI
2 InvalidPrimary NSAPI
3 Invalidfield
4 SNDCPfailure
5 RAB setup failure
6 NoGPRS context
7 PDPestablishtimeout
8 PDPactivate timeout
9 PDPmodifytimeout
10 PDPinactive maxtimeout
11 PDPlowerlayer error
12 PDPduplicate
13 Access technologychange
14 PDPunknown reason
PSnetworkcause
25 LLCorSNDCPfailure
26 Insufficientresources
27 Missing orunknownAPN


## Página 589

A76XX Series_AT Command Manual_V1.09
www.simcom.com 588 /652
28 Unknown PDPaddressor PDPtype
29 UserAauthenticationfailed
30 Activation rejected byGGSN
31 Activation rejected,unspecified
32 Serviceoption notsupported
33 Requested serviceoption notsubscribed
34 Serviceoption temporarilyoutoforder
35 NSAPIalreadyused(notsent)
36 Regulardeactivation
37 QoS notaccepted
38 Networkfailure
39 Reactivation required
40 Feature notsupported
41 Semantic errorin theTFToperation
42 Syntacticalerror intheTFToperation
43 Unknown PDPcontext
44 PDPcontextwithoutTFTalready activated
45 Semantic errors in packetfilter
46 Syntacticalerrors in packetfilter
81 Invalidtransaction identifier
95 Semanticallyincorrectmessage
96 Invalidmandatory information
97 Message non-existent/not implemented
98 Message type notcompatiblewith state
99 IEnon-existent/notimplemented
100 ConditionalIEerror
101 Message notcompatible with state
111 Protocolerror,unspecified
28.3 SummaryofCMEERRORCodes
This resultcode issimilar tothe regularERRORresultcode.Theformatof <err>canbe eithernumeric or
verbose string,by settingAT+CMEE command.
DefinedValues
Codeof<err> Meaning


## Página 590

A76XX Series_AT Command Manual_V1.09
www.simcom.com 589 /652
0 phone failure
1 no connection tophone
2 phone adaptorlink reserved
3 operation notallowed
4 operation notsupported
5 PH-SIM PINrequired
6 PH-FSIM PIN required
7 PH-FSIM PUKrequired
10 SIM notinserted
11 SIM PINrequired
12 SIM PUKrequired
13 SIM failure
14 SIM busy
15 SIM wrong
16 incorrectpassword
17 SIM PIN2 required
18 SIM PUK2 required
20 memoryfull
21 invalid index
22 notfound
23 memoryfailure
24 textstring too long
25 invalid charactersin textstring
26 dial stringtoo long
27 invalid charactersin dialstring
30 no network service
31 network timeout
32 network notallowed -emergencycalls only
40 network personalization PINrequired
41 network personalization PUKrequired
42 network subsetpersonalization PINrequired
43 network subsetpersonalization PUKrequired
44 service providerpersonalization PINrequired
45 service providerpersonalization PUKrequired
46 corporate personalization PINrequired
47 corporate personalization PUKrequired
50 Incorrectparameters
100 unknown error
103 IllegalMESSAGE
106 IllegalME
107 GPRS services not allowed


## Página 591

A76XX Series_AT Command Manual_V1.09
www.simcom.com 590 /652
111 PLMN not allowed
112 Locationarea not allowed
113 Roaming not allowed inthis locationarea
132 service option notsupported
133 requested serviceoption notsubscribed
134 service option temporarilyout oforder
148 unspecifiedGPRS error
149 PDPauthenticationfailure
150 invalid mobile class
151 ATcommand timeout
Examples
AT+CPIN="1234","1234"
+CMEERROR:SIM failure
28.4 SummaryofCMSERRORCodes
Finalresultcode +CMSERROR: <err>indicates an errorrelated tomobile equipmentornetwork.The
operationis similartoERROR resultcode.None ofthe followingcommands inthe samecommandlineis
executed.NeitherERRORnorOKresultcodeshallbereturned.ERRORisreturnednormally whenerroris
related tosyntax orinvalidparameters.The formatof<err>canbe eithernumericor verbose.Thisis set
with commandAT+CMEE.
DefinedValues
Codeof<err> Meaning
300 ME failure
301 SMS serviceof MEreserved
302 Operation notallowed
303 Operation notsupported
304 Invalid PDUmode parameter
305 Invalid textmodeparameter
310 SIM notinserted
311 SIM PINrequired
312 PH-SIM PINrequired
313 SIM failure


## Página 592

A76XX Series_AT Command Manual_V1.09
www.simcom.com 591 /652
314 SIM busy
315 SIM wrong
316 SIM PUKrequired
317 SIM PIN2 required
318 SIM PUK2 required
320 Memoryfailure
321 Invalid memoryindex
322 Memoryfull
330 SMSC addressunknown
331 no network service
332 Networktimeout
340 no +CNMAacknowledgement expected
341 Bufferoverflow
342 SMS size more than expected
500 unknown error
Examples
AT+CMGS=02112345678
+CMSERROR:304


## Página 593

A76XX Series_AT Command Manual_V1.09
www.simcom.com 592 /652
ATCommandsforWEBSOCKET
29.1 OverviewofATCommandsforwebsocket
Command Description
AT+WSSTART StartMQTTservice
AT+WSSTOP StopMQTTservice
AT+WSCONNECT Acquireaclient
AT+WSDISC Releaseaclient
AT+WSSEND Setthe SSLcontext(only forSSL/TLSMQTT)
29.2 DetailedDescriptionofATCommandsforwebsocket(S)
Webcosket,Currently only 1603,1606 and 1803 issupported.
29.2.1 AT+WSSTART Startwebsocketservice
AT+WSSTARTis usedto startwebsocketservice byactivating PDPcontext.Youmustexecute this
command beforeany otherwebsocketrelated operations.
Execute Command
AT+WSSTART
Response
1)Ifstartwebsocketservicesuccessfully:
OK
+WSSTART:0
2)Iffailed:
OK
+WSSTART:<err>
3)Ifwebsocket servicehave started successfully andyou executed
AT+WSSTARTagain:
ERROR
Max ResponseTime 12000ms
ParameterSaving Mode -


## Página 594

A76XX Series_AT Command Manual_V1.09
www.simcom.com 593 /652
Reference
DefinedValues
<err> Theresult code,pleaserefertoChapter29.3
Examples
AT+WSSTART
OK
+WSSTART:0
29.2.2 AT+WSSTOP Stopwebsocketservice
AT+WSSTOPisusedto stopwebsocketservice.
AT+CWSSTOP Stopwebsocketservice
Execute Command
AT+WSSTOP
Response
1)Ifstop websocketservice successfully:
OK
+WSSTOP:0
2)Iffailed:
+WSSTOP:<err>
ERROR
3)IfMQTTservicehave stopped successfully and you executed
AT+CMQTTSTOPagain:
ERROR
Max ResponseTime 12000ms
ParameterSaving Mode -
Reference
DefinedValues
<err> Theresult code,pleaserefertochapter 29.3


## Página 595

A76XX Series_AT Command Manual_V1.09
www.simcom.com 594 /652
Examples
AT+WSSTOP
OK
+WSSTOP:0
29.2.3 AT+WSCONNECT Connecttowebsocketserver
AT+WSCONNECTisusedto connectto awebsocketserver.
AT+WSCONNECT ConnecttoWebsocketserver
TestCommand
AT+WSCONNECT=?
Response
+WSCONNECT:(9-256),(1-64800)
OK
Read Command
AT+WSCONNECT?
Response
1)ifconnected:
+WSCONNECT:
<connect_status>[,<server_addr>,<server_port>,<server_path>
]
OK
2)ifnotconnected:
+WSCONNECT:<connect_status>
OK
WriteCommand
AT+WSCONNECT=<server_a
ddr>,[time_out]
Response
1)Ifsuccessfully:
OK
+WSCONNECT:0
2)Iffailed:
OK
+WSCONNECT:<err>
3)Iffailed:
+WSCONNECT:<err>
ERROR
4)Iffailed:


## Página 596

A76XX Series_AT Command Manual_V1.09
www.simcom.com 595 /652
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<connect_status>
Thisparameterhas thefollowing two values:
0 the currentdevice isnotconnected.
1 the currentdevice isconnected.
<server_addr>
Thestringthatdescribed the serveraddress andport.Therangeof
the string lengthis 9to256bytes.Thestringshouldbe likethis
"ws://116.247.119.165:5141/test",mustbegin with "ws://". Ifthe
<server_addr>notinclude theport, the defaultportis80.Ifthe
<server_addr>notinclude thepath,the defaultpath is /.
<server_port> Thewebsocketconnetport,the defaultport is80.
<server_path> The websocketconnetpath,the defaultpathis /.
<time_out> Thetimeoutvalueforconnect.Theunitissecond.Therangeis60sto
180s.Thedefaultvalue is120s(notset the timeoutvalue).
<err> Theresultcode:0issuccess.Othervaluesarefailure.Pleasereferto
chapter29.3.
Examples
AT+WSCONNECT="ws://121.40.165.18:8800",120
OK
+WSCONNECT:0
AT+WSCONNECT?
+WEBSOCKETCONNECT:
1,"121.40.165.18",8800,"/"
OK
29.2.4 AT+WSDISC Disconnectfromserver
AT+WSDISCisusedtodisconnectfrom theserver.
AT+WSDISC Disconnectfromserver


## Página 597

A76XX Series_AT Command Manual_V1.09
www.simcom.com 596 /652
TestCommand
AT+WSDISC=?
Response:
+WSDISC:(0,60-180)
OK
WriteCommand
AT+WSDISC=<timeout>
Response
1)Ifdisconnectsuccessfully:
+WSDISC:0
OK
2)Ifdisconnectsuccessfully:
OK
+WSDISC:0
3)Iffailed:
OK
+WSDISC:<err>
4)Iffailed:
ERROR
5)Iffailed:
+WSDISC:<err>
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference
DefinedValues
<timeout> Thetimeout valuefordisconnection.Theunitis second.Therangeis
60sto 180s.Thedefaultvalueis 0s (notsetthe timeoutvalue).
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter29.3.
Examples
AT+CMQTTDISC=120
OK
+WSDISC:0


## Página 598

A76XX Series_AT Command Manual_V1.09
www.simcom.com 597 /652
29.2.5 AT+WSSEND Publishamessagetoserver
AT+WSSENDispublish amessageto server.
AT+WSSEND publishamessagetoserver
TestCommand
AT+WSSEND=?
Response
+WSSEND:(1-1024),(0-1)
OK
AT+WSSEND=<dataLength>,
<datatype>
Response
1)Ifsuccessfully:
>
<inputdatahere>
OK
+WSSEND:0,<dataLength>
2)Iffailed:
OK
+WSSEND:<err>
3)Iffailed:
+WSSEND:<err>
ERROR
4)Iffailed:
ERROR
ParameterSaving Mode -
Max ResponseTime -
Reference -
DefinedValues
<dataLength> Thelength ofinputtopicdata.Therangeis from1to1024 bytes.
<datatype> Thepublish message’stype.Therangeis from0to1.
0 text message
1 binary message
<err> Theresult code:0is success.Othervalues arefailure.Pleasereferto
chapter29.3.
Examples
AT+WSSEND=0,9,1


## Página 599

A76XX Series_AT Command Manual_V1.09
www.simcom.com 598 /652
>
OK
+WSSEND:0,10
29.3 CommandResultCodes
29.3.1 Descriptionof<err>
<err> Description
0 operation succeeded
1 failed
2 Sendhandshakefail
3 parsehandshakefail
4 Readwritesocketfail
5 Connecttohostfail
6 invalidparameter
7 Network have opend
8 Network noopen
29.4 UnsolicitedResultCodes
URC Description
+WSDISC:<cause>
Whenclient disconnectpassively,
URC"+WSDISC"willbe reported,
then userneed toconnectMQTT
serveragain.
+WSRECEIVE:<datalen>
<data> Whileclient receivemessage,URC
"+WSRECEIVE"will bereported.


## Página 600

A76XX Series_AT Command Manual_V1.09
www.simcom.com 599 /652
ATCommandsforLWM2M
30.1 OverviewofATCommandsforLWM2M
Command Description
AT+LWSTART StartLWM2M service
AT+LWSTOP Stop LWM2M Service
AT+LWCNF Config theLWM2M
AT+LWOPEN Registerto aLWM2M server
AT+LWCLOSE DeregistertoLWM2M server
AT+LWADDOBJ AddaLWM2M object
AT+LWDELOBJ Delete aLWM2M object
AT+LWREADRSP Send readresponse toLWM2Mserver
AT+LWWRITERSP Send write responseto LWM2M server
AT+LWEXECUTERSP Send execute responsetoLWM2M server
30.2 DetailedDescriptionofATCommandsforLWM2M
30.2.1 AT+LWSTART StartLWM2Mservice
AT+LWSTARTisusedto startLWM2Mservice by activating PDPcontext.YoumustexecuteAT+LWSTART
beforeany otherLWM2M related operations exceptAT+LWCNF.
AT+LWSTART StartLWM2Mservice
TestCommand
AT+LWSTART=?
Response
OK
Execution Command
AT+LWSTART
Response
1)
OK


## Página 601

A76XX Series_AT Command Manual_V1.09
www.simcom.com 600 /652
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
Examples
AT+LWSTART
OK
30.2.2 AT+LWSTOP StopLWM2MService
AT+LWSTOPisusedtostop LWM2M serviceby deactivatingPDPcontextWhenyou are no longerusing
the LWM2Mservice,use thiscommand.
AT+LWSTOP StopLWM2MService
TestCommand
AT+LWSTOP=?
Response
OK
Execution Command
AT+LWSTOP
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
Examples
AT+LWSTOP
OK


## Página 602

A76XX Series_AT Command Manual_V1.09
www.simcom.com 601 /652
30.2.3 AT+LWCNFConfigtheLWM2M
AT+LWCNFis used toconfigthe LWM2M.
AT+LWCNF ConfigtheLWM2M
TestCommand
AT+LWCNF=?
Response
+LWCNF:"server",<ipaddress>
+LWCNF:"serverport",<serverport>
+LWCNF:"endpointname",<endpointname>
+LWCNF:"connecttype",(4,6)
+LWCNF:"lifetime",<lifetime>
+LWCNF:"localport",<localport>
OK
WriteCommand
AT+LWCNF=<server>,<ipaddress>
AT+LWCNF=<serverport>,<serverport>
AT+LWCNF=<endpointname>,<endpointname>
AT+LWCNF=<connecttype>,4 or6
AT+LWCNF=<lifetime>,<lifetime>
AT+LWCNF=<localport>,<localport>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<server> TheLWM2M serveraddress URLoripaddress.
<serverport> TheLWM2M serverport,therangeis from0to 65535.
<enpointname> TheLWM2M client devicename.
<connecttype> Thetype ofLWM2M serveraddress IPV4 orIPV6.
<lifetime> Theconnection lifetime.the maxvalue is65535.
<localport> TheLWM2M client devicelocalport.Therange is from 0to65535.
Examples


## Página 603

A76XX Series_AT Command Manual_V1.09
www.simcom.com 602 /652
AT+LWCNF="server","leshan.eclipseprojects.io"
OK
AT+LWCNF="serverport","5683"
OK
AT+LWCNF="endpointname","simcom"
OK
AT+LWCNF="connectiontype","4"
OK
AT+LWCNF="lifetime","800"
OK
AT+LWCNF="localport","56833"
OK
30.2.4 AT+LWOPEN RegistertoaLWM2Mserver
AT+LWOPENisusedtoregister toaLWM2M sever,makesure you registerto aLWM2M severbefore you
executeAT+LWCLOSEcommand.
AT+LWOPEN RegistertoaLWM2Mserver
TestCommand
AT+LWOPEN=?
Response
OK
Execute Command
AT+LWOPEN
Response
1)
OK
+LMOPEN:<lwm2mId>
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference


## Página 604

A76XX Series_AT Command Manual_V1.09
www.simcom.com 603 /652
DefinedValues
<lwm2mId> TheLWM2M session ID.the range isfrom 0to 1.
Examples
AT+LWOPEN=?
OK
AT+LWOPEN
OK
LMOPEN:0
30.2.5 AT+LWCLOSE DeregistertoaLWM2Mserver
This commandis used toDeregistertoaLWM2M server.
AT+LWCLOSE DeregistertoaLWM2Mserver
TestCommand
AT+LWCLOSE=?
Response
OK
WriteCommand
AT+LWCLOSE=<lwm2mId>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> AT+LWOPENreturn the LWM2M session ID.the range isfrom 0to1.
Examples
AT+LWCLOSE=0
OK


## Página 605

A76XX Series_AT Command Manual_V1.09
www.simcom.com 604 /652
30.2.6 AT+LWADDOBJ AddaLWM2Mobject
AT+LWADDOBJis used toadd aLWM2M object.
AT+LWADDOBJ AddaLWM2Mobject
TestCommand
AT+LWADDOBJ=?
Response
OK
WriteCommand
AT+LWADDOBJ=<lwm2mId>,<o
bjectId>,<instanceId>,<resourc
eCnt>,<resourceId>,<resouceId
>….
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> AT+LWOPENreturn the LWM2M session ID.the range isfrom 0to1.
<objectId> TheLWM2M objectIDyou wanttoadd.the range isfrom0to 65535,
But0-7 hasalreadyused.
<instanceId> TheLWM2M objectinstanceID.Therange isfrom0to 65535.
<resourceCnt> TheLWM2M resourcecount.Therange isfrom 1to15.
<resourceId> TheLWM2M resourceID.Therangeis from0to65535.
Examples
AT+LWADDOBJ=?
OK
AT+LWADDOBJ=0,3303,0,6,5518,5601,5602,5603,5604,5605
OK


## Página 606

A76XX Series_AT Command Manual_V1.09
www.simcom.com 605 /652
30.2.7 AT+LWDELOBJ DeleteaLWM2Mobject
AT+LWDELOBJ is usedto deleteaLWM2M object.
AT+LWDELOBJ DeleteaLWM2Mobject
TestCommand
AT+LWDELOBJ=?
Response
OK
WriteCommand
AT+LWDELOBJ=<lwm2mId>,<o
bjectId>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> AT+LWOPENreturn the LWM2M session ID.the range isfrom 0to1.
<objectId> TheLWM2M objectIDyou wanttodelete.the range is from 0to
65535.
Examples
AT+LWDELOBJ=?
OK
AT+LWDELOBJ=0,3303
OK
30.2.8 AT+LWREADRSP SendreadresponsetoLWM2Mserver
Youcanuse thiscommand tosendreadresponse toLWM2M server.
AT+LWREADRSP SendreadresponsetoLWM2Mserver
TestCommand
AT+LWREADRSP=?
Response
OK


## Página 607

A76XX Series_AT Command Manual_V1.09
www.simcom.com 606 /652
WriteCommand
AT+LWREADRSP=<lwm2mId>,<
objectId>,<instanceId>,<resour
ceCnt>,<resourceId>,<valuetyp
e>,<valuelen>,<value>,<
resourceId >,<valuetype>…
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> TheLWM2M session ID,accordingto “+LWREAD”returned.
<objectId> TheLWM2M objectID,accordingto “+LWREAD”returned.
<instanceId> TheLWM2M objectinstanceID,according to“+LWREAD”returned.
<resourceCnt> TheLWM2M resourcecount,accordingto “+LWREAD”returned.
<resourceId> TheLWM2M resourceID,according to“+LWREAD”returned.
<valuetype> Thetype ofvalue ofreponse.
 IInteger
 FFloat
 BBoolean
 DUINT8arraydata
 SString
<valuelen> Thelength ofvalue.
<value> Thereponse value.
Examples
AT+LWREADRSP=?
OK
+LWREAD:0,3303,0,1,5602
AT+LWREADRSP=0,3303,0,1,5602,"F",5,"15623"
OK
Mustexecutethe this commandafter URC“+LWREAD”returned.“+LWREAD”see30.2.11
NOTE


## Página 608

A76XX Series_AT Command Manual_V1.09
www.simcom.com 607 /652
30.2.9 AT+LWWRITERSP SendresponsetoaLWM2Mserver
This commandis used tosendaresponseto LWM2Mserver.
AT+LWWRITERSP SendreponsetoaLWM2Mserver
TestCommand
AT+LWWRITERSP=?
Response
+LWWRITERSP:<lwm2mId>,<result>
OK
Execute Command
AT+LWWRITERSP=<lwm2mId>,
<result>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> TheLWM2M session ID,accordingto “+LWWRITE”returned.
<result> According to“+LWWRITE”,returnthe result, see30.3
Examples
+LMWRITE:0,3335,0,1,5750,S,1,"p"
AT+LWWRITERSP=0,0
OK
Mustexecutethe this commandafter URC“+LWWRITE”returned.“+LWWRITE”see30.2.12
NOTE


## Página 609

A76XX Series_AT Command Manual_V1.09
www.simcom.com 608 /652
30.2.10AT+LWEXECUTERSP SendresponsetoLWM2Mserver
YoucanuseAT+LWEXECUTERSPsendresponseto LWM2Mserver.
AT+LWEXECUTERSP SendresponsetoLWM2Mserver
TestCommand
AT+LWEXECUTERSP=?
Response
OK
WriteCommand
AT+LWEXECUTERSP=<lwm2mI
d>,<result>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> TheLWM2M session ID,accordingto “+LWEXECUTE”returned.
<result> According to“+LWEXECUTE”,returnthe result, see 30.3
Examples
AT+LWEXECUTERSP=?
OK
+LWEXECUTE:0,3303,0,5605,1,"0"
AT+LWEXECUTERSP=0,0
OK
Mustexecutethe this commandafter URC“+LWEXECUTE”returned.“+LWEXECUTE”see30.2.13
NOTE


## Página 610

A76XX Series_AT Command Manual_V1.09
www.simcom.com 609 /652
30.2.11+LWREAD LWM2MclientresponseofLWM2Mserveroperateread
LWM2M clientresponse ofLWM2M serveroperate read.
+LWREAD LWM2MclientresponseofLWM2Mserveroperateread
Response
+LWREAD:<lwm2mId>,<objectId>,<instanceId>,<resourceCnt>,
<resourceId>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> TheLWM2M session ID.the range isfrom 0to 1.
<objectId> TheLWM2M objectIDyou wanttoadd.the range isfrom8to 65535.
<instanceId> TheLWM2M objectinstanceID.Therange isfrom0to 65535.
<resourceCnt> TheLWM2M resourcecount.Therange isfrom 1to15.
<resourceId> TheLWM2M resourceID.Therangeis from0to65535.
30.2.12+LWWRITE LWM2MclientresponseofLWM2Mserveroperatewrite
LWM2M clientresponse ofLWM2M serveroperate write.
+LWWRITE LWM2MclientresponseofLWM2Mserveroperateread
Response
+LWWRITE:
<lwm2mId>,<objectId>,<instanceId>,<resourceCnt>,<resourceId>
,<valuetype>,<valuelen>,<value>,<resourceId>,<valuetype>…
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> TheLWM2M session ID.the range isfrom 0to 1.


## Página 611

A76XX Series_AT Command Manual_V1.09
www.simcom.com 610 /652
<objectId> TheLWM2M objectIDyou wanttoadd.the range isfrom8to 65535.
<instanceId> TheLWM2M objectinstanceID.Therange isfrom0to 65535.
<resourceCnt> TheLWM2M resourcecount.Therange isfrom 1to15.
<resourceId> TheLWM2M resourceID.Therangeis from0to65535.
<valuetype> Thetype ofvalue ofreponse.
 IInteger
 FFloat
 BBoolean
 DUINT8arraydata
SString
<valuelen> Thelength ofvalue.
<value> Thereponse value.
30.2.13+LWEXECUTE LWM2MclientresponseofLWM2Mserveroperateexecute
LWM2M clientresponse ofLWM2M serveroperate execute.
+LWEXECUTE LWM2MclientresponseofLWM2Mserveroperateexecute
Response
1)
+LWEXECUTE:<lwm2mId>,<objectId>,<instanceId>,
<resourceId>,<len>,<buffer>
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<lwm2mId> TheLWM2M session ID.the range isfrom 0to 1.
<objectId> TheLWM2M objectID.therange isfrom8to 65535.
<instanceId> TheLWM2M objectinstanceID.Therange isfrom0to 65535.
<resourceId> TheLWM2M resourcecount.Therange isfrom 1to15.
<len> Theresponse bufferlen.
<buffer> Theresponse buffer.


## Página 612

A76XX Series_AT Command Manual_V1.09
www.simcom.com 611 /652
30.3 CommandResultCodes
<result> Description
0 Noerror
1 Ignore
65 Created
66 Deleted
68 Changed
69 Content
95 Continue
128 Badrequest
129 Unauthorized
130 Badoption
132 Not found
133 Methodno allowed
134 Not acceptable
136 Req entityincomplete
140 Precondition failed
141 Entitytoo large
160 Internalserver error
161 Not implemented
163 Serviceunavailable


## Página 613

A76XX Series_AT Command Manual_V1.09
www.simcom.com 612 /652
ATCommandsforCOAP
31.1 OverviewofATCommandsforCOAP
Command Description
AT+COAPSTART ActivePDP
AT+COAPSTOP Deactive PDP
AT+COAPOPEN Open aCOAPserver
AT+COAPCLOSE CloseaCOAPserver
AT+COAPHEAD Config thehead ofCOAP
AT+COAPOPTION Config theoption ofCOAP
AT+COAPSEND Send COAPmessagetothe server
AT+COAPSENDTX Send COAPmessagetothe server bytransparenttransmission
31.2 DetailedDescriptionofATCommandsforCOAP
31.2.1 AT+COAPSTART ActivePDP
AT+COAPSTARTis usedto active PDPcontext.You mustexecuteAT+COAPSTARTbeforeany other
COAPrelated operations.
AT+COAPSTART ActivePDP
TestCommand
AT+COAPSTART=?
Response
OK
Execution Command
AT+COAPSTART
Response
1)
OK
2)
ERROR


## Página 614

A76XX Series_AT Command Manual_V1.09
www.simcom.com 613 /652
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
Examples
AT+COAPSTART
OK
31.2.2 AT+COAPSTOP DeactivePDP
AT+LWSTOPisusedtodeactive PDPcontext When you are nolongerusingthe COAPservice,usethis
command.
AT+COAPSTOP DeactivePDP
TestCommand
AT+COAPSTOP=?
Response
OK
Execution Command
AT+COAPSTOP
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
Examples
AT+COAPSTOP
OK


## Página 615

A76XX Series_AT Command Manual_V1.09
www.simcom.com 614 /652
31.2.3 AT+COAPOPEN OpenaCOAPserver
AT+COAPOPENisusedto openaCOAPsever,makesure you open aCOAPseverbeforeyou execute
AT+COAPCLOSEcommand.
AT+COAPOPEN OpenaCOAPserver
TestCommand
AT+COAPOPEN=?
Response
OK
Execute Command
AT+COAPOPEN=<server>,<ser
verport>
Response
1)
OK
+COAPOPEN:<coapId>
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<server> TheCOAPserveraddress URLoripaddress
<serverport> TheCOAPserverport,the range isfrom0 to65535.
<coap_sessionId> TheCOAPsession ID.the range isfrom 0to1.
Examples
AT+COAPOPEN=?
OK
AT+COAPOPEN="47.108.134.22",5683
OK
COAPOPEN:0


## Página 616

A76XX Series_AT Command Manual_V1.09
www.simcom.com 615 /652
31.2.4 AT+COAPCLOSE CloseaCOAPserver
This commandis used toDeregistertoaCOAPserver.
AT+COAPCLOSE CloseaCOAPserver
TestCommand
AT+COAPCLOSE=?
Response
OK
WriteCommand
AT+COAPCLOSE=<
coap_sessionId>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<coap_sessionId> AT+COAPOPENreturn the COAPsession ID.therangeis from0to1.
Examples
AT+COAPCLOSE=0
OK
31.2.5 AT+COAPHEAD ConfigtheheadofCOAP
AT+COAPHEAD isused toconfig thehead ofCOAP
AT+COAPHEAD ConfigtheheadofCOAP
TestCommand
AT+COAPHEAD=?
Response
OK
WriteCommand
AT+COAPHEAD=<coap_sessio
nId>,<msgId>,<tkl>,<token>
Response
1)
OK


## Página 617

A76XX Series_AT Command Manual_V1.09
www.simcom.com 616 /652
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<coap_sessionId> AT+COAPOPENreturn the COAPsession ID.therangeis from0to1.
<msgId> TheCOAPmessageID,the rangeis 0to65535.
<tkl> Thelength oftoken,the range is0to8.
<token> Thetoken ofCOAPmessage.
Examples
AT+COAPHEAD=?
OK
AT+COAPHEAD=0,35691,1,"1"
OK
31.2.6 AT+COAPOPTION ConfigtheoptionofCOAP
AT+COAPOPTIONis used toconfigthe optionof COAP.
AT+COAPOPTION ConfigtheoptionofCOAP
TestCommand
AT+COAPOPTION=?
Response
OK
WriteCommand
AT+COAPOPTION=<
coap_sessionId>,<
opt_count>,<optNum>,<optValu
e>,<opt_count>…..
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference


## Página 618

A76XX Series_AT Command Manual_V1.09
www.simcom.com 617 /652
DefinedValues
<coap_sessionId> AT+COAPOPENreturn the COAPsession ID.therangeis from0to1.
<opt_count> Thenum ofoption you wantto config.therangeis 0to10.
<optNum> Thetype ofoption.
<optValue> Thevalue ofthe option
Examples
AT+COAPOPTION=?
OK
AT+COAPOPTION=0,1,7,"5683"
OK
31.2.7 AT+COAPSEND SendCOAPmessagetotheserver
Youcanuse thiscommand toSend COAPmessagetothe server
AT+COAPSEND SendCOAPmessagetotheserver
TestCommand
AT+COAPSEND=?
Response
OK
WriteCommand
AT+COAPSEND=<
coap_sessionId>,<
type>,<method>,<data_len>,<d
ata>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<coap_sessionId> AT+COAPOPENreturn the COAPsession ID.therangeis from0to1.
<type> Themessage type of COAP,itcanset“con”,“non”,“ack”,“rst”.
<method> Themessage method ofCOAP,itcan set
“get”,“post”,“put”,“delete”,“fetch”,“patch”,“ipatch”.


## Página 619

A76XX Series_AT Command Manual_V1.09
www.simcom.com 618 /652
<data_len> Thedata length ofCOAPmessage.the rangeis 0to2000.
<data> Thedata ofCOAPmessage
Examples
AT+COAPSEND=?
OK
AT+COAPSEND=0,"con","get",5,"12345"
OK
31.2.8 AT+COAPSENDTX SendCOAPmessagetotheserverbytransparent
transmission
This commandis used toSend COAPmessage tothe server bytransparenttransmission.
AT+COAPSENDTX SendCOAPmessagetotheserverbytransparenttransmission
TestCommand
AT+COAPSENDTX=?
Response
+COAPSENDTX:<coap_sessionId>,<result>
OK
Execute Command
AT+COAPSENDTX=<
coap_sessionId>,<
type>,<method>,<data_len>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<coap_sessionId> AT+COAPOPENreturn the COAPsession ID.therangeis from0to1.
<result> According to“+COAPSENDTX”,returnthe result.
Examples
AT+COAPSENDTX=?
OK


## Página 620

A76XX Series_AT Command Manual_V1.09
www.simcom.com 619 /652
AT+COAPSENDTX=0,"con","get",5
>
01234
OK
31.2.9 +COAPRECVReceiveresponsemessagefromserver
Receive responsemessagefrom server
+COAPRECV Receiveresponsemessagefromserver
Response
1)
+COAPRECV:<requestorresponse>,from session
<coap_sessionId>,<receivedcode>,
<received tid>[,<len>,<data>]
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<request orresponse> Themessage type of COAPserverresponse.
<coap_sessionId> TheCOAPserverresponse which coap session
<received code> Thereceivedcode ofthe message
<received tid> Thereceivedtid ofthe message
<len> Theresponse datalen.
<data> Theresponse data.


## Página 621

A76XX Series_AT Command Manual_V1.09
www.simcom.com 620 /652
ATCommandsforSMTPS
32.1 OverviewofATCommandsforSMTPS
Command Description
AT+CSMTPSCFG Configurethe SMTPcontext
AT+CSMTPSSRV SetSMTPserver addressandportnumber
AT+CSMTPSAUTH SMTPserver authentication
AT+CSMTPSFROM Senderaddress and name
AT+CSMTPSRCPT Recipientaddressandname(TO/CC/BCC)
AT+CSMTPSSUB E-mailsubject
AT+CSMTPSBODY E-mailbody
AT+CSMTPSBCH E-mailbodycharacterset
AT+CSMTPSFILE Selectattachment
AT+CSMTPSSEND Initiatesessionandsende-mail
AT+CSMTPSSTOP Forcetostopsending e-mail
AT+CSMTPSCLEAN Clean mailcontent and setting
Currently,onlyASR1603 andASR1803SsupportSMTPS.
32.2 DetailedDescriptionofATCommandsforSMTPS
32.2.1 AT+CSMTPSCFG ConfigtheSMTPcontext
ThiscommandisusedtoselectSMTPsslcontextandpdpcontext.SMTPclientwillinitiatesessionwiththe
specified contextto sendan e-mail.
Execution commandwillsetthe sslcontext andpdpcontextas defaultvalue.
AT+CSMTPSCFG ConfigtheSMTPcontext
NOTE


## Página 622

A76XX Series_AT Command Manual_V1.09
www.simcom.com 621 /652
TestCommand
AT+CSMTPSCFG=?
Response
OK
WriteCommand
/*selectthe sslcontext*/
AT+CSMTPSCFG="sslCtxId
"[,<sslCtxId>]
Response
1)fthe “sslCtxId”is default:
+CSMTPSCFG:"sslCtxId",<sslCtxId>
OK
2) the“sslCtxId”is notdefault:
OK
3) error
ERROR
WriteCommand
/*selectthe pdp context*/
AT+CSMTPSCFG="pdpCtxI
d"[,<pdpCtxId>]
Response
1)the “pdpCtxId”isdefault:
+CSMTPSCFG:"pdpCtxId",<pdpCtxId>
OK
2) the“pdpCtxId”is notdefault:
OK
3) error
ERROR
Execution Command
AT+CSMTPSCFG
Response
OK
or
ERROR
ParameterSaving Mode -
Reference
DefinedValues
<sslCtxId> SMTPSSLcontextid.the defaultis0.
<pdpCtxId> SMTPPDPcontextid.the defaultis1.
Example
AT+CSMTPSCFG="sslCtxId",0
OK


## Página 623

A76XX Series_AT Command Manual_V1.09
www.simcom.com 622 /652
AT+CSMTPSCFG="sslCtxId"
+CSMTPSCFG:"sslCtxId",0
OK
32.2.2 AT+CSMTPSSRV SetSMTPserveraddressandportnumber
This command is used to set SMTPserver address and server’s port number.SMTPclient will initiateTCP
sessionwith the specified servertosendan e-mail.
Read commandreturns currentSMTPserver addressandportnumber.
Execution commandwillclearSMTPserveraddress andsetthe portnumberas defaultvalue.
AT+CSMTPSSRV SetSMTPserveraddressandportnumber
TestCommand
AT+CSMTPSSRV=?
Response
OK
Read Command
AT+CSMTPSSRV?
Response
+CSMTPSSRV:<server>,<port>,<server_type>
OK
WriteCommand
AT+CSMTPSSRV=<server>,
<port>[,<server_type>]
Response
OK
or
ERROR
Execution Command
AT+CSMTPSSRV
Response
OK
or
ERROR
ParameterSaving Mode -
Reference
DefinedValues
<server> SMTP server address, non empty string with double quotes,
mandatory andASCIItext stringup to127characters.
<port> Port number of SMTP server in decimal format, from 1 to 65535, and
defaultport is465forSMTP.
<server_type> Thetype ofserver:


## Página 624

A76XX Series_AT Command Manual_V1.09
www.simcom.com 623 /652
1 – SMTPserver.
2 – SMTPSserver withSSL3.0/TLS1.0/TLS1.1/TLS1.2 supported
3 – SMTPSserver withSTARTTLS
Example
AT+CSMTPSSRV="smtp.server.
com",425
OK
AT+CSMTPSSRV?
+CSMTPSSRV:
"smtp.server.com",425,2
OK
AT+SMTPSRV
OK
AT+SMTPSRV?
+SMTPSRV:"",465,2
OK
32.2.3 AT+CSMTPSAUTH SMTPserverauthentication
Thissynchronous commandis usedto controlSMTPauthentication duringconnectionwith SMTPserver.If
SMTPserver requiresauthentication while loggingin the server,TEmustset the authentication controlflag
andprovide usernameand password correctlybefore sending ane-mail.
Read command returns current SMTP server authentication control flag, if the flag is 0, both <user> and
<pwd> are emptystrings.
Execution Command clearsusernameandpassword.
AT+CSMTPSAUTH SMTPserverauthentication
TestCommand
AT+CSMTPSAUTH=?
Response
+CSMTPSAUTH: (listofsupported<flag>s)
OK
Read Command
AT+CSMTPSAUTH?
Response
+CSMTPSAUTH: <flag>,<user>, <pwd>
OK
WriteCommand
AT+CSMTPSAUTH=
<flag>[, <user>,<pwd>]
Response
OK
or
ERROR


## Página 625

A76XX Series_AT Command Manual_V1.09
www.simcom.com 624 /652
Execution Command
AT+CSMTPSAUTH
Response
OK
or
ERROR
DefinedValues
<flag> SMTPserverauthentication control flag,integertype.
0 – SMTPserver doesn’trequire authentication, factoryvalue.
1 – SMTPserver requiresauthentication.
<user> User name to be used forSMTPauthentication, non empty string with
doublequotes and upto127 characters.
<pwd> Password to be used for SMTP authentication, string with double
quotes and upto127 characters.
NOTE: If <flag> is 0, <user> and <pwd> must be omitted (i.e. only
<flag>ispresent).
Example
AT+CSMTPSAUTH?
+CSMTPSAUTH:0,"",""
OK
AT+CSMTPSAUTH=1,"username
","password"
OK
AT+CSMTPSAUTH?
+CSMTPSAUTH: 1, "username",
"password"
OK
AT+CSMTPSAUTH
OK
AT+CSMTPSAUTH?
+CSMTPSAUTH:0,"", ""
OK
32.2.4 AT+CSMTPSFROM Senderaddressandname
This synchronous command is used to set sender’s address and name, which are used to construct e-mail
header.Thesender’saddressmustbe correctifthe SMTPserver requires.
Read commandreturns currentsender’saddress andname.
Execution commandwillclearsender’saddressandname.


## Página 626

A76XX Series_AT Command Manual_V1.09
www.simcom.com 625 /652
AT+CSMTPSFROM Senderaddressandname
TestCommand
AT+CSMTPSFROM=?
Response
OK
Read Command
AT+CSMTPSFROM?
Response
+CSMTPSFROM:<saddr>,<sname>
OK
WriteCommand
AT+CSMTPSFROM=
<saddr>[,<sname>]
Response
OK
or
ERROR
Execution Command
AT+CSMTPSFROM
Response
OK
or
ERROR
DefinedValues
<saddr> E-mail sender address (MAIL FROM), non empty string with double
quotes, mandatory and ASCII text up to 127 characters. <saddr> will
be presentin the headerof the e-mail sentby SMTPclient in the field:
"From:".
<sname> E-mail sender name, string with double quotes, and alphanumeric
ASCII text up to63 characters. <sname> will be present in the header
of thee-mail sent by SMTPclient inthe field:"From:".
Example
AT+CSMTPSFROM="senderadd
ress@server.com","sendernam
e"
OK
AT+CSMTPSFROM?
+CSMTPSFROM:
"senderaddress@server.com","
sendername"
OK
AT+CSMTPSFROM
OK
AT+CSMTPSFROM?


## Página 627

A76XX Series_AT Command Manual_V1.09
www.simcom.com 626 /652
+CSMTPSFROM:"", ""
OK
32.2.5 AT+CSMTPSRCPT Recipientaddressandname(TO/CC/BCC)
This synchronous command is used to set recipient address/name and kind (TO/CC/BCC). If only the
parameter of “kind” is present, the command will clear all recipients of this kind, and if only parameters of
“kind”and “index”are present, thecommand willclearthe specifiedrecipient.
Read commandreturns currentrecipientaddress/nameandkind list.
Execution commandwillclearallrecipientinformation.
AT+CSMTPSRCPT Recipientaddressandname(TO/CC/BCC)
TestCommand
AT+CSMTPSRCPT=?
Response
+CSMTPSRCPT: (list of supported <kind>s), (list of supported
<index>s)
OK
Read Command
AT+CSMTPSRCPT?
Response
[+CSMTPSRCPT:<kind>,<index>,<raddr>, <rname>
[<CR><LF>…]]
OK
or
OK
or
ERROR
WriteCommand
AT+CSMTPSRCPT=<kind>[,
<index>[,<raddr>[,<rname>]
]]
Response
OK
or
ERROR
Execution Command
AT+CSMTPSRCPT
Response
OK
or
ERROR
DefinedValues
<kind> Recipient kind, the kinds of TO and CC are used to construct e-mail
headerin thefield: “To:”or“Cc:”.


## Página 628

A76XX Series_AT Command Manual_V1.09
www.simcom.com 627 /652
0 – TO,normalrecipient.
1 – CC,Carbon Copy recipient.
2 – BCC,Blind Carbon Copy recipient.
<index> Index ofthe kind ofrecipient,decimalformat, andfrom0to4.
<raddr> Recipientaddress,nonemptystringwithdoublequotes,andupto127
characters.
<rname> Recipient name, string type with double quotes, and up to 63
characters.
Example
AT+CSMTPSRCPT=0,0,"rcptadd
ress_to@server.com",
"rcptname_to"
OK
AT+CSMTPSRCPT?
+CSMTPSRCPT:
0,0,"rcptaddress_to@server.co
m","rcptname_to"
OK
AT+CSMTPSRCPT=1,0,"rcptadd
ress_cc@server.com","rcptnam
e_cc"
OK
AT+CSMTSPRCPT?
+CSMTPSRCPT:
0,0,"rcptaddress_to@server.co
m","rcptname_to"
+CSMTPSRCPT:
1,0,"rcptaddress_cc@server.co
m","rcptname_cc"
OK
32.2.6 AT+CSMTPSSUB E-mailsubject
This synchronouscommand isusedtoset thesubjectofe-mail,whichis used toconstructe-mail header.
Read commandreturns currente-mail subject.
Execution commandwillclearthe subject.
AT+CSMTPSSUB E-mailsubject
TestCommand
AT+CSMTPSSUB=?
Response
OK


## Página 629

A76XX Series_AT Command Manual_V1.09
www.simcom.com 628 /652
Read Command
AT+CSMTPSSUB?
Response
+SMTPSUB: <subject_len>,<subject_character><CR><LF>
[<subject>]
OK
WriteCommand
AT+CSMTPSSUB=<subject_
len>[,<subject_character>]
Response
>
OK
or
ERROR
Execution Command
AT+CSMTPSSUB
Response
OK
or
ERROR
DefinedValues
<subject> E-mail subject, string with double quotes, and ASCII text up to 511
characters. <subject> will be present in the header of the e-mail sent
by SMTPS client in the field: “Subject: ”. For write command, it can
inputany binarydata.
<subject_len> Thelength ofsubjectcontent
<subject_character> Thecharactersetof subject.Defaultis utf-8.
Example
AT+CSMTPSSUB?
+CSMTPSSUB:0,"UTF-8"
OK
AT+CSMTPSSUB=19,"utf-8"
>THISISATESTMAIL
OK
AT+SMTPSSUB?
+SMTPSSUB:19,"utf-8"
THIS ISATESTMAIL
OK
32.2.7 AT+CSMTPSBODY E-mailbody
This commandis used tosete-mail body,whichwillbe sent toSMTPserverwith textformat.


## Página 630

A76XX Series_AT Command Manual_V1.09
www.simcom.com 629 /652
Read command returns current e-mail body. If the process of sending an e-mail is ongoing, the command
willreturn “ERROR”directly.Executioncommand clears emailbody.
AT+CSMTPSBODY E-mailbody
TestCommand
AT+CSMTPSBODY=?
Response
OK
Read Command
AT+CSMTPSBODY?
Response
+CSMTPSBODY:<body_len><CR><LF>
[<body>]
OK
WriteCommand
AT+CSMTPSBODY=<body_l
en>
Response
>
OK
Execution Command
AT+CSMTPSBODY
Response
OK
DefinedValues
<body> E-mail body,up to5120 characters.
<body_len> Thelength ofemail body.
Example
AT+CSMTPSBODY=38
>THIS IS A TEST MAIL FROM
SIMCOM MODULE
OK
AT+CSMTPSBODY?
+CSMTPSBODY:38
THIS IS A TEST MAIL FROM
SIMCOM MODULE
OK
32.2.8 AT+CSMTPSBCH E-mailbodycharacterset
Thissynchronouscommandisusedtosetthebodycharactersetofe-mail.
Readcommandreturnscurrente-mailbodycharacterset.
AT+CSMTPSBCH E-mailbodycharacterset
TestCommand
AT+CSMTPSBCH=?
Response
OK


## Página 631

A76XX Series_AT Command Manual_V1.09
www.simcom.com 630 /652
Read Command
AT+CSMTPSBCH?
Response
+CSMTPSBCH:<charset>
OK
WriteCommand
AT+CSMTPSBCH=<charset
>
Response
OK
or
ERROR
Execute Command
AT+CSMTPSBCH
Response
OK
or
ERROR
DefinedValues
<charset> E-mail body character, string with double quotes. By default, it is
“utf-8”.The maximumlength is 19bytes.
Example
AT+CSMTPSBCH=?
OK
AT+CSMTPSBCH="gb2312"
OK
AT+CSMTPSBCH?
+CSMTPSBCH:"gb2312"
OK
32.2.9 AT+CSMTPSFILE Selectattachment
Thesynchronous commandis used toselectfileas e-mailattachment.
Read commandreturns currentallselectedattachments with fullpath.
AT+CSMTPSFILE Selectattachment
TestCommand
AT+CSMTPSFILE=?
Response
+CSMTPSFILE:(listofsupported<index>s)
OK
Read Command
AT+CSMTPSFILE?
Response
[+CSMTPSFILE:<index>, <filename>,<filesize>
[<CR><LF>…]]


## Página 632

A76XX Series_AT Command Manual_V1.09
www.simcom.com 631 /652
OK
WriteCommand
AT+CSMTPSFILE=<index>[,
<filename>]
Response
OK
or
[+CSMTPS:<err>]
ERROR
Execution Command
AT+CSMTPSFILE
Response
OK
DefinedValues
<index> Index for attachments, from 1 to 10. According to the sequence of
<index>,SMTPclient willencode andsendallattachments.
<filename> String type with double quotes, the name of a file which is under
currentdirectory (refer tofile system commands).SMTPclient doesn’t
allow two attachments with the same file name. For write command, if
the file name contains non-ASCII characters, this parameter should
contain aprefixof{non-ascii}.
<filesize> File size in decimal format. The total size of all attachments can’t
exceed 10MB.
<err> Theerrorinformation.
Example
AT+CSMTPSFILE=1,"E:/file1.txt"
OK
AT+CSMTPSFILE=1,{non-ascii}"
E6B58BE8AF95E99984E4BBB62
E6A7067"
OK
AT+CSMTPSFILE?
+CSMTPSFILE:1,"E:/file1.txt"
OK
AT+CSMTPSFILE=2,"U:/ file2.txt
"
OK
AT+CSMTPSFILE?
+CSMTPSFILE:1,"E:/file1.txt"
+CSMTPSFILE:2,"U:/file2.txt"
OK


## Página 633

A76XX Series_AT Command Manual_V1.09
www.simcom.com 632 /652
32.2.10AT+CSMTPSSEND Initiatesessionandsende-mail
This asynchronous command is used to initiate TCP/SSL session with SMTP server and send an e-mail
afterallmandatory parameters have been setcorrectly.
AT+CSMTPSSEND Initiatesessionandsende-mail
TestCommand
AT+CSMTPSSEND=?
Response
OK
Execution Command
AT+CSMTPSSEND
Response
OK
+CSMTPSSEND: <err>
or
ERROR
or
+CSMTPSSEND: <err>
ERROR
DefinedValues
<err> The error information. 0 indicates success. Other values indicate
failure.
Example
AT+CSMTPSSEND
OK
+CSMTPSSEND:0
32.2.11 AT+CSMTPSSTOP Forcetostopsendinge-mail
The synchronous command is used to force to stop sending e-mail and close the TCP/SSL session while
sending ane-mail isongoing. Otherwise,the commandwillreturn “ERROR”directly..
AT+CSMTPSSTOP Forcetostopsendinge-mail
TestCommand
AT+CSMTPSSTOP=?
Response
OK
Execution Command
AT+CSMTPSSTOP
Response
OK
or


## Página 634

A76XX Series_AT Command Manual_V1.09
www.simcom.com 633 /652
ERROR
Example
AT+CSMTPSSTOP
OK
32.2.12AT+CSMTPSCLEAN Cleanmailcontentandsetting
Thesynchronous commandis used toclean mailcontentand setting.
AT+CSMTPSCLEAN Cleanmailcontentandsetting
Execution Command
AT+CSMTPSCLEAN
Response
OK
or
ERROR
Example
AT+CSMTPSCLEAN
OK
32.3 SummaryofresultcodesforSMTPS
Codeof<errcode> Meaning
0 SMTPSoperation succeeded
600 Busy
601 Networkerror
602 Socketerror
603 Oversize
604 Duplicatefile
605 Timeout
606 Transferfailed
607 Memoryerror
608 Invalidparameter
609 EFSerror
610 SMTPservererror


## Página 635

A76XX Series_AT Command Manual_V1.09
www.simcom.com 634 /652
611 Authentication failure
612 Usercancel
655 Unknown error


## Página 636

A76XX Series_AT Command Manual_V1.09
www.simcom.com 635 /652
ATCommandsforTelecom
self-registration
33.1 OverviewofATCommandsforTelecomself-registration
Command Description
AT+HWVER Hardware versionnumberquery
AT+AUTOREGCFG Datadomain self-registrationstatus query
33.2 DetailedDescriptionofATCommandsforTelecomself-registration
33.2.1 AT+HWVER Hardwareversionnumberquery
AT+HWVER Hardwareversionnumberquery
TestCommand
AT+HWVER=?
Response
OK
Execution Command
AT+HWVER
Response
1)
+HWVER:<ver>
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues


## Página 637

A76XX Series_AT Command Manual_V1.09
www.simcom.com 636 /652
<ver> Hardware versionnumber
Examples
AT+HWVER=?
OK
AT+HWVER
+HWVER:V1.02
OK
33.2.2 AT+AUTOREGCFG Datadomainself-registrationstatusquery
AT+AUTOREGCFG Datadomainself-registrationstatusquery
TestCommand
AT+AUTOREGCFG=?
Response
+AUTOREGCFG:<info>
OK
Read Command
AT+AUTOREGCFG?
Response
1)
+AUTOREGCFG:<info>
OK
2)
ERROR
WriteCommand
AT+AUTOREGCFG=<domain>[,
<state>]
Response
+AUTOREGCFG:<info>
OK
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<info> Thedata domainself-registration status
<domain> “CTCC”
<state> “ENABLE”
“DISABLE”
“CLEANFLAG”


## Página 638

A76XX Series_AT Command Manual_V1.09
www.simcom.com 637 /652
Examples
AT+AUTOREGCFG=?
+AUTOREGCFG:the switchofautoreg isclosed
OK
AT+AUTOREGCFG?
+AUTOREGCFG:the switchofautoreg isclosed
OK
AT+AUTOREGCFG="CTCC","ENABLE"
OK
AT+AUTOREGCFG?
+AUTOREGCFG:the switchofautoreg isopened
OK
A7630series authenticationinstructions,no testingrequired.
NOTE


## Página 639

A76XX Series_AT Command Manual_V1.09
www.simcom.com 638 /652
ATCommandsforPSM
34.1 OverviewofATCommandsforPSM
Command Description
AT*COMCFG Enablecat1_1bis
AT+CPSMS PSM mode
AT+MEDCR Setpsmtimeandhw psm
34.2 DetailedDescriptionofATCommandsforpsm
34.2.1 AT*COMCFG Setcat1_1bis
AT*COMCFG Set cat1_1bis
AT*COMCFG Setcat1_1bis
TestCommand
AT*COMCFG=?
Response
*COMCFG:<mode>,<tokens>or
<UMTS_WB_AMR>,<GSM_WB_AMR>,<VENDOR>,<MAN
UFACTURE>,<LTE_CATEGORY>,<MULTISLOT_POWERPRO
FILE>,<LTE_SMS_ONLY>,<EC_SUPPORTED>,<EPCO_SUPP
ORTED>,<HCCP_SUPPORTED>,<MDRB_SUPPORTED>,<CP_
BACKOFF>,<NSLP>,<EAB>,<ROHC>
OK
Execution Command
AT*COMCFG
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference


## Página 640

A76XX Series_AT Command Manual_V1.09
www.simcom.com 639 /652
DefinedValues
<mode> configure MTinfo
0getMTconfiguration
1setMTconfiguration
<UMTS_WB_AMRvalue> 0or1indicate WBAMRis used in UMTS
<GSM _WB_AMRvalue> 0or1indicate WBAMRis used in GSM
<VENDORvalue> vendorinfo
0NONE
1ATT
2CMCC,
4IOT
8TELCEL
16 H3G
32 VDF
64 SILVER
128ORG
256TMOBILE
512VERIZON
1024 HP
<MANUFACTUREvalue> manufacture info
0NONE
1M_SILVER
<LTECATEGORYvalue> manufacture info
1、Cat1
2、Cat1_bis
Examples
AT*COMCFG=0,16
*COMCFG:,,,,1
OK
AT*COMCFG=1,,,,1
OK
Setlte category cat1
AT*COMCFG=1,,,,1 Setlte category cat1_bis
OK
34.2.2 AT+CPSMS Powersavingmodesetting
AT+CPSMS Powersaving modesetting


## Página 641

A76XX Series_AT Command Manual_V1.09
www.simcom.com 640 /652
AT+CPSMS Powersavingmodesetting
TestCommand
AT+CPSMS=?
Response
+CPSMS:(0-2)
OK
Read Command Response
AT+CPSMS?
+CPSMS:<mode>,[<Requested_Periodic-RAU>],[
<Requested_GPRS-READY-timer>],[<Requested_P
eriodic-TAU>],[<Requested_Active-Time>]
OK
Execution Command
AT+CPSMS=
<mode>[,<Requested_Pe
riodic-RAU>[,<Requested_GPR
SREADY-timer>[,<Requested_P
eri
odic-TAU>[,<Requested_Active
Time>]]]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<mode> integertype.Indication todisable orenablethe useof PSMin the UE
0Disablethe use ofPSM
1Enable the useof PSM
2Disablethe use ofPSM anddiscardallparameters forPSM or,if
available,resettothe manufacturerspecificdefaultvalues.
<Requested_Periodic-RAU> stringtype; one byte inan 8bitformat. Requestedextended
periodic RAUvalue(T3312)tobe allocated tothe UEin
GERAN/UTRAN.Therequested
extended periodicRAUvalue iscoded as one byte (octet3)of the
GPRSTimer3information
element codedas bitformat(e.g. "01000111"equals 70hours).For
the codingandthe valuerange,seetheGPRSTimer3IEin3GPPTS
24.008 [8]Table 10.5.163a/3GPPTS24.008.Seealso3GPPTS
23.682 [149]and3GPPTS23.060[47].Thedefaultvalue,ifavailable,
is Smanufacturerspecific.
<Requested_GPRS-READY-
timer>
stringtype; one byte inan 8bitformat. Requested
GPRS READYtimervalue (T3314)tobeallocated tothe UEin
GERAN/UTRAN.The
requested GPRSREADYtimervalue iscoded asone byte(octet 2)of


## Página 642

A76XX Series_AT Command Manual_V1.09
www.simcom.com 641 /652
the GPRSTimer
information elementcoded as bitformat(e.g. "01000011"equals3
decihours or18 minutes).
Forthe coding andthe value range,seethe GPRSTimerIEin3GPP
TS24.008 [8]
Table 10.5.172/3GPPTS24.008.Seealso3GPPTS23.060 [47].The
defaultvalue,ifavailable,is manufacturerspecific.
<Requested_Periodic-TAU> stringtype; one byte inan 8bitformat. Requestedextended
periodicTAUvalue (T3412)to beallocated tothe UEinE-UTRAN.
Therequested extended
periodicTAUvalueiscodedasonebyte(octet3)oftheGPRSTimer3
information element
coded as bitformat (e.g."01000111"equals70 hours).Forthe coding
andthe value range, see
the GPRSTimer3IEin 3GPPTS24.008 [8]Table10.5.163a/3GPP
TS24.008. See also
3GPPTS23.682 [149]and3GPPTS23.401[82].Thedefaultvalue,if
available,is
manufacturerspecific.
<Requested_Active-Time> stringtype; one byte inan 8bitformat. RequestedActiveTime
value (T3324)tobeallocated tothe UE.TherequestedActiveTime
value iscoded as onebyte
(octet3)oftheGPRSTimer2informationelementcoded asbitformat
(e.g. "00100100"equals
4minutes).Forthecodingandthevaluerange,seetheGPRSTimer2
IEin
3GPPTS24.008 [8]Table10.5.163/3GPPTS24.008.Seealso3GPP
TS23.682 [149],
3GPPTS23.060 [47]and3GPPTS23.401 [82].Thedefaultvalue,if
available,is manufacturer
specific
Examples
AT+CPSMS=1,,,"01101111","00001111"
OK Sett3412-exttime30s,t3324 time30s
34.2.3 AT+MEDCR Set/GetMEDATACOMMRESERVER
AT+MEDCR Set/GetMEDATACOMMRESERVER
TestCommand
AT+MEDCR=?
Response
+MEDCR


## Página 643

A76XX Series_AT Command Manual_V1.09
www.simcom.com 642 /652
OK
Read Command
AT+MEDCR?
Response
+MEDCR:<flag>,<position>,<value>
OK
Write Command
AT+
+MEDCR=<flag>,<position>[,<c
onfigVal>]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<flag> anumeric parameterwhichdeterminatesthe operation
0: setoperation
1: getoperation
<position> anumeric parameter,range (0-255)
8: optforspecificIOTwithout4Gtofasten registration
9: optforno detach/attachforAT*BANDinitated23GNWmode
change
10:optforlimitEF updateforStartCS/PS
11:optforremapping PScid(default5)since NWrejects 2ndsame
APN
12:optforusing invalid KSI(7)incasenotpower offnormally
13:optforsetting LTEinitialAPNcidmap fromebi/nsapi
30:optforcontrolcell change auto-reporting
46:LTE
47:UMTS
48:GSM
<configVal> anumeric parameter,the configure valueofeachopt, range (0-255)
Examples
AT+MDECR=0,71,2
OK Sett3412 time2min
AT+MEDCR=0,103,1
OK Enable HWpsm


## Página 644

A76XX Series_AT Command Manual_V1.09
www.simcom.com 643 /652
Currently,onlyASR1603_011_051 andASR1606_011_069 branchversions are supported.
NOTE


## Página 645

A76XX Series_AT Command Manual_V1.09
www.simcom.com 644 /652
ATCommandsforUSB
35.1 OverviewofATCommandsforUSB
Command Description
AT+DIALMODE Config USBNETnetwork
AT$MYCONFIG Config USBNETmode
AT+USBNETIP Config USBNETip
AT+USBNETMAC Config USBNETmac
35.2 DetailedDescriptionofATCommandsforusb
35.2.1 AT+DIALMODE ConfigUSBNETnetwork
AT+DIALMODE ConfigUSBNETnetwork
AT+DIALMODE ConfigUSBNETnetwork
TestCommand
AT+DIALMODE=?
Response
+DIALMODE:(0-1)
OK
Read Command
AT+DIALMODE?
Response
+DIALMODE:<modes>
OK
Execution Command
AT+DIALMODE=<mode>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference


## Página 646

A76XX Series_AT Command Manual_V1.09
www.simcom.com 645 /652
DefinedValues
<mode> TheAuto Dial status Enable/Disable,the defaultvalueis 1.
0–Enable USBNETnetwork
1–Disable USBNETnetwork
Thefunction willtake effectimmediately.
Examples
AT+DIALMODE=1
OK DisableUSBNETnetwork
35.2.2 AT$MYCONFIG SetRNDIS/ECMMode
AT$MYCONFIG SetRNDIS/ECM Mode
AT$MYCONFIG SetRNDIS/ECMMode
TestCommand
AT$MYCONFIG=?
Response
$MYCONFIG:\"usbnetmode\",(0,2),(0,1),<macName>
OK
Read Command
AT$MYCONFIG?
Response
$MYCONFIG:\"usbnetmode\",<netmode>,<netport>
OK
Execution Command
AT$MYCONFIG="USBNETMOD
E"[,<netmode>[,<netport>][,<m
acName>]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<netmode> TheRNDIS/ECMmode,the defaultvalueis 0.
0–RNDIS
1–ECM
2–AUTOadapt system
Thefunction willresetmodemthen take effect.
<netport> Thenameof Netcard,thedefault valueis1
0–ETH
1–USB


## Página 647

A76XX Series_AT Command Manual_V1.09
www.simcom.com 646 /652
<macName> Setusbnetnameundermac pc
Examples
AT$MYCONFIG="USBNETMODE",1
OK Changeto ECM
35.2.3 AT+USBNETIPChangeRNDIS/ECMPrivateIPtoPublicIP
AT+USBNETIP Change RNDIS/ECM Private IPtoPublic IP
AT+USBNETIP ChangeRNDIS/ECMPrivateIPtoPublicIP
TestCommand
AT+USBNETIP=?
Response
+USBNETIP:(0-1)[,(0-255)[,(0-255)[,(1-254)]]]
OK
Read Command
AT+USBNETIP?
Response
+USBNETIP:mode
OK
Execution Command
AT+USBNETIP=mode[,tpos[,dh
cps[,dhcpe]]]
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<mode > 0–Private Ip(default, 192.168.0.xxx etc.)
1–Public Ip(theip fromthe network)The functionwillreset
modem then takeeffect.
<tpos> Thethird position ofGatewayAddress
<dhcps> Dhcp startvalue
<dhcpe> Dhcp end value
Examples


## Página 648

A76XX Series_AT Command Manual_V1.09
www.simcom.com 647 /652
AT+USBNETIP=0,10,117
OK Changeusbnetip
35.2.4 AT+USBNETMAC SetUSBNETMACAddress
AT+USBNETMAC SetUSBNETMACAddress
AT+USBNETMAC SetUSBNETMACAddress
TestCommand
AT+USBNETMAC=?
Response
1)
OK
2)
ERROR
Read Command
AT+USBNETMAC?
Response
1)
+USBNETMAC:<mac_display>
OK
2)
ERROR
3)
+CMEERROR: <err>
Execution Command
AT+USBNETMAC=<mac>
Response
1)
OK
2)
ERROR
ParameterSaving Mode AUTO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<mac > String type, the MACaddress ofUSBNET,maximum
length 12
<mac_display > String type,displaythe MACaddressof USBNET,split with
a'-'.
Examples
AT+USBNETMAC=?
OK


## Página 649

A76XX Series_AT Command Manual_V1.09
www.simcom.com 648 /652
AT+USBNETMAC?
+USBNETMAC: F0-0C-29-A3-9B-6D
OK
AT+USBNETMAC=AABBCCDDEEFF
OK


## Página 650

A76XX Series_AT Command Manual_V1.09
www.simcom.com 649 /652
ATCommandsforJammingDetect
36.1 OverviewofATCommandsforJammingDetect
Command Description
AT+SJDR Enable JammingDetect
AT+SJDCFG Config Jamming Detect
36.2 DetailedDescriptionofATCommandsforJammingDetect
36.2.1 AT+SJDR EnableJammingDetect
AT+SJDR Enable Jamming Detect
AT+SJDR EnableJammingDetect
TestCommand
AT+SJDR=?
Response
+SJDR:(0,1)
OK
Read Command
AT+SJDR?
Response
+SJDR:<modes>
OK
Execution Command
AT+SJDR=<mode>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues


## Página 651

A76XX Series_AT Command Manual_V1.09
www.simcom.com 650 /652
<mode> to EnableJammingdetect,default close.
0–ColseJammingDetect
1–Enable JammingDetect
Thefunction willtake effectimmediately.
Examples
AT+SJDR=1
OK Enable Jamming Detect
36.2.2 AT+SJDCFG ConfigJammingDetect
AT+SJDCFG ConfigJamming Detect
AT+SJDCFG ConfigJammingDetect
TestCommand
AT+SJDCFG=?
Response
+SJDCFG:"period",(0-120)
+SJDCFG:"mnl",(0-31)
+SJDCFG:"minch",(0-254)
+SJDCFG:"detecstat",(0-1)
OK
Read Command
AT+SJDCFG?
Response
+SJDCFG:"period",<value>
+SJDCFG:"mnl",<value>
+SJDCFG:"minch",<value>
+SJDCFG:"detecstat",<value>
OK
Execution Command
AT+SJDCFG=<type>,<value>
Response
1)
OK
2)
ERROR
ParameterSaving Mode NO_SAVE
Max ResponseTime 9000ms
Reference
DefinedValues
<type> “period”PeriodofURC ofautojamming detection report.Whensetto
‘0’,no periodic reporting.Defaultvalue:0.Range: 0-120,unit:s
“mnl”Theminrxlev threshold (ForGSM networkonly). Defaultvalue:


## Página 652

A76XX Series_AT Command Manual_V1.09
www.simcom.com 651 /652
17.Range:0-31
“minch”Theminimum channel numberorARFCNnumberwhich is
jammed.Default value: 5.Range:0-254
"detecstat"Enableordisable to report thejamming detection URC
when the jamming ischanged.Default value is1
<value> <value>
Examples
AT+SJDCFG="period",5
OK SetJammingDetect reportevery 5seconds