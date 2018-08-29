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

#include "Base.h"
#include "DateTime.h"
#include "PcapHelper.h"

#include <cstdio>
#include <cstring>
#include <string>

// #include <netinet/if_ether.h>
#define LINE_LEN 16

int frame_num = 0;
FILE *m_record_file;

void handler_dump(u_char *buf, const struct pcap_pkthdr *header,
                  const u_char *data)
{   
    u_int i = 0;
    
    frame_num++;
    
    /* print pkt timestamp and pkt len */
    printf("#%d %ld:%d (%d)\n", frame_num,
           header->ts.tv_sec, (int)header->ts.tv_usec, header->len);
    
    /* Print the packet */
    for (i = 1; (i < header->caplen + 1); i++) {
        printf("%.2x ", data[i - 1]);
        if ((i % LINE_LEN) == 0) {
            printf("\n");
        }
    }

    printf("\n\n");
}

int load_pcap_file(const char *fname, pcap_handler act)
{
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];

    // open a dump file
    if ((fp = pcap_open_offline(fname, errbuf)) == NULL) {
        LOGFILE(LOG_ERROR, "Error opening dump file %s", fname);
        return -1;
    }

    frame_num = 0;

    // read packets
    pcap_loop(fp, 0, act, NULL);

    // close file
    pcap_close(fp);
    //close_tcp_stream(-1);

    return 0;
}

void handler_qdata(u_char *buf, struct pcap_pkthdr *header, const u_char *data)
{
    //fprintf(m_record_file, "\n%s %c %s %zu\n", datebuf, tc, code, data_len);
    //fwrite(&data_len, sizeof(data_len),  1, m_record_file); // length
    //fwrite(zmq_msg_data(&msg), data_len, 1, m_record_file); // data

    char *str, line[128];
    char date[64], tc, code[16];
    unsigned long len;
    struct timeval tv = { 0, 0 };
    struct tm stm;

    frame_num++;
    
    do {
        str = fgets(line, sizeof(line), m_record_file);
    } while (NULL != str && strlen(str) > 0);

    if (NULL == str) return;

    // datebuf, tc, code, data_len
    int usec;
    str = fgets(line, sizeof(line), m_record_file);
    sscanf(str, "%s %c %s %zu", &date[0], &tc, &code[0], &len);
    sscanf(date, "%02d%02d%02d.%3d", &stm.tm_hour, &stm.tm_min, &stm.tm_sec, &usec);
    tv.tv_sec   = stm.tm_hour * 60 * 60L + stm.tm_min * 60 + stm.tm_sec;
    tv.tv_usec  = usec * 1000L;
    header->ts = tv;

    printf("[%06d]: %s", frame_num, str);

    size_t data_len;
    fread(&data_len, sizeof(data_len), 1, m_record_file);
    header->caplen = data_len;
    header->len    = data_len;
    fread(buf, data_len, 1, m_record_file);
}


int save_pcap_file(const char *fname, pcap_handler act)
{
    pcap_t *fp;
    pcap_dumper_t *dumper;

    // open a dump file
    if (NULL == (fp = pcap_open_dead(DLT_EN10MB, 1 << 16))) {
        LOGFILE(LOG_ERROR, "Error opening pcap dead: %s", pcap_strerror(errno));
        return -1;
    }

    std::string fname_pcap = fname;
    size_t lastdot = fname_pcap.find_last_of(".");
    fname_pcap = ((lastdot == std::string::npos) ? fname_pcap : fname_pcap.substr(0, lastdot));
    fname_pcap += ".pcap";

    m_record_file = fopen(fname, "rb");

    if (NULL == (dumper = pcap_dump_open(fp, fname_pcap.c_str()))) {
        LOGFILE(LOG_ERROR, "Error opening dump file %s", fname);
        pcap_close(fp);
        return -1;        
    }

    frame_num = 0;

    pcap_pkthdr pcap_hdr;
    u_char *pdata = new u_char[1024 * 64];
    do {
        pcap_hdr.caplen = pcap_hdr.len = 0;
        act(pdata, &pcap_hdr, NULL);
        pcap_dump((u_char *)dumper, &pcap_hdr, pdata);
    } while (pcap_hdr.caplen > 0);

    // close file
    delete [] pdata;
    pcap_dump_close(dumper);
    fclose(m_record_file);

    return 0;
}
