/*
 * Copyright (c) 2011, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         Slip-radio driver
 * \author
 *         Niclas Finne <nfi@sics.se>
 *         Joakim Eriksson <joakime@sics.se>
 */
#include "contiki.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "dev/slip.h"
#include <string.h>
#include "net/netstack.h"
#include "net/packetbuf.h"

#define DEBUG DEBUG_NONE
#include "net/uip-debug.h"

#define SLIP_END     0300
#define SLIP_ESC     0333
#define SLIP_ESC_END 0334
#define SLIP_ESC_ESC 0335

static uip_lladdr_t _rime_send_addr;

uint8_t slip_sicslowpan_output(uip_lladdr_t *localdest);
void slip_sicslowpan_set_packet_sent_callback(void (*f)(void*,int,int));


/*---------------------------------------------------------------------------*/
static void
packet_sent(void *ptr, int status, int transmissions)
{
  uint8_t buf[20];
  uint8_t sid;
  int pos;
  rimeaddr_t *dest = ptr;   

  sid = *((uint8_t *)ptr);
  PRINTF("Slip-radio: packet sent! sid: %d, status: %d, tx: %d\n", sid, status, transmissions); /* packet callback from lower layers */
  /*  neighbor_info_packet_sent(status, transmissions); */
  pos = 0;
  buf[pos++] = '!';
  buf[pos++] = 'S';
  buf[pos++] = 0; //sid;
  buf[pos++] = status; /* one byte ? */
  buf[pos++] = transmissions;
  slip_write(buf, pos);
}
/*---------------------------------------------------------------------------*/
static void  
slip_input_callback(void)
{
  uint8_t *data = &uip_buf[UIP_LLH_LEN];
  int len = uip_len; 

  //PRINTF("slip-radio slip cmd %c %c len(%u)\n", data[0], data[1],uip_len);
  slip_arch_writeb(SLIP_END);
  if(data[0] == '!') {
    if (data[1] == 'R'){
  	int i;
        //PRINTF("slip-radio: got send rime addr %u", data[2]);
	for (i=0; i<RIMEADDR_SIZE-1; i++) {
		PRINTF(":%u", data[3+i]);
	}
	PRINTF("\n"); 
	rimeaddr_copy(&_rime_send_addr, (rimeaddr_t *)&data[2]);
	uip_len = 0;
	return ;
    }    
  } else if(uip_buf[0] == '?') {
    PRINTF("Got request message of type %c\n", uip_buf[1]);
    if(data[1] == 'M') {
	int i;
      /* this is just a test so far... just to see if it works */
      uip_buf[0] = '!';
      uip_buf[1] = 'M';
      for(i = 0; i < 8; i++) {
        uip_buf[2 + i] = uip_lladdr.addr[i];
      }
      uip_len = 10;
      slip_write(uip_buf, uip_len);
      return ;
    }
  } else {
        slip_sicslowpan_output(&_rime_send_addr);
        uip_len = 0;
	return ;
  }
  return ;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void
init(void)
{
  rimeaddr_copy(&_rime_send_addr, &rimeaddr_null);
  slip_sicslowpan_set_packet_sent_callback(packet_sent);
  process_start(&slip_process, NULL);
  slip_set_input_callback(slip_input_callback);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PROCESS(slip_radio_process, "Slip radio process");
AUTOSTART_PROCESSES(&slip_radio_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(slip_radio_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();

  printf("Slip Radio started...\n");

  init();

  etimer_set(&et, CLOCK_SECOND * 3);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      etimer_reset(&et);
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
