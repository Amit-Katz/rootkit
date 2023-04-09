#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/icmp.h>

#define HIDDEN_IP "192.168.1.167"

static unsigned int hook_ip(
    void *priv,
    struct sk_buff *skb,
    const struct nf_hook_state *state)
{
    struct iphdr *ip_header;

    if (!skb)
        return NF_ACCEPT;

    ip_header = ip_hdr(skb);

    if ((ip_header->protocol == IPPROTO_UDP ||
         ip_header->protocol == IPPROTO_ICMP) &&
        ip_header->saddr == in_aton(HIDDEN_IP))
        return NF_DROP;

    return NF_ACCEPT;
}
