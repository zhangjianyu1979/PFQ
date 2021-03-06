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

#ifndef PFQ_GROUP_H
#define PFQ_GROUP_H

#include <pfq/define.h>
#include <pfq/kcompat.h>
#include <pfq/atomic.h>
#include <pfq/sparse.h>
#include <pfq/types.h>
#include <pfq/bpf.h>

#include <linux/pf_q.h>

typedef struct pfq_kernel_stats pfq_group_stats_t;
struct pfq_group_counters;

struct pfq_group
{
        int policy;                                     /* group policy */
        int pid;	                                /* process id/tgid */

	pfq_id_t owner;					/* owner's pfq id */

        atomic_long_t sock_id[Q_CLASS_MAX];		/* list of (bitwise) socket ids that joined this group, for each different class:
        						   Q_CLASS_DEFAULT, Q_CLASS_USER_PLANE, Q_CLASS_CONTROL_PLANE etc... */

        atomic_long_t bp_filter;			/* struct sk_filter pointer */

        atomic_long_t comp;                             /* struct pfq_lang_computation_tree *  (new functional program) */
        atomic_long_t comp_ctx;                         /* void *: storage context (new functional program) */

	pfq_group_stats_t __percpu *stats;
	struct pfq_group_counters __percpu *counters;

        bool   enabled;
        bool   vlan_filt;                               /* enable/disable vlan filtering */
        char   vid_filters[4096];                       /* vlan filters */

};


struct pfq_lang_computation_tree;

extern int  pfq_group_join_free(pfq_id_t id, unsigned long class_mask, int policy);
extern int  pfq_group_join(pfq_gid_t gid, pfq_id_t id, unsigned long class_mask, int policy);
extern int  pfq_group_leave(pfq_gid_t gid, pfq_id_t id);
extern int  pfq_group_set_prog(pfq_gid_t gid, struct pfq_lang_computation_tree *prog, void *ctx);
extern void pfq_group_leave_all(pfq_id_t id);

extern unsigned long pfq_group_get_groups(pfq_id_t id);
extern unsigned long pfq_group_get_all_sock_mask(pfq_gid_t gid);

extern int  pfq_group_get_context(pfq_gid_t gid, int level, int size, void __user *context);
extern void pfq_group_set_filter(pfq_gid_t gid, struct sk_filter *filter);

extern struct pfq_group * pfq_group_get(pfq_gid_t gid);

extern bool pfq_group_vlan_filters_enabled(pfq_gid_t gid);
extern bool pfq_group_check_vlan_filter(pfq_gid_t gid, int vid);
extern bool pfq_group_toggle_vlan_filters(pfq_gid_t gid, bool value);
extern void pfq_group_set_vlan_filter(pfq_gid_t gid, bool value, int vid);

extern bool pfq_group_policy_access(pfq_gid_t gid, pfq_id_t id, int policy);
extern bool pfq_group_access(pfq_gid_t gid, pfq_id_t id);

extern void pfq_group_lock(void);
extern void pfq_group_unlock(void);

extern int  pfq_groups_init(void);
extern void pfq_groups_destruct(void);

static inline
bool pfq_group_has_joined(pfq_gid_t gid, pfq_id_t id)
{
        return (pfq_group_get_all_sock_mask(gid) & (1UL << (__force int)id));
}

static inline
bool pfq_group_is_free(pfq_gid_t gid)
{
	struct pfq_group * g;
	g = pfq_group_get(gid);
	if (g && g->enabled)
		return false;
	return true;
}


static inline
int pfq_get_tgid(void)
{
	return current->tgid;
}

#endif /* PFQ_GROUP_H */
