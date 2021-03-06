/***************************************************************
 *
 * (C) 2011-16 Nicola Bonelli <nicola@pfq.io>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 ****************************************************************/

#ifndef PFQ_DEFINE_H
#define PFQ_DEFINE_H

#include <pfq/types.h>

#define Q_MAX_ID			((int)sizeof(long)<<3)
#define Q_MAX_GID			((int)sizeof(long)<<3)
#define Q_BUFF_BATCH_LEN		((int)sizeof(__int128)<<3)

#define Q_BUFF_LOG_LEN			16
#define Q_BUFF_QUEUE_LEN		512

#define Q_MAX_STEERING_MASK	        512

#define Q_MAX_DEVICE			4096
#define Q_MAX_DEVICE_MASK		(Q_MAX_DEVICE-1)
#define Q_MAX_QUEUE			256
#define Q_MAX_QUEUE_MASK		(Q_MAX_QUEUE-1)

#define Q_MAX_TX_SKB_COPY		256

#define Q_GRACE_PERIOD			200 /* msec */

#define Q_FUN_SYMB_LEN			256
#define Q_FUN_SIGN_LEN			1024
#define Q_FUN_MAX_ENTRIES		1024

#define Q_MAX_CPU			256
#define Q_MAX_CPU_MASK			(Q_MAX_CPU-1)

#define Q_GROUP_PERSIST_MEM		64
#define Q_GROUP_PERSIST_DATA		1024

#define Q_MAX_SOCKQUEUE_LEN		262144

#define Q_INVALID_ID			(__force pfq_id_t)-1


#endif /* PFQ_DEFINE_H */
