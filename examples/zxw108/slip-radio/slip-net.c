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

#include "contiki.h"
#include "net/netstack.h"
#include "net/uip.h"
#include "net/packetbuf.h"
#include "dev/slip.h"
#include <stdio.h>

#define SLIP_END     0300
#define SLIP_ESC     0333
#define SLIP_ESC_END 0334
#define SLIP_ESC_ESC 0335

#define DEBUG   0 
#if DEBUG
#define PRINTF(...)  do{
			printf(__VA_ARGS__);\
			}while(0)
#else
#define PRINTF(...)
#endif

void slip_sicslowpan_init(void);
uint8_t slip_sicslowpan_output(uip_lladdr_t *localdest);
void slip_sicslowpan_input(void);
void slip_sicslowpan_setinput_callback(void (*f)(void));


/*--------------------------------------------------------------------------*/
static void radio_input_callback(void)
{
  const	rimeaddr_t *src;

  PRINTF("radio_input_callback() len(%u)\n", uip_len);
#if 0   
  {
	int i;
  	printf("slip-radio uip_buf:\r\n");
	for (i=0; i<uip_len; i++)
		printf("%02X ", uip_buf[i]);
	printf("\r\nslip-radio uip buf end\r\n");
 } 
#endif
        
  src = packetbuf_addr(PACKETBUF_ADDR_SENDER);
  if(!rimeaddr_cmp(src, &rimeaddr_null)) {
        uint8_t buf[RIMEADDR_SIZE+4];
	rimeaddr_copy((rimeaddr_t *)&buf[2], src);
	buf[0] = '!';
	buf[1] = 'R';
	slip_write(buf, RIMEADDR_SIZE+2);
  }   
 
  if (uip_len > 0) {
 	slip_write(&uip_buf[UIP_LLH_LEN], uip_len);
	/*slip_send();*/
	uip_len = 0;
  }
}
/*--------------------------------------------------------------------------*/

static void
slipnet_init(void)
{
    PRINTF("slip net driver init\n");
    slip_sicslowpan_setinput_callback(radio_input_callback);
    slip_sicslowpan_init();	
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void
slipnet_input(void)
{
	slip_sicslowpan_input();
}
/*---------------------------------------------------------------------------*/
const struct network_driver slipnet_driver = {
  "slipnet",
  slipnet_init,
  slipnet_input
};
/*---------------------------------------------------------------------------*/
