//Filename: chksum.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/version.h>

// this struct stores the required infos to register a Netfilter hook
static struct nf_hook_ops nfho;

// the following function will be called at the pre-routing hook
unsigned int hook_func(void *priv,
		struct sk_buff *skb,
                const struct nf_hook_state *state) {
	printk(KERN_INFO "Packet!\n");
	return NF_ACCEPT;
}

// initialize and register the hook
static int __init initialize(void) {
	nfho.hook	= hook_func;
	nfho.hooknum	= NF_INET_PRE_ROUTING;
	nfho.pf		= PF_INET;		//ipv4
	nfho.priority	= NF_IP_PRI_FIRST;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    	nf_register_net_hook(&init_net, &nfho);
#else
    	nf_register_hook(&nfho);
#endif
	return 0;
}

static void __exit cleanup(void) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    	nf_unregister_net_hook(&init_net, &nfho);
#else
    	nf_unregister_hook(&nfho);
#endif
}

module_init(initialize);
module_exit(cleanup);
