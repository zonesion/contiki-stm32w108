/*
 * Copyright (c) 2010, STMicroelectronics.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the Contiki OS
 *
 * $Id: contiki-conf.h,v 1.2 2010/10/27 14:05:24 salvopitru Exp $
 */
/*---------------------------------------------------------------------------*/
/**
* \file
*     contiki-conf.h for MBXXX.
* \author
*     Salvatore Pitrulli <salvopitru@users.sourceforge.net>
*     Chi-Anh La <la@imag.fr>
*     Simon Duquennoy <simonduq@sics.se>
*/
/*---------------------------------------------------------------------------*/

#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#ifdef PLATFORM_CONF_H
#include PLATFORM_CONF_H
#else
#include "platform-conf.h"
#endif /* PLATFORM_CONF_H */


/* Include Project Specific conf */
#ifdef PROJECT_CONF_H
#include "project-conf.h"
#endif /* PROJECT_CONF_H */

/* Radio and 802.15.4 params */
/* 802.15.4 radio channel */
#define RF_CHANNEL                              16
/* 802.15.4 PAN ID */
#define IEEE802154_CONF_PANID                   0x1234

#define ST_CONF_RADIO_AUTOACK                   1
/* Number of buffers for incoming frames */
#define RADIO_RXBUFS                            2

////////////////////////////////////////////////////
/* RDC params */
/* TX routine passes the cca/ack result in the return parameter */
#define RDC_CONF_HARDWARE_ACK                   1
/* TX routine does automatic cca and optional backoff */
#define RDC_CONF_HARDWARE_CSMA                  0
/* RDC debug with LED */
#define RDC_CONF_DEBUG_LED                      0 //1
/* Channel check rate (per second) */
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE    8
/* Use ACK for optimization (LPP, XMAC) */
#define WITH_ACK_OPTIMIZATION                   0


/* ContikiMAC config */
#define CONTIKIMAC_CONF_COMPOWER                1
#define CONTIKIMAC_CONF_BROADCAST_RATE_LIMIT    0
#define CONTIKIMAC_CONF_ANNOUNCEMENTS           0

/* CXMAC config */
#define CXMAC_CONF_ANNOUNCEMENTS                0
#define CXMAC_CONF_COMPOWER                     1

/* XMAC config */
#define XMAC_CONF_ANNOUNCEMENTS                 0
#define XMAC_CONF_COMPOWER                      1

/* Other (RAM saving) */
#define ENERGEST_CONF_ON                        0
#define QUEUEBUF_CONF_NUM                       2
#define QUEUEBUF_CONF_REF_NUM                   0


#if UIP_CONF_IPV6
#ifndef NETSTACK_CONF_NETWORK
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#endif
#else
#define NETSTACK_CONF_NETWORK rime_driver
#endif

#ifndef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     csma_driver
#endif

#ifndef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nullrdc_driver
#define NULLRDC_802154_AUTOACK 	 0 	
#define NULLRDC_802154_AUTOACK_HW 1
#endif

#ifndef NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8
#endif
#ifndef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154
#endif
#define NETSTACK_CONF_RADIO   stm32w_radio_driver 
/* Addresses, Sizes and Interfaces */
/* 8-byte addresses here, 2 otherwise */
#define RIMEADDR_CONF_SIZE                   8
#define UIP_CONF_LL_802154                   1
/* Set to 0 for non ethernet links */
#define UIP_CONF_LLH_LEN                     0


#if UIP_CONF_IPV6

#define UIP_CONF_NETIF_MAX_ADDRESSES         3

/* TCP, UDP, ICMP */
#ifndef UIP_CONF_TCP
#define UIP_CONF_TCP                         1 
#endif
#ifndef UIP_CONF_UDP
#define UIP_CONF_UDP                         1 
#endif
//#define UIP_CONF_UDP_CONNS		     1 
//#define UIP_CONF_TCP_CONNS		     2
#define UIP_CONF_UDP_CHECKSUMS               1

/* ND and Routing */
#ifndef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER                      1
#endif

/* Prevent SDCC compile error when UIP_CONF_ROUTER == 0 */
#if !UIP_CONF_ROUTER
#define UIP_CONF_DS6_AADDR_NBU               1
#endif
#define UIP_CONF_ND6_SEND_RA                 0
#define UIP_CONF_IP_FORWARD                  0
#define RPL_CONF_STATS                       0
#define RPL_CONF_MAX_DAG_ENTRIES             1
#ifndef RPL_CONF_OF
#define RPL_CONF_OF rpl_of_etx
#endif

#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER       10000

#ifndef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU                 4 /* Handle n Neighbors */
#endif
#ifndef UIP_CONF_DS6_ROUTE_NBU
#define UIP_CONF_DS6_ROUTE_NBU               4 /* Handle n Routes */
#endif

/* uIP */
#ifndef UIP_CONF_UBFFER_SIZE   
#define UIP_CONF_BUFFER_SIZE                 130
#endif
#define UIP_CONF_IPV6_QUEUE_PKT              0
#define UIP_CONF_IPV6_CHECKS                 1
#define UIP_CONF_IPV6_REASSEMBLY             0
#define UIP_CONF_RECEIVE_WINDOW              60

/* 6lowpan */
#define SICSLOWPAN_CONF_COMPRESSION          SICSLOWPAN_COMPRESSION_HC06
#ifndef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG                  1 /* About 2KB of CODE if 1 */
#endif
#define SICSLOWPAN_CONF_MAXAGE               8

/* Define our IPv6 prefixes/contexts here */
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS    1
#define SICSLOWPAN_CONF_ADDR_CONTEXT_0 { \
  addr_contexts[0].prefix[0] = 0xaa; \
  addr_contexts[0].prefix[1] = 0xaa; \
}

#else /* UIP_CONF_IPV6 */
/* Network setup for non-IPv6 (rime). */
#define UIP_CONF_IP_FORWARD                  1
#define UIP_CONF_BUFFER_SIZE               108
#define RIME_CONF_NO_POLITE_ANNOUCEMENTS     0
#endif /* UIP_CONF_IPV6 */


#endif /* __CONTIKI_CONF_H__ */
