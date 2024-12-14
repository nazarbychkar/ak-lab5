/* hello1_modified.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_AUTHOR("Nazar Bychkar <nazar@bychkar.com>");
MODULE_DESCRIPTION("Hello1 module");
MODULE_LICENSE("Dual BSD/GPL");

struct timing_data {
    struct list_head node;
    ktime_t start_time;
    ktime_t end_time;
};

static LIST_HEAD(timing_list);

void print_hello_messages(unsigned int count)
{
    unsigned int idx;
    struct timing_data *entry;

    for (idx = 0; idx < count; ++idx) {
        entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry) {
            pr_err("Failed to allocate memory for timing_data\n");
            return;
        }

        entry->start_time = ktime_get();
        pr_info("Hello, world!\n");
        entry->end_time = ktime_get();

        list_add_tail(&entry->node, &timing_list);
    }
}
EXPORT_SYMBOL(print_hello_messages);

static int __init module_load(void)
{
    pr_info("Hello1 module has been loaded\n");
    return 0;
}

static void __exit module_unload(void)
{
    struct timing_data *entry, *next_entry;
    s64 elapsed_time;

    list_for_each_entry_safe(entry, next_entry, &timing_list, node) {
        elapsed_time = ktime_to_ns(ktime_sub(entry->end_time, entry->start_time));
        pr_info("Elapsed time for message: %lld ns\n", elapsed_time);
        list_del(&entry->node);
        kfree(entry);
    }

    pr_info("Hello1 module has been unloaded\n");
}

module_init(module_load);
module_exit(module_unload);

