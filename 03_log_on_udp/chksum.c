//Filename: chksum.c
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/version.h>
#include <linux/ip.h>
#include <linux/udp.h>

#define IP_HDR_LEN 20

// this struct stores the required infos to register a Netfilter hook
static struct nf_hook_ops nfho;
struct iphdr *iph;
struct udphdr *udp_header;
struct sk_buff *sock_buff;

// the following function will be called at the pre-routing hook
unsigned int hook_func(void *priv,
		struct sk_buff *skb,
                const struct nf_hook_state *state) {
	__u16 dst_port, src_port;

	sock_buff = skb;

	if (!sock_buff) {
		return NF_ACCEPT;
	}

	iph = (struct iphdr *)skb_network_header(sock_buff);

	if (!iph){
		return NF_ACCEPT;
	}

	if (iph->protocol==IPPROTO_UDP) {
		udp_header = (struct udphdr *) skb_header_pointer (skb, IP_HDR_LEN, 0, NULL);
		src_port = ntohs(udp_header->source);
		dst_port = ntohs(udp_header->dest);
		if (dst_port == 5000) {
			printk(KERN_INFO "Got UDP frame with dest %u, zeroing checksum\n", dst_port);
			udp_header->check = 0;
		}
	}

	return NF_ACCEPT;
}

// initialize and register the hook
static int __init initialize(void) {
	nfho.hook	= hook_func;
	nfho.hooknum	= NF_INET_POST_ROUTING;
	nfho.pf		= PF_INET;		//ipv4
	nfho.priority	= NF_IP_PRI_FIRST;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    	nf_register_net_hook(&init_net, &nfho);
#else
    	nf_register_hook(&nfho);
#endif
	printk(KERN_INFO "Registering module\n");
	return 0;
}

static void __exit cleanup(void) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    	nf_unregister_net_hook(&init_net, &nfho);
#else
    	nf_unregister_hook(&nfho);
#endif
	printk(KERN_INFO "Unregistering module\n");
}

module_init(initialize);
module_exit(cleanup);
