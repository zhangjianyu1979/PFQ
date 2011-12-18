/***************************************************************
 *                                                
 * (C) 2011 - Nicola Bonelli <nicola.bonelli@cnit.it>   
 *            Andrea Di Pietro <andrea.dipietro@for.unipi.it>
 *
 ****************************************************************/

#include <mpdb-queue.h>

bool 
mpdb_enqueue(struct pfq_opt *pq, struct sk_buff *skb)
{
    struct pfq_queue_descr  *queue_descr = (struct pfq_queue_descr *)pq->q_mem;

    if (!atomic_read((atomic_t *)&queue_descr->disable))  
    {
        size_t packet_len = skb->len + skb->mac_len;
        size_t caplen     = min_t(size_t, pq->q_caplen, packet_len);
        size_t slot_len   = ALIGN(sizeof(struct pfq_hdr) + caplen, 8);

        uint64_t new_data    = atomic64_add_return(0x0000000100000000ULL|slot_len, (atomic64_t *)&queue_descr->data);
        uint64_t queue_size  = DBMP_QUEUE_SIZE(new_data);
        bool     queue_index = DBMP_QUEUE_INDEX(new_data);

        if (queue_size <= pq->q_queue_mem)
        {
            /* enqueue skb */

            struct pfq_hdr * hdr = (struct pfq_hdr *)((char *)(queue_descr+1) + (queue_index ? pq->q_queue_mem : 0) + queue_size - slot_len);
            char * pkt = (char *)(hdr+1);

            /* setup the header */

            hdr->len      = packet_len;
            hdr->caplen   = caplen;
            hdr->if_index = skb->dev->ifindex;
            hdr->hw_queue = skb_get_rx_queue(skb);                      

            if (pq->q_tstamp != 0)
            {
                struct timespec ts;
                skb_get_timestampns(skb, &ts); 
                hdr->tstamp.tv.sec  = ts.tv_sec;
                hdr->tstamp.tv.nsec = ts.tv_nsec;
            }

           /* copy caplen bytes of packet */
           
            if (caplen &&
                skb_copy_bits(skb, /* offset */ -skb->mac_len, pkt, caplen) != 0)
                return false;

           /* commit the slot with release semantic */
           wmb();

           hdr->commit = 1;
           
           /* watermark */

           if ( (queue_size > ( pq->q_queue_mem >> 1)) && 
                           queue_descr->poll_wait ) {
                wake_up_interruptible(&pq->q_waitqueue);
           }

           return true;
        }
        else if ( (queue_size - slot_len) <= pq->q_queue_mem )
        {
            uint64_t valid_data =  new_data - (0x0000000100000000ULL|slot_len);
            
            atomic64_set((atomic64_t *)&queue_descr->valid_data, valid_data);
            
            /* release semantic: volatile variables are not reordered */
            atomic_set((atomic_t *)&queue_descr->disable,1);
        }
    }
    
    if ( queue_descr->poll_wait ) {
        wake_up_interruptible(&pq->q_waitqueue);
    }
    
    return false;
}

