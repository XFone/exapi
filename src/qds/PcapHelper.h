/*
 * $Id: $
 *
 * PcapHelper use libpcap to load or save log/data/packet files
 *
 * Copyright (c) 2014-2017 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#ifndef _PCAP_HELPER_H_
#define _PCAP_HELPER_H_

#include "Base.h"
#include "Log.h"

#include <pcap.h>
#include <time.h>

BEGIN_EXTERN_C

extern int frame_num;

//pcap_handler
void handler_dump(u_char *buf, const struct pcap_pkthdr *header, const u_char *data);

void handler_qdata(u_char *buf, struct pcap_pkthdr *header, const u_char *data);

int load_pcap_file(const char *fname, pcap_handler act);

int save_pcap_file(const char *fname, pcap_handler act);

END_EXTERN_C


#endif /* _PCAP_HELPER_H_ */